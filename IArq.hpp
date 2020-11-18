//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IARQ_HPP
#define INTAIRNET_LINKLAYER_GLUE_IARQ_HPP

#include "L2Packet.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	
	/**
	 * Specifies the interface the ARQ sublayer must implement.
	 */
	class IArq {
		public:
			
			/**
			 * Notify the ARQ sublayer from the RLC sublayer about an outbound packet.
			 * @param num_bits The number of bits present in the queue corresponding to 'id'.
			 * @param mac_id The MAC ID.
			 */
			virtual void notifyOutgoing(unsigned int num_bits, const MacId& mac_id) = 0;
			
			/**
			 * The MAC sublayer may request a new segment to send when a transmission opportunity arises.
			 * @param num_bits Number of bits that should be contained in the segment.
			 * @param mac_id MAC ID corresponding to the communication link for which this segment is requested.
			 * @return A pointer to the segment that should be transmitted next.
			 */
			virtual L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id) = 0;
			
			/**
			 * The MAC sublayer may query whether a particular link should be ARQ protected.
			 * It does so when new links are established.
			 * @param mac_id MAC ID corresponding to the communication link for which ARQ protection is queried.
			 * @return Whether the link should be ARQ protected.
			 */
			virtual bool shouldLinkBeArqProtected(const MacId& mac_id) const = 0;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_IARQ_HPP
