#include "../debug.h"
#include "../log.h"

#include "spritefontloader.h"
#include "contentmanager.h"
#include "spritefontparam.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../file/filesystem.h"
#include "../file/memoryfile.h"
#include "../graphics/color.h"
#include "../graphics/customtextureatlas.h"
#include "../graphics/graphicsdevice.h"
#include "../graphics/image.h"
#include "../graphics/spritefont.h"
#include "../graphics/texture.h"
#include "../math/rect.h"
#include "../math/rectf.h"
#include "../math/mathhelpers.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

SpriteFontLoader::SpriteFontLoader(ContentManager *contentManager)
	: ContentLoader<SpriteFont>(contentManager)
{
	STACK_TRACE;
	SetDefaultPath("assets://fonts/");
}

SpriteFontLoader::SpriteFontLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoader<SpriteFont>(contentManager)
{
	STACK_TRACE;
	SetDefaultPath(defaultPath);
}

SpriteFontLoader::~SpriteFontLoader()
{
	STACK_TRACE;
	while (m_spriteFonts.size() > 0)
	{
		SpriteFontList::iterator itor = m_spriteFonts.begin();
		LOG_WARN(LOGCAT_ASSETS, "SpriteFontLoader: \"%s:%d\" not explicitly freed.\n", (*itor)->GetFontFile().c_str(), (*itor)->GetSize());
		Free(itor, TRUE);
	}

	m_spriteFonts.clear();
}

void SpriteFontLoader::OnNewContext()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "SpriteFontLoader: reloading previous fonts for new OpenGL context.\n");

	SpriteFontList::iterator itor;
	for (itor = m_spriteFonts.begin(); itor != m_spriteFonts.end(); ++itor)
	{
		SpriteFont *font = *itor;
		if (font->GetTexture() == NULL)
		{
			File *fontFile = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->Open(font->GetFontFile(), FILEMODE_READ | FILEMODE_BINARY | FILEMODE_MEMORY);
			ASSERT(fontFile != NULL);

			Load(fontFile, font->GetSize(), font);

			SAFE_DELETE(fontFile);
		}
	}
}

void SpriteFontLoader::OnLostContext()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "SpriteFontLoader: invoking lost OpenGL context event for all loaded fonts.\n");

	SpriteFontList::iterator itor;
	for (itor = m_spriteFonts.begin(); itor != m_spriteFonts.end(); ++itor)
	{
		SpriteFont *font = *itor;
		font->OnLostContext();
	}
}

void SpriteFontLoader::SetDefaultPath(const stl::string &path)
{
	STACK_TRACE;
	if (path.length() == 0)
	{
		m_defaultPath = "";
		return;
	}

	m_defaultPath = path;

	if (m_defaultPath[m_defaultPath.length() - 1] != '/')
		m_defaultPath += '/';

	LOG_INFO(LOGCAT_ASSETS, "SpriteFontLoader: default path set to \"%s\"\n", m_defaultPath.c_str());
}

SpriteFont* SpriteFontLoader::Get(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	ASSERT(params != NULL);

	const SpriteFontParam *spriteFontParams = (const SpriteFontParam*)params;
	ASSERT(spriteFontParams->size != 0);
	uint8_t size = spriteFontParams->size;

	SpriteFont *font;

	stl::string filename = AddDefaultPathIfNeeded(name);

	SpriteFontList::iterator itor = Find(filename, size);
	if (itor != m_spriteFonts.end())
		font = *itor;
	else
	{
		File *fontFile = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->Open(filename, FILEMODE_READ | FILEMODE_BINARY | FILEMODE_MEMORY);
		ASSERT(fontFile != NULL);

		font = Load(fontFile, size, NULL);

		SAFE_DELETE(fontFile);

		if (font != NULL)
			m_spriteFonts.push_back(font);
	}

	font->Reference();

	return font;
}

void SpriteFontLoader::Free(SpriteFont *content)
{
	STACK_TRACE;
	ASSERT(content != NULL);
	ASSERT(content->IsReferenceCounted() == TRUE);

	for (SpriteFontList::iterator itor = m_spriteFonts.begin(); itor != m_spriteFonts.end(); ++itor)
	{
		if (*itor == content)
		{
			Free(itor, FALSE);
			break;
		}
	}
}

void SpriteFontLoader::Free(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	ASSERT(params != NULL);

	const SpriteFontParam *spriteFontParams = (const SpriteFontParam*)params;
	ASSERT(spriteFontParams->size != 0);
	uint8_t size = spriteFontParams->size;

	stl::string filename = AddDefaultPathIfNeeded(name);

	SpriteFontList::iterator itor = Find(filename, size);
	if (itor != m_spriteFonts.end())
		Free(itor, FALSE);
}

stl::string SpriteFontLoader::GetNameOf(SpriteFont *content) const
{
	STACK_TRACE;
	ASSERT(!"Unimplemented");
	return "";
}

stl::string SpriteFontLoader::AddDefaultPathIfNeeded(const stl::string &fontFile)
{
	STACK_TRACE;
	if (fontFile[0] == '/' || m_defaultPath.length() == 0 || fontFile.substr(0, 9) == "assets://")
		return fontFile;
	else
		return m_defaultPath + fontFile;
}

