#include "FileResourceLoader.h"

#include "resources/Resource.h"
#include "resources/FileResource.h"

namespace fs = boost::filesystem;

using namespace resources;
using namespace resources::loaders;

//////////////////////////////////////////////////////////////////////////

Resource FileResourceLoader::get(const ResourceId &id)
{
	if (id.getType() & "File")
	{
		fs::path p((m_basePath / id.getName()).native_directory_string());

		if (fs::exists(p))
		{
			FileData *pData = new FileData(this);
			pData->setId(id);
			pData->setPath(p);
			return FileResource(pData);
		}
	}
	return Resource();
}

bool FileResourceLoader::load(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::load(res, flags))
		return true;

	FileResource file = res;
	res->setLoaded(fs::exists(file->getPath()));
	return res->isLoaded();
}

bool FileResourceLoader::unload(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::unload(res, flags))
		return true;

	return true;
}
