//
// Created by Konrad Fuger on 10.12.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_DELAYMAC_HPP
#define INTAIRNET_LINKLAYER_GLUE_DELAYMAC_HPP

#include "IMac.hpp"
#include "IOmnetPluggable.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {

	class DelayMac : public IMac, public IOmnetPluggable {
	private:
		unsigned int nextPktSize = 0;
		MacId nextMacId;
	public:
		explicit DelayMac(const MacId& id);

		void notifyOutgoing(unsigned long num_bits, const MacId& mac_id) override;

		void passToLower(L2Packet* packet, unsigned int center_frequency) override;

		void passToUpper(L2Packet* packet) override;

		void injectIntoUpper(L2Packet* packet) override;

		void receiveFromLower(L2Packet* packet, uint64_t center_frequency) override;

		void onEvent(double time) override;
	};

}


#endif //INTAIRNET_LINKLAYER_GLUE_DELAYMAC_HPP
