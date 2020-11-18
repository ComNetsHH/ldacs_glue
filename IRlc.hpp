//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IRLC_HPP
#define INTAIRNET_LINKLAYER_GLUE_IRLC_HPP

#include "MacId.hpp"
#include "L2Packet.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	class L3Packet;
	
	/**
	 * Specifies the interface the RLC sublayer must implement.
	 */
	class IRlc {
		public:
			/**
			 * When the RLC sublayer receives a new data packet from the upper layer.
			 * @param data
			 */
			virtual void receiveFromUpper(L3Packet* data) = 0;
			
			/**
			 * The ARQ sublayer requests a new segment to send from the RLC sublayer.
			 * @param num_bits Number of bits that should be contained in the segment.
			 * @param mac_id MAC ID corresponding to the communication link for which this segment is requested.
			 * @return A pointer to the segment.
			 */
			virtual L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id) = 0;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_IRLC_HPP
