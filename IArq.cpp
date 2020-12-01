//
// Created by Sebastian Lindner on 01.12.20.
//

#include "IArq.hpp"
#include "IRlc.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

void IArq::injectIntoUpper(L2Packet* packet) {
	// Just forward it.
	this->upper_layer->receiveInjectionFromLower(packet);
}
