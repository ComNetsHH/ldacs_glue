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

#include "IRlc.hpp"
#include "IOmnetPluggable.hpp"
#include <deque>

#ifndef INTAIRNET_LINKLAYER_GLUE_PASSTHROUGHRLC_H
#define INTAIRNET_LINKLAYER_GLUE_PASSTHROUGHRLC_H

namespace TUHH_INTAIRNET_MCSOTDMA {

	class PassThroughRlc : public IRlc, public IOmnetPluggable {

	private:
		std::deque<L3Packet*> networkLayerPackets;
		std::deque<L2Packet*> linkLayerPackets;
	public:
		void receiveFromUpper(L3Packet* data, MacId dest, PacketPriority priority = PRIORITY_DEFAULT) override;

		void receiveFromLower(L2Packet* packet) override;

		void receiveInjectionFromLower(L2Packet* packet, PacketPriority priority = PRIORITY_LINK_MANAGEMENT) override;

		virtual L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id) override;

		void init();

		void onEvent(double time) override;


	};

}

#endif //INTAIRNET_LINKLAYER_GLUE_PASSTHROUGHRLC_H
