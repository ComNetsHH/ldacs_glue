//
// Created by Sebastian Lindner on 14.12.20.
//

#include "IPhy.hpp"
#include "IMac.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

void IPhy::onReception(L2Packet* packet, uint64_t center_frequency) {
	assert(upper_layer && "IPhy::onReception for unset upper layer.");
	upper_layer->receiveFromLower(packet, center_frequency);
}

bool IPhy::isTransmitterIdle(unsigned int slot_offset, unsigned int num_slots) const {
	// Should check whether the transmitter is utilized by either A2G or A2A during the specified time slots.
	return true;
}

IMac* IPhy::getUpperLayer() {
	return this->upper_layer;
}

void IPhy::setUpperLayer(IMac* mac) {
	this->upper_layer = mac;
}
