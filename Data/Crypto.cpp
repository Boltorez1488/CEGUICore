#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <modes.h>
#include <md5.h>
#include <files.h>
#include <hex.h>
#include <aes.h>
using namespace std;
using namespace CryptoPP;
using namespace CryptoPP::Weak;

#include "../Header.h"

string Unpack(CEGUI::String fname, byte* key_b, size_t size)
{
	byte key[AES::BLOCKSIZE];
	byte iv[AES::BLOCKSIZE];
	memset(iv, 0x00, AES::BLOCKSIZE);

	MD5 hash;
	hash.Update(key_b, size);
	hash.Final(key);

	string data;

	CFB_Mode< AES >::Decryption d(key, sizeof(key), iv);
	FileSource sb(fname.c_str(), true, new StreamTransformationFilter(d, new StringSink(data)));

	return data;
}