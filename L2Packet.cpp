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
		if (header->isBroadcastType())
			return SYMBOLIC_LINK_ID_BROADCAST;		
		else if (header->isUnicastType())
			throw std::runtime_error("unicast packet's getDestination not yet implemented");
	}
	// Default to UNSET.
	return SYMBOLIC_ID_UNSET;
}

const MacId& L2Packet::getOrigin() const {
	if (headers.empty())
		return SYMBOLIC_ID_UNSET;
	auto* header = (L2HeaderSH*) headers.at(0);
	return header->src_id;
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

// int L2Packet::getRequestIndex() const {
// 	for (int i = 0; i < headers.size(); i++)
// 		if (headers.at(i)->frame_type == L2Header::FrameType::link_establishment_request)
// 			return i;
// 	return -1;
// }

// int L2Packet::getReplyIndex() const {
// 	for (int i = 0; i < headers.size(); i++)
// 		if (headers.at(i)->frame_type == L2Header::FrameType::link_establishment_reply)
// 			return i;
// 	return -1;
// }

// int L2Packet::getBeaconIndex() const {
// 	for (int i = 0; i < headers.size(); i++)
// 		if (headers.at(i)->frame_type == L2Header::FrameType::beacon)
// 			return i;
// 	return -1;
// }

void L2Packet::erase(size_t index) {
	if (index >= this->headers.size() || index >= this->payloads.size())
		throw std::invalid_argument("L2Packet::erase for index out of bounds.");
	delete headers.at(index);	
	this->headers.erase(this->headers.begin() + index);
	delete payloads.at(index);
	this->payloads.erase(this->payloads.begin() + index);
}

bool L2Packet::isDME() const {
	if (getOrigin() == SYMBOLIC_LINK_ID_DME)
		return true;
	for (auto *header : this->headers)
		if (header->isDMERequest() || header->isDMEResponse())
			return true;
	return false;
}

bool L2Packet::empty() const {
	if (this->payloads.empty())
		return true;
	size_t payload_size = 0;
	for (const auto *payload : payloads)
		if (payload != nullptr)
			payload_size += payload->getBits();
	return payload_size == 0;
}