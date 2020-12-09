//
// Created by Konrad Fuger on 06.12.20.
//

#include "PassThroughRlc.hpp"
#include <string>
#include <iostream>

using namespace TUHH_INTAIRNET_MCSOTDMA;


void PassThroughRlc::init() {
    double time = getTime() +1;
    debug("TEST " + std::to_string(time));
    emit("x", 1.0);

}

void PassThroughRlc::onEvent(double time) {

}

void PassThroughRlc::receiveFromUpper(L3Packet* data, MacId dest, PacketPriority priority) {
    networkLayerPackets.push_back(data);
    emit("rlc_nw_queue", networkLayerPackets.size());
    debug("rlc_nw_queue" + networkLayerPackets.size());
}

void PassThroughRlc::receiveFromLower(L2Packet* packet) {
    linkLayerPackets.push_back(packet);

}

void PassThroughRlc::receiveInjectionFromLower(L2Packet* packet, PacketPriority priority) {

}

L2Packet* PassThroughRlc::requestSegment(unsigned int num_bits, const MacId& mac_id) {
    auto packet = new L2Packet();
    return packet;
}

