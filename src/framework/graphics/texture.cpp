#include "../debug.h"

#include "texture.h"

Texture::Texture(uint32_t textureName, uint16_t width, uint16_t height, IMAGE_FORMAT format)
	: Content()
{
	STACK_TRACE;
	ASSERT(textureName != 0);
	m_textureName = textureName;
	m_width = width;
	m_height = height;
	m_format = format;
	m_invalidated = FALSE;
}

Texture::~Texture()
{
	STACK_TRACE;
}

void Texture::Reload(uint32_t textureName)
{
	ASSERT(m_invalidated == TRUE);
	ASSERT(textureName != 0);
	m_textureName = textureName;
	m_invalidated = FALSE;
}

