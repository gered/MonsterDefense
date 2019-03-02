#include "../debug.h"
#include "../log.h"

#include "graphicsdevice.h"

#include "blendstate.h"
#include "bufferobject.h"
#include "color.h"
#include "debugshader.h"
#include "geometrydebugrenderer.h"
#include "glincludes.h"
#include "glutils.h"
#include "graphicscontextresource.h"
#include "image.h"
#include "indexbuffer.h"
#include "renderstate.h"
#include "shader.h"
#include "simplecolorshader.h"
#include "simplecolortextureshader.h"
#include "simpletextureshader.h"
#include "simpletexturevertexlerpshader.h"
#include "standardshader.h"
#include "solidcolortexturecache.h"
#include "sprite2dshader.h"
#include "sprite3dshader.h"
#include "texture.h"
#include "vertexattribs.h"
#include "vertexbuffer.h"
#include "../gamewindow.h"
#include "../math/camera.h"
#include "../math/mathhelpers.h"

GraphicsDevice::GraphicsDevice(GameWindow *window)
{
	STACK_TRACE;
	m_viewport = Rect(0, 0, 0, 0);
	m_boundVertexBuffer = NULL;
	m_boundIndexBuffer = NULL;
	m_boundTexture = NULL;
	m_boundShader = NULL;
	m_shaderVertexAttribsSet = FALSE;

	// The OpenGL ES 2.0 spec says GL_MAX_VERTEX_ATTRIBS must be at least 8
	m_enabledVertexAttribIndices.reserve(8);

	m_window = window;
	m_camera = new Camera(this);
	ASSERT(m_camera != NULL);
	m_usingDefaultCamera = TRUE;

	m_currentTextureParams = TEXPARAM_DEFAULT;

	m_debugRenderer = NULL;

	m_solidColorTextures = new SolidColorTextureCache(this);
	ASSERT(m_solidColorTextures != NULL);

	m_simpleColorShader = new SimpleColorShader();
	ASSERT(m_simpleColorShader != NULL);
	m_simpleColorTextureShader = new SimpleColorTextureShader();
	ASSERT(m_simpleColorTextureShader != NULL);
	m_simpleTextureShader = new SimpleTextureShader();
	ASSERT(m_simpleTextureShader != NULL);
	m_simpleTextureVertexLerpShader = new SimpleTextureVertexLerpShader();
	ASSERT(m_simpleTextureVertexLerpShader != NULL);
	m_sprite2dShader = new Sprite2DShader();
	ASSERT(m_sprite2dShader != NULL);
	m_sprite3dShader = new Sprite3DShader();
	ASSERT(m_sprite3dShader != NULL);
	m_debugShader = new DebugShader();
	ASSERT(m_debugShader != NULL);

	RegisterManagedResource(m_simpleColorShader);
	RegisterManagedResource(m_simpleColorTextureShader);
	RegisterManagedResource(m_simpleTextureShader);
	RegisterManagedResource(m_simpleTextureVertexLerpShader);
	RegisterManagedResource(m_sprite2dShader);
	RegisterManagedResource(m_sprite3dShader);
	RegisterManagedResource(m_debugShader);
}

GraphicsDevice::~GraphicsDevice()
{
	STACK_TRACE;
	UnregisterManagedResource(m_simpleColorShader);
	UnregisterManagedResource(m_simpleColorTextureShader);
	UnregisterManagedResource(m_simpleTextureShader);
	UnregisterManagedResource(m_simpleTextureVertexLerpShader);
	UnregisterManagedResource(m_sprite2dShader);
	UnregisterManagedResource(m_sprite3dShader);
	UnregisterManagedResource(m_debugShader);

	if (m_usingDefaultCamera)
		SAFE_DELETE(m_camera);
	SAFE_DELETE(m_debugRenderer);
	SAFE_DELETE(m_solidColorTextures);
	SAFE_DELETE(m_simpleColorShader);
	SAFE_DELETE(m_simpleColorTextureShader);
	SAFE_DELETE(m_simpleTextureShader);
	SAFE_DELETE(m_simpleTextureVertexLerpShader);
	SAFE_DELETE(m_sprite2dShader);
	SAFE_DELETE(m_sprite3dShader);
	SAFE_DELETE(m_debugShader);
}

