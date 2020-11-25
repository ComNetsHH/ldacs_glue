//
// Created by Sebastian Lindner on 25.11.20.
//

#include "L2Packet.hpp"
#include "L2PacketCallback.hpp"

TUHH_INTAIRNET_MCSOTDMA::L2Packet::~L2Packet() {
	for (L2PacketSentCallback* callback : callbacks)
		callback->notifyOnOutgoingPacket(this);
}

void TUHH_INTAIRNET_MCSOTDMA::L2Packet::addCallback(TUHH_INTAIRNET_MCSOTDMA::L2PacketSentCallback* callback) {
	this->callbacks.push_back(callback);
}

const TUHH_INTAIRNET_MCSOTDMA::MacId& TUHH_INTAIRNET_MCSOTDMA::L2Packet::getDestination() const {
	return this->dest_id;
}

unsigned int TUHH_INTAIRNET_MCSOTDMA::L2Packet::getBits() const {
	unsigned int bits = 0;
	for (size_t i = 0; i < headers.size(); i++)
		bits += headers.at(i)->getBits() + payloads.at(i)->getBits();
	return bits;
}

void TUHH_INTAIRNET_MCSOTDMA::L2Packet::addPayload(TUHH_INTAIRNET_MCSOTDMA::L2Header* header,
                                                   TUHH_INTAIRNET_MCSOTDMA::L2Packet::Payload* payload) {
	// Ensure that the first header is a base header.
	if (headers.empty() && header->frame_type != L2Header::FrameType::base)
		throw std::invalid_argument("First header of a packet *must* be a base header.");
	
	// Ensure that later headers are *not* base headers.
	if (!headers.empty() && header->frame_type == L2Header::FrameType::base)
		throw std::invalid_argument("Later headers of a packet cannot be a base header.");
	
	// Set the unicast destination ID if possible.
	if (header->frame_type == L2Header::FrameType::unicast) {
		MacId header_dest_id = ((L2HeaderUnicast*) header)->getDestId();
		// Sanity check that the destination ID is actually set.
		if (header_dest_id == SYMBOLIC_ID_UNSET)
			throw std::runtime_error("Cannot add a unicast header with an unset destination ID.");
		// If currently there's no set destination, we set it now.
		if (this->dest_id == SYMBOLIC_ID_UNSET)
			this->dest_id = header_dest_id;
			// If there is a set non-broadcast destination, it must be unicast.
			// So if these differ, throw an error.
		else if (this->dest_id != SYMBOLIC_LINK_ID_BROADCAST && this->dest_id != SYMBOLIC_LINK_ID_BEACON && header_dest_id != this->dest_id)
			throw std::runtime_error("Cannot add a unicast header to this packet. It already has a unicast destination ID. Current dest='" + std::to_string(this->dest_id.getId()) + "' header dest='" + std::to_string(header_dest_id.getId()) + "'.");
	}
	
	// Set the broadcast destination ID if possible.
	if (header->frame_type == L2Header::FrameType::broadcast) {
		// If currently there's no set destination, we set it now.
		if (this->dest_id == SYMBOLIC_ID_UNSET)
			this->dest_id = SYMBOLIC_LINK_ID_BROADCAST;
			// If there already is a set destination, it may only be a beacon.
		else if (this->dest_id != SYMBOLIC_LINK_ID_BEACON)
			throw std::runtime_error("Cannot add a broadcast header to this packet. It already has a destination ID: '" + std::to_string(this->dest_id.getId()) + "'.");
	}
	
	// Set the beacon destination ID if possible.
	if (header->frame_type == L2Header::FrameType::beacon) {
		// If currently there's no set destination, we set it now.
		if (this->dest_id == SYMBOLIC_ID_UNSET)
			this->dest_id = SYMBOLIC_LINK_ID_BEACON;
			// If there already is a set destination, throw an error, as beacon headers must come first.
		else
			throw std::runtime_error("Cannot add a beacon header to this packet. It already has a destination ID: '" + std::to_string(this->dest_id.getId()) + "'.");
	}
	
	headers.push_back(header);
	payloads.push_back(payload);
}
