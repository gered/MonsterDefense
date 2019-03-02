#include "../debug.h"
#include "../log.h"
#include "../common.h"

#include "imageloader.h"

#include "contentmanager.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../graphics/image.h"
#include "../file/file.h"
#include "../file/filesystem.h"

ImageLoader::ImageLoader(ContentManager *contentManager)
	: ContentLoader<Image>(contentManager)
{
	STACK_TRACE;
	SetDefaultPath("assets://images/");
}

ImageLoader::ImageLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoader<Image>(contentManager)
{
	STACK_TRACE;
	SetDefaultPath(defaultPath);
}

ImageLoader::~ImageLoader()
{
	STACK_TRACE;
	while (m_images.size() > 0)
	{
		ImageMap::iterator itor = m_images.begin();
		LOG_WARN(LOGCAT_ASSETS, "ImageLoader: \"%s\" not explicitly freed.\n", itor->first.c_str());
		Free(itor, TRUE);
	}

	m_images.clear();
}

void ImageLoader::SetDefaultPath(const stl::string &path)
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

	LOG_INFO(LOGCAT_ASSETS, "ImageLoader: default path set to \"%s\"\n", m_defaultPath.c_str());
}

Image* ImageLoader::Get(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	Image *image;

	stl::string filename = AddDefaultPathIfNeeded(name);

	ImageMap::iterator itor = m_images.find(filename);
	if (itor != m_images.end())
		image = itor->second;
	else
	{
		image = Load(filename);
		if (image != NULL)
			m_images[filename] = image;
	}

	image->Reference();

	return image;
}

void ImageLoader::Free(Image *content)
{
	STACK_TRACE;
	ASSERT(content != NULL);
	ASSERT(content->IsReferenceCounted() == TRUE);

	for (ImageMap::iterator itor = m_images.begin(); itor != m_images.end(); ++itor)
	{
		if (itor->second == content)
		{
			Free(itor, FALSE);
			break;
		}
	}
}

void ImageLoader::Free(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	stl::string filename = AddDefaultPathIfNeeded(name);

	ImageMap::iterator itor = m_images.find(filename);
	if (itor != m_images.end())
		Free(itor, FALSE);
}

stl::string ImageLoader::GetNameOf(Image *content) const
{
	STACK_TRACE;
	ASSERT(content != NULL);
	ASSERT(content->IsReferenceCounted() == TRUE);

	for (ImageMap::const_iterator itor = m_images.begin(); itor != m_images.end(); ++itor)
	{
		if (itor->second == content)
			return itor->first;
	}

	return "";
}

stl::string ImageLoader::AddDefaultPathIfNeeded(const stl::string &imageFile)
{
	STACK_TRACE;
	if (imageFile[0] == '/' || m_defaultPath.length() == 0 || imageFile.substr(0, 9) == "assets://")
		return imageFile;
	else
		return m_defaultPath + imageFile;
}

Image* ImageLoader::Load(const stl::string &file)
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "ImageLoader: loading \"%s\"\n", file.c_str());

	File *imageFile = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->Open(file, FILEMODE_READ | FILEMODE_BINARY | FILEMODE_MEMORY);
	ASSERT(imageFile != NULL);

	Image *image = Image::CreateFrom(imageFile);
	ASSERT(image != NULL);

	SAFE_DELETE(imageFile);

	return image;
}

void ImageLoader::Free(ImageMap::iterator &itor, BOOL force)
{
	STACK_TRACE;
	Image *image = itor->second;
	image->Release();

	if (image->IsReferenced() && force)
	{
		// if this is being forced, we need to manually decrease the Content's
		// internal reference counter (or the destructor will have a fit).
		// I suppose this is probably kind of "hack-ish"...
		uint32_t numToRelease = image->GetNumReferences();
		for (uint32_t i = 0; i < numToRelease; ++i)
			image->Release();
	}

	if (!image->IsReferenced())
	{
		LOG_INFO(LOGCAT_ASSETS, "ImageLoader: %sfreed \"%s\"\n", (force ? "forcefully " : ""), itor->first.c_str());
		SAFE_DELETE(image);
		m_images.erase(itor);
	}
}


