#ifndef __CONTEXTS_CONTENTCACHE_H_INCLUDED__
#define __CONTEXTS_CONTENTCACHE_H_INCLUDED__

#include "../framework/common.h"

class ContentManager;
class SpriteFont;
class Texture;
class TextureAtlas;

#include <stl/string.h>

class ContentCache
{
public:
	ContentCache(ContentManager *contentManager);
	virtual ~ContentCache();

	void OnLoadGame();

	Texture* GetUISkin() const                             { return m_uiSkin; }
	const stl::string& GetUISkinFilename() const           { return m_uiSkinFilename; }
	Texture* GetGamePadButtons() const                     { return m_gamePadButtons; }
	SpriteFont* GetTitleFont() const                       { return m_titleFont; }
	SpriteFont* GetFont() const                            { return m_standardFont; }
	SpriteFont* GetSmallFont() const                       { return m_smallFont; }
	SpriteFont* GetUIFont() const                          { return m_uiFont; }
	const stl::string& GetUIFontFilename() const           { return m_uiFontFilename; }
	uint8_t GetUIFontSize() const                          { return m_uiFontSize; }

	TextureAtlas* GetEnvironment() const                   { return m_environment; }
	TextureAtlas* GetParticles() const                     { return m_particles; }
	TextureAtlas* GetItems() const                         { return m_items; }
	Texture* GetShadow() const                             { return m_shadow; }

private:
	ContentManager *m_contentManager;

	Texture *m_uiSkin;
	stl::string m_uiSkinFilename;
	Texture *m_gamePadButtons;
	SpriteFont *m_titleFont;
	SpriteFont *m_standardFont;
	SpriteFont *m_smallFont;
	SpriteFont *m_uiFont;
	stl::string m_uiFontFilename;
	uint8_t m_uiFontSize;

	Texture *m_environmentTexture;
	TextureAtlas *m_environment;
	Texture *m_particlesTexture;
	TextureAtlas *m_particles;
	Texture *m_itemsTexture;
	TextureAtlas *m_items;
	Texture *m_shadow;
};

#endif

