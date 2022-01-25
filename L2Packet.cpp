//
// Created by Sebastian Lindner on 01.12.20.
//

#include <cassert>
#include "L2Packet.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

L2Packet::L2Packet() = default;

L2Packet::L2Packet(const L2Packet& other) : headers(other.headers), payloads(other.payloads), callbacks(other.callbacks), hasChannelError(hasChannelError) {}

L2Packet* L2Packet::copy() const {
	auto* copy = new L2Packet();
	copy->hasChannelError = this->hasChannelError;
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

void L2Packet::addMessage(L2Header* header, L2Packet::Payload* payload) {
	// Ensure that the first header is a base header.
	if (headers.empty() && header->frame_type != L2Header::FrameType::base)
		throw std::invalid_argument("First header of a packet *must* be a base header.");

	// Ensure that later headers are *not* base headers.
	if (!headers.empty() && header->frame_type == L2Header::FrameType::base)
		throw std::invalid_argument("Later headers of a packet cannot be a base header.");

	// Set the unicast destination ID if possible.
	if (header->isUnicastType()) {
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
		if (current_destination != SYMBOLIC_LINK_ID_BROADCAST && current_destination != SYMBOLIC_ID_UNSET && current_destination != SYMBOLIC_LINK_ID_BEACON)
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

void L2Packet::addMessage(std::pair<L2Header*, Payload*> message) {
	this->addMessage(message.first, message.second);
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
		if (headers.at(i) == nullptr)
			continue;
		L2Packet::Payload* payload = payloads.at(i);
		bits += headers.at(i)->getBits() + (payload == nullptr ? 0 : payload->getBits());
	}
	return bits;
}

MacId L2Packet::getDestination() const {
	for (const L2Header *header : headers) {
		// Return BROADCAST-type destinations immediately, s.t. a single BROADCAST determines a packet's destination.
		if (header->frame_type == L2Header::FrameType::beacon)
			return SYMBOLIC_LINK_ID_BEACON;
		else if (header->isBroadcastType())
			return SYMBOLIC_LINK_ID_BROADCAST;
		// Return UNICAST-type destinations.
		else if (header->isUnicastType())
			return ((L2HeaderUnicast*) header)->getDestId();
	}
	// Default to UNSET.
	return SYMBOLIC_ID_UNSET;
}

const MacId& L2Packet::getOrigin() const {
	if (headers.empty())
		return SYMBOLIC_ID_UNSET;
	auto* base_header = (L2HeaderBase*) headers.at(0);
	return base_header->src_id;
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
	std::string result = "[ ";
	for (int i = 0; i < headers.size(); i++) {
	    auto headerType = headers[i]->frame_type;
	    int size = (int)headers[i]->getBits();
	    if (headerType == L2Header::FrameType::broadcast) {
            result += "BC(" + std::to_string(size) + "),";
	    }
	    else if (headerType == L2Header::FrameType::base) {
            result += "B(" + std::to_string(size) + "),";
	    } else if (headerType == L2Header::FrameType::link_establishment_request) {
            result += "LINK_REQ(" + std::to_string(size) + "),";
        } else if (headerType == L2Header::FrameType::link_establishment_reply) {
            result += "LINK_REP(" + std::to_string(size) + "),";
        } else if (headerType == L2Header::FrameType::unicast) {
            result += "U(" + std::to_string(size) + "),";	    
	    } else {
            result += "H,";
        }

        if(payloads[i] == nullptr) {
            result += "N(0) ";
        }
        else {
            result += "P(" + std::to_string((int)payloads[i]->getBits()) + ") ";
        }
        if(i != headers.size() -1) {
            result += "| ";
        }

	}
	result += "]";
	return result;
}

int L2Packet::getRequestIndex() const {
	for (int i = 0; i < headers.size(); i++)
		if (headers.at(i)->frame_type == L2Header::FrameType::link_establishment_request)
			return i;
	return -1;
}

int L2Packet::getReplyIndex() const {
	for (int i = 0; i < headers.size(); i++)
		if (headers.at(i)->frame_type == L2Header::FrameType::link_establishment_reply)
			return i;
	return -1;
}

int L2Packet::getBeaconIndex() const {
	for (int i = 0; i < headers.size(); i++)
		if (headers.at(i)->frame_type == L2Header::FrameType::beacon)
			return i;
	return -1;
}

void L2Packet::erase(size_t index) {
	if (index >= this->headers.size() || index >= this->payloads.size())
		throw std::invalid_argument("L2Packet::erase for index out of bounds.");
	delete headers.at(index);	
	this->headers.erase(this->headers.begin() + index);
	delete payloads.at(index);
	this->payloads.erase(this->payloads.begin() + index);
}