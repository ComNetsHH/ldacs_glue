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

#include "IPhy.hpp"
#include "IMac.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

void IPhy::onReception(L2Packet* packet, uint64_t center_frequency) {
	assert(upper_layer && "IPhy::onReception for unset upper layer.");
	upper_layer->receiveFromLower(packet, center_frequency);
}

IMac* IPhy::getUpperLayer() {
	return this->upper_layer;
}

void IPhy::setUpperLayer(IMac* mac) {
	this->upper_layer = mac;
}

void IPhy::setRadio(IRadio* radio) {
	this->radio = radio;
}

void IPhy::tuneReceiver(uint64_t center_frequency) {
	if (rx_frequencies.size() == num_receivers)
		throw std::runtime_error("IPhy::tuneReceiver exceeds number of available receivers.");
	rx_frequencies.push_back(center_frequency);
}

void IPhy::update(uint64_t num_slots) {
	rx_frequencies.clear();
}
