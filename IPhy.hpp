//
// Created by Sebastian Lindner on 18.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IPHY_HPP
#define INTAIRNET_LINKLAYER_GLUE_IPHY_HPP

#include "L2Packet.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	
	class IMac; // Forward-declaration so that we can keep a pointer to the MAC sublayer.
	
	/**
	 * Specifies the interface the PHY layer must implement.
	 */
	class IPhy {
		public:
			
			/**
			 * Notify the PHY layer about an outbound packet.
			 * @param num_bits
			 * @param data
			 * @param center_frequency
			 */
			virtual void receiveFromUpper(unsigned int num_bits, L2Packet* data, unsigned int center_frequency) = 0;
			
			/**
			 * @return From the current modulation and coding, return the current datarate in bits per slot.
			 */
			virtual unsigned long getCurrentDatarate() const = 0;
			
			/**
			 * Connects the MAC sublayer above.
			 * @param mac
			 */
			void setUpperLayer(IMac* mac) {
				this->upper_layer = mac;
			}
			
			/**
			 * @return A pointer to the MAC sublayer above.
			 */
			IMac* getUpperLayer() {
				return this->upper_layer;
			}
		
		protected:
			IMac* upper_layer = nullptr;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_IPHY_HPP
