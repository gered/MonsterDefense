#include "../debug.h"
#include "../log.h"

#include "image.h"

#include "../file/file.h"
#include "../file/memoryfile.h"

#include <string.h>

#define STBI_NO_STDIO
#include <stb_image.h>

Image* Image::CreateFrom(File *file)
{
	STACK_TRACE;
	ASSERT(file != NULL);
	ASSERT(file->IsOpen());

	uint8_t *imageBytes = NULL;
	size_t imageSize = file->GetFileSize();

	// if the file object is a MemoryFile, we can skip needing to allocate + copy memory around for stb_image
	if (file->GetFileType() == FILETYPE_MEMORY)
	{
		MemoryFile *memoryFile = (MemoryFile*)file;
		imageBytes = (uint8_t*)memoryFile->GetFileData();
	}
	else
	{
		// load raw file bytes fully into memory (can't pass our File instance to stb_image)
		imageBytes = new uint8_t[imageSize];
		ASSERT(imageBytes != NULL);
		file->Read((int8_t*)imageBytes, imageSize);
	}

	// load using stb_image
	int width;
	int height;
	int componentsPerPixel;
	uint8_t *pixels = stbi_load_from_memory(imageBytes, imageSize, &width, &height, &componentsPerPixel, 0);
	if (pixels == NULL)
	{
		LOG_ERROR(LOGCAT_ASSETS, "Error loading image \"%s\": %s\n", file->GetFilename().c_str(), stbi_failure_reason());
		if (file->GetFileType() != FILETYPE_MEMORY)
			SAFE_DELETE_ARRAY(imageBytes);

		return NULL;
	}
	else
	{
		// don't need this anymore
		if (file->GetFileType() != FILETYPE_MEMORY)
			SAFE_DELETE_ARRAY(imageBytes);

		int bpp = componentsPerPixel * 8;
		IMAGE_FORMAT format;
		// TODO: detect IMAGE_FORMAT_ALPHA
		if (componentsPerPixel == 3)
			format = IMAGE_FORMAT_RGB;
		else if (componentsPerPixel == 4)
			format = IMAGE_FORMAT_RGBA;
		else
		{
			ASSERT(!"Unrecognized componentsPerPixel value.");
			stbi_image_free(pixels);
			return NULL;
		}

		Image *output = new Image(pixels, FALSE, width, height, bpp, format);
		ASSERT(output != NULL);
		return output;
	}
}

Image* Image::CreateFrom(const Image *source)
{
	STACK_TRACE;
	ASSERT(source != NULL);
	return CreateFrom(source, 0, 0, source->GetWidth() - 1, source->GetHeight() - 1);
}

Image* Image::CreateFrom(const Image *source, uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	STACK_TRACE;
	ASSERT(right > left);
	ASSERT(bottom > top);
	ASSERT(left < source->GetWidth());
	ASSERT(right < source->GetWidth());
	ASSERT(top < source->GetHeight());
	ASSERT(bottom < source->GetHeight());

	Image *output = Create((right - left) + 1, (bottom - top) + 1, source->GetFormat());
	ASSERT(output != NULL);

	output->Copy(source, left, top, right, bottom, 0, 0);

	return output;
}

Image* Image::Create(uint16_t width, uint16_t height, IMAGE_FORMAT format)
{
	STACK_TRACE;
	ASSERT(width != 0);
	ASSERT(height != 0);

	int bpp = 0;
	if (format == IMAGE_FORMAT_RGB)
		bpp = 24;
	else if (format == IMAGE_FORMAT_RGBA)
		bpp = 32;
	else if (format == IMAGE_FORMAT_ALPHA)
		bpp = 8;

	ASSERT(bpp != 0);

	uint8_t *pixels = new uint8_t[(width * height) * (bpp / 8)];
	ASSERT(pixels != NULL);

	Image *output = new Image(pixels, TRUE, width, height, bpp, format);
	ASSERT(output != NULL);

	return output;
}

Image::Image(uint8_t *pixels, BOOL didWeAllocateMem, uint16_t width, uint16_t height, uint8_t bpp, IMAGE_FORMAT format)
	: Content()
{
	STACK_TRACE;
	ASSERT(pixels != NULL);
	ASSERT(width != 0);
	ASSERT(height != 0);
	ASSERT(bpp != 0);
	m_didWeAllocateMem = didWeAllocateMem;
	m_pixels = pixels;
	m_width = width;
	m_height = height;
	m_bpp = bpp;
	m_format = format;
	m_pitch = width * (bpp / 8);
}

