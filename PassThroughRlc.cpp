// The L-Band Digital Aeronautical Communications System (LDACS) Link Layer Glue Library provides interfaces and common classes necessary for the LDACS Air-Air Medium Access Control simulator.
// Copyright (C) 2023  Sebastian Lindner, Konrad Fuger, Musab Ahmed Eltayeb Ahmed, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "PassThroughRlc.hpp"
#include <string>
#include <iostream>
#include "IArq.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;


void PassThroughRlc::init() {
	double time = getTime() + 1;
	debug("TEST " + std::to_string(time));
	emit("x", 1.0);

}

void PassThroughRlc::onEvent(double time) {

}

void PassThroughRlc::receiveFromUpper(L3Packet* data, MacId dest, PacketPriority priority) {
	networkLayerPackets.push_back(data);
	emit("rlc_nw_queue", size_t(100));
	debug("rlc_nw_queue");
	IArq* arq = getLowerLayer();
	arq->notifyOutgoing(100, dest);

}

void PassThroughRlc::receiveFromLower(L2Packet* packet) {
	linkLayerPackets.push_back(packet);

}

void PassThroughRlc::receiveInjectionFromLower(L2Packet* packet, PacketPriority priority) {

}

L2Packet* PassThroughRlc::requestSegment(unsigned int num_bits, const MacId& mac_id) {
	auto packet = new L2Packet();
	return packet;
}

