#include "ZipProvider.h"
#include <minizip/unzip.h>
#include <minizip/ioapi_mem.h>

ZipProvider::ZipProvider(const String& arc_name)
{
	zipstream = NULL;
	isCrypto = false;
	setArchive(arc_name);
}

ZipProvider::ZipProvider(const String& crypto_name, byte* k, size_t size)
{
	isCrypto = true;
	zipstream = NULL;
	key = k;
	key_size = size;
	setArchive(crypto_name);
}

ZipProvider::~ZipProvider()
{
	if(zipstream)
		closeArchive();
}

void ZipProvider::setKey(BYTE* k, size_t k_size)
{
	key = k;
	key_size = k_size;
}

#include <shlwapi.h>
#define FNMATCH(p, s)    PathMatchSpec(s, p)
#pragma comment(lib, "shlwapi.lib")
bool nameMatchesPattern(const String& name, const String& pattern)
{
	return !FNMATCH(pattern.c_str(), name.c_str());
}


void ZipProvider::setArchive(const String& archive)
{
	if (zipstream)
		closeArchive();
	arc_name = archive;
	openArchive();
}

void ZipProvider::loadRawDataContainer(const String& filename, RawDataContainer& output, const String& resourceGroup)
{
	const String final_filename = getFinalFilename(filename, resourceGroup);

	if (zipstream == 0)
	{
		CEGUI_THROW(InvalidRequestException(
			"'" + final_filename + "' cannot be "
			"loaded because the archive has not been set"));
	}

	Poco::Zip::ZipArchive zip(*zipstream);

	Poco::Zip::ZipArchive::FileHeaders::const_iterator it = zip.findHeader(final_filename.c_str());

	if (it == zip.headerEnd())
	{
		CEGUI_THROW(InvalidRequestException("'" + final_filename +
			"' does not exist"));
	}

	Poco::Zip::ZipInputStream zipin(*zipstream, it->second);
	ostringstream out(ios::binary);
	Poco::StreamCopier::copyStream(zipin, out);

	size_t size = out.str().size();
	uint8* buffer = CEGUI_NEW_ARRAY_PT(uint8, size, RawDataContainer);

	memcpy(buffer, out.str().c_str(), size);

	output.setData(buffer);
	output.setSize(size);

	zipstream->seekg(0, ios::beg);
}

size_t ZipProvider::getResourceGroupFileNames(std::vector<String>& out_vec, const String& file_pattern, const String& resource_group)
{
	// look-up resource group name
	ResourceGroupMap::const_iterator iter =
		d_resourceGroups.find(resource_group.empty() ? d_defaultResourceGroup :
			resource_group);
	// get directory that's set for the resource group
	const String dir_name(
		iter != d_resourceGroups.end() ? (*iter).second : "");

	size_t entries = 0;

	// exit now if no zip file is loaded
	if (!zipstream)
		return entries;

	Poco::Zip::ZipArchive zip(*zipstream);
	Poco::Zip::ZipArchive::FileHeaders::const_iterator it = zip.headerBegin();
	for (; it != zip.headerEnd(); ++it)
	{
		Poco::Zip::ZipInputStream zipis(*zipstream, it->second);
		auto fname = it->second.getFileName();
		if (!nameMatchesPattern(fname, dir_name + file_pattern))
			continue;
		{
			std::ofstream os("test.dat");
			Poco::StreamCopier::copyStream(zipis, os);
		}
		out_vec.push_back(String(fname).substr(dir_name.length()));
		entries++;
	}

	return entries;
}

bool ZipProvider::doesFileExist(const String& filename)
{
	std::ifstream dataFile(filename.c_str(), std::ios::binary | std::ios::ate);

	if (dataFile)
		return true;

	return false;
}

extern string Unpack(const CEGUI::String fname, byte* key, size_t size);
void ZipProvider::openArchive()
{
	if(isCrypto)
	{
		auto data = Unpack(arc_name, key, key_size);

		zipstream = new istringstream(data, ios::binary);

		if (zipstream->bad())
		{
			CEGUI_THROW(InvalidRequestException(
				"'" + arc_name + "' does not exist"));
		}
		return;
	}
	
	zipstream = new ifstream(arc_name.c_str(), ios::binary);

	if (zipstream->bad())
	{
		CEGUI_THROW(InvalidRequestException(
			"'" + arc_name + "' does not exist"));
	}
}

void ZipProvider::closeArchive()
{
	delete zipstream;
	zipstream = NULL;
}
