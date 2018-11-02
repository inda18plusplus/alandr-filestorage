//
// Created by lunar on 26/10/18.
//

#ifndef FILESTORAGE_IPADDRESS_HPP
#define FILESTORAGE_IPADDRESS_HPP

#include <string>
#include <cstdint>

#include <arpa/inet.h>

namespace SSLWrap {

	class IpAddress {

		in_addr _addr; //The ip in network order
		bool _isValid;

	public:
		IpAddress();
		IpAddress(uint32_t ip);
		IpAddress(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3);
		IpAddress(const std::string& addr);

		in_addr addr() const { return _addr; }

		static const IpAddress None;
		static const IpAddress Any;
		static const IpAddress Loopback;
		static const IpAddress Broadcast;

	};

}

#endif //FILESTORAGE_IPADDRESS_HPP
