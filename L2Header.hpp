//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_L2HEADER_HPP
#define INTAIRNET_LINKLAYER_GLUE_L2HEADER_HPP

#include <algorithm>
#include <vector>
#include "MacId.hpp"
#include "CPRPosition.hpp"
#include "SequenceNumber.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	
	/**
	 * Specifies the MC-SOTDMA layer-2 header.
	 */
	class L2Header {
		public:
			enum FrameType {
				unset,
				base,
				beacon,
				broadcast,
				unicast,
				link_establishment_request,
				link_establishment_reply
			};
			
			explicit L2Header(L2Header::FrameType frame_type)	: frame_type(frame_type), crc_checksum(0) {}
			
			virtual unsigned int getBits() const {
				return 3 /* frame type */
				       + 16 /* CRC */;
			}
			
			/** This frame's type. */
			const FrameType frame_type;
			/** CRC checksum. */
			unsigned int crc_checksum;
	};
	
	class L2HeaderBase : public L2Header {
		public:
			L2HeaderBase(const MacId& icao_id, unsigned int offset, unsigned short length_current, unsigned short length_next, unsigned int timeout)
					: L2Header(FrameType::base), icao_id(icao_id), offset(offset), length_current(length_current), length_next(length_next), timeout(timeout) {
				if (icao_id == SYMBOLIC_ID_UNSET)
					throw std::invalid_argument("Cannot instantiate a header with an unset ICAO ID.");
			}
			
			/** Number of slots until this reservation is next transmitted. */
			unsigned int offset;
			/** Number of slots this frame occupies. */
			unsigned short length_current;
			/** Number of slots next frame will occupy. */
			unsigned short length_next;
			/** Remaining number of repetitions this reservation remains valid for. */
			unsigned int timeout;
			
			unsigned int getBits() const override {
				return icao_id.getBits()
				       + 8 /* offset */
				       + 4 /* length_current */
				       + 4 /* length_next */
				       + 8 /* timeout */
				       + L2Header::getBits();
			}
			
			const MacId& getId() const {
				return this->icao_id;
			}
		
		protected:
			/** Source ID. */
			const MacId icao_id;
	};
	
	class L2HeaderBroadcast : public L2Header {
		public:
			L2HeaderBroadcast()
					: L2Header(FrameType::broadcast) {}
			
			unsigned int getBits() const override {
				return L2Header::getBits();
			}
	};
	
	class L2HeaderBeacon : public L2Header {
		public:
			L2HeaderBeacon(const CPRPosition& position, bool is_cpr_odd, unsigned int num_hops_to_ground_station, unsigned short pos_quality)
					: L2Header(L2Header::FrameType::beacon), position(position), is_cpr_odd(is_cpr_odd), num_hops_to_ground_station(num_hops_to_ground_station), pos_quality(pos_quality) {}
			
			CPRPosition position;
			bool is_cpr_odd;
			unsigned int num_hops_to_ground_station;
			unsigned short pos_quality;
			
			unsigned int getBits() const override {
				return position.getBits()
				       + 1 /* odd/even identifier */
				       + 5 /* number of hops to ground station */
				       + 2 /* position quality */
				       + L2Header::getBits();
			}
	};
	
	class L2HeaderUnicast : public L2Header {
		public:
			L2HeaderUnicast(const MacId& icao_dest_id, bool use_arq, const SequenceNumber& seqno, const SequenceNumber& seqno_next_expected, unsigned int arq_ack_slot, FrameType frame_type)
					: L2Header(frame_type), icao_dest_id(icao_dest_id), use_arq(use_arq), seqno(seqno), seqno_next_expected(seqno_next_expected), arq_ack_slot(arq_ack_slot) {
				if (icao_dest_id == SYMBOLIC_ID_UNSET)
					throw std::invalid_argument("Cannot instantiate a header with an unset ICAO ID.");
				if (frame_type != FrameType::unicast && frame_type != FrameType::link_establishment_reply && frame_type != FrameType::link_establishment_request)
					throw std::invalid_argument("Cannot instantiate a unicast header with a non-unicast type.");
			}
			
			L2HeaderUnicast(const MacId& icao_dest_id, bool use_arq, SequenceNumber arq_seqno, SequenceNumber arq_ack_no, unsigned int arq_ack_slot)
					: L2HeaderUnicast(icao_dest_id, use_arq, arq_seqno, arq_ack_no, arq_ack_slot, FrameType::unicast) {}
			
			const MacId& getDestId() const {
				return this->icao_dest_id;
			}
			
			unsigned int getBits() const override {
				return 1 /* Whether ARQ is used */
				       + 8 /* ARQ sequence number */
				       + 8 /* ARQ ACK number */
				       + 8 /* ARQ slot indication number */
				       + icao_dest_id.getBits() /* destination ID */
				       + L2Header::getBits();

				       // TODO: add size of srej list
			}
			
			/** Whether the ARQ protocol is followed for this transmission, i.e. acknowledgements are expected. */
			bool use_arq;
			/** ARQ sequence number. */
            SequenceNumber seqno;
			/** ARQ acknowledgement. */
            SequenceNumber seqno_next_expected;
            /** Selective rejection list. */
            std::vector<SequenceNumber> srej_list;
			/** The offset to the next reserved slot where an acknowledgement is expected. */
			unsigned int arq_ack_slot;
		
		protected:
			/** Destination ICAO ID. */
			const MacId icao_dest_id;
	};
	
	class L2HeaderLinkEstablishmentRequest : public L2HeaderUnicast {
		public:
			L2HeaderLinkEstablishmentRequest(const MacId& icao_dest_id, bool use_arq, const SequenceNumber& arq_seqno, const SequenceNumber& arq_ack_no, unsigned int arq_ack_slot)
					: L2HeaderUnicast(icao_dest_id, use_arq, arq_seqno, arq_ack_no, arq_ack_slot, FrameType::link_establishment_request) {}
	};
	
	class L2HeaderLinkEstablishmentReply : public L2HeaderUnicast {
		public:
			L2HeaderLinkEstablishmentReply(const MacId& icao_dest_id, bool use_arq, unsigned int arq_seqno, unsigned int arq_ack_no, unsigned int arq_ack_slot)
					: L2HeaderUnicast(icao_dest_id, use_arq, arq_seqno, arq_ack_no, arq_ack_slot, FrameType::link_establishment_reply) {}
	};
	
}

#endif //INTAIRNET_LINKLAYER_GLUE_L2HEADER_HPP
