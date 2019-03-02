#ifndef __FRAMEWORK_CONTENT_STATICMESHLOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_STATICMESHLOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloader.h"
#include "../assets/static/staticmesh.h"
#include "../math/matrix4x4.h"
#include <stl/map.h>
#include <stl/string.h>

class ContentManager;
class TextureLoader;

typedef stl::map<stl::string, StaticMesh*> StaticMeshMap;

/**
 * Content loader for static meshes.
 */
class StaticMeshLoader : public ContentLoader<StaticMesh>
{
public:
	/**
	 * Creates a static mesh loader.
	 * @param contentManager the parent content manager
	 */
	StaticMeshLoader(ContentManager *contentManager);

	/**
	 * Creates a static mesh loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with mesh filenames
	 */
	StaticMeshLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~StaticMeshLoader();

	/**
	 * Sets a transformation matrix that will be applied to every vertex
	 * for all subsequently loaded meshes.
	 * @param transform the transformation matrix to set
	 */
	void SetTransform(const Matrix4x4 &transform)          { m_transform = transform; }

	/**
	 * Sets the default path to use if, when loading, paths aren't provided
	 * with static mesh filenames.
	 * @param path the default path to use when loading if needed
	 */
	void SetDefaultPath(const stl::string &path);

	/**
	 * Retrieves the specified static mesh either the cache or from the
	 * platform's backing storage.
	 * @param name the path and filename of the static mesh to retrieve
	 * @param params unused, should be NULL
	 * @return the loaded static mesh or NULL on failure
	 */
	StaticMesh* Get(const stl::string &name, const ContentParam *params);

	/**
	 * Frees the specified static mesh. Meshes are only actually freed
	 * when they have no more references to them.
	 * @param content the static mesh to be freed
	 */
	void Free(StaticMesh *content);

	/**
	 * Frees the specified static mesh if the static mesh pointed to by the 
	 * given path and filename has been loaded. Meshes are only actually 
	 * freed when they have no more references to them.
	 * @param name the path and filename of the static mesh to free
	 * @param params unused, should be NULL
	 */
	void Free(const stl::string &name, const ContentParam *params);

	/**
	 * Returns the path and filename that the given static mesh was loaded from.
	 * @param content the static mesh to get the path and filename for
	 * @return the path and filename that the static mesh was loaded from
	 */
	stl::string GetNameOf(StaticMesh *content) const;

private:
	stl::string AddDefaultPathIfNeeded(const stl::string &meshFile);
	StaticMesh* Load(const stl::string &file);
	void Free(StaticMeshMap::iterator &itor, BOOL force);

	Matrix4x4 m_transform;
	stl::string m_defaultPath;
	StaticMeshMap m_meshes;
};

#endif
