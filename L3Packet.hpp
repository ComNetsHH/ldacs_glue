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

#ifndef INTAIRNET_LINKLAYER_GLUE_L3PACKET_HPP
#define INTAIRNET_LINKLAYER_GLUE_L3PACKET_HPP

#include "MacId.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;
namespace inet {
	class Packet;
}

class L3Packet {
public:
	/** default constructor **/
	L3Packet();

	/** packet id */
	unsigned int packetId = 0;

	/** Size of the contained inet packet **/
	int size = 0;

	/** Offset shows how much data was already fragmented and put into a L2Packet **/
	int offset = 0;

	/** The MacId resolved from the NW layer**/
	MacId dest;

	/** Pointer to the original data **/
	inet::Packet* original;
};

#endif //INTAIRNET_LINKLAYER_GLUE_L3PACKET_HPP
