// The L-Band Digital Aeronautical Communications System (LDACS) Link Layer Glue Library provides interfaces and common classes necessary for the LDACS Air-Air Medium Access Control simulator.
// Copyright (C) 2023  Sebastian Lindner, Konrad Fuger, Musab Ahmed Eltayeb Ahmed, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef INTAIRNET_LINKLAYER_GLUE_DELAYMAC_HPP
#define INTAIRNET_LINKLAYER_GLUE_DELAYMAC_HPP

#include "IMac.hpp"
#include "IOmnetPluggable.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {

	class DelayMac : public IMac, public IOmnetPluggable {
	private:
	    int nextPktSize = 0;
		MacId nextMacId;

		int counter = 0;
	public:
		explicit DelayMac(const MacId& id);

		void notifyOutgoing(unsigned long num_bits, const MacId& mac_id) override;

		void passToLower(L2Packet* packet, unsigned int center_frequency) override;

		void passToUpper(L2Packet* packet) override;

		void injectIntoUpper(L2Packet* packet) override;

		void receiveFromLower(L2Packet* packet, uint64_t center_frequency) override;

		void onEvent(double time) override;

        void update(uint64_t num_slots);

		void execute();

		void onSlotEnd();
	};

}


#endif //INTAIRNET_LINKLAYER_GLUE_DELAYMAC_HPP
