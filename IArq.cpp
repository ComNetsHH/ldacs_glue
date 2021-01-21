//
// Created by Sebastian Lindner on 01.12.20.
//

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

