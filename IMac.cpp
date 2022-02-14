//
// Created by Sebastian Lindner on 01.12.20.
//

#include <cassert>
#include "IMac.hpp"
#include "IArq.hpp"
#include "IPhy.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

IMac::IMac(const MacId& id) : id(id), position_map(), position_quality_map() {
	updatePosition(id, CPRPosition(), CPRPosition::PositionQuality::hi);
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

void IMac::reportNumHopsToGS(const MacId& id, unsigned int num_hops) const {
	assert(upper_layer && "MCSOTDMA_Mac::getNumHopsToGS for unset ARQ layer.");
	upper_layer->reportNumHopsToGS(id, num_hops);
}

const CPRPosition& IMac::getPosition(const MacId& id) const {
	try {
		return position_map.at(id);
	} catch (const std::out_of_range& e) {
		throw std::out_of_range("MCSOTDMA_Mac::getPosition for unknown ID: " + std::string(e.what()));
	}
}

void IMac::updatePosition(const MacId& id, const CPRPosition& position, CPRPosition::PositionQuality pos_quality) {
	position_map[id] = position;
	position_quality_map[id] = pos_quality;
}

CPRPosition::PositionQuality IMac::getPositionQuality(const MacId& id) const {
	try {
		return position_quality_map.at(id);
	} catch (const std::out_of_range& e) {
		throw std::out_of_range("MCSOTDMA_Mac::getPositionQuality for unknown ID: " + std::string(e.what()));
	}
}

bool IMac::isTransmitterIdle(unsigned int slot_offset, unsigned int num_slots) const {
	assert(lower_layer && "IMac::isTransmitterIdle for unset lower layer.");
	return lower_layer->isTransmitterIdle(slot_offset, num_slots);
}

bool IMac::isAnyReceiverIdle(unsigned int slot_offset, unsigned int num_slots) const {
	assert(lower_layer && "IMac::isAnyReceiverIdle for unset lower layer.");
	return lower_layer->isAnyReceiverIdle(slot_offset, num_slots);
}

void IMac::update(uint64_t num_slots) {
	current_slot += num_slots;
}

uint64_t IMac::getCurrentSlot() const {
	return current_slot;
}

bool IMac::isThereMoreData(const MacId& mac_id) const {
	assert(upper_layer && "IMac::isThereMoreData for unset upper layer.");
	return upper_layer->isThereMoreData(mac_id);
}

void IMac::notifyAboutNewLink(const MacId& id) {
	assert(upper_layer && "IMac::notifyAboutNewLink for unset upper layer.");
	upper_layer->notifyAboutNewLink(id);
}

void IMac::notifyAboutRemovedLink(const MacId& id) {
	assert(upper_layer && "IMac::notifyAboutRemovedLink for unset upper layer.");
	upper_layer->notifyAboutRemovedLink(id);
}

void IMac::setBroadcastTargetCollisionProb(double value) {
	throw std::runtime_error("setBroadcastTargetCollisionProb not implemented");
}

void IMac::setBcSlotSelectionMinNumCandidateSlots(int value) {
	throw std::runtime_error("setBcSlotSelectionMinNumCandidateSlots not implemented");
}

void IMac::setBcSlotSelectionMaxNumCandidateSlots(int value) {
	throw std::runtime_error("setBcSlotSelectionMaxNumCandidateSlots not implemented");
}

void IMac::setContentionMethod(ContentionMethod method) {
	throw std::runtime_error("setContentionMethod not implemented");
}

void IMac::setAlwaysScheduleNextBroadcastSlot(bool value) {
	throw std::runtime_error("setAlwaysScheduleNextBroadcastSlot not implemented");
}

void IMac::setMinBeaconOffset(unsigned int value) {
	throw std::runtime_error("setMinBeaconOffset not implemented");
}

void IMac::setMaxBeaconOffset(unsigned int value) {
	throw std::runtime_error("setMaxBeaconOffset not implemented");
}

void IMac::onBeaconReception(MacId origin_id, L2HeaderBeacon header) {
	passUpBeaconFct(origin_id, header);
}

void IMac::setForceBidirectionalLinks(bool flag) {
	this->should_force_bidirectional_links = flag;
}

void IMac::setWriteResourceUtilizationIntoBeacon(bool flag) {
	throw std::runtime_error("setWriteResourceUtilizationIntoBeacon not implemented");
}

void IMac::setEnableBeacons(bool flag) {
	throw std::runtime_error("setEnableBeacons not implemented");
}

void IMac::setAdvertiseNextBroadcastSlotInCurrentHeader(bool flag) {
	throw std::runtime_error("setAdvertiseNextBroadcastSlotInCurrentHeader not implemented");
}

void IMac::setPPLinkBurstOffset(unsigned int value) {
	throw std::runtime_error("setPPLinkBurstOffset not implemented");
}

void IMac::setPPLinkBurstOffsetAdaptive(bool value) {
	throw std::runtime_error("setPPLinkBurstOffsetAdaptive not implemented");
}