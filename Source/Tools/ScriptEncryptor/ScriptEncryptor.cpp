//
// Copyright (c) 2008-2017 Flock SDK developers & contributors. 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 

#include <Flock/Core/Platform.h>
#include <Flock/Engine/Engine.h>
#include <Flock/IO/FileSystem.h>
#include <Flock/IO/Log.h>
#include <Flock/LuaScript/LuaScript.h>
#include <Flock/Resource/ResourceCache.h>
#include <Flock/Resource/ResourceEvents.h>

#include "ScriptEncryptor.h"

#include <CryptoPP/osrng.h>
using CryptoPP::AutoSeededRandomPool;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include <CryptoPP/cryptlib.h>
using CryptoPP::Exception;

#include <CryptoPP/hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include <CryptoPP/filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include <CryptoPP/aes.h>
using CryptoPP::AES;

#include <CryptoPP/modes.h>
using CryptoPP::CFB_Mode;

ScriptEncryptor::ScriptEncryptor(FlockSDK::Context* context) : FlockSDK::Application(context) {}

void ScriptEncryptor::Setup()
{
	AutoSeededRandomPool prng;
	byte key[AES::DEFAULT_KEYLENGTH];
	prng.GenerateBlock(key, sizeof(key));

	byte iv[AES::BLOCKSIZE];
	prng.GenerateBlock(iv, sizeof(iv));

	string plain = "CFB Mode Test";
	string cipher, encoded, recovered;

	// Pretty print key
	encoded.clear();
	StringSource(key, sizeof(key), true, new HexEncoder(new StringSink(encoded)));
	cout << "key: " << encoded << endl;

	// Pretty print iv
	encoded.clear();
	StringSource(iv, sizeof(iv), true, new HexEncoder(new StringSink(encoded)));
	cout << "iv: " << encoded << endl;

	try
	{
		cout << "plain text: " << plain << endl;

		CFB_Mode< AES >::Encryption e;
		e.SetKeyWithIV(key, sizeof(key), iv);

		// CFB mode must not use padding. Specifying
		//  a scheme will result in an exception
		StringSource(plain, true, new StreamTransformationFilter(e, new StringSink(cipher)));
	}
	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	// Pretty print
	encoded.clear();
	StringSource(cipher, true, new HexEncoder(new StringSink(encoded)));
	cout << "cipher text: " << encoded << endl;

	try
	{
		CFB_Mode< AES >::Decryption d;
		d.SetKeyWithIV(key, sizeof(key), iv);

		// The StreamTransformationFilter removes
		//  padding as required.
		StringSource s(cipher, true, new StreamTransformationFilter(d, new StringSink(recovered)));
		cout << "recovered text: " << recovered << endl;
	}
	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}
} 

void ScriptEncryptor::Start()
{
}

void ScriptEncryptor::Stop()
{
}

void ScriptEncryptor::Exit(void) 
{
}

int main(int argc, char **argv) 
{
    auto *DPContext = new FlockSDK::Context();
    auto *DPGame    = new ScriptEncryptor(DPContext);
    DPGame->argc_   = argc;
    DPGame->argv_   = (argc > 1 && argv[1]) ? argv[1] : FlockSDK::String::EMPTY;

    #if defined(FLOCK_SECURITY) // Which means FLOCK_SECURITY_KEY was also defined through CMake.
      FLOCKSDK_LOGINFO(FlockSDK::String("FLOCK_SECURITY_KEY: ") + FLOCK_SECURITY_KEY);
    #endif

    return (FlockSDK::SharedPtr<ScriptEncryptor>(DPGame))->Run();
} 
