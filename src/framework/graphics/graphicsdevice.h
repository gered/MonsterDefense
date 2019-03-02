#ifndef __FRAMEWORK_GRAPHICS_GRAPHICSDEVICE_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_GRAPHICSDEVICE_H_INCLUDED__

#include "../common.h"

#include "textureparameters.h"
#include "../math/matrix4x4.h"
#include "../math/matrixstack.h"
#include "../math/rect.h"

#include <stl/list.h>
#include <stl/vector.h>

class BufferObject;
class Camera;
class DebugShader;
class GameWindow;
class GeometryDebugRenderer;
class GraphicsContextResource;
class Image;
class IndexBuffer;
class Shader;
class SimpleColorShader;
class SimpleColorTextureShader;
class SimpleTextureShader;
class SimpleTextureVertexLerpShader;
class SolidColorTextureCache;
class Sprite2DShader;
class Sprite3DShader;
class Texture;
class VertexBuffer;
struct Color;

typedef stl::list<GraphicsContextResource*> ManagedResourceList;
typedef stl::vector<uint32_t> EnabledVertexAttribList;

/**
 * Provides an abstraction over the underlying OpenGL context.
 */
class GraphicsDevice
{
public:
	/**
	 * Creates a graphics device object based on a parent window that is
	 * hosting the OpenGL context.
	 * @param window a window with an active OpenGL context associated with it
	 */
	GraphicsDevice(GameWindow *window);

	virtual ~GraphicsDevice();

	/**
	 * New OpenGL graphics context creation callback.
	 */
	void OnNewContext();

	/**
	 * Lost OpenGL graphics context callback.
	 */
	void OnLostContext();

	/**
	 * Viewport resize callback.
	 * @param size the new viewport size
	 */
	void OnResize(const Rect &size);

	/**
	 * Viewport resize callback.
	 * @param left the left X coordinate of the new viewport
	 * @param top the top Y coordinate of the new viewport
	 * @param right the right X coordinate of the new viewport
	 * @param bottom the bottom Y coordinate of the new viewport
	 */
	void OnResize(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom);

	/**
	 * Render callback.
	 */
	void OnRender();

	/**
	 * Clears the display.
	 * @param r red component of the color to clear the display to
	 * @param g green component of the color to clear the display to
	 * @param b blue component of the color to clear the display to
	 * @param a alpha component of the color to clear the display to
	 */
	void Clear(float r, float g, float b, float a);

	/**
	 * Clears the display.
	 * @param color the color to clear the display to
	 */
	void Clear(const Color &color);

	/**
	 * Creates a new Texture object from the raw image data provided.
	 * @param image the image to create the texture from
	 * @return the new texture or NULL on failure
	 */
	Texture* CreateTextureFromImage(const Image *image);

	/**
	 * Gets a texture that is filled with a single solid color.
	 * @param the color to get a texture for
	 * @return the texture or NULL on failure
	 */
	Texture* GetSolidColorTexture(const Color &color);

	/**
	 * Updates all of or just part of an existing texture with new image data.
	 * @param texture the texture to update
	 * @param image the new image data which should not have dimensions exceeding the texture
	 * @param destX the X coordinate on the texture to place the new image at
	 * @param destY the Y coordinate on the texture to place the new image at
	 */
	void UpdateTextureFromImage(const Texture *texture, const Image *image, uint16_t destX, uint16_t destY);

	/**
	 * Frees a texture object.
	 * @param texture the texture to be freed. This pointer will not be valid after this method returns.
	 */
	void FreeTexture(Texture *texture);

	/**
	 * Binds a texture for rendering.
	 * @param texture the texture to bind
	 */
	void BindTexture(const Texture *texture);

	/**
	 * Binds a texture that is filled with the specified solid color.
	 * @param color a color with which to find and bind a texture with
	 */
	void BindSolidColorTexture(const Color &color);

	/**
	 * Unbinds the currently bound texture.
	 */
	void UnbindTexture();

	/**
	 * Sets the current texture parameters which all subsequent
	 * texture management calls will use.
	 * @param params the texture parameters to set
	 */
	void SetTextureParameters(const TextureParameters &params);

	/**
	 * @return the current projection matrix
	 */
	Matrix4x4 GetProjectionMatrix() const                  { return m_projectionStack.top; }

