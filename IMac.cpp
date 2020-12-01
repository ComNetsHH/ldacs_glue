//
// Created by Sebastian Lindner on 01.12.20.
//

#include <cassert>
#include "IMac.hpp"
#include "IArq.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

void TUHH_INTAIRNET_MCSOTDMA::IMac::injectIntoUpper(L2Packet* packet) {
	assert(this->upper_layer && "IMac::injectIntoUpper called but upper layer is unset.");
	// Just forward it.
	this->upper_layer->injectIntoUpper(packet);
}
