//
// Created by Konrad Fuger on 06.12.20.
//

#include "PassThroughRlc.h"
#include <string>
#include <iostream>

using namespace TUHH_INTAIRNET_MCSOTDMA;


void PassThroughRlc::init() {
    double time = getTime() +1;
    debug("TEST " + std::to_string(time));

}

void PassThroughRlc::onEvent(double time) {

}

void PassThroughRlc::receiveFromUpper(L3Packet* data, MacId dest, PacketPriority priority) {

}

void PassThroughRlc::receiveFromLower(L2Packet* packet) {

}

void PassThroughRlc::receiveInjectionFromLower(L2Packet* packet, PacketPriority priority) {

}

L2Packet* PassThroughRlc::requestSegment(unsigned int num_bits, const MacId& mac_id) {

}

