//
// Created by lunar on 27/10/18.
//

#ifndef FILESTORAGE_FILE_HPP
#define FILESTORAGE_FILE_HPP

#include <string>
#include <cstdio>
#include <filesystem>

#include <SSLWrap/Packet.hpp>

class File {

	int _id = -1;

	std::string _data;
	std::string _dataDecr;

	static const size_t _ivSize = 12;
	unsigned char* _iv = nullptr;
	static const size_t _tagSize = 16;

	std::string _name;

	void generateIV();

public:
	File();
	File(const std::string &name);
	File(File&& f);

	void readEncrypted(std::filesystem::path path);
	void writeEncrypted(std::filesystem::path path);

	~File();

	void dump();
	void dump(std::filesystem::path outPath);

	//size_t size() const { return _size; }

	int id() const { return _id; }
	void setId(int id) { _id = id; }

	const std::string& name() const { return _name; }
	void setName(const std::string& name) { _name = name; }

	void encrypt(unsigned char *key);
	void decrypt(unsigned char *key);

	const std::string& data() { return _data; }

	friend SSLWrap::Packet& operator<<(SSLWrap::Packet& p, const File& f);
	friend SSLWrap::Packet& operator>>(SSLWrap::Packet& p, File& f);

};

SSLWrap::Packet& operator<<(SSLWrap::Packet& p, const File& f);
SSLWrap::Packet& operator>>(SSLWrap::Packet& p, File& f);

#endif //FILESTORAGE_FILE_HPP
