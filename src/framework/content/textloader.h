#ifndef __FRAMEWORK_CONTENT_TEXTLOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_TEXTLOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloader.h"
#include "../support/text.h"
#include <stl/map.h>
#include <stl/string.h>

class ContentManager;

typedef stl::map<stl::string, Text*> TextMap;

/**
 * Content loader for text files.
 */
class TextLoader : public ContentLoader<Text>
{
public:
	/**
	 * Creates a text file loader.
	 * @param contentManager the parent content manager
	 */
	TextLoader(ContentManager *contentManager);

	/**
	 * Creates a text file loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with text filenames
	 */
	TextLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~TextLoader();

	/**
	 * Sets the default path to use if, when loading, paths aren't provided
	 * with text filenames.
	 * @param path the default path to use when loading if needed
	 */
	void SetDefaultPath(const stl::string &path);

	/**
	 * Retrieves the specified image either the cache or from the
	 * platform's backing storage.
	 * @param name the path and filename of the text file to retrieve
	 * @param params unused, should be NULL
	 * @return the loaded text file or NULL on failure
	 */
	Text* Get(const stl::string &name, const ContentParam *params);

	/**
	 * Frees the specified text file. Text files are only actually freed
	 * when they have no more references to them.
	 * @param content the text file to be freed
	 */
	void Free(Text *content);

	/**
	 * Frees the specified text file if the text file pointed to by the 
	 * given path and filename has been loaded. Text files are only 
	 * actually freed when they have no more references to them.
	 * @param name the path and filename of the text file to free
	 * @param params unused, should be NULL
	 */
	void Free(const stl::string &name, const ContentParam *params);

	/**
	 * Returns the path and filename that the given text file was loaded from.
	 * @param content the text file to get the path and filename for
	 * @return the path and filename that the text file was loaded from
	 */
	stl::string GetNameOf(Text *content) const;

private:
	stl::string AddDefaultPathIfNeeded(const stl::string &textFile);
	Text* Load(const stl::string &file);
	void Free(TextMap::iterator &itor, BOOL force);

	stl::string m_defaultPath;
	TextMap m_textFiles;
};

#endif

