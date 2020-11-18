//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IMAC_HPP
#define INTAIRNET_LINKLAYER_GLUE_IMAC_HPP

#include "MacId.hpp"
#include "L2Packet.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	
	class IArq; // Forward-declaration so that we can keep a pointer to the ARQ sublayer.
	
	/**
	 * Specifies the interface the MAC sublayer must implement.
	 */
	class IMac {
		public:
			/**
			 * Notify the MAC sublayer about an outbound packet.
			 * @param num_bits The number of bits present in the queue corresponding to 'id'.
			 * @param mac_id The MAC ID.
			 */
			virtual void notifyOutgoing(unsigned int num_bits, const MacId& mac_id) = 0;
			
			/**
			 * When a transmission slot arrives, this passes down a packet to the PHY layer.
			 * @param packet
			 */
			virtual void passToLower(L2Packet* packet) = 0;
			
			/**
			 * Sets the ARQ sublayer above.
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
		
		protected:
			IArq* upper_layer = nullptr;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_IMAC_HPP
