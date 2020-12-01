//
// Created by Sebastian Lindner on 01.12.20.
//

#include "IMac.hpp"
#include "IArq.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

void TUHH_INTAIRNET_MCSOTDMA::IMac::injectIntoUpper(L2Packet* packet) {
	// Just forward it.
	this->upper_layer->injectIntoUpper(packet);
}