void GraphicsDevice::OnNewContext()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_GRAPHICS, "Initializing default state for new OpenGL context.\n");

	m_modelviewStack.Clear();
	m_modelviewStack.top = IDENTITY_MATRIX;
	m_projectionStack.Clear();
	m_projectionStack.top = IDENTITY_MATRIX;

	RENDERSTATE_DEFAULT.Apply();
	BLENDSTATE_DEFAULT.Apply();

	UnbindVertexBuffer();
	UnbindIndexBuffer();
	UnbindTexture();
	UnbindShader();

	m_debugRenderer = new GeometryDebugRenderer(this);
	ASSERT(m_debugRenderer != NULL);

	m_solidColorTextures->OnNewContext();

	for (ManagedResourceList::iterator i = m_managedResources.begin(); i != m_managedResources.end(); ++i)
		(*i)->OnNewContext();
}

void GraphicsDevice::OnLostContext()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_GRAPHICS, "Cleaning up objects/state specific to the lost OpenGL context.\n");

	SAFE_DELETE(m_debugRenderer);

	m_solidColorTextures->OnLostContext();

	for (ManagedResourceList::iterator i = m_managedResources.begin(); i != m_managedResources.end(); ++i)
		(*i)->OnLostContext();
}

void GraphicsDevice::OnResize(const Rect &size)
{
	STACK_TRACE;
	Rect viewport;
	
	// based on the orientation, we may need to swap the width/height
	// of the passed viewport dimensions
	SCREEN_ORIENTATION_ANGLE screenOrientation = m_window->GetScreenOrientation();
	if (screenOrientation == SCREEN_ANGLE_90 || screenOrientation == SCREEN_ANGLE_270)
	{
		LOG_INFO(LOGCAT_GRAPHICS, "Screen is rotated (angle = %d). OpenGL viewport will use swapped width and height.\n", (int)screenOrientation);
		// swap width and height
		viewport.left = size.top;
		viewport.top = size.left;
		viewport.right = size.bottom;
		viewport.bottom = size.right;
	}
	else
		viewport = size;

	LOG_INFO(LOGCAT_GRAPHICS, "Resizing OpenGL viewport to (%d, %d) - (%d, %d)\n", viewport.left, viewport.top, viewport.GetWidth(), viewport.GetHeight());
	GL_CALL(glViewport(viewport.left, viewport.top, viewport.GetWidth(), viewport.GetHeight()));

	// however, we don't want to use our adjusted viewport dimensions elsewhere
	// just use the original passed viewport dimensions from now on
	m_viewport = size;
	if (m_camera != NULL)
		m_camera->OnResize(size);
}

void GraphicsDevice::OnResize(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	STACK_TRACE;
	OnResize(Rect(left, top, right, bottom));
}

void GraphicsDevice::OnRender()
{
	STACK_TRACE;
	GLenum error = glGetError();
	ASSERT(error == GL_NO_ERROR);
	if (error != GL_NO_ERROR)
	{
		LOG_ERROR(LOGCAT_OPENGL, "OpenGL error \"%s\"\n", GetGLErrorString(error));

		// keep checking for and reporting errors until there are no more left
		while ((error = glGetError()) != GL_NO_ERROR)
			LOG_ERROR(LOGCAT_OPENGL, "OpenGL error \"%s\"\n", GetGLErrorString(error));
	}

	if (m_camera != NULL)
		m_camera->OnRender();
}

