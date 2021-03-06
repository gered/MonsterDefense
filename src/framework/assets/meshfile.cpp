#include "../debug.h"

#include "meshfile.h"

#include "../file/file.h"

MeshFile::MeshFile(File *file)
{
	STACK_TRACE;
	ASSERT(file->IsOpen());
	m_file = file;

	char ident[4];
	m_file->Read((int8_t*)ident, 4);
	if (ident[0] != 'M' || ident[1] != 'E' || ident[2] != 'S' || ident[3] != 'H')
		ASSERT(!"Invalid MESH identifier.");

	char version = m_file->ReadChar();
	if (version > 1)
		ASSERT(!"Unsupported MESH file version.");

	CollectChunks();
}

MeshFile::~MeshFile()
{
	STACK_TRACE;
	ChunkMap::iterator itor;
	for (itor = m_chunks.begin(); itor != m_chunks.end(); ++itor)
		SAFE_DELETE(itor->second);
}

void MeshFile::CollectChunks()
{
	STACK_TRACE;
	m_chunks.clear();

	// start off just after the header (start of the first chunk)
	m_file->Seek(5, FILESEEK_BEGINNING);

	uint32_t size;
	char ident[4];
	ident[3] = '\0';

	while (m_file->Read((int8_t*)ident, 3))
	{
		// chunk header
		size = m_file->ReadInt();

		// save chunk info
		ChunkDescriptor *descriptor = new ChunkDescriptor();
		ASSERT(descriptor != NULL);
		descriptor->start = m_file->Tell();
		descriptor->length = size;
		stl::string name(ident);
		m_chunks[name] = descriptor;

		// move to the next chunk
		m_file->Seek(size, FILESEEK_CURRENT);
	}
}

ChunkDescriptor* MeshFile::GetChunkDesc(const stl::string &name)
{
	STACK_TRACE;
	ChunkMap::iterator itor = m_chunks.find(name);
	if (itor == m_chunks.end())
		return NULL;
	else
		return itor->second;
}
