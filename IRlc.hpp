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

#ifndef INTAIRNET_LINKLAYER_GLUE_IRLC_HPP
#define INTAIRNET_LINKLAYER_GLUE_IRLC_HPP

#include "MacId.hpp"
#include "L2Packet.hpp"
#include "L3Packet.hpp"
#include "IRlc.hpp"
#include "INet.hpp"
#include <cassert>

namespace TUHH_INTAIRNET_MCSOTDMA {

	class IArq; // Forward-declaration so that we can keep a pointer to the ARQ sublayer.
	//class L3Packet; // Forward declaration so that we can accept layer-3 packets.


	/**
	 * Priorities for L3/L2 Packets
	 * For now only dummy priorites, have to be adapted later
	 */
	enum PacketPriority {
		PRIORITY_DEFAULT = 0,
		PRIORITY_LOWEST,
		PRIORITY_LOW,
		PRIORITY_MEDIUM,
		PRIORITY_HIGH,
		PRIORITY_HIGHEST,
		PRIORITY_LINK_MANAGEMENT
	};

	/**
	 * Specifies the interface the RLC sublayer must implement.
	 */
	class IRlc {
	public:
		/**
		 * When the RLC sublayer receives a new data packet from the upper layer.
		 * @param data The L3Packet
		 * @param dest The resolved Mac address
		 * @param priority Traffic priority
		 */
		virtual void receiveFromUpper(L3Packet* data, MacId dest, PacketPriority priority = PRIORITY_DEFAULT) = 0;

		/**
		 * When a packet comes in via PHY, MAC, ARQ, this function passes it from the ARQ to this RLC layer.
		 * @param packet
		 */
		virtual void receiveFromLower(L2Packet* packet) = 0;

		/**
		 * Link requests may be injected from the MAC sublayer below, through the ARQ sublayer, into this layer.
		 * @param packet The L3Packet
		 * @param priority Optional field to set the priority of injected packets
		 */
		virtual void receiveInjectionFromLower(L2Packet* packet, PacketPriority priority = PRIORITY_LINK_MANAGEMENT) = 0;

		/**
		 * The ARQ sublayer requests a new segment to send from the RLC sublayer.
		 * @param num_bits Number of bits that should be contained in the segment.
		 * @param mac_id MAC ID corresponding to the communication link for which this segment is requested.
		 * @return A pointer to the segment.
		 */
		virtual L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id) = 0;
		virtual L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id, int frameIdx) {
			return requestSegment(num_bits, mac_id);
		};

		/**
		 * Interface to acknowledge a sent frameIdx
		*/
		virtual void ack(MacId mac_id, int frameIdx) { }
		/**
		 * Interface to negatively acknowledge a sent frameIdx
		*/
		virtual void nack(MacId mac_id, int frameIdx) { }

		/**
		 * @param mac_id
		 * @return Whether there's more data to be sent for the given ID.
		 */
		virtual bool isThereMoreData(const MacId& mac_id) const = 0;

		void setUpperLayer(INet* upper_layer) {
			this->upper_layer = upper_layer;
		}

		INet* getUpperLayer() {
			return this->upper_layer;
		}

		/**
		 * @return Pointer to the ARQ sublayer below.
		 */
		IArq* getLowerLayer() {
			return this->lower_layer;
		}

		/**
		 * Connects the ARQ sublayer below.
		 * @param arq
		 */
		void setLowerLayer(IArq* arq) {
			this->lower_layer = arq;
		}

		/**
		 * @return The number of hops to the nearest ground station according to current routing information.
		 */
		unsigned int getNumHopsToGS() const {
			return upper_layer->getNumHopsToGroundStation();
		}

		/**
		 * When a neighbor's onSlotEnd comes in, this reports it to the upper layers.
		 * @param id
		 * @param num_hops
		 */
		void reportNumHopsToGS(const MacId& id, unsigned int num_hops) const {
			assert(this->upper_layer && "IRlc::reportNumHopsToGS for unset upper layer.");
			upper_layer->reportNumHopsToGS(id, num_hops);
		}

        /**
         * Request the current amount of data queued up for sending
         * @param dest
         */
		virtual unsigned int getQueuedDataSize(MacId dest) = 0;

	protected:
		IArq* lower_layer = nullptr;
		INet* upper_layer = nullptr;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_IRLC_HPP