void GraphicsDevice::Clear(float r, float g, float b, float a)
{
	STACK_TRACE;
	GL_CALL(glClearColor(r, g, b, a));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void GraphicsDevice::Clear(const Color &color)
{
	STACK_TRACE;
	Clear(color.r, color.g, color.b, color.a);
}

Texture* GraphicsDevice::CreateTextureFromImage(const Image *image)
{
	STACK_TRACE;
	ASSERT(IsPowerOf2(image->GetWidth()) && IsPowerOf2(image->GetHeight()));
	ASSERT(image->GetBpp() != 8 || image->GetFormat() == IMAGE_FORMAT_ALPHA);
	ASSERT(image->GetBpp() != 16);

	GLuint textureId;
	GL_CALL(glGenTextures(1, &textureId));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, textureId));

	m_currentTextureParams.Apply();

	const void* pixels = image->GetPixels();

	if (image->GetFormat() == IMAGE_FORMAT_ALPHA)
	{
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, image->GetWidth(), image->GetHeight(), 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels));
	}
	else
	{
		uint32_t components = 0;
		uint32_t format = 0;
		if (image->GetBpp() == 24)
		{
			components = GL_RGB;
			format = GL_RGB;
		}
		else if (image->GetBpp() == 32)
		{
			components = GL_RGBA;
			format = GL_RGBA;
		}
		ASSERT(components != 0);
		ASSERT(format != 0);
		// on opengles, these must match!
		ASSERT(components == format);

		//gluBuild2DMipmaps(GL_TEXTURE_2D, components, image->GetWidth(), image->GetHeight(), format, GL_UNSIGNED_BYTE, pixels);
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, components, image->GetWidth(), image->GetHeight(), 0, format, GL_UNSIGNED_BYTE, pixels));
	}

	LOG_INFO(LOGCAT_GRAPHICS, "Created Texture ID = %d, bpp = %d, size = %d x %d\n", textureId, image->GetBpp(), image->GetWidth(), image->GetHeight());
	Texture *texture = new Texture(textureId, image->GetWidth(), image->GetHeight(), image->GetFormat());

	// at this point, this newly created texture is still bound (was done manually
	// via the above glBindTexture call of course). so we should update internal
	// state to reflect this...
	m_boundTexture = texture;

	return texture;
}

Texture* GraphicsDevice::GetSolidColorTexture(const Color &color)
{
	STACK_TRACE;
	return m_solidColorTextures->Get(color);
}

void GraphicsDevice::UpdateTextureFromImage(const Texture *texture, const Image *image, uint16_t destX, uint16_t destY)
{
	STACK_TRACE;
	ASSERT(texture != NULL);
	ASSERT(texture->GetTextureName() != 0);
	ASSERT(texture->IsInvalidated() == FALSE);
	ASSERT(image != NULL);
	ASSERT(destX < texture->GetWidth());
	ASSERT(destY < texture->GetHeight());
	ASSERT(image->GetWidth() <= texture->GetWidth());
	ASSERT(image->GetHeight() <= texture->GetHeight());
	ASSERT(destX + image->GetWidth() <= texture->GetWidth());
	ASSERT(destY + image->GetHeight() <= texture->GetHeight());

	BindTexture(texture);

	const void* pixels = image->GetPixels();

	if (image->GetFormat() == IMAGE_FORMAT_ALPHA)
	{
		GL_CALL(glTexSubImage2D(GL_TEXTURE_2D, 0, destX, destY, image->GetWidth(), image->GetHeight(), GL_ALPHA, GL_UNSIGNED_BYTE, pixels));
	}
	else
	{
		uint32_t format = 0;
		if (image->GetBpp() == 24)
			format = GL_RGB;
		else if (image->GetBpp() == 32)
			format = GL_RGBA;
		ASSERT(format != 0);

		GL_CALL(glTexSubImage2D(GL_TEXTURE_2D, 0, destX, destY, image->GetWidth(), image->GetHeight(), format, GL_UNSIGNED_BYTE, pixels));
	}
}