Image::~Image()
{
	STACK_TRACE;
	if (m_didWeAllocateMem)
	{
		SAFE_DELETE(m_pixels);
	}
	else
	{
		stbi_image_free(m_pixels);
		m_pixels = NULL;
	}
	m_width = 0;
	m_height = 0;
	m_bpp = 0;
}

Color Image::GetColor(uint16_t x, uint16_t y) const
{
	STACK_TRACE;
	ASSERT(m_format == IMAGE_FORMAT_RGB || m_format == IMAGE_FORMAT_RGBA);

	if (m_format == IMAGE_FORMAT_RGB)
	{
		uint8_t *pixel = Get(x, y);
		return Color(*pixel, *(pixel + 1), *(pixel + 2));
	}
	else
	{
		uint8_t *pixel = Get(x, y);
		return Color(*pixel, *(pixel + 1), *(pixel + 2), *(pixel + 3));
	}
}

void Image::SetColor(uint16_t x, uint16_t y, const Color &color)
{
	STACK_TRACE;
	ASSERT(m_format == IMAGE_FORMAT_RGB || m_format == IMAGE_FORMAT_RGBA);

	if (m_format == IMAGE_FORMAT_RGB)
	{
		uint8_t *pixel = Get(x, y);
		uint8_t dummy;
		color.ToInts(pixel, pixel + 1, pixel + 2, &dummy);
	}
	else
	{
		uint8_t *pixel = Get(x, y);
		color.ToInts(pixel, pixel + 1, pixel + 2, pixel + 3);
	}
}

void Image::Copy(const Image *source, uint16_t destX, uint16_t destY)
{
	STACK_TRACE;
	ASSERT(source != NULL);
	Copy(source, 0, 0, source->GetWidth() - 1, source->GetHeight() - 1, destX, destY);
}

void Image::Copy(const Image *source, uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint16_t destX, uint16_t destY)
{
	STACK_TRACE;
	ASSERT(source != NULL);
	ASSERT(source->GetBpp() == m_bpp);
	ASSERT(right > left);
	ASSERT(bottom > top);
	ASSERT(left < source->GetWidth());
	ASSERT(right < source->GetWidth());
	ASSERT(top < source->GetHeight());
	ASSERT(bottom < source->GetHeight());
	ASSERT(destX < m_width);
	ASSERT(destY < m_height);
	ASSERT((right - left) < m_width);
	ASSERT((bottom - top) < m_height);
	ASSERT(destX + (right - left) < m_width);
	ASSERT(destY + (bottom - top) < m_height);

	uint8_t *sourcePixels = source->GetPixels() + source->GetOffsetFor(left, top);
	uint8_t *destPixels = m_pixels + GetOffsetFor(destX, destY);

	size_t lineWidthInBytes = ((right - left) + 1) * (m_bpp / 8);
	uint16_t numLinesToCopy = (bottom - top) + 1;

	for (uint16_t i = 0; i < numLinesToCopy; ++i)
	{
		memcpy(destPixels, sourcePixels, lineWidthInBytes);
		sourcePixels += source->GetPitch();
		destPixels += m_pitch;
	}
}

void Image::Clear()
{
	STACK_TRACE;
	memset(m_pixels, 0, GetSizeInBytes());
}

void Image::Clear(const Color &color)
{
	STACK_TRACE;
	ASSERT(m_format == IMAGE_FORMAT_RGB || m_format == IMAGE_FORMAT_RGBA);

	uint32_t sizeInBytes = GetSizeInBytes();
	uint8_t *pixel = m_pixels;

	if (m_format == IMAGE_FORMAT_RGB)
	{
		ASSERT(sizeInBytes % 3 == 0);
		uint8_t dummy;
		for (uint32_t i = 0; i < sizeInBytes; i += 3)
		{
			color.ToInts(pixel, pixel + 1, pixel + 2, &dummy);
			pixel += 3;
		}
	}
	else
	{
		ASSERT(sizeInBytes % 4 == 0);
		for (uint32_t i = 0; i < sizeInBytes; i += 4)
		{
			color.ToInts(pixel, pixel + 1, pixel + 2, pixel + 3);
			pixel += 4;
		}
	}
}

void Image::Clear(const uint32_t color)
{
	STACK_TRACE;
	Clear(Color::FromInt(color));
}

void Image::Clear(const uint8_t alpha)
{
	STACK_TRACE;
	ASSERT(m_format == IMAGE_FORMAT_ALPHA);
	memset(m_pixels, alpha, GetSizeInBytes());
}
