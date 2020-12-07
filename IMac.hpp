//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IMAC_HPP
#define INTAIRNET_LINKLAYER_GLUE_IMAC_HPP

#include "MacId.hpp"
#include "L2Packet.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	
	class IArq; // Forward-declaration so that we can keep a pointer to the ARQ sublayer.
	class IPhy; // Forward-declaration so that we can keep a pointer to the PHY layer.
	
	/**
	 * Specifies the interface the MAC sublayer must implement.
	 */
	class IMac {
		public:
			
			virtual ~IMac() = default;
			
			/**
			 * Notify the MAC sublayer about an outbound packet.
			 * @param num_bits The number of bits present in the queue corresponding to 'id'.
			 * @param mac_id The MAC ID.
			 */
			virtual void notifyOutgoing(unsigned long num_bits, const MacId& mac_id) = 0;
			
			/**
			 * Requests a segment from the upper layer.
			 * @param num_bits
			 * @param mac_id
			 * @return
			 */
			virtual L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id);
			
			/**
			 * When a transmission slot arrives, this passes down a packet to the PHY layer.
			 * @param packet
			 * @param center_frequency
			 */
			virtual void passToLower(L2Packet* packet, unsigned int center_frequency) = 0;
			
			/**
			 * When a packet comes in, this passes it up to the next upper layer.
			 * @param packet
			 */
			virtual void passToUpper(L2Packet* packet) = 0;
			
			/**
			 * Link requests must be injected into the RLC sublayer above.
			 * With this function, injection *up* the protocol stack is possible.
			 * @param packet
			 */
			virtual void injectIntoUpper(L2Packet* packet);
			
			/**
			 * Connects the ARQ sublayer above.
			 * @param arq
			 */
			void setUpperLayer(IArq* arq) {
				this->upper_layer = arq;
			}
			
			/**
			 * @return Pointer to the ARQ sublayer above.
			 */
			IArq* getUpperLayer() {
				return this->upper_layer;
			}
			
			/**
			 * Connects the PHY layer below.
			 * @param phy
			 */
			void setLowerLayer(IPhy* phy) {
				this->lower_layer = phy;
			}
			
			/**
			 * @return A pointer to the PHY layer below.
			 */
			IPhy* getLowerLayer() {
				return this->lower_layer;
			}
		
		protected:
			IArq* upper_layer = nullptr;
			IPhy* lower_layer = nullptr;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_IMAC_HPP