void GraphicsDevice::FreeTexture(Texture *texture)
{
	STACK_TRACE;
	ASSERT(texture != NULL);
	ASSERT(texture->GetTextureName() != 0);   // technically not an error, glDeleteTextures() will be fine with it

	if (texture == m_boundTexture)
		UnbindTexture();

	uint32_t textureName = texture->GetTextureName();
	GL_CALL(glDeleteTextures(1, &textureName));

	LOG_INFO(LOGCAT_GRAPHICS, "Deleted Texture ID = %d.\n", textureName);

	SAFE_DELETE(texture);
}

void GraphicsDevice::BindTexture(const Texture *texture)
{
	STACK_TRACE;
	ASSERT(texture != NULL);
	ASSERT(texture->IsInvalidated() == FALSE);
	if (texture != m_boundTexture)
		GL_CALL(glBindTexture(GL_TEXTURE_2D, texture->GetTextureName()));
	m_boundTexture = texture;
}

void GraphicsDevice::BindSolidColorTexture(const Color &color)
{
	STACK_TRACE;
	Texture *texture = m_solidColorTextures->Get(color);
	BindTexture(texture);
}

void GraphicsDevice::UnbindTexture()
{
	STACK_TRACE;
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	m_boundTexture = NULL;
}

void GraphicsDevice::SetProjectionMatrix(const Matrix4x4 &m)
{
	STACK_TRACE;
	SCREEN_ORIENTATION_ANGLE screenOrientation = m_window->GetScreenOrientation();
	if (screenOrientation != SCREEN_ANGLE_0)
	{
		// apply a rotation immediately _after_ the projection matrix transform
		float angle = (float)screenOrientation;
		Matrix4x4 adjusted = Matrix4x4::CreateRotationZ(-DegreesToRadians(angle)) * m;
		m_projectionStack.top = adjusted;
	}
	else
		m_projectionStack.top = m;
}

void GraphicsDevice::PushProjectionMatrix()
{
	STACK_TRACE;
	m_projectionStack.Push();
	// with MatrixStack, pushing does not change the top matrix, so
	// we don't need to re-set the projection matrix with OpenGL
}

void GraphicsDevice::PopProjectionMatrix()
{
	STACK_TRACE;
	m_projectionStack.Pop();
}

Matrix4x4 GraphicsDevice::GetOrthographicProjectionMatrix()
{
	STACK_TRACE;
	Matrix4x4 ortho = Matrix4x4::CreateOrthographic((float)m_viewport.left, (float)m_viewport.right, (float)m_viewport.top, (float)m_viewport.bottom, 0.0f, 1.0f);

	SCREEN_ORIENTATION_ANGLE screenOrientation = m_window->GetScreenOrientation();
	if (screenOrientation != SCREEN_ANGLE_0)
	{
		// apply a rotation immediately _after_ the projection matrix transform
		float angle = (float)screenOrientation;
		Matrix4x4 adjusted = Matrix4x4::CreateRotationZ(-DegreesToRadians(angle)) * ortho;
		return adjusted;
	}
	else
		return ortho;
}

void GraphicsDevice::SetModelViewMatrix(const Matrix4x4 &m)
{
	STACK_TRACE;
	m_modelviewStack.top = m;
}

void GraphicsDevice::PushModelViewMatrix()
{
	STACK_TRACE;
	m_modelviewStack.Push();
	// with MatrixStack, pushing does not change the top matrix, so
	// we don't need to re-set the modelview matrix with OpenGL
}

void GraphicsDevice::PopModelViewMatrix()
{
	STACK_TRACE;
	m_modelviewStack.Pop();
}

