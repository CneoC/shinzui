#include "FITextureConverters.h"

#include "resources/FileResource.h"
#include "resources/texture/FITextureResource.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

using namespace resources;
using namespace resources::converters;

//////////////////////////////////////////////////////////////////////////

Resource FITextureConverters::ConvertFromFile::convert(const Resource &res, const ResourceType &type)
{
	FileResource file(res, DONT_CONVERT);
	if (file && (type & FITextureData::getName() ||
		fipImage::identifyFIF(file->getPath().native_file_string().c_str()) != FIF_UNKNOWN))
	{
		FITextureData *pData = new FITextureData(this);
		pData->setId(ResourceId(FITextureData::getName(), res->getId().getName()));
		pData->setSource(res);
		return FITextureResource(pData);
	}
	return Resource();
}

bool FITextureConverters::ConvertFromFile::load(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::load(res, flags))
		return true;

	FileResource file = res->getSource();
	if (!file->isLoaded())
		return false;

	FITextureResource texture(res);
	fipImage &img = texture->getTexture();

	if (!img.load(file->getPath().native_file_string().c_str()))
	{
		return false;
	}

	texture->setLoaded(true);

	return true;
}

bool FITextureConverters::ConvertFromFile::unload(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::unload(res, flags))
		return true;

	FITextureResource texture(res);
	fipImage &img = texture->getTexture();

	img.clear();

	texture->setLoaded(false);

	return true;
}