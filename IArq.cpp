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

#include <cassert>
#include "IArq.hpp"
#include "IRlc.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

void IArq::injectIntoUpper(L2Packet* packet) {
	assert(this->upper_layer && "IArq::injectIntoUpper called but upper layer is unset.");
	// Just forward it.
	this->upper_layer->receiveInjectionFromLower(packet);
}

void IArq::receiveFromLower(L2Packet* packet) {
	assert(this->upper_layer && "IArq::receiveFromLower called but upper layer is unset.");
	// Parse header and update state.
	processIncomingHeader(packet);
	// Pass it on.
	this->upper_layer->receiveFromLower(packet);
}

unsigned int IArq::getNumHopsToGS() const {
	assert(this->upper_layer && "IArq::getNumHopsToGS called but upper layer is unset.");
	return upper_layer->getNumHopsToGS();
}

void IArq::reportNumHopsToGS(const MacId& id, unsigned int num_hops) const {
	assert(this->upper_layer && "IArq::reportNumHopsToGS called but upper layer is unset.");
	upper_layer->reportNumHopsToGS(id, num_hops);
}

bool IArq::isThereMoreData(const MacId& mac_id) const {
	assert(this->upper_layer && "IArq::isThereMoreData called but upper layer is unset.");
	return upper_layer->isThereMoreData(mac_id);
}

size_t IArq::getMaxNumRtxAttempts() const {
	return this->max_num_rtx_attempts;
}