//
// Created by lunar on 26/10/18.
//

#include "IpAddress.hpp"

#include <arpa/inet.h>

namespace SSLWrap {

	const IpAddress IpAddress::None;
	const IpAddress IpAddress::Any(0,0,0,0);
	const IpAddress IpAddress::Loopback(127,0,0,1);
	const IpAddress IpAddress::Broadcast(255,255,255,255);

	IpAddress::IpAddress() : _addr({0}), _isValid(false) {

	}

	IpAddress::IpAddress(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) :
		IpAddress(b0 << 24 | b1 << 16 | b2 << 8 | b3 << 0) {

	}

	IpAddress::IpAddress(uint32_t ip) : _addr({htonl(ip)}), _isValid(true) {

	}

	IpAddress::IpAddress(const std::string &addr) {

		in_addr newAddr;

		if(inet_aton(addr.c_str(), &newAddr) != -1) {

			_addr = newAddr;
			_isValid = true;

		} else {

			_addr = {0};
			_isValid = false;

		}

	}

}