//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IRLC_HPP
#define INTAIRNET_LINKLAYER_GLUE_IRLC_HPP

#include "MacId.hpp"
#include "L2Packet.hpp"
#include "IRlc.hpp"
#include "INet.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	
	class IArq; // Forward-declaration so that we can keep a pointer to the ARQ sublayer.
	class L3Packet; // Forward declaration so that we can accept layer-3 packets.


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
			
		
		protected:
			IArq* lower_layer = nullptr;
			INet* upper_layer = nullptr;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_IRLC_HPP
