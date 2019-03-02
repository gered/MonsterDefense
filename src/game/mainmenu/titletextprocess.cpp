#include "../../framework/debug.h"

#include "titletextprocess.h"

#include "../../gameapp.h"
#include "../../contexts/contentcache.h"
#include "../../contexts/rendercontext.h"
#include "../../framework/graphics/color.h"
#include "../../framework/graphics/graphicsdevice.h"
#include "../../framework/graphics/spritebatch.h"
#include "../../framework/graphics/spritefont.h"
#include "../../states/gamestate.h"
#include <string.h>
#include <stl/string.h>

TitleTextProcess::TitleTextProcess(GameState *gameState, ProcessManager *processManager)
	: GameProcess(gameState, processManager)
{
	STACK_TRACE;
	m_versionText += "Built: " __DATE__ " @ " __TIME__;
	m_versionText += " / Config: ";
#ifdef DESKTOP
	m_versionText += "DESKTOP ";
#elif defined MOBILE
	m_versionText += "MOBILE ";
#endif
#ifdef DEBUG
	m_versionText += "DEBUG ";
#else
	m_versionText += "RELEASE ";
#endif
#ifdef DEBUG_STACK_TRACE
	m_versionText += "DEBUG_STACK_TRACE ";
#endif
#ifdef DEBUG_ASSERT_BREAK
	m_versionText += "DEBUG_ASSERT_BREAK ";
#endif
}

TitleTextProcess::~TitleTextProcess()
{
	STACK_TRACE;
}

void TitleTextProcess::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	SpriteFont *titleFont = GetGameApp()->GetContentCache()->GetTitleFont();
	SpriteFont *smallFont = GetGameApp()->GetContentCache()->GetSmallFont();
	SpriteFont *versionFont = GetGameApp()->GetContentCache()->GetUIFont();
	const float TITLEFONT_SCALE = 2.8f * renderContext->GetScreenScale();
	const float SMALLFONT_SCALE = renderContext->GetScreenScale();
	const float VERSIONFONT_SCALE = 1.0f;
	uint16_t smallFontLineHeight = smallFont->GetLetterHeight() * SMALLFONT_SCALE;
	uint16_t versionFontLineHeight = versionFont->GetLetterHeight() * VERSIONFONT_SCALE;

	uint16_t titleX = 10 * renderContext->GetScreenScale();
	uint16_t titleY = 10 * renderContext->GetScreenScale();
	
	renderContext->GetSpriteBatch()->Render(titleFont, titleX + TITLEFONT_SCALE, titleY + TITLEFONT_SCALE, COLOR_BLACK, TITLEFONT_SCALE, "MONSTER\nDEFENSE");
	renderContext->GetSpriteBatch()->Render(titleFont, titleX, titleY, COLOR_WHITE, TITLEFONT_SCALE, "MONSTER\nDEFENSE");
	
	uint16_t footerX = 5 * renderContext->GetScreenScale();
	uint16_t footerY = renderContext->GetGraphicsDevice()->GetViewportHeight() - smallFontLineHeight - versionFontLineHeight - (5 * renderContext->GetScreenScale());

	renderContext->GetSpriteBatch()->Render(smallFont, footerX, footerY, COLOR_WHITE, SMALLFONT_SCALE, "By: Gered King <gered@blarg.ca>  2012");

	uint16_t versionWidth;
	uint16_t versionHeight;
	versionFont->MeasureString(&versionWidth, &versionHeight, VERSIONFONT_SCALE, m_versionText.c_str());

	uint16_t versionX = 5 * renderContext->GetScreenScale();
	uint16_t versionY = renderContext->GetGraphicsDevice()->GetViewportHeight() - versionHeight - (5 * renderContext->GetScreenScale());
	renderContext->GetSpriteBatch()->Render(versionFont, versionX, versionY, Color(0.5f, 0.5f, 0.5f, 1.0f), VERSIONFONT_SCALE, m_versionText.c_str());
}
