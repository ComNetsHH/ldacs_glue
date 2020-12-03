//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IARQ_HPP
#define INTAIRNET_LINKLAYER_GLUE_IARQ_HPP

#include "L2Packet.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	
	class IMac; // Forward-declaration so that we can keep a pointer to the MAC sublayer.
	class IRlc; // Forward-declaration so that we can keep a pointer to the RLC sublayer.
	
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
			
			/**
			 * Connects the RLC sublayer above.
			 * @param rlc
			 */
			void setUpperLayer(IRlc* rlc) {
				this->upper_layer = rlc;
			}
			
			/**
			 * @return Pointer to the RLC sublayer above.
			 */
			IRlc* getUpperLayer() {
				return this->upper_layer;
			}
			
			/**
			 * Connects the MAC sublayer below.
			 * @param mac
			 */
			void setLowerLayer(IMac* mac) {
				this->lower_layer = mac;
			}
			
			/**
			 * @return Pointer to the MAC sublayer below.
			 */
			IMac* getLowerLayer() {
				return this->lower_layer;
			}
			
			/**
			 * Link requests must be injected into the RLC sublayer above.
			 * With this function, injection *up* the protocol stack is possible.
			 * @param packet
			 */
			void injectIntoUpper(L2Packet* packet);
			
			/**
			 * The MAC may call this function to notify the ARQ sublayer of a newly established link.
			 * @param id
			 */
			virtual void notifyAboutNewLink(const MacId& id) = 0;
			
			/**
			 * The MAC may call this function to notify the ARQ sublayer of a deleted link.
			 * @param id
			 */
			virtual void notifyAboutRemovedLink(const MacId& id) = 0;
		
		protected:
			IRlc* upper_layer = nullptr;
			IMac* lower_layer = nullptr;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_IARQ_HPP