void GraphicsDevice::SetCamera(Camera *camera)
{
	STACK_TRACE;
	// using the default camera but a new camera is being provided?
	if (m_usingDefaultCamera && camera != NULL)
	{
		LOG_INFO(LOGCAT_GRAPHICS, "Overriding default camera with new camera instance.\n");
		SAFE_DELETE(m_camera);
		m_usingDefaultCamera = FALSE;
		m_camera = camera;
	}

	// not using the default camera already, but setting a new camera
	else if (!m_usingDefaultCamera && camera != NULL)
	{
		LOG_INFO(LOGCAT_GRAPHICS, "Setting new camera instance.\n");
		m_camera = camera;
	}

	// not using the default camera, and clearing ("nulling") the camera
	else if (!m_usingDefaultCamera && camera == NULL)
	{
		LOG_INFO(LOGCAT_GRAPHICS, "Falling back to default camera.\n");
		m_camera = new Camera(this);
		ASSERT(m_camera != NULL);
		m_usingDefaultCamera = TRUE;
	}
}

void GraphicsDevice::RegisterManagedResource(GraphicsContextResource *resource)
{
	STACK_TRACE;
	ASSERT(resource != NULL);

	// make sure this resource isn't in our list already
	stl::list<BufferObject*>::iterator i;
	for (ManagedResourceList::iterator i = m_managedResources.begin(); i != m_managedResources.end(); ++i)
	{
		ASSERT((*i) != resource);
		if ((*i) == resource)
			return;
	}

	m_managedResources.push_back(resource);
}

void GraphicsDevice::UnregisterManagedResource(GraphicsContextResource *resource)
{
	STACK_TRACE;
	ASSERT(resource != NULL);
	m_managedResources.remove(resource);
}

void GraphicsDevice::UnregisterAllManagedResources()
{
	STACK_TRACE;
	m_managedResources.clear();
}

void GraphicsDevice::BindVertexBuffer(VertexBuffer *buffer)
{
	STACK_TRACE;
	ASSERT(buffer != NULL);
	ASSERT(buffer->GetNumElements() > 0);

	// don't bind this buffer if it's already bound!
	if (m_boundVertexBuffer == buffer)
		return;

	if (!buffer->IsClientSideBuffer())
		BindVBO(buffer);
	else
		BindClientBuffer(buffer);

	m_boundVertexBuffer = buffer;
	if (m_shaderVertexAttribsSet)
		ClearSetShaderVertexAttributes();
}

void GraphicsDevice::UnbindVertexBuffer()
{
	STACK_TRACE;
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

	m_boundVertexBuffer = NULL;
	if (m_shaderVertexAttribsSet)
		ClearSetShaderVertexAttributes();
}

void GraphicsDevice::BindIndexBuffer(IndexBuffer *buffer)
{
	STACK_TRACE;
	ASSERT(buffer != NULL);
	ASSERT(buffer->GetNumElements() > 0);

	// don't bind this buffer if it's already bound!
	if (m_boundIndexBuffer == buffer)
		return;

	if (!buffer->IsClientSideBuffer())
		BindIBO(buffer);
	else
		BindClientBuffer(buffer);

	m_boundIndexBuffer = buffer;
}

void GraphicsDevice::UnbindIndexBuffer()
{
	STACK_TRACE;
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	m_boundIndexBuffer = NULL;
}

void GraphicsDevice::BindShader(Shader *shader)
{
	STACK_TRACE;
	ASSERT(shader != NULL);
	ASSERT(shader->IsReadyForUse() == TRUE);
	GL_CALL(glUseProgram(shader->GetProgramId()));
	
	m_boundShader = shader;
	if (m_shaderVertexAttribsSet)
		ClearSetShaderVertexAttributes();

	m_boundShader->OnBind();
}

void GraphicsDevice::UnbindShader()
{
	STACK_TRACE;
	GL_CALL(glUseProgram(0));

	if (m_boundShader != NULL)
		m_boundShader->OnUnbind();

	m_boundShader = NULL;
	if (m_shaderVertexAttribsSet)
		ClearSetShaderVertexAttributes();
}

void GraphicsDevice::BindVBO(VertexBuffer *buffer)
{
	STACK_TRACE;
	if (buffer->IsDirty())
		buffer->Update();

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer->GetBufferId()));
}

