//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_L2HEADER_HPP
#define INTAIRNET_LINKLAYER_GLUE_L2HEADER_HPP

#include <algorithm>
#include <vector>
#include <array>
#include "MacId.hpp"
#include "CPRPosition.hpp"
#include "SequenceNumber.hpp"
#include "LinkProposal.hpp"
#include "SlotDuration.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {

	/**
	 * Data Link Layer Headers.
	 */
	class L2Header {
	public:
		enum FrameType {
			unset,
			base,
			broadcast,
			unicast,
			link_establishment_request,
			link_establishment_reply,
			dme_request,
			dme_response
		};		
		enum Modulation {
			BPSK,
			QPSK
		};
		class Direction {
		public:
			bool North = false;
			bool NorthEast = false;
			bool East = false;
			bool SouthEast = false;
			bool South = false;
			bool SouthWest = false;
			bool West = false;
			bool NorthWest = false;
		};

		L2Header() : frame_type(unset) {}
		explicit L2Header(L2Header::FrameType frame_type) : frame_type(frame_type) {}		
		L2Header(const L2Header& other) = default;
		virtual ~L2Header() = default;

		virtual L2Header* copy() const {
			return new L2Header(*this);
		}

		virtual unsigned int getBits() const {
			return 3 /* frame type */;
		}

		bool operator==(const L2Header& other) const {
			return frame_type == other.frame_type;
		}

		bool operator!=(const L2Header& other) const {
			return !((*this) == other);
		}

		bool isUnicastType() const {
			return frame_type == L2Header::link_establishment_reply || frame_type == L2Header::link_establishment_request || frame_type == L2Header::unicast;
		}

		bool isBroadcastType() const {
			return frame_type == L2Header::broadcast;
		}		

		bool isDMERequest() const {
			return frame_type == L2Header::dme_request;
		}

		bool isDMEResponse() const {
			return frame_type == L2Header::dme_response;
		}

		/** This frame's type. */
		const FrameType frame_type;
	};

	class L2HeaderSH : public L2Header {
	public:

		L2HeaderSH() : L2Header(L2Header::FrameType::broadcast) {}
		L2HeaderSH(MacId id) : L2Header(L2Header::FrameType::broadcast), src_id(id) {}
		L2HeaderSH(const L2HeaderSH &other) : L2Header(other) {
			signature = other.signature;
			src_id = other.src_id; 
			slot_offset = other.slot_offset;
			slot_duration = other.slot_duration;
			position = other.position;
			time_src = other.time_src;
			num_hops = other.num_hops;
			time_tx = other.time_tx; 
			request_time_rx = other.request_time_rx;
			response_time_rx = other.response_time_rx; 
			link_status = other.link_status;
			link_utilizations = other.link_utilizations; 
			link_proposals = other.link_proposals; 
			link_requests = other.link_requests; 			
			link_reply = other.link_reply;
			is_pkt_start = other.is_pkt_start;
			is_pkt_end = other.is_pkt_end; 			
		}

		L2HeaderSH* copy() const override {
			return new L2HeaderSH(*this);
		}

		/** ECC-384. Not actually used in the simulator. */
		class Signature {
		public:
			static unsigned int getBits() {
				return 96*8;
			}
		};

		class LinkStatus {
		public:
			int num_proposals = 0;
			/** indicates that links in particular directions are wanted */
			Direction direction;
			/** number of resource utilization messages */
			int num_utilizations = 0;
			/** datarates for (direction, priority)-pairs */
			int datarates = 0;

			static unsigned int getBits() {
				return 4 /* num_proposals */
				+ 4 /* direction */
				+ 4 /* num_utilizations */
				+ 16*8 /* datarates */
				;
			}
		};

		class LinkUtilizationMessage {
		public:
			LinkUtilizationMessage() : slot_offset(0), slot_duration(SlotDuration::twentyfour_ms), num_bursts_forward(0), num_bursts_reverse(0), period(0), center_frequency(0), timeout(0) {}
			LinkUtilizationMessage(int slot_offset, SlotDuration slot_duration, int num_bursts_forward = 0, int num_bursts_reverse = 0, int period = 0, int center_frequency = 0, int timeout = 0)
				: slot_offset(slot_offset), slot_duration(slot_duration), num_bursts_forward(num_bursts_forward), num_bursts_reverse(num_bursts_reverse), period(period), center_frequency(center_frequency), timeout(timeout) {}
			bool operator==(LinkUtilizationMessage const& rhs) const { return slot_offset == rhs.slot_duration && slot_duration == rhs.slot_duration && num_bursts_forward == rhs.num_bursts_forward && num_bursts_reverse == rhs.num_bursts_reverse && period == rhs.period && center_frequency == rhs.center_frequency && timeout == rhs.timeout; }
			bool operator!=(LinkUtilizationMessage const& rhs) const { return !(*this == rhs);}
			

			int slot_offset = 0;
			SlotDuration slot_duration = SlotDuration::twentyfour_ms;
			int num_bursts_forward = 0, num_bursts_reverse = 0;
			int period = 0;
			int center_frequency = 0;
			int timeout = 0;

			static unsigned int getBits() {
				return 14 /* slot_offset */ + 2 /* slot_duration */ + 2 /* num_bursts_forward */ + 2 /* num_bursts_reverse */ + 3 /* period */ + 9 /* center frequency */ + 8 /* timeout */;
			}
		};

		class LinkProposalMessage {
		public:
			LinkProposalMessage() {}
			explicit LinkProposalMessage(const LinkProposal &proposal) : proposed_link(proposal) {}
			LinkProposal proposed_link;
			int noise = 0;

			static unsigned int getBits() {
				return 14 /* slot_offset */ + 2 /* slot_duration */ + 4 /* noise */ + 3 /* period */ + 9 /* center_frequency */
				;
			}
		};

		class LinkRequest {
		public:
			LinkRequest() {}
			LinkRequest(const MacId &dest_id, const LinkProposal &proposal) : dest_id(dest_id), proposed_link(proposal) {}
			LinkRequest(const MacId &dest_id, const LinkProposal &proposal, uint64_t generation_time) : dest_id(dest_id), proposed_link(proposal), generation_time(generation_time) {}

			Modulation modulation = Modulation::BPSK;
			int type = 0;
			MacId dest_id;
			LinkProposal proposed_link;
			int num_forward_bursts = 1, num_reverse_bursts = 1;
			/** Won't be part of an actual link request, but this allows the recipient to have a valid link establishment time statistic. */
			uint64_t generation_time = 0;

			static unsigned int getBits() {
				return 4 /*modulation*/ + 4 /*type*/ + MacId::getBits() + 14 /*slot_offset*/ 
					+ 2 /*slot_duration*/ + 3 /*period*/ + 9 /*center_frequency*/ + 4 /*forward/reverse*/;
			}
		};
		class LinkReply {
		public:
			LinkReply() {}
			LinkReply(const MacId &dest_id, const LinkProposal &proposal) : dest_id(dest_id), proposed_link(proposal) {}
			bool operator==(LinkReply const& rhs) const {return type == rhs.type && dest_id == rhs.dest_id && proposed_link == rhs.proposed_link && num_forward_bursts == rhs.num_forward_bursts && num_reverse_bursts == rhs.num_reverse_bursts;}
			bool operator!=(LinkReply const& rhs) const { return !(*this == rhs);}
			
			Modulation modulation = Modulation::BPSK;
			int type = 0;
			MacId dest_id = SYMBOLIC_ID_UNSET;
			LinkProposal proposed_link = LinkProposal();
			int num_forward_bursts = 1, num_reverse_bursts = 1;

			static unsigned int getBits() {
				return 4 /*modulation*/ + 4 /*type*/ + MacId::getBits() + 14 /*slot_offset*/ 
					+ 2 /*slot_duration*/ + 3 /*period*/ + 9 /*center_frequency*/ + 4 /*forward/reverse*/;
			}
		};
		
		Signature signature = Signature();
		MacId src_id = SYMBOLIC_ID_UNSET;
		/** advertises next transmission slot */
		unsigned int slot_offset = 0; 
		SlotDuration slot_duration = SlotDuration::twentyfour_ms;
		CPRPosition position;
		int time_src = 0;
		int num_hops = 0;
		int time_tx = 0;
		/** flag to request the transmission of reception time for two-way ranging */
		bool request_time_rx = false;
		/** flag to indicate that requested reception time is saved in this message */
		bool response_time_rx = false;
		LinkStatus link_status;
		std::vector<LinkUtilizationMessage> link_utilizations;
		std::vector<LinkProposalMessage> link_proposals;
		std::vector<LinkRequest> link_requests;
		LinkReply link_reply;
		bool is_pkt_end = false, is_pkt_start = false;

		unsigned int getBits() const override {
			return L2Header::getBits() + signature.getBits() + src_id.getBits() 
				+ 14 /* slot_offset */
				+ 2 /* slot_duration */
				+ position.getBits()
				+ 3 /* time_src */
				+ 4 /* num_hops */
				+ 64 /* time_tx */
				+ LinkStatus::getBits()
				+ link_utilizations.size() * LinkUtilizationMessage::getBits()
				+ link_proposals.size() * LinkProposalMessage::getBits()
				+ link_requests.size() * LinkRequest::getBits()
				;
		}
	};

	class L2HeaderPP : public L2Header {
	public:
		L2HeaderPP() : L2Header(L2Header::FrameType::unicast) {}
		L2HeaderPP(const MacId& dest_id) : L2Header(L2Header::FrameType::unicast), dest_id(dest_id) {}
		L2HeaderPP(const MacId& src_id, const MacId& dest_id) : L2Header(L2Header::FrameType::unicast), src_id(src_id), dest_id(dest_id) {}
		L2HeaderPP(const L2HeaderPP &other) : L2Header(other) {
			src_id = other.src_id;
			dest_id = other.dest_id;
			is_pkt_start = other.is_pkt_start;
			is_pkt_end = other.is_pkt_end;			
		}

		L2HeaderPP* copy() const override {
			return new L2HeaderPP(*this);
		}

		MacId src_id, dest_id;		
		bool is_pkt_end = false, is_pkt_start = false;
	};

	class L2HeaderDMERequest : public L2Header {
	public:
		L2HeaderDMERequest() : L2Header(FrameType::dme_request) {}
	};

	class L2HeaderDMEResponse : public L2Header {
	public:
		L2HeaderDMEResponse() : L2Header(FrameType::dme_response) {}
	};

	// class L2HeaderBase : public L2Header {
	// public:
	// 	L2HeaderBase(const MacId& icao_id, unsigned int burst_offset, unsigned int burst_length, unsigned int burst_length_tx, unsigned int timeout)
	// 			: L2Header(FrameType::base), src_id(icao_id), burst_offset(burst_offset), burst_length(burst_length), burst_length_tx(burst_length_tx), timeout(timeout), message_authentication_code(0) {
	// 	}

	// 	L2HeaderBase() : L2HeaderBase(SYMBOLIC_ID_UNSET, 0, 0, 0, 0) {}

	// 	L2HeaderBase(const L2HeaderBase& other) : L2Header((const L2Header&) other) {
	// 		burst_offset = other.burst_offset;
	// 		burst_length = other.burst_length;
	// 		burst_length_tx = other.burst_length_tx;
	// 		timeout = other.timeout;
	// 		src_id = other.src_id;
	// 		message_authentication_code = other.message_authentication_code;
	// 	}

	// 	L2HeaderBase* copy() const override {
	// 		return new L2HeaderBase(*this);
	// 	}

	// 	unsigned int getBits() const override {
	// 		return MacId::getBits()				   
	// 			   + L2Header::getBits()
	// 		       + 8 /* burst_offset */
	// 		       + 4 /* burst_length */
	// 		       + 4 /* burst_length_tx */
	// 		       + 8 /* timeout */
	// 		       + 96 /* message authentication code */
	// 			   + 1 /* padding */
	// 		       ;
	// 	}

	// 	bool operator==(const L2HeaderBase& other) const {
	// 		return other.burst_offset == burst_offset && other.burst_length == burst_length && other.burst_length_tx == burst_length_tx && other.timeout == timeout && other.src_id == src_id && other.message_authentication_code == message_authentication_code;
	// 	}

	// 	bool operator!=(const L2HeaderBase& other) const {
	// 		return !((*this) == other);
	// 	}

	// 	/** Number of slots until the next transmission. */
	// 	unsigned int burst_offset;
	// 	/** Number of slots next frame will occupy. */
	// 	unsigned int burst_length;
	// 	/** Number of initial transmission slots for the link initiator. */
	// 	unsigned int burst_length_tx;
	// 	/** Remaining number of repetitions this reservation remains valid for. */
	// 	unsigned int timeout;
	// 	/** Origin ID. */
	// 	MacId src_id;
	// 	/** Combines authentication with error detection / correction. */
	// 	unsigned int message_authentication_code;
	// };

	// class L2HeaderWithDestination : public L2Header {
	// public:
	// 	MacId dest_id;

	// 	explicit L2HeaderWithDestination(FrameType frame_type, const MacId& dest_id) : L2Header(frame_type), dest_id(dest_id) {}
	// 	L2HeaderWithDestination(const L2HeaderWithDestination &other) : L2HeaderWithDestination(other.frame_type, other.dest_id) {}
	// 	L2HeaderWithDestination* copy() const override {
	// 		return new L2HeaderWithDestination(*this);
	// 	}

	// 	unsigned int getBits() const override {
	// 		return MacId::getBits() + L2Header::getBits();
	// 	}

	// 	bool operator==(const L2HeaderWithDestination &other) {
	// 		return dest_id == other.dest_id && ((L2Header&) *this) == ((L2Header&) other);
	// 	}
	// 	bool operator!=(const L2HeaderWithDestination &other) {
	// 		return !(*this == other);
	// 	}

	// 	const MacId& getDestId() const {
	// 		return this->dest_id;
	// 	}
	// };

	// class L2HeaderBroadcast : public L2Header {
	// public:
	// 	L2HeaderBroadcast() : L2Header(FrameType::broadcast) {}

	// 	L2HeaderBroadcast(const L2HeaderBroadcast& other) : L2Header((const L2Header&) other) {
	// 		is_pkt_start = other.is_pkt_start;
	// 		is_pkt_end = other.is_pkt_end;
	// 		seqno = other.seqno;
	// 		payload_length = other.payload_length;
	// 	}

	// 	L2HeaderBroadcast* copy() const override {
	// 		return new L2HeaderBroadcast(*this);
	// 	}

	// 	/** Whether the contained L3 Packet starts with this fragment **/
	// 	bool is_pkt_start = false;

	// 	/** Whether the contained L3 Packet ends with this fragment **/
	// 	bool is_pkt_end = false;

	// 	bool operator==(const L2HeaderBroadcast& other) const {
	// 		return other.is_pkt_start == is_pkt_start && other.is_pkt_end == is_pkt_end && other.seqno == seqno && other.payload_length == payload_length;
	// 	}

	// 	bool operator!=(const L2HeaderBroadcast& other) const {
	// 		return !((*this) == other);
	// 	}

	// 	unsigned int getBits() const override {
	// 		return L2Header::getBits() 
	// 				+ 2 /** Two additional bits for start and end flags */
	// 				+ 8 /** seqno */
	// 				+ 12 /** payload length */
	// 				+ 7 /** padding */
	// 				;
	// 	}

	// 	unsigned int seqno = 0;
	// 	unsigned int payload_length = 0;
	// };	

	// class L2HeaderBeacon : public L2Header {
	// public:
	// 	enum CongestionLevel {
	// 		uncongested,
	// 		slightly_congested,
	// 		moderately_congested,
	// 		congested
	// 	};

	// 	L2HeaderBeacon(const CPRPosition& position, bool is_cpr_odd, L2HeaderBeacon::CongestionLevel congestion_level, CPRPosition::PositionQuality pos_quality)
	// 			: L2Header(L2Header::FrameType::beacon), position(position), is_cpr_odd(is_cpr_odd), congestion_level(congestion_level), payload_length(0) {}

	// 	L2HeaderBeacon() : L2HeaderBeacon(CPRPosition(), CPRPosition().odd, L2HeaderBeacon::CongestionLevel::uncongested, CPRPosition::PositionQuality::low) {}

	// 	L2HeaderBeacon(const L2HeaderBeacon& other) : L2Header((const L2Header&) other) {
	// 		position = other.position;
	// 		is_cpr_odd = other.is_cpr_odd;					
	// 		payload_length = other.payload_length;
	// 		congestion_level = other.congestion_level;			
	// 	}

	// 	L2HeaderBeacon* copy() const override {
	// 		return new L2HeaderBeacon(*this);
	// 	}

	// 	unsigned int getBits() const override {
	// 		return position.getBits()
	// 		       + 1 /* odd/even identifier */
	// 		       + 4 /* congestion level */
	// 		       + 12 /* payload length */
	// 		       + L2Header::getBits()
	// 			   + 6 /* padding */
	// 			   ;
	// 	}

	// 	bool operator==(const L2HeaderBeacon& other) const {
	// 		return other.position == position && other.is_cpr_odd == is_cpr_odd && congestion_level == other.congestion_level && payload_length == other.payload_length;
	// 	}

	// 	bool operator!=(const L2HeaderBeacon& other) const {
	// 		return !((*this) == other);
	// 	}

	// 	CPRPosition position;
	// 	bool is_cpr_odd;						
	// 	L2HeaderBeacon::CongestionLevel congestion_level;
	// 	unsigned int payload_length;
	// };

	// class L2HeaderUnicast : public L2HeaderWithDestination {
	// public:
	// 	L2HeaderUnicast(const MacId& icao_dest_id, bool use_arq, const SequenceNumber& seqno, const SequenceNumber& seqno_next_expected, FrameType frame_type)
	// 			: L2HeaderWithDestination(frame_type, icao_dest_id), use_arq(use_arq), seqno(seqno), seqno_next_expected(seqno_next_expected) {
	// 		if (frame_type != FrameType::unicast && frame_type != FrameType::link_establishment_reply && frame_type != FrameType::link_establishment_request)
	// 			throw std::invalid_argument("Cannot instantiate a unicast header with a non-unicast type.");
	// 	}

	// 	L2HeaderUnicast(const MacId& icao_dest_id, bool use_arq, SequenceNumber arq_seqno, SequenceNumber arq_ack_no)
	// 			: L2HeaderUnicast(icao_dest_id, use_arq, arq_seqno, arq_ack_no, FrameType::unicast) {}

	// 	L2HeaderUnicast(const L2HeaderUnicast& other) : L2HeaderWithDestination((const L2HeaderWithDestination&) other) {
	// 		is_pkt_start = other.is_pkt_start;
	// 		is_pkt_end = other.is_pkt_end;
	// 		use_arq = other.use_arq;
	// 		seqno = SequenceNumber(other.seqno);
	// 		seqno_next_expected = SequenceNumber(other.seqno_next_expected);			
	// 		burst_length_tx_desire = other.burst_length_tx_desire;
    //         srej_bitmap = other.srej_bitmap;
	// 		payload_length = other.payload_length;
	// 	}

	// 	L2HeaderUnicast* copy() const override {
	// 		return new L2HeaderUnicast(*this);
	// 	}

	// 	explicit L2HeaderUnicast(FrameType frame_type) : L2HeaderUnicast(SYMBOLIC_ID_UNSET, false, SequenceNumber(0), SequenceNumber(0), frame_type) {}

	// 	L2HeaderUnicast() : L2HeaderUnicast(FrameType::unicast) {}

	// 	unsigned int getBits() const override {
	// 		return L2HeaderWithDestination::getBits()
	// 			   + 1 /* Whether ARQ is used */				   
	// 		       + 2 /* RLC start and end flags */
	// 		       + 8 /* ARQ sequence number */
	// 		       + 8 /* ARQ ACK number */			       
	// 		       + 4 /* burst length desire */
	// 		       + 16 /* Srej bit map */
	// 			   + 12 /* payload length */			       
	// 			   + 6 /* padding */
	// 			   ;
	// 	}

	// 	bool operator==(const L2HeaderUnicast& other) const {
	// 		return other.is_pkt_start == is_pkt_start && other.is_pkt_end == is_pkt_end && other.use_arq == use_arq && other.seqno == seqno && other.seqno_next_expected == seqno_next_expected
	// 		       && burst_length_tx_desire == other.burst_length_tx_desire && payload_length == other.payload_length && ((L2HeaderWithDestination&) *this == (L2HeaderWithDestination&) other);
	// 	}

	// 	bool operator!=(const L2HeaderUnicast& other) const {
	// 		return !((*this) == other);
	// 	}

	// 	/** Whether the contained L3 Packet starts with this fragment **/
	// 	bool is_pkt_start = false;

	// 	/** Whether the contained L3 Packet ends with this fragment **/
	// 	bool is_pkt_end = false;

	// 	/** Whether the ARQ protocol is followed for this transmission, i.e. acknowledgements are expected. */
	// 	bool use_arq;
	// 	/** ARQ sequence number. */
	// 	SequenceNumber seqno;
	// 	/** ARQ acknowledgement. */
	// 	SequenceNumber seqno_next_expected;
	// 	/** Selective rejection list. */
    //     std::array<bool, 16> srej_bitmap = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};		
	// 	/** The number of transmission slots the local node would like to have, which will be respected upon link renewal. */
	// 	unsigned int burst_length_tx_desire = 0;
	// 	unsigned int payload_length = 0;

	// 	/** Sequence number setter */
	// 	void setSeqno(SequenceNumber seqno) {
	// 		this->seqno = seqno;
	// 	}

	// 	/** Next expected sequence number setter function */
	// 	void setSeqnoNextExpected(SequenceNumber seqno) {
	// 		this->seqno_next_expected = seqno;
	// 	}

	// 	/** Get srej list */
	// 	std::array<bool, 16> getSrejList() {
	// 		return this->srej_bitmap;
	// 	}

    //     /** Set srej list */
    //     void setSrejBitmap(std::array<bool, 16> srej) {
    //         this->srej_bitmap = srej;
	// 	}

	// 	/** Get sequence number */
	// 	SequenceNumber getSeqno() {
	// 		return this->seqno;
	// 	}

	// 	/** Get next expected sequence number */
	// 	SequenceNumber getSeqnoNextExpected() {
	// 		return this->seqno_next_expected;
	// 	}
	// };

	// class L2HeaderLinkRequest : public L2HeaderWithDestination {
	// public:
	// 	explicit L2HeaderLinkRequest(const MacId& dest_id) : L2HeaderWithDestination(FrameType::link_establishment_request, dest_id) {}
	// 	L2HeaderLinkRequest() : L2HeaderLinkRequest(SYMBOLIC_ID_UNSET) {}
	// 	L2HeaderLinkRequest(const L2HeaderLinkRequest& other) : L2HeaderLinkRequest(other.dest_id) {
	// 		timeout = other.timeout;
	// 		burst_offset = other.burst_offset;
	// 		burst_length = other.burst_length;
	// 		burst_length_tx = other.burst_length_tx;
	// 		reply_offset = other.reply_offset;
	// 		payload_length = other.payload_length;
	// 	}

	// 	L2HeaderLinkRequest* copy() const override {
	// 		return new L2HeaderLinkRequest(*this);
	// 	}

	// 	unsigned int getBits() const override {
	// 		return L2HeaderWithDestination::getBits() 
	// 				+ 8 /* reply_offset */ 					
	// 				+ 8 /* burst_offset */ 
	// 				+ 4 /* burst_length */ 
	// 				+ 4 /* burst_length_tx */ 					
	// 				+ 8 /* timeout */ 
	// 				+ 12 /* payload length */
	// 				+ 5 /* padding */
	// 				;
	// 	}

	// 	bool operator==(const L2HeaderLinkRequest& other) const {
	// 		return timeout == other.timeout
	// 			&& burst_offset == other.burst_offset
	// 			&& burst_length == other.burst_length
	// 			&& burst_length_tx == other.burst_length_tx
	// 			&& dest_id == other.dest_id
	// 			&& reply_offset == other.reply_offset
	// 			&& ((L2Header&) *this) == ((L2Header&) other);
	// 	}

	// 	bool operator!=(const L2HeaderLinkRequest& other) const {
	// 		return !((*this) == other);
	// 	}

	// 	unsigned int timeout = 0;
	// 	unsigned int burst_offset = 0;
	// 	unsigned int burst_length = 0;
	// 	unsigned int burst_length_tx = 0;
	// 	unsigned int reply_offset = 0;
	// 	unsigned int payload_length = 0;
	// };

	// class L2HeaderLinkReply : public L2HeaderWithDestination {
	// public:
	// 	explicit L2HeaderLinkReply(const MacId& dest_id) : L2HeaderWithDestination(FrameType::link_establishment_reply, dest_id) {}
	// 	L2HeaderLinkReply() : L2HeaderLinkReply(SYMBOLIC_ID_UNSET) {}
	// 	L2HeaderLinkReply(const L2HeaderLinkReply& other) : L2HeaderLinkReply(other.dest_id) {
	// 		timeout = other.timeout;
	// 		burst_offset = other.burst_offset;
	// 		burst_length = other.burst_length;
	// 		burst_length_tx = other.burst_length_tx;
	// 		payload_length = other.payload_length;
	// 	}

	// 	L2HeaderLinkReply* copy() const override {
	// 		return new L2HeaderLinkReply(*this);
	// 	}

	// 	unsigned int getBits() const override {
	// 		return L2HeaderWithDestination::getBits() 					
	// 				+ 8 /* burst_offset */ 
	// 				+ 4 /* burst_length */ 
	// 				+ 4 /* burst_length_tx */ 					
	// 				+ 8 /* timeout */ 
	// 				+ 12 /* payload length */
	// 				+ 5 /* padding */
	// 				;
	// 	}

	// 	bool operator==(const L2HeaderLinkReply& other) const {
	// 		return timeout == other.timeout
	// 		       && burst_offset == other.burst_offset
	// 		       && burst_length == other.burst_length
	// 		       && burst_length_tx == other.burst_length_tx
	// 		       && dest_id == other.dest_id
	// 		       && ((L2Header&) *this) == ((L2Header&) other);
	// 	}

	// 	bool operator!=(const L2HeaderLinkReply& other) const {
	// 		return !((*this) == other);
	// 	}

	// 	unsigned int timeout = 0;
	// 	unsigned int burst_offset = 0;
	// 	unsigned int burst_length = 0;
	// 	unsigned int burst_length_tx = 0;
	// 	unsigned int payload_length = 0;
	// };		

	inline std::ostream& operator<<(std::ostream& stream, const L2Header::FrameType& frame_type) {
		std::string s;
		switch (frame_type) {
			case L2Header::FrameType::unset: {s = "unset"; break;}
			case L2Header::FrameType::base: {s = "base"; break;}						
			case L2Header::FrameType::broadcast: {s = "broadcast"; break;}
			case L2Header::FrameType::unicast: {s = "unicast"; break;}
			case L2Header::FrameType::link_establishment_reply: {s = "link_reply"; break;}
			case L2Header::FrameType::link_establishment_request: {s = "link_request"; break;}
			case L2Header::FrameType::dme_request: {s = "dme_request"; break;}
			case L2Header::FrameType::dme_response: {s = "dme_response"; break;}
			default: {throw std::invalid_argument("please add new header type to operator<< of FrameType: " + std::to_string(frame_type) + "!");}
		}
		return stream << s;
	}

}

#endif //INTAIRNET_LINKLAYER_GLUE_L2HEADER_HPP
