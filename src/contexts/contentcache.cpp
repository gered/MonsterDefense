#include "../framework/common.h"
#include "../framework/debug.h"

#include "contentcache.h"

#include "../framework/content/contentmanager.h"
#include "../framework/content/spritefontparam.h"
#include "../framework/graphics/gridtextureatlas.h"
#include "../framework/graphics/spritefont.h"
#include "../framework/graphics/texture.h"
#include "../framework/graphics/textureatlas.h"

ContentCache::ContentCache(ContentManager *contentManager)
{
	STACK_TRACE;
	m_contentManager = contentManager;
	m_uiFontSize = 0;
}

ContentCache::~ContentCache()
{
	STACK_TRACE;
	m_contentManager->Free<Texture>(m_uiSkin);
	m_contentManager->Free<Texture>(m_gamePadButtons);
	m_contentManager->Free<SpriteFont>(m_titleFont);
	m_contentManager->Free<SpriteFont>(m_standardFont);
	m_contentManager->Free<SpriteFont>(m_smallFont);
	m_contentManager->Free<SpriteFont>(m_uiFont);
	SAFE_DELETE(m_environment);
	m_contentManager->Free<Texture>(m_environmentTexture);
	SAFE_DELETE(m_particles);
	m_contentManager->Free<Texture>(m_particlesTexture);
	SAFE_DELETE(m_items);
	m_contentManager->Free<Texture>(m_itemsTexture);
	m_contentManager->Free<Texture>(m_shadow);
}

void ContentCache::OnLoadGame()
{
	STACK_TRACE;
	m_uiSkinFilename = "assets://ui_skin.png";
	m_uiSkin = m_contentManager->Get<Texture>(m_uiSkinFilename);
	m_gamePadButtons = m_contentManager->Get<Texture>("assets://gamepad.png");
	m_titleFont = m_contentManager->Get<SpriteFont>("assets://fonts/AddLGBitmap09.ttf", SpriteFontParam(9));
	m_standardFont = m_contentManager->Get<SpriteFont>("assets://fonts/kongtext.ttf", SpriteFontParam(16));
	m_smallFont = m_contentManager->Get<SpriteFont>("assets://fonts/uni05_53.ttf", SpriteFontParam(9));
	m_uiFontFilename = "assets://fonts/dlxfont.ttf";
	m_uiFontSize = 8;
	m_uiFont = m_contentManager->Get<SpriteFont>(m_uiFontFilename, SpriteFontParam(m_uiFontSize));

	m_environmentTexture = m_contentManager->Get<Texture>("assets://environment.png");
	m_environment = new GridTextureAtlas(m_environmentTexture, 16, 16);
	m_particlesTexture = m_contentManager->Get<Texture>("assets://particles.png");
	m_particles = new GridTextureAtlas(m_particlesTexture, 8, 8);
	m_itemsTexture = m_contentManager->Get<Texture>("assets://items.png");
	m_items = new GridTextureAtlas(m_itemsTexture, 16, 16);

	m_shadow = m_contentManager->Get<Texture>("assets://shadow.png");
}

