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

IMac* IPhy::getUpperLayer() {
	return this->upper_layer;
}

void IPhy::setUpperLayer(IMac* mac) {
	this->upper_layer = mac;
}

void IPhy::setRadio(IRadio *radio) {
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
