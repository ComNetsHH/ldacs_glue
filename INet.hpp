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

#ifndef TUHH_INTAIRNET_MC_SOTDMA_INET_HPP
#define TUHH_INTAIRNET_MC_SOTDMA_INET_HPP

class L3Packet;

namespace TUHH_INTAIRNET_MCSOTDMA {
	class IRlc; // Forward-declaration so that we can keep a pointer to the RLC sublayer.	

	/**
	 * Network layer interface.
	 */
	class INet {
	public:
		virtual ~INet() = default;

		void setLowerLayer(IRlc* lower_layer) {
			this->lower_layer = lower_layer;
		}

		IRlc* getLowerLayer() {
			return this->lower_layer;
		}

		/**
		 * @return Number of hops to nearest ground station according to current routing information.
		 */
		virtual unsigned int getNumHopsToGroundStation() const = 0;

		/**
		 * When a neighbor's onSlotEnd comes in at a lower layer, this forwards this info to the Network Layer.
		 * @param id
		 * @param num_hops
		 */
		virtual void reportNumHopsToGS(const MacId& id, unsigned int num_hops) = 0;


		/**
		 * Is called when a L3Packet was received and reassembled
		 * @param packet
		 */
		virtual void receiveFromLower(L3Packet* packet) = 0;

	protected:
		IRlc* lower_layer = nullptr;
	};
}

#endif //TUHH_INTAIRNET_MC_SOTDMA_INET_HPP