	/**
	 * Sets a new projection matrix.
	 * @param m the new projection matrix
	 */
	void SetProjectionMatrix(const Matrix4x4 &m);

	/**
	 * Pushes the current projection matrix down the stack by one, duplicating
	 * the current matrix.
	 */
	void PushProjectionMatrix();

	/**
	 * Pops the current projection matrix, replacing the current matrix with
	 * the top one on the stack.
	 */
	void PopProjectionMatrix();

	/**
	 * @return an orthographic matrix appropriate for the current viewport dimensions
	 */
	Matrix4x4 GetOrthographicProjectionMatrix();

	/**
	 * @return the current modelview matrix
	 */
	Matrix4x4 GetModelViewMatrix() const                   { return m_modelviewStack.top; }

	/**
	 * Sets a new modelview matrix.
	 * @param m the new modelview matrix
	 */
	void SetModelViewMatrix(const Matrix4x4 &m);

	/**
	 * Pushes the current modelview matrix down the stack by one, duplicating
	 * the current matrix.
	 */
	void PushModelViewMatrix();

	/**
	 * Pops the current modelview matrix, replacing the current matrix with
	 * the top one on the stack.
	 */
	void PopModelViewMatrix();

	/**
	 * @return the current camera object
	 */
	Camera* GetCamera() const                              { return m_camera; }

	/**
	 * Sets a new camera object, or restores a default camera.
	 * @param camera the new camera object to set, or if NULL, sets a default
	 *               camera object
	 */
	void SetCamera(Camera *camera);

	/**
	 * Registers a resource to have it's lifecycle management events
	 * raised automatically.
	 * @param resource the resource to be registered
	 */
	void RegisterManagedResource(GraphicsContextResource *resource);

	/**
	 * Unregisters a resource that was previously registered. It's lifecycle
	 * management events will no longer be raised automatically.
	 * @param resource the resource to be unregistered
	 */
	void UnregisterManagedResource(GraphicsContextResource *resource);

	/**
	 * Unregisters all resources that were previously registered. Their
	 * lifecycle management events will no longer be raised automatically.
	 */
	void UnregisterAllManagedResources();

	/**
	 * Binds a vertex buffer for rendering. If the buffer has an associated
	 * VBO it's contents will be uploaded to video memory if necessary.
	 * @param buffer the vertex buffer to bind
	 */
	void BindVertexBuffer(VertexBuffer *buffer);

	/**
	 * Unbinds a vertex buffer.
	 */
	void UnbindVertexBuffer();

	/**
	 * Binds an index buffer for rendering. If the buffer has an associated
	 * IBO it's contents will be uploaded to video memory if necessary.
	 * @param buffer the index buffer to bind
	 */
	void BindIndexBuffer(IndexBuffer *buffer);

	/**
	 * Unbinds an index buffer.
	 */
	void UnbindIndexBuffer();

	/**
	 * Binds a shader for rendering.
	 * @param shader the shader to bind
	 */
	void BindShader(Shader *shader);

	/**
	 * Unbinds any currently bound shader.
	 */
	void UnbindShader();

	/**
	 * Renders the currently bound vertex buffer as triangles.
	 * @param buffer index buffer containing indices of the vertices to be rendered
	 */
	void RenderTriangles(const IndexBuffer *buffer);

	/**
	 * Renders the currently bound vertex buffer as triangles.
	 */
	void RenderTriangles();

	/**
	 * Renders the currently bound vertex buffer as triangles.
	 * @param startVertex the index of the first vertex to be rendered
	 * @param numTriangles the number of triangles to be rendered
	 */
	void RenderTriangles(uint32_t startVertex, uint32_t numTriangles);

	/**
	 * Renders the currently bound vertex buffer as lines.
	 * @param buffer index buffer containing indices of the vertices to be rendered
	 */
	void RenderLines(const IndexBuffer *buffer);

	/**
	 * Renders the currently bound vertex buffer as lines.
	 */
	void RenderLines();

	/**
	 * Renders the currently bound vertex buffer as lines.
	 * @param startVertex the index of the first vertex to be rendered
	 * @param numLines the number of lines to be rendered
	 */
	void RenderLines(uint32_t startVertex, uint32_t numLines);

	/**
	 * Renders the currently bound vertex buffer as points.
	 */
	void RenderPoints(const IndexBuffer *buffer);

