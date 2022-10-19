#pragma once

#include "Enums.h"
#include <stdint.h>

class SocketAddress;
class EndPoint
{
public:
	virtual AddressFamily GetAddressFamily() const = 0;

	virtual SocketAddress Serialize() const = 0;

	virtual uint64_t GetHashCode() const = 0;

public:
	EndPoint() {}
	virtual ~EndPoint() = 0 {}
};