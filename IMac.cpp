//
// Created by Sebastian Lindner on 01.12.20.
//

#include <cassert>
#include "IMac.hpp"
#include "IArq.hpp"
#include "IPhy.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

IMac::IMac(const MacId& id) : id(id), position_map() {
	updatePosition(id, CPRPosition(0, 0, 0));
}

void IMac::injectIntoUpper(L2Packet* packet) {
	assert(this->upper_layer && "IMac::injectIntoUpper called but upper layer is unset.");
	// Just forward it.
	this->upper_layer->injectIntoUpper(packet);
}

L2Packet* IMac::requestSegment(unsigned int num_bits, const MacId& mac_id) {
	return upper_layer->requestSegment(num_bits, mac_id);
}

bool IMac::shouldLinkBeArqProtected(const MacId& mac_id) const {
	assert(upper_layer && "MCSOTDMA_Mac's upper layer is unset.");
	return this->upper_layer->shouldLinkBeArqProtected(mac_id);
}

unsigned long IMac::getCurrentDatarate() const {
	assert(lower_layer && "MCSOTDMA_Mac::getCurrentDatarate for unset PHY layer.");
	return lower_layer->getCurrentDatarate();
}

unsigned int IMac::getNumHopsToGS() const {
	assert(upper_layer && "MCSOTDMA_Mac::getNumHopsToGS for unset ARQ layer.");
	return upper_layer->getNumHopsToGS();
}

const CPRPosition& IMac::getPosition(const MacId& id) const {
	try {
		return position_map.at(id);
	} catch (const std::out_of_range& e) {
		throw std::out_of_range("MCSOTDMA_Mac::getPosition for unknown ID: " + std::string(e.what()));
	}
}

void IMac::updatePosition(const MacId& id, const CPRPosition& position) {
	position_map[id] = position;
}
