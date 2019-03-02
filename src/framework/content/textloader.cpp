#include "../debug.h"
#include "../log.h"
#include "../common.h"

#include "textloader.h"

#include "contentmanager.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../support/text.h"
#include "../file/file.h"
#include "../file/filesystem.h"

TextLoader::TextLoader(ContentManager *contentManager)
	: ContentLoader<Text>(contentManager)
{
	STACK_TRACE;
}

TextLoader::TextLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoader<Text>(contentManager)
{
	STACK_TRACE;
	SetDefaultPath(defaultPath);
}

TextLoader::~TextLoader()
{
	STACK_TRACE;
	while (m_textFiles.size() > 0)
	{
		TextMap::iterator itor = m_textFiles.begin();
		LOG_WARN(LOGCAT_ASSETS, "TextLoader: \"%s\" not explicitly freed.\n", itor->first.c_str());
		Free(itor, TRUE);
	}

	m_textFiles.clear();
}

void TextLoader::SetDefaultPath(const stl::string &path)
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

	LOG_INFO(LOGCAT_ASSETS, "TextLoader: default path set to \"%s\"\n", m_defaultPath.c_str());
}

Text* TextLoader::Get(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	Text *text;

	stl::string filename = AddDefaultPathIfNeeded(name);

	TextMap::iterator itor = m_textFiles.find(filename);
	if (itor != m_textFiles.end())
		text = itor->second;
	else
	{
		text = Load(filename);
		if (text != NULL)
			m_textFiles[filename] = text;
	}

	text->Reference();

	return text;
}

void TextLoader::Free(Text *content)
{
	STACK_TRACE;
	ASSERT(content != NULL);
	ASSERT(content->IsReferenceCounted() == TRUE);

	for (TextMap::iterator itor = m_textFiles.begin(); itor != m_textFiles.end(); ++itor)
	{
		if (itor->second == content)
		{
			Free(itor, FALSE);
			break;
		}
	}
}

void TextLoader::Free(const stl::string &name, const ContentParam *params)
{
	STACK_TRACE;
	stl::string filename = AddDefaultPathIfNeeded(name);

	TextMap::iterator itor = m_textFiles.find(filename);
	if (itor != m_textFiles.end())
		Free(itor, FALSE);
}

stl::string TextLoader::GetNameOf(Text *content) const
{
	STACK_TRACE;
	ASSERT(content != NULL);
	ASSERT(content->IsReferenceCounted() == TRUE);

	for (TextMap::const_iterator itor = m_textFiles.begin(); itor != m_textFiles.end(); ++itor)
	{
		if (itor->second == content)
			return itor->first;
	}

	return "";
}

stl::string TextLoader::AddDefaultPathIfNeeded(const stl::string &textFile)
{
	STACK_TRACE;
	if (textFile[0] == '/' || m_defaultPath.length() == 0 || textFile.substr(0, 9) == "assets://")
		return textFile;
	else
		return m_defaultPath + textFile;
}

Text* TextLoader::Load(const stl::string &file)
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "TextLoader: loading \"%s\"\n", file.c_str());

	File *textFile = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->Open(file, FILEMODE_READ | FILEMODE_MEMORY);
	ASSERT(textFile != NULL);

	Text *text = Text::CreateFrom(textFile);
	ASSERT(text != NULL);

	SAFE_DELETE(textFile);

	return text;
}

void TextLoader::Free(TextMap::iterator &itor, BOOL force)
{
	STACK_TRACE;
	Text *text = itor->second;
	text->Release();

	if (text->IsReferenced() && force)
	{
		// if this is being forced, we need to manually decrease the Content's
		// internal reference counter (or the destructor will have a fit).
		// I suppose this is probably kind of "hack-ish"...
		uint32_t numToRelease = text->GetNumReferences();
		for (uint32_t i = 0; i < numToRelease; ++i)
			text->Release();
	}

	if (!text->IsReferenced())
	{
		LOG_INFO(LOGCAT_ASSETS, "TextLoader: %sfreed \"%s\"\n", (force ? "forcefully " : ""), itor->first.c_str());
		SAFE_DELETE(text);
		m_textFiles.erase(itor);
	}
}


