//
// Created by Sebastian Lindner on 18.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IPHY_HPP
#define INTAIRNET_LINKLAYER_GLUE_IPHY_HPP

#include <cassert>
#include "L2Packet.hpp"
#include "IRadio.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	
	class IMac; // Forward-declaration so that we can keep a pointer to the MAC sublayer.
	
	/**
	 * Specifies the interface the PHY layer must implement.
	 */
	class IPhy {
		public:
			
			virtual ~IPhy() = default;
			
			/**
			 * Notify the PHY layer about an outbound packet.
			 * @param num_bits
			 * @param data
			 * @param center_frequency
			 */
			virtual void receiveFromUpper(L2Packet* data, unsigned int center_frequency) = 0;
			
			/**
			 * @return From the current modulation and coding, return the current datarate in bits per slot.
			 */
			virtual unsigned long getCurrentDatarate() const = 0;
			
			/**
			 * Connects the MAC sublayer above.
			 * @param mac
			 */
			void setUpperLayer(IMac* mac);

            /**
            * Connects to a radio instance.
            * @param radio
            */
            void setRadio(IRadio* radio);
			
			/**
			 * @return A pointer to the MAC sublayer above.
			 */
			IMac* getUpperLayer();

			/**
			 * @param slot_offset
			 * @param num_slots
			 * @return Whether the single transmitter is idle during the specified time range.
			 */
			virtual bool isTransmitterIdle(unsigned int slot_offset, unsigned int num_slots) const = 0;

			/**
			 * @param slot_offset
			 * @param num_slots
			 * @return Whether any receiver is idle during the specified time range.
			 */
            virtual bool isAnyReceiverIdle(unsigned int slot_offset, unsigned int num_slots) const = 0;
			
			/**
			 * When this PHY receives a packet, it is transformed into a L2Packet* and passed into this function.
			 * @param packet
			 * @param center_frequency
			 */
			virtual void onReception(L2Packet* packet, uint64_t center_frequency);

			virtual void update(uint64_t num_slots);

            /**
             * Tune a receiver to a particular frequency, allowing reception of packets during the current time slot.
             * @param center_frequency
             * @throws std::runtime_error If the number of available receivers is exceeded.
             */
            virtual void tuneReceiver(uint64_t center_frequency);
		
		protected:
			IMac* upper_layer = nullptr;
			IRadio *radio = nullptr;
			/** Frequencies that receivers are tuned to in this time slot. */
            std::vector<uint64_t> rx_frequencies;
            /** One broadcast and one point-to-point receiver. */
            const size_t num_receivers = 2;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_IPHY_HPP
