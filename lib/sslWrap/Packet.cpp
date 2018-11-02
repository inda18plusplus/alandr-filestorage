//
// Created by lunar on 27/10/18.
//

#include "Packet.hpp"

#include <cstring>
#include <arpa/inet.h>

namespace SSLWrap {

	void Packet::append(const void *data, std::size_t size) {

		if(!data || size <= 0) {
			return;
		}

		std::size_t startSize = _data.size();
		_data.resize(startSize + size);
		memcpy(_data.data() + startSize, data, size);

	}

	Packet& Packet::operator<<(int32_t i) {
		i = htonl(i);
		append(&i, sizeof(i));

		return *this;
	}
	Packet& Packet::operator>>(int32_t &i) {
		i = ntohl(*reinterpret_cast<const int32_t*>(_data.data() + _readPos));
		_readPos += sizeof(i);

		return *this;
	}
	Packet& Packet::operator<<(uint32_t i) {
		i = htonl(i);
		append(&i, sizeof(i));

		return *this;
	}
	Packet& Packet::operator>>(uint32_t &i) {
		i = ntohl(*reinterpret_cast<const uint32_t*>(_data.data() + _readPos));
		_readPos += sizeof(i);

		return *this;
	}

	Packet& Packet::operator<<(const std::vector<unsigned char> &data) {
		*this << (uint32_t)data.size();

		append(data.data(), data.size());

		return *this;
	}
	Packet& Packet::operator>>(std::vector<unsigned char> &data) {
		data.clear();

		uint32_t len;
		*this >> len;

		data.assign(_data.begin() + _readPos, _data.begin() + (_readPos + len));
		_readPos += len;

		return *this;
	}

	Packet& Packet::operator<<(const char *c) {
		uint32_t len = strlen(c);
		*this << len;

		append(c, len);

		return *this;
	}
	Packet& Packet::operator>>(char *c) {
		uint32_t len;
		*this >> len;

		memcpy(c, _data.data() + _readPos, len);
		c[len] = 0;

		_readPos += len;

		return *this;
	}
	Packet& Packet::operator<<(const std::string &str) {
		*this << (uint32_t)str.length();

		append(str.c_str(), str.length());

		return *this;
	}
	Packet& Packet::operator>>(std::string &str) {
		str.clear();

		int len;
		*this >> len;

		str.assign(reinterpret_cast<char*>(_data.data() + _readPos), len);
		_readPos += len;

		return *this;
	}

}