	/**
	 * Renders the currently bound vertex buffer as points.
	 */
	void RenderPoints();

	/**
	 * Renders the currently bound vertex buffer as points.
	 * @param startVertex the index of the first vertex to be rendered
	 * @param numPoints the number of points to be rendered
	 */
	void RenderPoints(uint32_t startVertex, uint32_t numPoints);

	/**
	 * @return the top Y coordinate of the current viewport
	 */
	uint16_t GetViewportTop() const                        { return m_viewport.top; }

	/**
	 * @return the left X coordinate of the current viewport
	 */
	uint16_t GetViewportLeft() const                       { return m_viewport.left; }

	/**
	 * @return the bottom Y coordinate of the current viewport
	 */
	uint16_t GetViewportBottom() const                     { return m_viewport.bottom; }

	/**
	 * @return the right X coordinate of the current viewport
	 */
	uint16_t GetViewportRight() const                      { return m_viewport.right; }

	/**
	 * @return the current viewport width
	 */
	uint16_t GetViewportWidth() const                      { return m_viewport.GetWidth(); }

	/**
	 * @return the current viewport height
	 */
	uint16_t GetViewportHeight() const                     { return m_viewport.GetHeight(); }

	/**
	 * @return a debug geometry renderer
	 */
	GeometryDebugRenderer* GetDebugRenderer() const        { return m_debugRenderer; }

	/**
	 * @return built-in shader
	 */
	SimpleColorShader* GetSimpleColorShader() const        { return m_simpleColorShader; }

	/**
	 * @return built-in shader
	 */
	SimpleColorTextureShader* GetSimpleColorTextureShader() const { return m_simpleColorTextureShader; }

	/**
	 * @return built-in shader
	 */
	SimpleTextureShader* GetSimpleTextureShader() const    { return m_simpleTextureShader; }

	/**
	 * @return built-in shader
	 */
	Sprite2DShader* GetSprite2DShader() const              { return m_sprite2dShader; }

	/**
	 * @return built-in shader
	 */
	Sprite3DShader* GetSprite3DShader() const              { return m_sprite3dShader; }

	SimpleTextureVertexLerpShader* GetSimpleTextureVertexLerpShader() const { return m_simpleTextureVertexLerpShader; }

	/**
	 * @return built-in shader
	 */
	DebugShader* GetDebugShader() const                    { return m_debugShader; }

private:
	void BindVBO(VertexBuffer *buffer);
	void BindClientBuffer(VertexBuffer *buffer);
	void BindIBO(IndexBuffer *buffer);
	void BindClientBuffer(IndexBuffer *buffer);

	BOOL IsReadyToRender() const;

	void SetShaderVertexAttributes();
	void ClearSetShaderVertexAttributes();

	Rect m_viewport;

	ManagedResourceList m_managedResources;

	const VertexBuffer *m_boundVertexBuffer;
	const IndexBuffer *m_boundIndexBuffer;
	const Texture *m_boundTexture;
	Shader *m_boundShader;
	BOOL m_shaderVertexAttribsSet;
	EnabledVertexAttribList m_enabledVertexAttribIndices;

	GameWindow *m_window;
	Camera *m_camera;
	BOOL m_usingDefaultCamera;
	TextureParameters m_currentTextureParams;

	MatrixStack m_modelviewStack;
	MatrixStack m_projectionStack;

	GeometryDebugRenderer *m_debugRenderer;
	SolidColorTextureCache *m_solidColorTextures;

	SimpleColorShader *m_simpleColorShader;
	SimpleColorTextureShader *m_simpleColorTextureShader;
	SimpleTextureShader *m_simpleTextureShader;
	SimpleTextureVertexLerpShader *m_simpleTextureVertexLerpShader;
	Sprite2DShader *m_sprite2dShader;
	Sprite3DShader *m_sprite3dShader;
	DebugShader *m_debugShader;
};

inline void GraphicsDevice::SetTextureParameters(const TextureParameters &params)
{
	m_currentTextureParams = params;
}

inline BOOL GraphicsDevice::IsReadyToRender() const
{
	if (m_boundShader != NULL && m_boundVertexBuffer != NULL && m_shaderVertexAttribsSet)
		return TRUE;
	else
		return FALSE;
}

#endif
