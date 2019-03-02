#ifndef __FRAMEWORK_GRAPHICS_TEXTURE_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_TEXTURE_H_INCLUDED__

#include "../common.h"
#include "../content/content.h"
#include "imageformats.h"

/**
 * Represents a texture that can be bound and rendered with.
 */
class Texture : public Content
{
public:
	static CONTENT_TYPE GetType()
	{
		static CONTENT_TYPE typeName = "Texture";
		return typeName;
	}
	CONTENT_TYPE GetTypeOf() const                         { return GetType(); }

	/**
	 * Creates a new texture with the specified properties. This
	 * is not intended to be used directly by anything outside the
	 * framework classes.
	 * @param textureName a texture name or ID assigned by OpenGL
	 * @param width the width of the texture in pixels
	 * @param height the height of the texture in pixels
	 * @param format the format of the image the texture was created from
	 */
	Texture(uint32_t textureName, uint16_t width, uint16_t height, IMAGE_FORMAT format);

	virtual ~Texture();

	/**
	 * @return the texture name or ID assigned to this texture by OpenGL
	 */
	uint32_t GetTextureName() const                        { return m_textureName; }

	/**
	 * @return the width of this texture in pixels
	 */
	uint16_t GetWidth() const                              { return m_width; }

	/**
	 * @return the height of this texture in pixels
	 */
	uint16_t GetHeight() const                             { return m_height; }

	/**
	 * @return the format of the image this texture was created from
	 */
	IMAGE_FORMAT GetFormat() const                         { return m_format; }

	/**
	 * @return TRUE if the texture has been marked as invalid and needs to be recreated
	 */
	BOOL IsInvalidated() const                             { return m_invalidated; }

	/**
	 * Marks this texture as invalid and that it should be recreated. This is 
	 * not intended to be used directly by anything outside the framework
	 * classes.
	 */
	void Invalidate()                                      { m_invalidated = TRUE; }

	/**
	 * Resets the texture name / ID assigned to this texture by OpenGL. This
	 * is not intended to be used directly by anything outside the framework
	 * classes.
	 */
	void Reload(uint32_t textureName);

private:
	uint32_t m_textureName;
	uint16_t m_width;
	uint16_t m_height;
	IMAGE_FORMAT m_format;
	BOOL m_invalidated;
};

#endif