SpriteFont* SpriteFontLoader::Load(File *file, uint8_t size, SpriteFont *existing)
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "SpriteFontLoader: loading \"%s:%d\"\n", file->GetFilename().c_str(), size);

	// TODO: somehow find a way to base this on the font size requested, as this
	//       could be really wasteful for tiny fonts
	const int FONT_BITMAP_WIDTH = 1024;
	const int FONT_BITMAP_HEIGHT = 1024;
	const int NUM_GLYPHS = HIGH_GLYPH - LOW_GLYPH;

	// using a MemoryFile means we don't need to completely load the file data into a temporary data store
	// because opening a MemoryFile has already done this, directly from the source file
	ASSERT(file->GetFileType() == FILETYPE_MEMORY);
	MemoryFile *memoryFile = (MemoryFile*)file;

	stbtt_fontinfo fontinfo;
	const uint8_t* fileData = (uint8_t*)memoryFile->GetFileData();
	int initResult = stbtt_InitFont(&fontinfo, fileData, 0);
	if (!initResult)
		return NULL;

	// get glyph metrics for the "maximum size" glyph as an indicator of the
	// general size of each glyph in this font. Uppercase 'W' seems to be a 
	// pretty good glyph to represent this "maximum size".
	SpriteFontGlyphMetrics maxMetrics;
	BOOL maxMetricResult = GetGlyphMetrics(&fontinfo, 'W', size, &maxMetrics);
	ASSERT(maxMetricResult == TRUE);

	// if we can't even get this glyph's metrics then stop now
	if (!maxMetricResult)
		return NULL;

	// the raw bitmap that we will be building. using a fixed size which should hopefully be good enough for the limited charset
	// that we're allowing as long as the font pixel size doesn't get too large
	// this bitmap is *only* going to contain alpha values (not any kind of rgb colour data)
	Image *bitmap = Image::Create(FONT_BITMAP_WIDTH, FONT_BITMAP_HEIGHT, IMAGE_FORMAT_ALPHA);
	ASSERT(bitmap != NULL);
	bitmap->Clear();

	// the texture atlas to store the position/texcoords of each glyph
	CustomTextureAtlas *glyphs = new CustomTextureAtlas(bitmap->GetWidth(), bitmap->GetHeight());
	ASSERT(glyphs != NULL);

	// NOTE TO SELF: a lot of this feels "hackish" and that some weird font isn't going to play nice with this. clean this up at some point!

	// current position to draw the glyph to on the bitmap
	int x = 0;
	int y = 0;

	// total line height for each row of glyphs on the bitmap. this is not really the font height.
	// it is likely slightly larger then that. this adds the font's descent a second time to make
	// vertical room for a few of the glyphs (e.g. '{}' or '()', or even '$') which go off the
	// top/bottom by a slight bit
	int lineHeight = (maxMetrics.ascent - maxMetrics.descent) + maxMetrics.lineGap - maxMetrics.descent;

	for (int i = 0; i < NUM_GLYPHS; ++i)
	{
		// get metrics for the current glyph
		char c = (char)(i + LOW_GLYPH);
		SpriteFontGlyphMetrics glyphMetrics;
		BOOL metricResult = GetGlyphMetrics(&fontinfo, c, size, &glyphMetrics);
		ASSERT(metricResult == TRUE);

		// adjust each glyph's rect so that it has it's own space that doesn't
		// collide with any of it's neighbour glyphs (neighbour as seen on the 
		// font bitmap)
		if (glyphMetrics.dimensions.left < 0)
		{
			// bump the glyph over to the right by the same amount it was over to the left
			glyphMetrics.advance += -glyphMetrics.dimensions.left;
			glyphMetrics.dimensions.left = 0;
		}

		// do we need to move to the next row?
		if ((x + glyphMetrics.advance + glyphMetrics.dimensions.left) >= bitmap->GetWidth())
		{
			// yes
			x = 0;
			y += lineHeight;
			ASSERT((y + lineHeight) < bitmap->GetHeight());
		}

		// the destination bitmap pixel coordinates of this glyph. these are the
		// pixel coordinates that will be stored in the font's texture atlas
		// which will be used by the texture atlas to build texture coords
		Rect position(
			x, 
			y, 
			x + glyphMetrics.advance, 
			y + lineHeight
		);

		// top-left coords and dimensions to have stb_truetype draw the glyph at in the font bitmap
		int drawX = position.left + glyphMetrics.dimensions.left;
		int drawY = (position.bottom + glyphMetrics.descent) + glyphMetrics.descent - glyphMetrics.dimensions.bottom + maxMetrics.lineGap;
		int drawWidth = position.GetWidth();
		int drawHeight = position.GetHeight();

		// use stb_truetype to render the glyph onto the bitmap
		int pixelsDestOffset = drawX + (drawY * bitmap->GetWidth());
		stbtt_MakeGlyphBitmap(&fontinfo, bitmap->GetPixels() + pixelsDestOffset, drawWidth, drawHeight, bitmap->GetWidth(), maxMetrics.scale, maxMetrics.scale, glyphMetrics.index);

		// add the glyph position to the texture atlas (which will calc the texture coords for us)
		int newIndex = glyphs->Add(position);
		ASSERT(newIndex == ((int)c - LOW_GLYPH));

		// move to the next glyph's position in the bitmap
		x += maxMetrics.advance;
	}

	Texture *fontTexture = GetContentManager()->GetGameApp()->GetGraphicsDevice()->CreateTextureFromImage(bitmap);
	ASSERT(fontTexture != NULL);
	SAFE_DELETE(bitmap);
	if (fontTexture == NULL)
	{
		// clean up
		SAFE_DELETE(glyphs);
		return NULL;
	}

	// re-loading an existing font object instance, or creating a new one?
	SpriteFont *result;
	if (existing != NULL)
		result = existing;
	else
	{
		result = new SpriteFont();
		ASSERT(result != NULL);
	}
	result->Load(fontTexture, glyphs, file->GetFilename(), size);
	return result;
}

