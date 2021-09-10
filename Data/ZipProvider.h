#pragma once
#include "../Header.h"
#include <Poco/Zip/ZipArchive.h>
#include <Poco/Zip/Decompress.h>
#include <Poco/StreamCopier.h>
#include <Poco/Zip/ZipStream.h>

using namespace CEGUI;

class ZipProvider : public DefaultResourceProvider
{
public:
	ZipProvider(const String& arc_name);
	ZipProvider(const String& crypto_name, byte* key, size_t key_size);
	~ZipProvider();

	void setKey(BYTE* key, size_t key_size);

	void setArchive(const String& archive);

	void loadRawDataContainer(const String& filename,
		RawDataContainer& output,
		const String& resourceGroup) override;
	size_t getResourceGroupFileNames(std::vector<String>& out_vec,
		const String& file_pattern,
		const String& resource_group) override;

private:
	static bool doesFileExist(const String& filename);
	void openArchive();
	void closeArchive();

	String arc_name;
	istream* zipstream;
	
	bool isCrypto;
	BYTE* key;
	size_t key_size;
};

