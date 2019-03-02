#include "../debug.h"
#include "../log.h"
#include "../common.h"

#include "textureloader.h"

#include "contentmanager.h"
#include "imageloader.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../graphics/graphicsdevice.h"
#include "../graphics/image.h"
#include "../graphics/texture.h"
#include "../file/file.h"
#include "../file/filesystem.h"

TextureLoader::TextureLoader(ContentManager *contentManager)
	: ContentLoader<Texture>(contentManager)
{
	STACK_TRACE;
	SetDefaultPath("assets://textures/");
}

TextureLoader::TextureLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoader<Texture>(contentManager)
{
	STACK_TRACE;
	SetDefaultPath(defaultPath);
}

TextureLoader::~TextureLoader()
{
	STACK_TRACE;
	while (m_textures.size() > 0)
	{
		TextureMap::iterator itor = m_textures.begin();
		LOG_WARN(LOGCAT_ASSETS, "TextureLoader: \"%s\" not explicitly freed.\n", itor->first.c_str());
		Free(itor, TRUE);
	}

	m_textures.clear();
}

void TextureLoader::OnNewContext()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "TextureLoader: reloading previous textures for new OpenGL context.\n");

	for (TextureMap::iterator itor = m_textures.begin(); itor != m_textures.end(); ++itor)
	{
		Texture *temp = Load(itor->first);
		if (temp != NULL)
		{
			itor->second->Reload(temp->GetTextureName());
			SAFE_DELETE(temp);
		}
	}
}

void TextureLoader::OnLostContext()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "TextureLoader: resetting loaded texture IDs due to lost OpenGL context.\n");

	for (TextureMap::iterator itor = m_textures.begin(); itor != m_textures.end(); ++itor)
		itor->second->Invalidate();
}

void TextureLoader::SetDefaultPath(const stl::string &path)
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

	LOG_INFO(LOGCAT_ASSETS, "TextureLoader: default path set to \"%s\"\n", m_defaultPath.c_str());
}

Texture* TextureLoader::Get(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	Texture *texture;

	stl::string filename = AddDefaultPathIfNeeded(name);

	TextureMap::iterator itor = m_textures.find(filename);
	if (itor != m_textures.end())
		texture = itor->second;
	else
	{
		texture = Load(filename);
		if (texture != NULL)
			m_textures[filename] = texture;
	}

	texture->Reference();

	return texture;
}

void TextureLoader::Free(Texture *content)
{
	STACK_TRACE;
	ASSERT(content != NULL);
	ASSERT(content->IsReferenceCounted() == TRUE);

	for (TextureMap::iterator itor = m_textures.begin(); itor != m_textures.end(); ++itor)
	{
		if (itor->second == content)
		{
			Free(itor, FALSE);
			break;
		}
	}
}

void TextureLoader::Free(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	stl::string filename = AddDefaultPathIfNeeded(name);

	TextureMap::iterator itor = m_textures.find(filename);
	if (itor != m_textures.end())
		Free(itor, FALSE);
}

stl::string TextureLoader::GetNameOf(Texture *content) const
{
	STACK_TRACE;
	ASSERT(content != NULL);
	ASSERT(content->IsReferenceCounted() == TRUE);

	for (TextureMap::const_iterator itor = m_textures.begin(); itor != m_textures.end(); ++itor)
	{
		if (itor->second == content)
			return itor->first;
	}

	return "";
}

stl::string TextureLoader::AddDefaultPathIfNeeded(const stl::string &textureFile)
{
	STACK_TRACE;
	if (textureFile[0] == '/' || m_defaultPath.length() == 0 || textureFile.substr(0, 9) == "assets://")
		return textureFile;
	else
		return m_defaultPath + textureFile;
}

Texture* TextureLoader::Load(const stl::string &file)
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "TextureLoader: loading \"%s\"\n", file.c_str());

	Image *image = GetContentManager()->Get<Image>(file.c_str());
	ASSERT(image != NULL);
	if (image == NULL)
		return NULL;

	// create the texture
	Texture *texture = GetContentManager()->GetGameApp()->GetGraphicsDevice()->CreateTextureFromImage(image);
	ASSERT(texture != NULL);

	// don't need to keep the underlying image around if not being used elsewhere
	GetContentManager()->Free<Image>(image);

	if (texture == NULL)
		return NULL;
	else
		return texture;
}

void TextureLoader::Free(TextureMap::iterator &itor, BOOL force)
{
	STACK_TRACE;
	Texture *texture = itor->second;
	texture->Release();

	if (texture->IsReferenced() && force)
	{
		// if this is being forced, we need to manually decrease the Content's
		// internal reference counter (or the destructor will have a fit).
		// I suppose this is probably kind of "hack-ish"...
		uint32_t numToRelease = texture->GetNumReferences();
		for (uint32_t i = 0; i < numToRelease; ++i)
			texture->Release();
	}

	if (!texture->IsReferenced())
	{
		LOG_INFO(LOGCAT_ASSETS, "TextureLoader: %sfreed \"%s\"\n", (force ? "forcefully " : ""), itor->first.c_str());
		GetContentManager()->GetGameApp()->GetGraphicsDevice()->FreeTexture(texture);
		m_textures.erase(itor);
	}
}

