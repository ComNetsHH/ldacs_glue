//
// Created by Sebastian Lindner on 01.12.20.
//

#include <cassert>
#include "L2Packet.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

L2Packet::L2Packet() = default;

L2Packet::L2Packet(const L2Packet& other) : headers(other.headers), payloads(other.payloads), callbacks(other.callbacks) {}

L2Packet* L2Packet::copy() const {
	auto* copy = new L2Packet();
	for (const auto* header : headers)
		copy->headers.push_back(header == nullptr ? nullptr : header->copy());
	for (const auto* payload : payloads)
		copy->payloads.push_back(payload == nullptr ? nullptr : payload->copy());
	return copy;
}

L2Packet::~L2Packet() {
	for (auto* header : headers)
		delete header;
	for (auto* payload : payloads)
		delete payload;
}

void L2Packet::addPayload(L2Header* header, L2Packet::Payload* payload) {
	// Ensure that the first header is a base header.
	if (headers.empty() && header->frame_type != L2Header::FrameType::base)
		throw std::invalid_argument("First header of a packet *must* be a base header.");

	// Ensure that later headers are *not* base headers.
	if (!headers.empty() && header->frame_type == L2Header::FrameType::base)
		throw std::invalid_argument("Later headers of a packet cannot be a base header.");

	// Set the unicast destination ID if possible.
	if (header->frame_type == L2Header::FrameType::unicast || header->frame_type == L2Header::FrameType::link_establishment_request || header->frame_type == L2Header::FrameType::link_establishment_reply) {
		MacId header_dest_id = ((L2HeaderUnicast*) header)->getDestId();
		// Sanity check that the destination ID is actually set.
		if (header_dest_id == SYMBOLIC_ID_UNSET)
			throw std::runtime_error("Cannot add a unicast header with an unset destination ID.");
		// If there is a set non-broadcast destination, it must be unicast.
		// So if these differ, throw an error.
		const MacId current_destination = getDestination();
		if (current_destination != SYMBOLIC_ID_UNSET && current_destination != SYMBOLIC_LINK_ID_BROADCAST && current_destination != SYMBOLIC_LINK_ID_BEACON && header_dest_id != current_destination)
			throw std::runtime_error(
					"Cannot add a unicast header to this packet. It already has a unicast destination ID. Current dest='" + std::to_string(current_destination.getId()) + "' header dest='" + std::to_string(header_dest_id.getId()) + "'.");
	}

	// Set the broadcast destination ID if possible.
	if (header->frame_type == L2Header::FrameType::broadcast) {
		// If there already is a set destination, it may only be a beacon.
		const MacId current_destination = getDestination();
		if (current_destination != SYMBOLIC_ID_UNSET && current_destination != SYMBOLIC_LINK_ID_BEACON)
			throw std::runtime_error("Cannot add a broadcast header to this packet. It already has a destination ID: '" + std::to_string(getDestination().getId()) + "'.");
	}

	// Set the beacon destination ID if possible.
	if (header->frame_type == L2Header::FrameType::beacon) {
		// If there already is a set destination, throw an error, as beacon headers must come first.
		if (getDestination() != SYMBOLIC_ID_UNSET)
			throw std::runtime_error("Cannot add a beacon header to this packet. It already has a destination ID: '" + std::to_string(getDestination().getId()) + "'.");
	}

	headers.push_back(header);
	payloads.push_back(payload);
}

std::vector<L2Packet::Payload*>& L2Packet::getPayloads() {
	return this->payloads;
}

const std::vector<L2Header*>& L2Packet::getHeaders() {
	return this->headers;
}

unsigned int L2Packet::getBits() const {
	unsigned int bits = 0;
	for (size_t i = 0; i < headers.size(); i++) {
		L2Packet::Payload* payload = payloads.at(i);
		if (headers.at(i) == nullptr) {
			continue;
		}
		bits += headers.at(i)->getBits() + (payload == nullptr ? 0 : payload->getBits());
	}
	return bits;
}

const MacId& L2Packet::getDestination() const {
	for (L2Header* header : headers) {
		if (header->frame_type == L2Header::beacon)
			return SYMBOLIC_LINK_ID_BEACON;
		else if (header->frame_type == L2Header::broadcast)
			return SYMBOLIC_LINK_ID_BROADCAST;
		else if (header->frame_type == L2Header::link_establishment_reply || header->frame_type == L2Header::link_establishment_request || header->frame_type == L2Header::unicast)
			return ((L2HeaderUnicast*) header)->getDestId();
	}
	return SYMBOLIC_ID_UNSET;
}

const MacId& L2Packet::getOrigin() const {
	if (headers.empty())
		return SYMBOLIC_ID_UNSET;
	auto* base_header = (L2HeaderBase*) headers.at(0);
	return base_header->icao_src_id;
}

void L2Packet::addCallback(L2PacketSentCallback* callback) {
	this->callbacks.push_back(callback);
}

void L2Packet::validateHeader() const {
	if (headers.empty())
		throw std::logic_error("No headers present.");
	const L2Header* first_header = headers.at(0);
	if (first_header->frame_type != L2Header::base)
		throw std::runtime_error("First header is not a base header.");
}

void L2Packet::notifyCallbacks() {
	for (auto* callback : callbacks)
		callback->packetBeingSentCallback(this);
}

std::string L2Packet::print() {
	std::string result = "[";
	for (int i = 0; i < headers.size(); i++) {
		result += "H,";
		result += "P | ";
	}
	result += "]";
	return result;
}

