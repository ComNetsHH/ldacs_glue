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

#ifndef INTAIRNET_LINKLAYER_GLUE_IRADIO_HPP
#define INTAIRNET_LINKLAYER_GLUE_IRADIO_HPP

#include "L2Packet.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	class IPhy;

	class IRadio {
	protected:
		IPhy* phy = nullptr;
	public:
		virtual void sendToChannel(L2Packet* packet, uint64_t center_frequency) = 0;

		virtual void receiveFromChannel(L2Packet* packet, uint64_t center_frequency) = 0;

		void setPhy(IPhy* phy);

		IPhy* getPhy();
	};
}


#endif //INTAIRNET_LINKLAYER_GLUE_IRADIO_HPP
