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

#include "DelayMac.hpp"
#include "MacId.hpp"
#include "IArq.hpp"
#include "IPhy.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;
using namespace std;

DelayMac::DelayMac(const MacId& id) : IMac(id), nextMacId(-1) {

}

void DelayMac::notifyOutgoing(unsigned long num_bits, const MacId& mac_id) {
	nextPktSize = num_bits;
	nextMacId = mac_id;
}

void DelayMac::onEvent(double time) {
}

void DelayMac::passToLower(L2Packet* packet, unsigned int center_frequency) {
	debug("PASSING DOWN");
	IPhy* phy = getLowerLayer();
	phy->receiveFromUpper(packet, center_frequency);
}

void DelayMac::passToUpper(L2Packet* packet) {
	IArq* arq = getUpperLayer();
	arq->receiveFromLower(packet);
}

void DelayMac::injectIntoUpper(L2Packet* packet) {

}

void DelayMac::receiveFromLower(L2Packet* packet, uint64_t center_frequency) {
	IArq* arq = getUpperLayer();
	arq->receiveFromLower(packet);
}

void DelayMac::update(uint64_t num_slots) {}


void DelayMac::execute() {

    if(++counter % 10 != 0 ||  nextPktSize == 0) {
        return;
    }
    auto *packet = new L2Packet();

    auto *header = new L2HeaderSH();
    header->src_id = id;

    IArq* arq = getUpperLayer();
    auto upper_layer_data = arq->requestSegment(nextPktSize + 155 + 77, nextMacId);

    packet->addMessage(header, nullptr);

    for (size_t i = 0; i < upper_layer_data->getPayloads().size(); i++) {
        if (upper_layer_data->getHeaders().at(i)->frame_type != L2Header::base) {
            packet->addMessage(upper_layer_data->getHeaders().at(i)->copy(), upper_layer_data->getPayloads().at(i)->copy());
        }
    }

    passToLower(packet, 0);
}

void DelayMac::onSlotEnd() {

}
