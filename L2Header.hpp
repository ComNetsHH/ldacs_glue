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
			return frame_type == L2Header::broadcast || frame_type == L2Header::beacon;
		}

		/** This frame's type. */
		const FrameType frame_type;
	};

	class L2HeaderBase : public L2Header {
	public:
		L2HeaderBase(const MacId& icao_id, unsigned int burst_offset, unsigned int burst_length, unsigned int burst_length_tx, unsigned int timeout)
				: L2Header(FrameType::base), src_id(icao_id), burst_offset(burst_offset), burst_length(burst_length), burst_length_tx(burst_length_tx), timeout(timeout), message_authentication_code(0) {
		}

		L2HeaderBase() : L2HeaderBase(SYMBOLIC_ID_UNSET, 0, 0, 0, 0) {}

		L2HeaderBase(const L2HeaderBase& other) : L2Header((const L2Header&) other) {
			burst_offset = other.burst_offset;
			burst_length = other.burst_length;
			burst_length_tx = other.burst_length_tx;
			timeout = other.timeout;
			src_id = other.src_id;
			message_authentication_code = other.message_authentication_code;
			link_renewal_negotiated = other.link_renewal_negotiated;
		}

		L2HeaderBase* copy() const override {
			return new L2HeaderBase(*this);
		}

		unsigned int getBits() const override {
			return MacId::getBits()
				   + 1 /* link_renewal_negotiated flag */
			       + 8 /* burst_offset */
			       + 4 /* burst_length */
			       + 4 /* burst_length_tx */
			       + 8 /* timeout */
			       + 100 /* 100bit MAC */
			       + L2Header::getBits();
		}

		bool operator==(const L2HeaderBase& other) const {
			return other.burst_offset == burst_offset && other.burst_length == burst_length && other.burst_length_tx == burst_length_tx && other.timeout == timeout && other.src_id == src_id && other.message_authentication_code == message_authentication_code;
		}

		bool operator!=(const L2HeaderBase& other) const {
			return !((*this) == other);
		}

		/** Flag that indicates the next link renewal has been negotiated. */
		bool link_renewal_negotiated = false;
		/** Number of slots until the next transmission. */
		unsigned int burst_offset;
		/** Number of slots next frame will occupy. */
		unsigned int burst_length;
		/** Number of initial transmission slots for the link initiator. */
		unsigned int burst_length_tx;
		/** Remaining number of repetitions this reservation remains valid for. */
		unsigned int timeout;
		/** Origin ID. */
		MacId src_id;
		/** Combines authentication with error detection / correction. */
		unsigned int message_authentication_code;
	};

	class L2HeaderWithDestination : public L2Header {
	public:
		MacId dest_id;

		explicit L2HeaderWithDestination(FrameType frame_type, const MacId& dest_id) : L2Header(frame_type), dest_id(dest_id) {}
		L2HeaderWithDestination(const L2HeaderWithDestination &other) : L2HeaderWithDestination(other.frame_type, other.dest_id) {}
		L2HeaderWithDestination* copy() const override {
			return new L2HeaderWithDestination(*this);
		}

		unsigned int getBits() const override {
			return MacId::getBits() + L2Header::getBits();
		}

		bool operator==(const L2HeaderWithDestination &other) {
			return dest_id == other.dest_id && ((L2Header&) *this) == ((L2Header&) other);
		}
		bool operator!=(const L2HeaderWithDestination &other) {
			return !(*this == other);
		}

		const MacId& getDestId() const {
			return this->dest_id;
		}
	};

	class L2HeaderBroadcast : public L2HeaderWithDestination {
	public:
		L2HeaderBroadcast() : L2HeaderWithDestination(FrameType::broadcast, SYMBOLIC_LINK_ID_BROADCAST) {}

		L2HeaderBroadcast(const L2HeaderBroadcast& other) : L2HeaderWithDestination((const L2HeaderWithDestination&) other) {
			is_pkt_start = other.is_pkt_start;
			is_pkt_end = other.is_pkt_end;
		}

		L2HeaderBroadcast* copy() const override {
			return new L2HeaderBroadcast(*this);
		}

		/** Whether the contained L3 Packet starts with this fragment **/
		bool is_pkt_start = false;

		/** Whether the contained L3 Packet ends with this fragment **/
		bool is_pkt_end = false;

		bool operator==(const L2HeaderBroadcast& other) const {
			return other.is_pkt_start == is_pkt_start && other.is_pkt_end == is_pkt_end;
		}

		bool operator!=(const L2HeaderBroadcast& other) const {
			return !((*this) == other);
		}

		unsigned int getBits() const override {
			return L2HeaderWithDestination::getBits() + 2; // Two additional bits for start and end flags
		}
	};

	class L2HeaderBeacon : public L2Header {
	public:
		L2HeaderBeacon(const CPRPosition& position, bool is_cpr_odd, unsigned int num_hops_to_ground_station, CPRPosition::PositionQuality pos_quality)
				: L2Header(L2Header::FrameType::beacon), position(position), is_cpr_odd(is_cpr_odd), num_hops_to_ground_station(num_hops_to_ground_station), pos_quality(pos_quality) {}

		L2HeaderBeacon() : L2HeaderBeacon(CPRPosition(), CPRPosition().odd, 0, CPRPosition::PositionQuality::low) {}

		L2HeaderBeacon(const L2HeaderBeacon& other) : L2Header((const L2Header&) other) {
			position = other.position;
			is_cpr_odd = other.is_cpr_odd;
			num_hops_to_ground_station = other.num_hops_to_ground_station;
			pos_quality = other.pos_quality;
			next_hop_towards_gs = other.next_hop_towards_gs;
		}

		L2HeaderBeacon* copy() const override {
			return new L2HeaderBeacon(*this);
		}

		unsigned int getBits() const override {
			return position.getBits()
			       + 1 /* odd/even identifier */
			       + 5 /* number of hops to ground station */
			       + 2 /* position quality */
			       + L2Header::getBits();
		}

		bool operator==(const L2HeaderBeacon& other) const {
			return other.position == position && other.is_cpr_odd == is_cpr_odd && other.num_hops_to_ground_station == num_hops_to_ground_station && other.pos_quality == pos_quality && next_hop_towards_gs == other.next_hop_towards_gs;
		}

		bool operator!=(const L2HeaderBeacon& other) const {
			return !((*this) == other);
		}

		CPRPosition position;
		bool is_cpr_odd;
		unsigned int num_hops_to_ground_station;
		MacId next_hop_towards_gs = SYMBOLIC_ID_UNSET;
		CPRPosition::PositionQuality pos_quality;
	};

	class L2HeaderUnicast : public L2HeaderWithDestination {
	public:
		L2HeaderUnicast(const MacId& icao_dest_id, bool use_arq, const SequenceNumber& seqno, const SequenceNumber& seqno_next_expected, unsigned int arq_ack_slot, FrameType frame_type)
				: L2HeaderWithDestination(frame_type, icao_dest_id), use_arq(use_arq), seqno(seqno), seqno_next_expected(seqno_next_expected), arq_ack_slot(arq_ack_slot) {
			if (frame_type != FrameType::unicast && frame_type != FrameType::link_establishment_reply && frame_type != FrameType::link_establishment_request)
				throw std::invalid_argument("Cannot instantiate a unicast header with a non-unicast type.");
		}

		L2HeaderUnicast(const MacId& icao_dest_id, bool use_arq, SequenceNumber arq_seqno, SequenceNumber arq_ack_no, unsigned int arq_ack_slot)
				: L2HeaderUnicast(icao_dest_id, use_arq, arq_seqno, arq_ack_no, arq_ack_slot, FrameType::unicast) {}

		L2HeaderUnicast(const L2HeaderUnicast& other) : L2HeaderWithDestination((const L2HeaderWithDestination&) other) {
			is_pkt_start = other.is_pkt_start;
			is_pkt_end = other.is_pkt_end;
			use_arq = other.use_arq;
			seqno = other.seqno;
			seqno_next_expected = other.seqno_next_expected;
			arq_ack_slot = other.arq_ack_slot;
			burst_length_tx_desire = other.burst_length_tx_desire;
		}

		L2HeaderUnicast* copy() const override {
			return new L2HeaderUnicast(*this);
		}

		explicit L2HeaderUnicast(FrameType frame_type) : L2HeaderUnicast(SYMBOLIC_ID_UNSET, false, SequenceNumber(0), SequenceNumber(0), 0, frame_type) {}

		unsigned int getBits() const override {
			return 1 /* Whether ARQ is used */
			       + 2 /* RLC start and end flags */
			       + 8 /* ARQ sequence number */
			       + 8 /* ARQ ACK number */
			       + 8 /* ARQ slot indication number */
			       + 4 /* burst length desire */
			       + 16 /* Srej bit map */
			       + L2HeaderWithDestination::getBits();
		}

		bool operator==(const L2HeaderUnicast& other) const {
			return other.is_pkt_start == is_pkt_start && other.is_pkt_end == is_pkt_end && other.use_arq == use_arq && other.seqno == seqno && other.seqno_next_expected == seqno_next_expected
			       && other.arq_ack_slot == arq_ack_slot && burst_length_tx_desire == other.burst_length_tx_desire && ((L2HeaderWithDestination&) *this == (L2HeaderWithDestination&) other);
		}

		bool operator!=(const L2HeaderUnicast& other) const {
			return !((*this) == other);
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
		/** Selective rejection list. */
        std::array<bool, 16> srej_bitmap = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
		/** The offset to the next reserved slot where an acknowledgement is expected. */
		unsigned int arq_ack_slot;
		/** The number of transmission slots the local node would like to have, which will be respected upon link renewal. */
		unsigned int burst_length_tx_desire = 0;

		/** Sequence number setter */
		void setSeqno(SequenceNumber seqno) {
			this->seqno = seqno;
		}

		/** Next expected sequence number setter function */
		void setSeqnoNextExpected(SequenceNumber seqno) {
			this->seqno_next_expected = seqno;
		}

		/** Get srej list */
		std::array<bool, 16> getSrejList() {
			return this->srej_bitmap;
		}

		/** Get sequence number */
		SequenceNumber getSeqno() {
			return this->seqno;
		}

		/** Get next expected sequence number */
		SequenceNumber getSeqnoNextExpected() {
			return this->seqno_next_expected;
		}
	};

	class L2HeaderLinkRequest : public L2HeaderWithDestination {
	public:
		unsigned int timeout = 0;
		unsigned int burst_offset = 0;
		unsigned int burst_length = 0;
		unsigned int burst_length_tx = 0;

		explicit L2HeaderLinkRequest(const MacId& dest_id) : L2HeaderWithDestination(FrameType::link_establishment_request, dest_id) {}
		L2HeaderLinkRequest() : L2HeaderLinkRequest(SYMBOLIC_ID_UNSET) {}
		L2HeaderLinkRequest(const L2HeaderLinkRequest& other) : L2HeaderLinkRequest(other.dest_id) {
			timeout = other.timeout;
			burst_offset = other.burst_offset;
			burst_length = other.burst_length;
			burst_length_tx = other.burst_length_tx;
		}

		L2HeaderLinkRequest* copy() const override {
			return new L2HeaderLinkRequest(*this);
		}

		unsigned int getBits() const override {
			return L2HeaderWithDestination::getBits() + 8 + 8 + 4 + 4 + MacId::getBits();
		}

		bool operator==(const L2HeaderLinkRequest& other) const {
			return timeout == other.timeout
				&& burst_offset == other.burst_offset
				&& burst_length == other.burst_length
				&& burst_length_tx == other.burst_length_tx
				&& dest_id == other.dest_id
				&& ((L2Header&) *this) == ((L2Header&) other);
		}

		bool operator!=(const L2HeaderLinkRequest& other) const {
			return !((*this) == other);
		}
	};

	class L2HeaderLinkReply : public L2HeaderWithDestination {
	public:
		unsigned int timeout = 0;
		unsigned int burst_offset = 0;
		unsigned int burst_length = 0;
		unsigned int burst_length_tx = 0;

		explicit L2HeaderLinkReply(const MacId& dest_id) : L2HeaderWithDestination(FrameType::link_establishment_reply, dest_id) {}
		L2HeaderLinkReply() : L2HeaderLinkReply(SYMBOLIC_ID_UNSET) {}
		L2HeaderLinkReply(const L2HeaderLinkReply& other) : L2HeaderLinkReply(other.dest_id) {
			timeout = other.timeout;
			burst_offset = other.burst_offset;
			burst_length = other.burst_length;
			burst_length_tx = other.burst_length_tx;
		}

		L2HeaderLinkReply* copy() const override {
			return new L2HeaderLinkReply(*this);
		}

		unsigned int getBits() const override {
			return L2HeaderWithDestination::getBits() + 8 + 8 + 4 + 4 + MacId::getBits();
		}

		bool operator==(const L2HeaderLinkReply& other) const {
			return timeout == other.timeout
			       && burst_offset == other.burst_offset
			       && burst_length == other.burst_length
			       && burst_length_tx == other.burst_length_tx
			       && dest_id == other.dest_id
			       && ((L2Header&) *this) == ((L2Header&) other);
		}

		bool operator!=(const L2HeaderLinkReply& other) const {
			return !((*this) == other);
		}
	};

	class L2HeaderLinkEstablishmentRequest : public L2HeaderUnicast {
	public:
		L2HeaderLinkEstablishmentRequest(const MacId& icao_dest_id, bool use_arq, const SequenceNumber& arq_seqno, const SequenceNumber& arq_ack_no, unsigned int arq_ack_slot)
				: L2HeaderUnicast(icao_dest_id, use_arq, arq_seqno, arq_ack_no, arq_ack_slot, FrameType::link_establishment_request) {}

		L2HeaderLinkEstablishmentRequest() : L2HeaderLinkEstablishmentRequest(SYMBOLIC_ID_UNSET, false, SequenceNumber(0), SequenceNumber(0), 0) {}

		L2HeaderLinkEstablishmentRequest(const L2HeaderLinkEstablishmentRequest& other) : L2HeaderUnicast((const L2HeaderUnicast&) other) {
			offset = other.offset;
			length_next = other.length_next;
			timeout = other.timeout;
		}

		L2HeaderLinkEstablishmentRequest* copy() const override {
			return new L2HeaderLinkEstablishmentRequest(*this);
		}

		bool operator==(const L2HeaderLinkEstablishmentRequest& other) const {
			return other.offset == offset && other.length_next == length_next && other.timeout == timeout && (L2HeaderUnicast) (*this) == ((L2HeaderUnicast) other);
		}

		bool operator!=(const L2HeaderLinkEstablishmentRequest& other) const {
			return !((*this) == other);
		}

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

		L2HeaderLinkEstablishmentReply(const L2HeaderLinkEstablishmentReply& other) : L2HeaderUnicast((const L2HeaderUnicast&) other) {}

		L2HeaderLinkEstablishmentReply* copy() const override {
			return new L2HeaderLinkEstablishmentReply(*this);
		}

		bool operator==(const L2HeaderLinkEstablishmentReply& other) const {
			return (L2HeaderUnicast) (*this) == ((L2HeaderUnicast) other);
		}

		bool operator!=(const L2HeaderLinkEstablishmentReply& other) const {
			return !((*this) == other);
		}
	};

	inline std::ostream& operator<<(std::ostream& stream, const L2HeaderLinkEstablishmentRequest& request) {
		return stream << std::string("to=") << request.dest_id << std::string(" offset=") << std::to_string(request.offset) << std::string(" timeout=") << std::to_string(request.timeout) << std::string(" length_next=")
		              << std::to_string(request.length_next);
	}

}

#endif //INTAIRNET_LINKLAYER_GLUE_L2HEADER_HPP
