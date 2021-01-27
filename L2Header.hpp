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
			
			explicit L2Header(L2Header::FrameType frame_type)	: frame_type(frame_type) {}
			
			virtual unsigned int getBits() const {
				return 3 /* frame type */;
			}
			
			/** This frame's type. */
			const FrameType frame_type;
	};
	
	class L2HeaderBase : public L2Header {
		public:
			L2HeaderBase(const MacId& icao_id, unsigned int offset, unsigned short length_next, unsigned int timeout)
					: L2Header(FrameType::base), icao_src_id(icao_id), offset(offset), length_next(length_next), timeout(timeout), message_authentication_code(0) {
			}
			L2HeaderBase() : L2HeaderBase(SYMBOLIC_ID_UNSET, 0, 0, 0) {}
			
			unsigned int getBits() const override {
				return icao_src_id.getBits()
                       + 8 /* offset */
				       + 4 /* length_next */
				       + 8 /* timeout */
				       + 800 /* 100B MAC */
				       + L2Header::getBits();
			}
			
			/** Number of slots until the next transmission. */
			unsigned int offset;
			/** Number of slots next frame will occupy. */
			unsigned int length_next;
			/** Remaining number of repetitions this reservation remains valid for. */
			unsigned int timeout;
			/** Origin ID. */
			MacId icao_src_id;
			/** Combines authentication with error detection / correction. */
			unsigned int message_authentication_code;
	};
	
	class L2HeaderBroadcast : public L2Header {
		public:
			L2HeaderBroadcast()
					: L2Header(FrameType::broadcast) {}

            /** Whether the contained L3 Packet starts with this fragment **/
            bool is_pkt_start = false;

            /** Whether the contained L3 Packet ends with this fragment **/
            bool is_pkt_end = false;
			
			unsigned int getBits() const override {
				return L2Header::getBits() +2; // Two additional bits for start and end flags
			}
	};
	
	class L2HeaderBeacon : public L2Header {
		public:
			L2HeaderBeacon(const CPRPosition& position, bool is_cpr_odd, unsigned int num_hops_to_ground_station, CPRPosition::PositionQuality pos_quality)
					: L2Header(L2Header::FrameType::beacon), position(position), is_cpr_odd(is_cpr_odd), num_hops_to_ground_station(num_hops_to_ground_station), pos_quality(pos_quality) {}
			L2HeaderBeacon() : L2HeaderBeacon(CPRPosition(), CPRPosition().odd, 0, CPRPosition::PositionQuality::low) {}
			
			unsigned int getBits() const override {
				return position.getBits()
				       + 1 /* odd/even identifier */
				       + 5 /* number of hops to ground station */
				       + 2 /* position quality */
				       + L2Header::getBits();
			}
			
			CPRPosition position;
			bool is_cpr_odd;
			unsigned int num_hops_to_ground_station;
			CPRPosition::PositionQuality pos_quality;
	};
	
	class L2HeaderUnicast : public L2Header {
		public:
			L2HeaderUnicast(const MacId& icao_dest_id, bool use_arq, const SequenceNumber& seqno, const SequenceNumber& seqno_next_expected, unsigned int arq_ack_slot, FrameType frame_type)
					: L2Header(frame_type), icao_dest_id(icao_dest_id), use_arq(use_arq), seqno(seqno), seqno_next_expected(seqno_next_expected), srej_size(0), arq_ack_slot(arq_ack_slot) {
				if (frame_type != FrameType::unicast && frame_type != FrameType::link_establishment_reply && frame_type != FrameType::link_establishment_request)
					throw std::invalid_argument("Cannot instantiate a unicast header with a non-unicast type.");
			}
			
			L2HeaderUnicast(const MacId& icao_dest_id, bool use_arq, SequenceNumber arq_seqno, SequenceNumber arq_ack_no, unsigned int arq_ack_slot)
					: L2HeaderUnicast(icao_dest_id, use_arq, arq_seqno, arq_ack_no, arq_ack_slot, FrameType::unicast) {}
					
			explicit L2HeaderUnicast(FrameType frame_type) : L2HeaderUnicast(SYMBOLIC_ID_UNSET, false, SequenceNumber(0), SequenceNumber(0), 0, frame_type) {}
			
			const MacId& getDestId() const {
				return this->icao_dest_id;
			}

			unsigned int getBits() const override {
				return 1 /* Whether ARQ is used */
				       + 2 /* RLC start and end flags */
				       + 8 /* ARQ sequence number */
				       + 8 /* ARQ ACK number */
				       + 8 /* ARQ slot indication number */
				       + icao_dest_id.getBits() /* destination ID */
				       + 8 * srej_list.size()
				       + 8 /* List size of srej_list */
				       + L2Header::getBits();
			}

            /** Whether the contained L3 Packet starts with this fragment **/
            bool is_pkt_start = false;

            /** Whether the contained L3 Packet ends with this fragment **/
            bool is_pkt_end = false;

            /** Whether the ARQ protocol is followed for this transmission, i.e. acknowledgements are expected. */
			bool use_arq;
			/** ARQ sequence number. */
            SequenceNumber seqno;
			/** ARQ acknowledgement. */
            SequenceNumber seqno_next_expected;
            /** Selective rejection list size */
            unsigned int srej_size;
            /** Selective rejection list. */
            std::vector<SequenceNumber> srej_list;
			/** The offset to the next reserved slot where an acknowledgement is expected. */
			unsigned int arq_ack_slot;
            /** Sequence number setter */
			void setSeqno(SequenceNumber seqno) {
			    this->seqno = seqno;
			}
            /** Next expected sequence number setter function */
			void setSeqnoNextExpected(SequenceNumber seqno) {
			    this->seqno_next_expected = seqno;
			}
            /** Srej list setter function */
			void setSrejList(std::vector<SequenceNumber> list) {
			    this->srej_list = list;
			}
            /** Get srej list length */
			uint8_t getSrejListLength() {
			    return this->srej_list.size();
			}
            /** Get srej list */
			std::vector<SequenceNumber> getSrejList() {
			    return this->srej_list;
			}
            /** Get sequence number */
			SequenceNumber getSeqno() {
                return this->seqno;
			}
            /** Get next expected sequence number */
			SequenceNumber getSeqnoNextExpected() {
                return this->seqno_next_expected;
			}
			
			/** Destination ICAO ID. */
			MacId icao_dest_id;
	};
	
	class L2HeaderLinkEstablishmentRequest : public L2HeaderUnicast {
		public:
			L2HeaderLinkEstablishmentRequest(const MacId& icao_dest_id, bool use_arq, const SequenceNumber& arq_seqno, const SequenceNumber& arq_ack_no, unsigned int arq_ack_slot)
					: L2HeaderUnicast(icao_dest_id, use_arq, arq_seqno, arq_ack_no, arq_ack_slot, FrameType::link_establishment_request) {}
					
			L2HeaderLinkEstablishmentRequest() : L2HeaderLinkEstablishmentRequest(SYMBOLIC_ID_UNSET, false, SequenceNumber(0), SequenceNumber(0), 0) {}
			
			/** Number of slots until the next transmission. */
			unsigned int offset = 0;
			/** Number of slots next frame will occupy. */
			unsigned int length_next = 0;
			/** Remaining number of repetitions this reservation remains valid for. */
			unsigned int timeout = 0;
			
			unsigned int getBits() const override {
				return L2HeaderUnicast::getBits()
					+ 8 /* offset */
				    + 4 /* length_next */
				    + 8 /* timeout */;
			}
	};
	
	class L2HeaderLinkEstablishmentReply : public L2HeaderUnicast {
		public:
			L2HeaderLinkEstablishmentReply(const MacId& icao_dest_id, bool use_arq, const SequenceNumber& arq_seqno, const SequenceNumber& arq_ack_no, unsigned int arq_ack_slot)
					: L2HeaderUnicast(icao_dest_id, use_arq, arq_seqno, arq_ack_no, arq_ack_slot, FrameType::link_establishment_reply) {}
			
			L2HeaderLinkEstablishmentReply() : L2HeaderLinkEstablishmentReply(SYMBOLIC_ID_UNSET, false, SequenceNumber(0), SequenceNumber(0), 0) {}
	};

	inline std::ostream& operator<<(std::ostream& stream, const L2HeaderLinkEstablishmentRequest& request) {
		return stream << std::string("to=") << request.icao_dest_id << std::string(" offset=") << std::to_string(request.offset) << std::string(" timeout=") << std::to_string(request.timeout) << std::string(" length_next=") << std::to_string(request.length_next);
	}
	
}

#endif //INTAIRNET_LINKLAYER_GLUE_L2HEADER_HPP
