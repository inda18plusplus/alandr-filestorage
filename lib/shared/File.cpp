//
// Created by lunar on 27/10/18.
//

#include "File.hpp"

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <random>
#include <vector>

#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

#include <openssl/evp.h>

File::File() {
	_iv = new unsigned char[_ivSize];
}

File::File(const std::string &name) : _name(name) {

	_iv = new unsigned char[_ivSize];

	FILE* fp = fopen(name.c_str(), "rb");

	fseek(fp, 0, SEEK_END);
	size_t sz = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	_dataDecr.resize(sz);

	fread(_dataDecr.data(), 1, sz, fp);

	fclose(fp);

}

File::File(File &&f) {

	_id = f._id;

	_data = std::move(f._data);
	_dataDecr = std::move(f._dataDecr);

	_iv = f._iv;

	_name = std::move(f._name);

	f._iv = nullptr;

}

File::~File() {

	delete[] _iv;

}

void File::dump() {

	dump({});

}

void File::dump(std::filesystem::path outPath) {
	FILE* fp = fopen(
			(outPath / std::filesystem::path(_name)).string().c_str(),
			"wb");

	fwrite(_dataDecr.data(), 1, _dataDecr.size(), fp);

	fclose(fp);
}

void File::readEncrypted(std::filesystem::path path) {
	FILE* fd = fopen(path.string().c_str(), "rb");

	fread(&_id, sizeof(_id), 1, fd);

	int namezs;
	fread(&namezs, sizeof(namezs), 1, fd);
	_name.resize(namezs);
	fread(_name.data(), sizeof(char), namezs, fd);

	fread(_iv, sizeof(char), _ivSize, fd);

	int filesz;
	fread(&filesz, sizeof(filesz), 1, fd);
	_data.resize(filesz);
	fread(_data.data(), sizeof(char), filesz, fd);

	fclose(fd);
}
void File::writeEncrypted(std::filesystem::path path) {
	FILE* fd = fopen(path.string().c_str(), "wb");

	fwrite(&_id, sizeof(_id), 1, fd);

	int namezs = _name.length();
	fwrite(&namezs, sizeof(namezs), 1, fd);
	fwrite(_name.data(), sizeof(char), namezs, fd);

	fwrite(_iv, sizeof(char), _ivSize, fd);

	int filesz = _data.length();
	fwrite(&filesz, sizeof(filesz), 1, fd);
	fwrite(_data.data(), sizeof(char), filesz, fd);

	fclose(fd);
}

void File::generateIV() {
	std::random_device rd;
	std::mt19937 rng(rd());

	std::uniform_int_distribution<unsigned char> dist;

	for (size_t i = 0; i < _ivSize; ++i) {
		_iv[i] = dist(rng);
	}
}

void File::encrypt(unsigned char *key) {

	generateIV();

	std::stringstream ss;
	ss << _id << " " << _dataDecr;

	CryptoPP::GCM<CryptoPP::AES>::Encryption e;
	e.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, _iv, _ivSize);

	auto aef = new CryptoPP::AuthenticatedEncryptionFilter(e, new CryptoPP::StringSink(_data), false, _tagSize);
	CryptoPP::StringSource(ss.str(), true, aef);

}

void File::decrypt(unsigned char *key) {

	std::stringstream decrSS;

	CryptoPP::GCM<CryptoPP::AES>::Decryption d;
	d.SetKeyWithIV(key, CryptoPP::AES::DEFAULT_KEYLENGTH, _iv, _ivSize);

	auto adf = new CryptoPP::AuthenticatedDecryptionFilter(
			d,
			new CryptoPP::FileSink(decrSS),
			CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS,
			_tagSize
	);

	try {
		CryptoPP::StringSource(_data, true, adf);
	} catch (CryptoPP::Exception& e) {
		if(e.GetErrorType() == CryptoPP::Exception::DATA_INTEGRITY_CHECK_FAILED){
			std::cerr <<
			          "File verification failed, make sure you put in the right password "
			          "and that the server hasn't mucked with the file!\n\n";
		} else {
			std::cerr << e.what() << "\n\n";
			throw e;
		}
	}

	int encId;
	decrSS >> encId;

	if(_id != encId) {
		std::cerr << "File id doesn't match encrypted id!\n\n";
	}

	decrSS >> _dataDecr;

}

SSLWrap::Packet& operator<<(SSLWrap::Packet& p, const File& f) {

	std::vector<unsigned char> IVData(File::_ivSize, '\0');
	memcpy(IVData.data(), f._iv, File::_ivSize);

	p << f._id << f._name << IVData << f._data;

	return p;

}
SSLWrap::Packet& operator>>(SSLWrap::Packet& p, File& f) {

	std::vector<unsigned char> IVData;

	p >> f._id >> f._name >> IVData >> f._data;

	f._iv = new unsigned char[File::_ivSize];

	memcpy(f._iv, IVData.data(), File::_ivSize);

	return p;

}