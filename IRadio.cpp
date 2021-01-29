//
// Created by Konrad Fuger on 14.12.20.
//

#include "IRadio.hpp"
#include "IPhy.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	void IRadio::receiveFromChannel(L2Packet* packet, uint64_t center_frequency) {
		phy->onReception(packet, center_frequency);
	};

	void IRadio::setPhy(IPhy* phy) { this->phy = phy; };

	IPhy* IRadio::getPhy() { return phy; };
};

