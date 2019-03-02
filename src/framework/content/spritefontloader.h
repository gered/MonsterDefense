#ifndef __FRAMEWORK_CONTENT_SPRITEFONTLOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_SPRITEFONTLOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloader.h"
#include "../graphics/spritefont.h"
#include "../math/rect.h"
#include <stb_truetype.h>
#include <stl/list.h>
#include <stl/string.h>

class ContentManager;
class File;

typedef stl::list<SpriteFont*> SpriteFontList;

struct SpriteFontGlyphMetrics
{
	int index;
	float scale;
	Rect dimensions;
	int ascent;
	int descent;
	int lineGap;
	int advance;
	int letterWidth;
	int letterHeight;

	SpriteFontGlyphMetrics()
	{
		index = 0;
		scale = 0.0f;
		dimensions = Rect(0, 0, 0, 0);
		ascent = 0;
		descent = 0;
		lineGap = 0;
		advance = 0;
		letterWidth = 0;
		letterHeight = 0;
	}
};

/**
 * Content loader for sprite fonts.
 */
class SpriteFontLoader : public ContentLoader<SpriteFont>
{
public:
	/**
	 * Creates a sprite font loader.
	 * @param contentManager the parent content manager
	 */
	SpriteFontLoader(ContentManager *contentManager);

	/**
	 * Creates a sprite font loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with font filenames
	 */
	SpriteFontLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~SpriteFontLoader();

	/**
	 * New OpenGL graphics context creation callback.
	 */
	void OnNewContext();

	/**
	 * Lost OpenGL graphics context callback.
	 */
	void OnLostContext();

	/**
	 * Sets the default path to use if, when loading, paths aren't provided
	 * with font filenames.
	 * @param path the default path to use when loading if needed
	 */
	void SetDefaultPath(const stl::string &path);

	/**
	 * Retrieves the specified sprite font either the cache or from the
	 * platform's backing storage.
	 * @param name the path and filename of the sprite font to retrieve
	 * @param params specifies font parameters, like size
	 * @return the loaded sprite font or NULL on failure
	 */
	SpriteFont* Get(const stl::string &name, const ContentParam *params);

	/**
	 * Frees the specified sprite font. Fonts are only actually freed
	 * when they have no more references to them.
	 * @param content the sprite font to be freed
	 */
	void Free(SpriteFont *content);

	/**
	 * Frees the specified sprite font if the sprite font pointed to by the
	 * given path and filename has been loaded. Fonts are only actually 
	 * freed when they have no more references to them.
	 * @param name the path and filename of the sprite font to free
	 * @param params font parameters that further describe the
	 *               exact font object to be freed
	 */
	void Free(const stl::string &name, const ContentParam *params);

	/**
	 * Returns the path and filename that the given sprite font was loaded from.
	 * @param content the sprite font to get the path and filename for
	 * @return the path and filename that the sprite font was loaded from
	 */
	stl::string GetNameOf(SpriteFont *content) const;

private:
	stl::string AddDefaultPathIfNeeded(const stl::string &fontFile);
	SpriteFont* Load(File *file, uint8_t size, SpriteFont *existing);
	void Free(SpriteFontList::iterator &itor, BOOL force);
	SpriteFontList::iterator Find(const stl::string &file, uint8_t size);

	BOOL GetGlyphMetrics(stbtt_fontinfo *fontInfo, char glyph, uint8_t size, SpriteFontGlyphMetrics *glyphMetrics);

	stl::string m_defaultPath;
	SpriteFontList m_spriteFonts;
};

#endif
