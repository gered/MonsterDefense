#ifndef __FRAMEWORK_CONTENT_TEXTURELOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_TEXTURELOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloader.h"
#include "../graphics/texture.h"
#include <stl/map.h>
#include <stl/string.h>

class ContentManager;

typedef stl::map<stl::string, Texture*> TextureMap;

/**
 * Content loader for textures.
 */
class TextureLoader : public ContentLoader<Texture>
{
public:
	/**
	 * Creates a texture loader.
	 * @param contentManager the parent content manager
	 */
	TextureLoader(ContentManager *contentManager);

	/**
	 * Creates a texture loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with texture filenames
	 */
	TextureLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~TextureLoader();

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
	 * with texture filenames.
	 * @param path the default path to use when loading if needed
	 */
	void SetDefaultPath(const stl::string &path);

	/**
	 * Retrieves the specified texture either the cache or from the
	 * platform's backing storage.
	 * @param name the path and filename of the texture to retrieve
	 * @param params unused, should be NULL
	 * @return the loaded texture or NULL on failure
	 */
	Texture* Get(const stl::string &name, const ContentParam *params);

	/**
	 * Frees the specified texture. Textures are only actually freed
	 * when they have no more references to them.
	 * @param content the texture to be freed
	 */
	void Free(Texture *content);

	/**
	 * Frees the specified texture if the texture pointed to by the given path
	 * and filename has been loaded. Textures are only actually freed when
	 * they have no more references to them.
	 * @param name the path and filename of the texture to free
	 * @param params unused, should be NULL
	 */
	void Free(const stl::string &name, const ContentParam *params);

	/**
	 * Returns the path and filename that the given texture was loaded from.
	 * @param content the texture to get the path and filename for
	 * @return the path and filename that the texture was loaded from
	 */
	stl::string GetNameOf(Texture *content) const;

private:
	stl::string AddDefaultPathIfNeeded(const stl::string &textureFile);
	Texture* Load(const stl::string &file);
	void Free(TextureMap::iterator &itor, BOOL force);

	stl::string m_defaultPath;
	TextureMap m_textures;
};

#endif
