#ifndef __FRAMEWORK_CONTENT_KEYFRAMEMESHLOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_KEYFRAMEMESHLOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloader.h"
#include "../assets/animation/keyframemesh.h"
#include "../math/matrix4x4.h"
#include <stl/map.h>
#include <stl/string.h>

class ContentManager;

typedef stl::map<stl::string, KeyframeMesh*> KeyframeMeshMap;

/**
 * Content loader for keyframe meshes.
 */
class KeyframeMeshLoader : public ContentLoader<KeyframeMesh>
{
public:
	/**
	 * Creates a keyframe mesh loader.
	 * @param contentManager the parent content manager
	 */
	KeyframeMeshLoader(ContentManager *contentManager);

	/**
	 * Creates a keyframe mesh loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with mesh filenames
	 */
	KeyframeMeshLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~KeyframeMeshLoader();

	/**
	 * Sets a transformation matrix that will be applied to every vertex
	 * for all subsequently loaded meshes.
	 * @param transform the transformation matrix to set
	 */
	void SetTransform(const Matrix4x4 &transform)          { m_transform = transform; }

	/**
	 * Sets the default path to use if, when loading, paths aren't provided
	 * with keyframe mesh filenames.
	 * @param path the default path to use when loading if needed
	 */
	void SetDefaultPath(const stl::string &path);

	/**
	 * Retrieves the specified keyframe mesh either the cache or from the
	 * platform's backing storage.
	 * @param name the path and filename of the keyframe mesh to retrieve
	 * @param params unused, should be NULL
	 * @return the loaded keyframe mesh or NULL on failure
	 */
	KeyframeMesh* Get(const stl::string &name, const ContentParam *params);

	/**
	 * Frees the specified keyframe mesh. Meshes are only actually freed
	 * when they have no more references to them.
	 * @param content the keyframe mesh to be freed
	 */
	void Free(KeyframeMesh *content);

	/**
	 * Frees the specified keyframe mesh if the keyframe mesh pointed to by the 
	 * given path and filename has been loaded. Meshes are only actually 
	 * freed when they have no more references to them.
	 * @param name the path and filename of the keyframe mesh to free
	 * @param params unused, should be NULL
	 */
	void Free(const stl::string &name, const ContentParam *params);

	/**
	 * Returns the path and filename that the given keyframe mesh was loaded from.
	 * @param content the keyframe mesh to get the path and filename for
	 * @return the path and filename that the keyframe mesh was loaded from
	 */
	stl::string GetNameOf(KeyframeMesh *content) const;

private:
	stl::string AddDefaultPathIfNeeded(const stl::string &meshFile);
	KeyframeMesh* Load(const stl::string &file);
	void Free(KeyframeMeshMap::iterator &itor, BOOL force);

	Matrix4x4 m_transform;
	stl::string m_defaultPath;
	KeyframeMeshMap m_meshes;
};

#endif
