#ifndef __FRAMEWORK_CONTENT_CONTENTMANAGER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_CONTENTMANAGER_H_INCLUDED__

#include "../debug.h"
#include "content.h"
#include "contentparam.h"
#include "contentloader.h"
#include <stl/map.h>
#include <stl/string.h>

class BaseGameApp;

typedef stl::map<CONTENT_TYPE, ContentLoaderBase*> ContentLoaderMap;

/**
 * Serves as the central point for accessing content provided by a number
 * of registered content loaders for each of the individual content types.
 */
class ContentManager
{
public:
	/**
	 * Creates a content manager object.
	 * @param gameApp parent game application object
	 */
	ContentManager(BaseGameApp *gameApp);

	virtual ~ContentManager();

	/**
	 * Initial game load after initialization success callback.
	 */
	void OnLoadGame();

	/**
	 * New OpenGL graphics context creation callback.
	 */
	void OnNewContext();

	/**
	 * Lost OpenGL graphics context callback.
	 */
	void OnLostContext();

	/**
	 * Registers a content loader that will handle loading and freeing of
	 * content objects that match a specific content type.
	 * @param loader the pre-initialized content loader object to register
	 */
	void RegisterLoader(ContentLoaderBase *loader);

	/**
	 * Retrieves the specified content either from a cache or from the
	 * platform's backing storage.
	 * @param name the path and filename of the content to retrieve
	 * @param <T> the type of content to load
	 * @return the loaded content or NULL on failure
	 */
	template <class T> T* Get(const stl::string &name);

	/**
	 * Retrieves the specified content either from a cache or from the
	 * platform's backing storage.
	 * @param name the path and filename of the content to retrieve
	 * @param params content-type-specified parameters used during loading or NULL
	 * @param <T> the type of content to load
	 * @return the loaded content or NULL on failure
	 */
	template <class T> T* Get(const stl::string &name, const ContentParam &params);

	/**
	 * Frees the specified content if the content pointed to by the given path
	 * and filename has been loaded. Content is only actually freed when it has
	 * no more references to it.
	 * @param name the path and filename of the content to free
	 * @param <T> the type of content to be freed
	 */
	template <class T> void Free(const stl::string &name);

	/**
	 * Frees the specified content if the content pointed to by the given path
	 * and filename has been loaded. Content is only actually freed when it has
	 * no more references to it.
	 * @param name the path and filename of the content to free
	 * @param params content-type-specific parameters that further describe the
	 *               exact content object to be freed, or NULL
	 * @param <T> the type of content to be freed
	 */
	template <class T> void Free(const stl::string &name, const ContentParam &params);

	/**
	 * Frees the specified content object. Content is only actually freed
	 * when it has no more references to it.
	 * @param content the content object to be freed
	 * @param <T> the type of content to be freed
	 */
	template <class T> void Free(T *content);

	/**
	 * Returns the path and filename that the given content object was
	 * loaded from.
	 * @param content the content object to get the path and filename for
	 * @param <T> the type of content to get the path and filename for
	 * @return the path and filename that the content object was loaded from
	 */
	template <class T> stl::string GetNameOf(T *content) const;

	/** 
	 * Returns the content loader object responsible for managing content of
	 * the specified type.
	 * @param <T> the content type who's content loader object is to be returned
	 * @return the content loader object or NULL if none exists
	 */ 
	template <class T> ContentLoader<T>* GetLoaderForType() const;

	/**
	 * @return the parent game app object
	 */
	BaseGameApp* GetGameApp() const                        { return m_gameApp; }
	
private:
	template <class T> ContentLoader<T>* GetLoader() const;
	ContentLoaderBase* GetLoaderFor(CONTENT_TYPE type) const;

	BaseGameApp *m_gameApp;
	ContentLoaderMap m_loaders;
};

template <class T>
T* ContentManager::Get(const stl::string &name)
{
	STACK_TRACE;
	ContentLoader<T> *loader = GetLoader<T>();
	T* content = loader->Get(name, NULL);
	return content;
}

template <class T>
T* ContentManager::Get(const stl::string &name, const ContentParam &params)
{
	STACK_TRACE;
	ContentLoader<T> *loader = GetLoader<T>();
	T* content = loader->Get(name, &params);
	return content;
}

template <class T>
void ContentManager::Free(const stl::string &name)
{
	STACK_TRACE;
	ContentLoader<T> *loader = GetLoader<T>();
	loader->Free(name, NULL);
}

template <class T>
void ContentManager::Free(const stl::string &name, const ContentParam &params)
{
	STACK_TRACE;
	ContentLoader<T> *loader = GetLoader<T>();
	loader->Free(name, &params);
}

template <class T>
void ContentManager::Free(T *content)
{
	STACK_TRACE;
	ContentLoader<T> *loader = GetLoader<T>();
	loader->Free(content);
}

template <class T>
stl::string ContentManager::GetNameOf(T *content) const
{
	STACK_TRACE;
	ContentLoader<T> *loader = GetLoader<T>();
	return loader->GetNameOf(content);
}

template <class T>
ContentLoader<T>* ContentManager::GetLoaderForType() const
{
	STACK_TRACE;
	return GetLoader<T>();
}

template <class T>
inline ContentLoader<T>* ContentManager::GetLoader() const
{
	ContentLoaderBase *loaderBase = GetLoaderFor(T::GetType());
	ASSERT(loaderBase != NULL);
	return (ContentLoader<T>*)loaderBase;
}



#endif
