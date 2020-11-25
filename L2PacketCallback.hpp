//
// Created by Sebastian Lindner on 25.11.20.
//

#ifndef TUHH_INTAIRNET_MC_SOTDMA_L2PACKETCALLBACK_HPP
#define TUHH_INTAIRNET_MC_SOTDMA_L2PACKETCALLBACK_HPP

#include "L2Packet.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	class L2PacketSentCallback {
		public:
			virtual void notifyOnOutgoingPacket(TUHH_INTAIRNET_MCSOTDMA::L2Packet* packet) = 0;
	};
}

#endif //TUHH_INTAIRNET_MC_SOTDMA_L2PACKETCALLBACK_HPP