void GraphicsDevice::BindClientBuffer(VertexBuffer *buffer)
{
	STACK_TRACE;
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void GraphicsDevice::BindIBO(IndexBuffer *buffer)
{
	STACK_TRACE;
	if (buffer->IsDirty())
		buffer->Update();

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->GetBufferId()));
}

void GraphicsDevice::BindClientBuffer(IndexBuffer *buffer)
{
	STACK_TRACE;
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void GraphicsDevice::SetShaderVertexAttributes()
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundShader != NULL);
	ASSERT(m_enabledVertexAttribIndices.empty() == TRUE);

	uint32_t numAttributes = m_boundShader->GetNumAttributes();
	for (uint32_t i = 0; i < numAttributes; ++i)
	{
		int32_t bufferAttribIndex = 0;
		if (m_boundShader->IsAttributeMappedToStandardType(i))
		{
			VERTEX_ATTRIBS standardType = m_boundShader->GetAttributeMappedStandardType(i);
			bufferAttribIndex = m_boundVertexBuffer->GetIndexOfStandardAttrib(standardType);
			ASSERT(bufferAttribIndex != -1);
			if (bufferAttribIndex == -1)
				continue;
		}
		else
			bufferAttribIndex = m_boundShader->GetAttributeMappedBufferIndex(i);

		uint32_t offset = 0;
		GLint size = 0;

		const VertexBufferAttribute *bufferAttribInfo = m_boundVertexBuffer->GetAttributeInfo((uint32_t)bufferAttribIndex);
		size = bufferAttribInfo->size;
		offset = bufferAttribInfo->offset;
		ASSERT(size != 0);

		// convert the offset into a pointer
		// client-side vertex data has a full pointer to the first element of the attribute data
		// VBO just specifies an offset in bytes from zero to the first element of the attribute data
		const void *buffer = NULL;
		if (m_boundVertexBuffer->IsClientSideBuffer())
			buffer = (float*)m_boundVertexBuffer->GetBuffer() + offset;
		else
			buffer = (int8_t*)NULL + (offset * sizeof(float));

		GL_CALL(glEnableVertexAttribArray(i));
		GL_CALL(glVertexAttribPointer(i, size, GL_FLOAT, FALSE, m_boundVertexBuffer->GetElementWidthInBytes(), buffer));

		m_enabledVertexAttribIndices.push_back(i);
	}

	m_shaderVertexAttribsSet = TRUE;
}

void GraphicsDevice::ClearSetShaderVertexAttributes()
{
	STACK_TRACE;
	while (!m_enabledVertexAttribIndices.empty())
	{
		uint32_t index = m_enabledVertexAttribIndices.back();
		m_enabledVertexAttribIndices.pop_back();
		GL_CALL(glDisableVertexAttribArray(index));
	}

	m_shaderVertexAttribsSet = FALSE;
}

void GraphicsDevice::RenderTriangles(const IndexBuffer *buffer)
{
	STACK_TRACE;
	ASSERT(buffer != NULL);
	ASSERT(buffer->IsClientSideBuffer() == TRUE);
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundIndexBuffer == NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	int numVertices = buffer->GetNumElements();
	ASSERT(numVertices % 3 == 0);
	GL_CALL(glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_SHORT, buffer->GetBuffer()));
}

void GraphicsDevice::RenderTriangles()
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		int numIndices = m_boundIndexBuffer->GetNumElements();
		ASSERT(numIndices % 3 == 0);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = m_boundIndexBuffer->GetBuffer();
		else
			offset = NULL;

		GL_CALL(glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the whole vertex buffer
		ASSERT(m_boundVertexBuffer->GetNumElements() % 3 == 0);
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, m_boundVertexBuffer->GetNumElements()));
	}
}

