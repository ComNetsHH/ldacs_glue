// The L-Band Digital Aeronautical Communications System (LDACS) Link Layer Glue Library provides interfaces and common classes necessary for the LDACS Air-Air Medium Access Control simulator.
// Copyright (C) 2023  Sebastian Lindner, Konrad Fuger, Musab Ahmed Eltayeb Ahmed, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef TUHH_INTAIRNET_INETPACKETPAYLOAD_HPP
#define TUHH_INTAIRNET_INETPACKETPAYLOAD_HPP

#include "L2Packet.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

namespace inet {
    class Packet;
}


class InetPacketPayload : public L2Packet::Payload {
public:
    inet::Packet* original = nullptr;
    unsigned int size = 0;
    unsigned int offset = 0;
    unsigned int getBits() const override;

    Payload* copy() const override;
    ~InetPacketPayload();
};


#endif //TUHH_INTAIRNET_INETPACKETPAYLOAD_HPP
