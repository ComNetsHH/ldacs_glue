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

#include "PassThroughArq.hpp"
#include "IMac.hpp"
#include "IRlc.hpp"
#include <string>

using namespace TUHH_INTAIRNET_MCSOTDMA;

void PassThroughArq::notifyOutgoing(unsigned int num_bits, const MacId& mac_id) {
	debug("PassThroughArq::notifyOutgoing " + std::to_string(num_bits));
	IMac* mac = getLowerLayer();
	mac->notifyOutgoing(num_bits, mac_id);
}

L2Packet* PassThroughArq::requestSegment(unsigned int num_bits, const MacId& mac_id) {
	debug("PassThroughArq::requestSegment " + std::to_string(num_bits));
	IRlc* rlc = getUpperLayer();
	return rlc->requestSegment(num_bits, mac_id);
}

bool PassThroughArq::shouldLinkBeArqProtected(const MacId& mac_id) const {
	return false;
}

void PassThroughArq::injectIntoUpper(L2Packet* packet) {
	debug("PassThroughArq::injectIntoUpper");
	IRlc* rlc = getUpperLayer();
	return rlc->receiveInjectionFromLower(packet);
}

void PassThroughArq::receiveFromLower(L2Packet* packet) {
	debug("PassThroughArq::receiveFromLower");
	IRlc* rlc = getUpperLayer();
	return rlc->receiveFromLower(packet);
}

void PassThroughArq::notifyAboutNewLink(const MacId& id) {
	return;
}

void PassThroughArq::notifyAboutRemovedLink(const MacId& id) {
	return;
}

void PassThroughArq::processIncomingHeader(L2Packet* incoming_packet) {
	return;
}

void PassThroughArq::onEvent(double time) {

}
