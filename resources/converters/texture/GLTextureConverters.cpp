#include "GLTextureConverters.h"

#include "resources/texture/FITextureResource.h"
#include "resources/texture/GLTextureResource.h"
#include "resources/texture/GLFrameBufferResource.h"

#include <FreeImage.h>

using namespace resources;
using namespace resources::converters;

//////////////////////////////////////////////////////////////////////////

Resource GLTextureConverters::ConvertFromFI::convert(const Resource &res, const ResourceType &type)
{
	FITextureResource font(res, DONT_CONVERT);
	if (font && type & GLTextureData::getName())
	{
		GLTextureData *pData = new GLTextureData(this);
		pData->setId(ResourceId(GLTextureData::getName(), res->getId().getName()));
		pData->setSource(res);
		return GLTextureResource(pData);
	}
	return Resource();
}

bool GLTextureConverters::ConvertFromFI::load(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::load(res, flags))
		return true;

	if (!res->getSource()->isLoaded())
		return false;

	GLTextureResource texture(res);
	FITextureResource fiTexture(texture->getSource());

	fipImage img = fiTexture->getTexture();

	// Allocate GL resources
	GLuint textureId;
	glGenTextures(1, &textureId);
	texture->setTexture(textureId);

	texture->bind();

	// Set texture parameters
	// TODO: Make this configurable.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// If the picture isn't a power of two and non power of two textures aren't supported, resize.
	bool isPowerOfTwo = math::isPowerOfTwo(img.getHeight()) && math::isPowerOfTwo(img.getScanWidth());
	if (!isPowerOfTwo && !GLEW_ARB_texture_non_power_of_two)
	{
		fipImage tmp(img.getImageType(),
			math::getNextPowerOfTwo(img.getHeight()),
			math::getNextPowerOfTwo(img.getWidth()), 
			img.getBitsPerPixel());
		img.copySubImage(tmp, 0, 0, img.getWidth(), img.getHeight());
		img = tmp;
	}

	// TODO: Support other input types (HDR and other special textures)

	GLenum	format;								// gl texture format
	u32		bpp = img.getBitsPerPixel() >> 3;	// bytes per pixel
	if (bpp == 4)
	{
		img.convertTo32Bits();
		format	= GL_RGBA;
		bpp		= 4;
	}
	else
	{
		img.convertTo24Bits();
		format	= GL_RGB;
		bpp		= 3;
	}

	// If we support BGRA textures.
	if (GLEW_EXT_bgra)
	{
		if (format == GL_RGBA)
			format = GL_BGRA;
		else if (format == GL_RGB)
			format = GL_BGR;
	}
	else
	{
		BYTE *pPixels = img.accessPixels();

		// Swap RGB(A) -> BGR(A)
		for (u32 y = 0; y < img.getHeight(); y++)
		{
			// Determine scan line range
			u32 x = y * img.getScanWidth() * bpp;
			u32 count = x + img.getWidth() * bpp;
			for (; x < count; x += bpp)
			{
				// Swap red and green
				std::swap(pPixels[x + 0], pPixels[x + 2]);
			}
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0,
		bpp, img.getWidth(), img.getHeight(),
		0, format, GL_UNSIGNED_BYTE,
		img.accessPixels());

	texture->unbind();

	texture->setLoaded(true);

	return true;
}

bool GLTextureConverters::ConvertFromFI::unload(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::unload(res, flags))
		return true;

	GLTextureResource texture(res);

	GLuint textureId = texture->getTexture();
	texture->setTexture(GL_NONE);

	glDeleteTextures(1, &textureId);

	texture->setLoaded(false);

	return true;
}

//////////////////////////////////////////////////////////////////////////

Resource GLTextureConverters::ConvertFromFB::convert(const Resource &res, const ResourceType &type)
{
	GLFrameBufferResource fb(res, DONT_CONVERT);
	if (fb && type & GLTextureData::getName())
	{
		GLTextureData *pData = new GLTextureData(this);
		pData->setId(ResourceId(GLTextureData::getName(), res->getId().getName()));
		pData->setSource(res);
		return GLTextureResource(pData);
	}
	return Resource();
}

bool GLTextureConverters::ConvertFromFB::load(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::load(res, flags))
		return true;

	// source doesn't need to be loaded in this case

	GLTextureResource texture(res);
	if (!texture)
		return false;

	GLFrameBufferResource fb(texture->getSource());
	if (!fb)
		return false;

	FrameBufferDef def = fb->getDef();

	// Allocate GL resources
	GLuint textureId;
	glGenTextures(1, &textureId);
	texture->setTexture(textureId);

	texture->bind();

	// Set texture parameters
	// TODO: Make this configurable.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	math::Vector2i size = def->getSize();
	// If the picture isn't a power of two and non power of two textures aren't supported, resize.
	bool isPowerOfTwo = math::isPowerOfTwo(size.x) && math::isPowerOfTwo(size.y);
	if (!isPowerOfTwo && !GLEW_ARB_texture_non_power_of_two)
	{
		size.x = math::getNextPowerOfTwo(size.x);
		size.y = math::getNextPowerOfTwo(size.y); 
	}

	// TODO: Support other types (HDR and other special textures)
	glTexImage2D(GL_TEXTURE_2D, 0,
		def->getBpp(), size.x, size.y,
		0, def->getBpp() == 3? GL_RGB: GL_RGBA, GL_UNSIGNED_BYTE,
		NULL);

	texture->unbind();

	texture->setLoaded(true);

	return true;
}

bool GLTextureConverters::ConvertFromFB::unload(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::unload(res, flags))
		return true;

	GLTextureResource texture(res);

	GLuint textureId = texture->getTexture();
	texture->setTexture(GL_NONE);

	glDeleteTextures(1, &textureId);

	texture->setLoaded(false);

	return true;
}