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

	class L2Packet; // forward declaration so that L2PacketSetCallback can use it.

	class L2PacketSentCallback {
	public:
		/**
		 * Callback interface for when a transmission slot arrives which is sending a particular packet.
		 * @param packet
		 */
		virtual void packetBeingSentCallback(TUHH_INTAIRNET_MCSOTDMA::L2Packet* packet) = 0;
	};

	/**
	 * Wraps around an original packet implementation.
	 * It keeps a pointer to the original packet and adds functionality specific to the MC-SOTDMA protocol.
	 * When MC-SOTDMA operation finishes, the original packet is passed on to the respective receiving layer.
	 */
	class L2Packet {

		friend class LinkManagerTests;

	public:
		/**
		 * Interface for a wrapper of an upper-layer packet.
		*/
		class Payload {
		public:
			virtual unsigned int getBits() const = 0;

			virtual ~Payload() = default;

			virtual Payload* copy() const = 0;
		};

		L2Packet();

		L2Packet(const L2Packet& other);

		L2Packet* copy() const;

		virtual ~L2Packet();

		/**
		 * Add a (header, payload)-pair.
		 * These will be deleted by this L2Packet's destructor.
		 * @param header
		 * @param payload
		 */
		void addMessage(L2Header* header, Payload* payload);

		void addMessage(std::pair<L2Header*, Payload*> message);

		/**
		 * @return All payloads.
		 */
		std::vector<Payload*>& getPayloads();

		/**
		 * @return All headers.
		 */
		const std::vector<L2Header*>& getHeaders();
		void replaceBaseHeader(std::size_t i, L2HeaderBase header);

		/**
		 * @return Total size of this packet in bits, consisting of both headers and payloads.
		 */
		unsigned int getBits() const;

		/**
		 * @return This packet's destination ID.
		 */
		MacId getDestination() const;

		/**
		 * @return This packet's originator ID.
		 */
		const MacId& getOrigin() const;

		/**
		 * Register a callback that should be notified when this packet is about to be sent.
		 * For example it is used for a P2P LinkManager that is notified when its link request is about to be sent,
		 * so that it can compute the propsal.
		 * @param callback
		 */
		void addCallback(L2PacketSentCallback* callback);

		/**
		 * Notify all currently registered callbacks.
		 */
		void notifyCallbacks();

		/**
		 * Prints the entire packet structure for convenience
		 */
		std::string print();

		/**
		 * @return Index of a contained link request, or -1 if it doesn't contain one.
		 */
		int getRequestIndex() const;

		/**
		 * @return Index of a contained link reply, or -1 if it doesn't contain one.
		 */
		int getReplyIndex() const;

		/**
		 * @return Index of a contained link reply, or -1 if it doesn't contain one.
		 */
		int getBeaconIndex() const;		

		/**
		 * Erases header and payload at the given index.
		 */
		void erase(size_t index);

		/**
		 * Flag that indicates whether an error was introduced by transmitting over the channel
		 */
		bool hasChannelError = false;

		/**
		 * Distance from the sender at which the packet was received. Intended to calculate SINR. 
		 */
		double receptionDist = 0.0;

		/**
		 * SNR when the packet is received
		 */
		double snr = 0.0;

		bool isDME() const;

	protected:
		/**
		 * Ensures that at least one header is present, which must be a base header.
		 * @throws std::logic_error If no headers are present.
		 * @throws std::runtime_error If first header is not a base header.
		 */
		void validateHeader() const;

	protected:
		/** Several headers can be concatenated to fill one packet. */
		std::vector<L2Header*> headers;

		/** Several payloads can be concatenated (with resp. headers) to fill one packet. */
		std::vector<Payload*> payloads;

		/** Holds all registered callbacks. */
		std::vector<L2PacketSentCallback*> callbacks;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_L2PACKET_HPP
