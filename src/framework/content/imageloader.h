#ifndef __FRAMEWORK_CONTENT_IMAGELOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_IMAGELOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloader.h"
#include "../graphics/image.h"
#include <stl/map.h>
#include <stl/string.h>

class ContentManager;

typedef stl::map<stl::string, Image*> ImageMap;

/**
 * Content loader for images.
 */
class ImageLoader : public ContentLoader<Image>
{
public:
	/**
	 * Creates a image loader.
	 * @param contentManager the parent content manager
	 */
	ImageLoader(ContentManager *contentManager);

	/**
	 * Creates a image loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with image filenames
	 */
	ImageLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~ImageLoader();

	/**
	 * Sets the default path to use if, when loading, paths aren't provided
	 * with image filenames.
	 * @param path the default path to use when loading if needed
	 */
	void SetDefaultPath(const stl::string &path);

	/**
	 * Retrieves the specified image either the cache or from the
	 * platform's backing storage.
	 * @param name the path and filename of the image to retrieve
	 * @param params unused, should be NULL
	 * @return the loaded image or NULL on failure
	 */
	Image* Get(const stl::string &name, const ContentParam *params);

	/**
	 * Frees the specified image. Images are only actually freed
	 * when they have no more references to them.
	 * @param content the image to be freed
	 */
	void Free(Image *content);

	/**
	 * Frees the specified image if the image pointed to by the given path
	 * and filename has been loaded. Images are only actually freed when
	 * they have no more references to them.
	 * @param name the path and filename of the image to free
	 * @param params unused, should be NULL
	 */
	void Free(const stl::string &name, const ContentParam *params);

	/**
	 * Returns the path and filename that the given image was loaded from.
	 * @param content the image to get the path and filename for
	 * @return the path and filename that the image was loaded from
	 */
	stl::string GetNameOf(Image *content) const;

private:
	stl::string AddDefaultPathIfNeeded(const stl::string &imageFile);
	Image* Load(const stl::string &file);
	void Free(ImageMap::iterator &itor, BOOL force);

	stl::string m_defaultPath;
	ImageMap m_images;
};

#endif

