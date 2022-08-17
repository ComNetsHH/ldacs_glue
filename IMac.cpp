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

void IMac::onBeaconReception(MacId origin_id, CPRPosition position) {
	passUpBeaconFct(origin_id, position);
}

void IMac::setAdvertiseNextBroadcastSlotInCurrentHeader(bool flag) {
	throw std::runtime_error("setAdvertiseNextBroadcastSlotInCurrentHeader not implemented");
}

const std::vector<int> IMac::getChannelSensingObservation() const {
	throw std::runtime_error("getChannelSensingObservation not implemented");
}

void IMac::setLearnDMEActivity(bool value) {
	throw std::runtime_error("setLearnDMEActivity not implemented");
}

void IMac::passPrediction(const std::vector<std::vector<double>>& prediction_mat) {
	throw std::runtime_error("passPrediction not implemented");
}

void IMac::setDutyCycle(unsigned int period, double max, unsigned int min_num_supported_pp_links) {
	throw std::runtime_error("setDutyCycle not implemented");
}

void IMac::setMaxNoPPLinkEstablishmentAttempts(int value) {
	throw std::runtime_error("setMaxNoPPLinkEstablishmentAttempts not implemented");
}

void IMac::setConsiderDutyCycle(bool flag) {
	throw std::runtime_error("setConsiderDutyCycle not implemented");
}

void IMac::setMinNumSupportedPPLinks(unsigned int value) {
	throw std::runtime_error("setMinNumSupportedPPLinks not implemented");
}

void IMac::setForcePPPeriod(bool flag, int value) {
	throw std::runtime_error("setForcePPPeriod not implemented");
}