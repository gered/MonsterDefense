#include "../debug.h"
#include "../log.h"
#include "../common.h"

#include "solidcolortexturecache.h"
#include "color.h"
#include "graphicsdevice.h"
#include "image.h"
#include "texture.h"
#include <stl/map.h>

SolidColorTextureCache::SolidColorTextureCache(GraphicsDevice *graphicsDevice)
{
	STACK_TRACE;
	m_graphicsDevice = graphicsDevice;
}

SolidColorTextureCache::~SolidColorTextureCache()
{
	STACK_TRACE;
	FreeAll();
}

void SolidColorTextureCache::OnNewContext()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "SolidColorTextureCache: regenerating previous textures for new OpenGL context.\n");

	for (ColorTextureMap::iterator itor = m_textures.begin(); itor != m_textures.end(); ++itor)
	{
		Color color = Color::FromInt(itor->first);
		Texture *temp = CreateFor(color);
		if (temp != NULL)
		{
			itor->second->Reload(temp->GetTextureName());
			SAFE_DELETE(temp);
		}
	}
}

void SolidColorTextureCache::OnLostContext()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "SolidColorTextureCache: resetting generated texture IDs due to lost OpenGL context.\n");

	for (ColorTextureMap::iterator itor = m_textures.begin(); itor != m_textures.end(); ++itor)
		itor->second->Invalidate();
}

Texture* SolidColorTextureCache::Get(const Color &color)
{
	STACK_TRACE;
	Texture *texture;
	uint32_t colorInt = color.ToInt();

	ColorTextureMap::iterator itor = m_textures.find(colorInt);
	if (itor != m_textures.end())
		texture = itor->second;
	else
	{
		texture = CreateFor(color);
		if (texture != NULL)
			m_textures[colorInt] = texture;
	}

	return texture;
}

Texture* SolidColorTextureCache::CreateFor(const Color &color)
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "SolidColorTextureCache: creating texture for color 0x%8x.\n", color.ToInt());

	Image *img = Image::Create(8, 8, IMAGE_FORMAT_RGBA);
	ASSERT(img != NULL);

	img->Clear(color);
	Texture *texture = m_graphicsDevice->CreateTextureFromImage(img);
	ASSERT(texture != NULL);

	SAFE_DELETE(img);

	return texture;
}

void SolidColorTextureCache::FreeAll()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "SolidColorTextureCache: freeing all generated textures.\n");
	for (ColorTextureMap::iterator itor = m_textures.begin(); itor != m_textures.end(); ++itor)
		m_graphicsDevice->FreeTexture(itor->second);

	m_textures.clear();
}

