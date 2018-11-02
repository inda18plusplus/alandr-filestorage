//
// Created by lunar on 31/10/18.
//

#include "Commands.hpp"

SSLWrap::Packet& operator<<(SSLWrap::Packet& p, COMMAND c) {
	p << (int)c;

	return p;
}
SSLWrap::Packet& operator>>(SSLWrap::Packet& p, COMMAND& c) {
	p >> (int&)c;

	return p;
}
