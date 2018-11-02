//
// Created by lunar on 27/10/18.
//

#ifndef FILESTORAGE_PACKET_HPP
#define FILESTORAGE_PACKET_HPP

#include <vector>
#include <string>

#include <cstdint>

namespace SSLWrap {

	class Socket;

	class Packet {
		friend class Socket;

		std::vector<unsigned char> _data;
		std::size_t _readPos = 0;

	public:
		Packet() = default;

		void append(const void* data, std::size_t size);
		void clear() { _data.clear(); _readPos = 0; };

		const void* data() const { return _data.empty() ? nullptr : _data.data(); }
		std::size_t size() const { return _data.size(); }

		Packet& operator<<(int32_t i); //This style shamelessly stolen from SFML
		Packet& operator>>(int32_t& i);
		Packet& operator<<(uint32_t i);
		Packet& operator>>(uint32_t& i);

		Packet& operator<<(const std::vector<unsigned char>& data);
		Packet& operator>>(std::vector<unsigned char>& data);

		Packet& operator<<(const char* c);
		Packet& operator>>(char* c);
		Packet& operator<<(const std::string& str);
		Packet& operator>>(std::string& str);

	};

}


#endif //FILESTORAGE_PACKET_HPP
