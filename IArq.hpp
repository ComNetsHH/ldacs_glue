//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IARQ_HPP
#define INTAIRNET_LINKLAYER_GLUE_IARQ_HPP

#include "L2Packet.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	class IArq {
		public:
			
			/**
			 * Notify the ARQ sublayer from the RLC sublayer about an outbound packet.
			 * @param num_bits The number of bits present in the queue corresponding to 'id'.
			 * @param mac_id The MAC ID.
			 */
			virtual void notifyOutgoing(unsigned int num_bits, const MacId& mac_id) = 0;
			
			/**
			 * Request a new segment from the MAC sublayer to send from the ARQ sublayer.
			 * @param num_bits Number of bits that should be contained in the segment.
			 * @param mac_id MAC ID corresponding to the communication link for which this segment is requested.
			 * @return A pointer to the segment.
			 */
			virtual L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id) = 0;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_IARQ_HPP