void GraphicsDevice::RenderTriangles(uint32_t startVertex, uint32_t numTriangles)
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint32_t numVertices = numTriangles * 3;

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		ASSERT((m_boundIndexBuffer->GetNumElements() - startVertex) >= numVertices);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		uint32_t indexOffset = startVertex * m_boundIndexBuffer->GetElementWidthInBytes();
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = ((int8_t*)m_boundIndexBuffer->GetBuffer() + indexOffset);
		else
			offset = ((int8_t*)NULL + indexOffset);

		GL_CALL(glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the given range using only the bound vertex buffer
		ASSERT((m_boundVertexBuffer->GetNumElements() - startVertex) >= numVertices);
		GL_CALL(glDrawArrays(GL_TRIANGLES, startVertex, numVertices));
	}
}

void GraphicsDevice::RenderLines(const IndexBuffer *buffer)
{
	STACK_TRACE;
	ASSERT(buffer != NULL);
	ASSERT(buffer->IsClientSideBuffer() == TRUE);
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundIndexBuffer == NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint32_t numVertices = buffer->GetNumElements();
	ASSERT(numVertices % 2 == 0);
	GL_CALL(glDrawElements(GL_LINES, numVertices, GL_UNSIGNED_SHORT, buffer->GetBuffer()));
}

void GraphicsDevice::RenderLines()
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		int numIndices = m_boundIndexBuffer->GetNumElements();
		ASSERT(numIndices % 2 == 0);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = m_boundIndexBuffer->GetBuffer();
		else
			offset = NULL;

		GL_CALL(glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the whole vertex buffer
		ASSERT(m_boundVertexBuffer->GetNumElements() % 2 == 0);
		GL_CALL(glDrawArrays(GL_LINES, 0, m_boundVertexBuffer->GetNumElements()));
	}
}

void GraphicsDevice::RenderLines(uint32_t startVertex, uint32_t numLines)
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint32_t numVertices = numLines * 2;

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		ASSERT((m_boundIndexBuffer->GetNumElements() - startVertex) >= numVertices);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		uint32_t indexOffset = startVertex * m_boundIndexBuffer->GetElementWidthInBytes();
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = ((int8_t*)m_boundIndexBuffer->GetBuffer() + indexOffset);
		else
			offset = ((int8_t*)NULL + indexOffset);

		GL_CALL(glDrawElements(GL_LINES, numVertices, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the given range using only the bound vertex buffer
		ASSERT((m_boundVertexBuffer->GetNumElements() - startVertex) >= numVertices);
		GL_CALL(glDrawArrays(GL_LINES, startVertex, numVertices));
	}
}

void GraphicsDevice::RenderPoints(const IndexBuffer *buffer)
{
	STACK_TRACE;
	ASSERT(buffer != NULL);
	ASSERT(buffer->IsClientSideBuffer() == TRUE);
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundIndexBuffer == NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint32_t numVertices = buffer->GetNumElements();
	GL_CALL(glDrawElements(GL_POINTS, numVertices, GL_UNSIGNED_SHORT, buffer->GetBuffer()));
}

void GraphicsDevice::RenderPoints()
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		int numIndices = m_boundIndexBuffer->GetNumElements();

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = m_boundIndexBuffer->GetBuffer();
		else
			offset = NULL;

		GL_CALL(glDrawElements(GL_POINTS, numIndices, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the whole vertex buffer
		GL_CALL(glDrawArrays(GL_POINTS, 0, m_boundVertexBuffer->GetNumElements()));
	}
}

void GraphicsDevice::RenderPoints(uint32_t startVertex, uint32_t numPoints)
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		ASSERT((m_boundIndexBuffer->GetNumElements() - startVertex) >= numPoints);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		uint32_t indexOffset = startVertex * m_boundIndexBuffer->GetElementWidthInBytes();
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = ((int8_t*)m_boundIndexBuffer->GetBuffer() + indexOffset);
		else
			offset = ((int8_t*)NULL + indexOffset);

		GL_CALL(glDrawElements(GL_POINTS, numPoints, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the given range using only the bound vertex buffer
		ASSERT((m_boundVertexBuffer->GetNumElements() - startVertex) >= numPoints);
		GL_CALL(glDrawArrays(GL_POINTS, startVertex, numPoints));
	}
}