void SpriteFontLoader::Free(SpriteFontList::iterator &itor, BOOL force)
{
	STACK_TRACE;
	SpriteFont *font = *itor;
	font->Release();

	if (font->IsReferenced() && force)
	{
		// if this is being forced, we need to manually decrease the Content's
		// internal reference counter (or the destructor will have a fit).
		// I suppose this is probably kind of "hack-ish"...
		uint32_t numToRelease = font->GetNumReferences();
		for (uint32_t i = 0; i < numToRelease; ++i)
			font->Release();
	}

	if (!font->IsReferenced())
	{
		LOG_INFO(LOGCAT_ASSETS, "SpriteFontLoader: %sfreed \"%s:%d\"\n", (force ? "forcefully " : ""), font->GetFontFile().c_str(), font->GetSize());
		SAFE_DELETE(font);
		m_spriteFonts.erase(itor);
	}
}

SpriteFontList::iterator SpriteFontLoader::Find(const stl::string &file, uint8_t size)
{
	STACK_TRACE;
	stl::string fullFilename = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->TranslateFilePath(file);

	SpriteFontList::iterator itor;
	for (itor = m_spriteFonts.begin(); itor != m_spriteFonts.end(); ++itor)
	{
		SpriteFont *font = *itor;
		if (font->GetFontFile() == fullFilename && font->GetSize() == size)
			break;
	}

	return itor;
}

BOOL SpriteFontLoader::GetGlyphMetrics(stbtt_fontinfo *fontInfo, char glyph, uint8_t size, SpriteFontGlyphMetrics *metrics)
{
	STACK_TRACE;
	ASSERT(metrics != NULL);

	// need to properly scale! sizes as returned from most (all?) metric-related functions will be huge
	// (I will at this point admit that I don't fully understand what units the metric functions are working with
	// and why this scale value is necessary... I just haven't read enough into this stuff I guess)
	metrics->scale = stbtt_ScaleForPixelHeight(fontInfo, (float)size);

	metrics->letterHeight = size;

	metrics->index = stbtt_FindGlyphIndex(fontInfo, glyph);
	if (metrics->index == 0)
		return FALSE;

	stbtt_GetGlyphBox(fontInfo, metrics->index, &metrics->dimensions.left, &metrics->dimensions.top, &metrics->dimensions.right, &metrics->dimensions.bottom);
	stbtt_GetFontVMetrics(fontInfo, &metrics->ascent, &metrics->descent, &metrics->lineGap);
	stbtt_GetGlyphHMetrics(fontInfo, metrics->index, &metrics->advance, NULL);

	// adjust all the metrics we got by the font size scale value
	// (I guess this puts them from whatever units they were in to pixel units)
	metrics->dimensions.left = (int)ceil(metrics->dimensions.left * metrics->scale);
	metrics->dimensions.top = (int)ceil(metrics->dimensions.top * metrics->scale);
	metrics->dimensions.right = (int)ceil(metrics->dimensions.right * metrics->scale);
	metrics->dimensions.bottom = (int)ceil(metrics->dimensions.bottom * metrics->scale);
	metrics->ascent = (int)ceil(metrics->ascent * metrics->scale);
	metrics->descent = (int)ceil(metrics->descent * metrics->scale);
	metrics->lineGap = (int)ceil(metrics->lineGap * metrics->scale);
	metrics->advance = (int)ceil(metrics->advance * metrics->scale);
	metrics->letterWidth = metrics->dimensions.right - metrics->dimensions.left;

	// seen some pixel/bitmap fonts that have the total ascent/descent calculated height
	// greater then the pixel height. this just figures out this difference, if present,
	// and sets an appropriate line gap equal to it (in these cases, linegap was 0)
	int calculatedHeight = metrics->ascent - metrics->descent;
	int heightDifference = abs(calculatedHeight - metrics->letterHeight);
	if (heightDifference != metrics->lineGap && metrics->lineGap == 0)
		metrics->lineGap = heightDifference;

	return TRUE;
}
