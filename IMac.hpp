//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IMAC_HPP
#define INTAIRNET_LINKLAYER_GLUE_IMAC_HPP

#include "MacId.hpp"
#include "L2Packet.hpp"
#include "L2Header.hpp"
#include "Timestamp.hpp"
#include "ContentionMethod.hpp"
#include <map>
#include <functional>

namespace TUHH_INTAIRNET_MCSOTDMA {

	class IArq; // Forward-declaration so that we can keep a pointer to the ARQ sublayer.
	class IPhy; // Forward-declaration so that we can keep a pointer to the PHY layer.

	/**
	 * Specifies the interface the MAC sublayer must implement.
	 */
	class IMac {
	public:
		explicit IMac(const MacId& id);

		virtual ~IMac() = default;

		/**
		 * Notify the MAC sublayer about an outbound packet.
		 * @param num_bits The number of bits present in the queue corresponding to 'id'.
		 * @param mac_id The MAC ID.
		 */
		virtual void notifyOutgoing(unsigned long num_bits, const MacId& mac_id) = 0;

		/**
		 * Requests a segment from the upper layer.
		 * @param num_bits
		 * @param mac_id
		 * @return
		 */
		virtual L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id);

		/**
		 * @param mac_id
		 * @return Whether there's more data to be sent for the given ID.
		 */
		virtual bool isThereMoreData(const MacId& mac_id) const;

		/**
		 * When a transmission slot arrives, this passes down a packet to the PHY layer.
		 * @param packet
		 * @param center_frequency
		 */
		virtual void passToLower(L2Packet* packet, unsigned int center_frequency) = 0;

		/**
		 * Define what happens when the PHY passes a just-received data packet to the MAC.
		 * @param packet
		 */
		virtual void receiveFromLower(L2Packet* packet, uint64_t center_frequency) = 0;

		/**
		 * When a packet comes in, this passes it up to the next upper layer.
		 * @param packet
		 */
		virtual void passToUpper(L2Packet* packet) = 0;

		/**
		 * Link requests must be injected into the RLC sublayer above.
		 * With this function, injection *up* the protocol stack is possible.
		 * @param packet
		 */
		virtual void injectIntoUpper(L2Packet* packet);

		/**
		 * Connects the ARQ sublayer above.
		 * @param arq
		 */
		void setUpperLayer(IArq* arq) {
			this->upper_layer = arq;
		}

		/**
		 * @return Pointer to the ARQ sublayer above.
		 */
		IArq* getUpperLayer() {
			return this->upper_layer;
		}

		/**
		 * Connects the PHY layer below.
		 * @param phy
		 */
		void setLowerLayer(IPhy* phy) {
			this->lower_layer = phy;
		}

		/**
		 * @return A pointer to the PHY layer below.
		 */
		IPhy* getLowerLayer() {
			return this->lower_layer;
		}

		/**
		 * Queries the ARQ sublayer above.
		 * @param mac_id
		 * @return Whether the specified link should be ARQ protected.
		 */
		bool shouldLinkBeArqProtected(const MacId& mac_id) const;

		/**
		 * Queres the PHY layer below.
		 * @return The current data rate in bits per slot.
		 */
		unsigned long getCurrentDatarate() const;

		/**
		 * @return The number of hops to the nearest ground station according to current routing information.
		 */
		unsigned int getNumHopsToGS() const;

		/**
		 * When a neighbor's onSlotEnd comes in, this reports it to the upper layers.
		 * @param id
		 * @param num_hops
		 */
		void reportNumHopsToGS(const MacId& id, unsigned int num_hops) const;

		/**
		 * @param id
		 * @return The current belief ot the respective user's geographic position.
		 */
		const CPRPosition& getPosition(const MacId& id) const;

		CPRPosition::PositionQuality getPositionQuality(const MacId& id) const;

		/**
		 * Update the belief of the respective user's geographic position.
		 * @param id
		 * @param position
		 */
		void updatePosition(const MacId& id, const CPRPosition& position, CPRPosition::PositionQuality position_quality);

		const MacId& getMacId() const {
			return this->id;
		}

		bool isTransmitterIdle(unsigned int slot_offset, unsigned int num_slots) const;

		bool isAnyReceiverIdle(unsigned int slot_offset, unsigned int num_slots) const;

		/** Increment time. Updates the linked PHY. */
		virtual void update(uint64_t num_slots);

		uint64_t getCurrentSlot() const;

		/**
		 * The MAC calls this function to notify the ARQ sublayer of a newly negotiated link.
		 * @param id
		 */
		void notifyAboutNewLink(const MacId& id);

		/**
		 * The MAC calls this function to notify the ARQ sublayer of a deleted link.
		 * @param id
		 */
		void notifyAboutRemovedLink(const MacId& id);

		/**
		 * Set target collision probability during broadcast slot selection.
		 * @param value
		 */
		virtual void setBroadcastTargetCollisionProb(double value);

		/**
		 * Set minimum number of candidate slots during broadcast slot selection.
		 * @param value
		 */
		virtual void setBcSlotSelectionMinNumCandidateSlots(int value);

		/**
		 * Set maximum number of candidate slots during broadcast slot selection.
		 * @param value
		 */
		virtual void setBcSlotSelectionMaxNumCandidateSlots(int value);

		/**
		 * @param method: Broadcast contention method.
		 */
		virtual void setContentionMethod(ContentionMethod method);

		/**
		 * If 'true': always schedule the next broadcast slot and advertise it in the header.
		 * If 'false: only schedule the next broadcast slot if there's more data queued up.
		 * @param value
		 */
		virtual void setAlwaysScheduleNextBroadcastSlot(bool value);
		virtual void setAdvertiseNextBroadcastSlotInCurrentHeader(bool flag);		
		virtual void setMinBeaconOffset(unsigned int value);
		virtual void setMaxBeaconOffset(unsigned int value);

		/** 
		 * When a beacon arrives at the MAC, it may be passed up to the Network Layer through this function.
		 */
		virtual void onBeaconReception(MacId origin_id, L2HeaderBeacon header);

		void setOmnetPassUpBeaconFct(std::function<void (MacId origin_id, L2HeaderBeacon header)> func) {
			this->passUpBeaconFct = func;
		}

		/**
		 * @param flag: If true, P2P links will always be bidirectional, i.e. have TX reservations for both sides.
		 * */
		virtual void setForceBidirectionalLinks(bool flag);		
		virtual void setWriteResourceUtilizationIntoBeacon(bool flag);
		virtual void setEnableBeacons(bool flag);
		virtual void setPPLinkBurstOffset(unsigned int value);
		virtual void setPPLinkBurstOffsetAdaptive(bool value);

	protected:
		IArq* upper_layer = nullptr;
		IPhy* lower_layer = nullptr;
		MacId id;
		std::map<MacId, CPRPosition> position_map;
		std::map<MacId, CPRPosition::PositionQuality> position_quality_map;
		uint64_t current_slot = 0;
		std::function<void (MacId origin_id, L2HeaderBeacon header)> passUpBeaconFct = [] (MacId origin_id, L2HeaderBeacon header) {/* do nothing */};
		bool should_force_bidirectional_links = true;
		bool should_initialize_bidirectional_links = false;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_IMAC_HPP
