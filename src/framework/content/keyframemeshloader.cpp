#include "../debug.h"
#include "../log.h"

#include "keyframemeshloader.h"
#include "contentmanager.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../assets/animation/keyframemesh.h"
#include "../assets/animation/keyframemeshfile.h"
#include "../file/file.h"
#include "../file/filesystem.h"
#include "../math/matrix4x4.h"
#include <stl/string.h>
#include <stl/map.h>

KeyframeMeshLoader::KeyframeMeshLoader(ContentManager *contentManager)
	: ContentLoader<KeyframeMesh>(contentManager)
{
	STACK_TRACE;
	m_transform = IDENTITY_MATRIX;
	SetDefaultPath("assets://meshes/");
}

KeyframeMeshLoader::KeyframeMeshLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoader<KeyframeMesh>(contentManager)
{
	STACK_TRACE;
	m_transform = IDENTITY_MATRIX;
	SetDefaultPath(defaultPath);
}

KeyframeMeshLoader::~KeyframeMeshLoader()
{
	STACK_TRACE;
	while (m_meshes.size() > 0)
	{
		KeyframeMeshMap::iterator itor = m_meshes.begin();
		LOG_WARN(LOGCAT_ASSETS, "KeyframeMeshLoader: \"%s\" not explicitly freed.\n", itor->first.c_str());
		Free(itor, TRUE);
	}

	m_meshes.clear();
}

void KeyframeMeshLoader::SetDefaultPath(const stl::string &path)
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

	LOG_INFO(LOGCAT_ASSETS, "KeyframeMeshLoader: default path set to \"%s\"\n", m_defaultPath.c_str());
}

KeyframeMesh* KeyframeMeshLoader::Get(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	KeyframeMesh *mesh;

	stl::string filename = AddDefaultPathIfNeeded(name);

	KeyframeMeshMap::iterator itor = m_meshes.find(filename);
	if (itor != m_meshes.end())
		mesh = itor->second;
	else
	{
		mesh = Load(filename);
		if (mesh != NULL)
			m_meshes[filename] = mesh;
	}

	mesh->Reference();

	return mesh;
}

void KeyframeMeshLoader::Free(KeyframeMesh *content)
{
	STACK_TRACE;
	ASSERT(content != NULL);
	ASSERT(content->IsReferenceCounted() == TRUE);

	for (KeyframeMeshMap::iterator itor = m_meshes.begin(); itor != m_meshes.end(); ++itor)
	{
		if (itor->second == content)
		{
			Free(itor, FALSE);
			break;
		}
	}
}

void KeyframeMeshLoader::Free(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	stl::string filename = AddDefaultPathIfNeeded(name);

	KeyframeMeshMap::iterator itor = m_meshes.find(filename);
	if (itor != m_meshes.end())
		Free(itor, FALSE);
}

stl::string KeyframeMeshLoader::GetNameOf(KeyframeMesh *content) const
{
	STACK_TRACE;
	ASSERT(content != NULL);
	ASSERT(content->IsReferenceCounted() == TRUE);

	for (KeyframeMeshMap::const_iterator itor = m_meshes.begin(); itor != m_meshes.end(); ++itor)
	{
		if (itor->second == content)
			return itor->first;
	}

	return "";
}

stl::string KeyframeMeshLoader::AddDefaultPathIfNeeded(const stl::string &meshFile)
{
	STACK_TRACE;
	if (meshFile[0] == '/' || m_defaultPath.length() == 0 || meshFile.substr(0, 9) == "assets://")
		return meshFile;
	else
		return m_defaultPath + meshFile;
}

KeyframeMesh* KeyframeMeshLoader::Load(const stl::string &file)
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "KeyframeMeshLoader: loading \"%s\"\n", file.c_str());

	// load the file data
	File *f = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->Open(file, FILEMODE_READ | FILEMODE_BINARY | FILEMODE_MEMORY);
	KeyframeMeshFile *meshFile = new KeyframeMeshFile(f, m_transform);
	ASSERT(meshFile != NULL);

	// convert it into a mesh object
	KeyframeMesh *mesh = new KeyframeMesh(meshFile);
	SAFE_DELETE(meshFile);

	// close the file
	SAFE_DELETE(f);

	return mesh;
}

void KeyframeMeshLoader::Free(KeyframeMeshMap::iterator &itor, BOOL force)
{
	STACK_TRACE;
	KeyframeMesh *mesh = itor->second;
	mesh->Release();

	if (mesh->IsReferenced() && force)
	{
		// if this is being forced, we need to manually decrease the Content's
		// internal reference counter (or the destructor will have a fit).
		// I suppose this is probably kind of "hack-ish"...
		uint32_t numToRelease = mesh->GetNumReferences();
		for (uint32_t i = 0; i < numToRelease; ++i)
			mesh->Release();
	}

	if (!mesh->IsReferenced())
	{
		LOG_INFO(LOGCAT_ASSETS, "KeyframeMeshLoader: %sfreed \"%s\"\n", (force ? "forcefully " : ""), itor->first.c_str());
		SAFE_DELETE(mesh);
		m_meshes.erase(itor);
	}
}

