//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_L2PACKET_HPP
#define INTAIRNET_LINKLAYER_GLUE_L2PACKET_HPP

#include "L2Header.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace TUHH_INTAIRNET_MCSOTDMA {
	
	class L2PacketSentCallback;
	
	/**
	 * Wraps around an original packet implementation.
	 * It keeps a pointer to the original packet and adds functionality specific to the MC-SOTDMA protocol.
	 * When MC-SOTDMA operation finishes, the original packet is passed on to the respective receiving layer.
	 */
	class L2Packet {
		public:
			/**
			 * Interface for a wrapper of an upper-layer packet.
		    */
			class Payload {
				public:
					virtual unsigned int getBits() const = 0;
			};
			
			explicit L2Packet() : headers(), payloads(), dest_id(SYMBOLIC_ID_UNSET) {}
			
			virtual ~L2Packet();
			
			void addPayload(L2Header* header, Payload* payload);
			
			/**
			 * @return All payloads.
			 */
			const std::vector<Payload*>& getPayloads() {
				return this->payloads;
			}
			
			/**
			 * @return All headers.
			 */
			const std::vector<L2Header*>& getHeaders() {
				return this->headers;
			}
			
			/**
			 * @return Total size of this packet in bits, consisting of both headers and payloads.
			 */
			unsigned int getBits() const;
			
			/**
			 * @return This packet's destination ID.
			 */
			const MacId& getDestination() const;
			
			void addCallback(L2PacketSentCallback* callback);
		
		protected:
			/**
			 * Several headers can be concatenated to fill one packet.
			 */
			std::vector<L2Header*> headers;
			
			/**
			 * Several payloads can be concatenated (with resp. headers) to fill one packet.
			 */
			std::vector<Payload*> payloads;
			
			MacId dest_id;
			
			/** Holds all registered callbacks. */
			std::vector<L2PacketSentCallback*> callbacks;
		
		protected:
			/**
			 * Ensures that at least one header is present, which must be a base header.
			 * @throws std::logic_error If no headers are present.
			 * @throws std::runtime_error If first header is not a base header.
			 */
			void validateHeader() const {
				if (headers.empty())
					throw std::logic_error("No headers present.");
				const L2Header* first_header = headers.at(0);
				if (first_header->frame_type != L2Header::base)
					throw std::runtime_error("First header is not a base header.");
			}
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_L2PACKET_HPP
