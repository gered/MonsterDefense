#include "../debug.h"
#include "../log.h"

#include "staticmeshloader.h"

#include "contentmanager.h"
#include "textureloader.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../file/file.h"
#include "../file/filesystem.h"
#include "../assets/static/staticmesh.h"
#include "../assets/static/staticmeshfile.h"
#include "../math/matrix4x4.h"

StaticMeshLoader::StaticMeshLoader(ContentManager *contentManager)
	: ContentLoader<StaticMesh>(contentManager)
{
	STACK_TRACE;
	m_transform = IDENTITY_MATRIX;
	SetDefaultPath("assets://meshes/");
}

StaticMeshLoader::StaticMeshLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoader<StaticMesh>(contentManager)
{
	STACK_TRACE;
	m_transform = IDENTITY_MATRIX;
	SetDefaultPath(defaultPath);
}

StaticMeshLoader::~StaticMeshLoader()
{
	STACK_TRACE;
	while (m_meshes.size() > 0)
	{
		StaticMeshMap::iterator itor = m_meshes.begin();
		LOG_WARN(LOGCAT_ASSETS, "StaticMeshLoader: \"%s\" not explicitly freed.\n", itor->first.c_str());
		Free(itor, TRUE);
	}

	m_meshes.clear();
}

void StaticMeshLoader::SetDefaultPath(const stl::string &path)
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

	LOG_INFO(LOGCAT_ASSETS, "StaticMeshLoader: default path set to \"%s\"\n", m_defaultPath.c_str());
}

StaticMesh* StaticMeshLoader::Get(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	StaticMesh *mesh;

	stl::string filename = AddDefaultPathIfNeeded(name);

	StaticMeshMap::iterator itor = m_meshes.find(filename);
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

void StaticMeshLoader::Free(StaticMesh *content)
{
	STACK_TRACE;
	ASSERT(content != NULL);
	ASSERT(content->IsReferenceCounted() == TRUE);

	for (StaticMeshMap::iterator itor = m_meshes.begin(); itor != m_meshes.end(); ++itor)
	{
		if (itor->second == content)
		{
			Free(itor, FALSE);
			break;
		}
	}
}

void StaticMeshLoader::Free(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	stl::string filename = AddDefaultPathIfNeeded(name);

	StaticMeshMap::iterator itor = m_meshes.find(filename);
	if (itor != m_meshes.end())
		Free(itor, FALSE);
}

stl::string StaticMeshLoader::GetNameOf(StaticMesh *content) const
{
	STACK_TRACE;
	ASSERT(content != NULL);
	ASSERT(content->IsReferenceCounted() == TRUE);

	for (StaticMeshMap::const_iterator itor = m_meshes.begin(); itor != m_meshes.end(); ++itor)
	{
		if (itor->second == content)
			return itor->first;
	}

	return "";
}

stl::string StaticMeshLoader::AddDefaultPathIfNeeded(const stl::string &meshFile)
{
	STACK_TRACE;
	if (meshFile[0] == '/' || m_defaultPath.length() == 0 || meshFile.substr(0, 9) == "assets://")
		return meshFile;
	else
		return m_defaultPath + meshFile;
}

StaticMesh* StaticMeshLoader::Load(const stl::string &file)
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "StaticMeshLoader: loading \"%s\"\n", file.c_str());

	// load the file data
	File *f = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->Open(file, FILEMODE_READ | FILEMODE_BINARY | FILEMODE_MEMORY);
	StaticMeshFile *meshFile = new StaticMeshFile(f, m_transform);
	ASSERT(meshFile != NULL);

	// convert it into a mesh object
	StaticMesh *mesh = new StaticMesh(meshFile, GetContentManager());
	SAFE_DELETE(meshFile);

	// done with the file now
	SAFE_DELETE(f);

	return mesh;
}

void StaticMeshLoader::Free(StaticMeshMap::iterator &itor, BOOL force)
{
	STACK_TRACE;
	StaticMesh *mesh = itor->second;
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
		LOG_INFO(LOGCAT_ASSETS, "StaticMeshLoader: %sfreed \"%s\"\n", (force ? "forcefully " : ""), itor->first.c_str());
		SAFE_DELETE(mesh);
		m_meshes.erase(itor);
	}
}

