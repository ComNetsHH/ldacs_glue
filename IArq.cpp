//
// Created by Sebastian Lindner on 01.12.20.
//

#include <cassert>
#include "IArq.hpp"
#include "IRlc.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

void IArq::injectIntoUpper(L2Packet* packet) {
	assert(this->upper_layer && "IArq::injectIntoUpper called but upper layer is unset.");
	// Just forward it.
	this->upper_layer->receiveInjectionFromLower(packet);
}
