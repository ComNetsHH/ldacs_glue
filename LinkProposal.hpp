#ifndef TUHH_INTAIRNET_MC_SOTDMA_LINKPROPOSAL_HPP
#define TUHH_INTAIRNET_MC_SOTDMA_LINKPROPOSAL_HPP

#include "SlotDuration.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
	class LinkProposal {
	public:
		LinkProposal() {}
		LinkProposal(const LinkProposal &other) : slot_offset(other.slot_offset), slot_duration(other.slot_duration), period(other.period), center_frequency(other.center_frequency), num_tx_initiator(other.num_tx_initiator), num_tx_recipient(other.num_tx_recipient) {}				
		bool operator==(LinkProposal const& rhs) const {return slot_offset == rhs.slot_offset && slot_duration == rhs.slot_duration && period == rhs.period && center_frequency == rhs.center_frequency && num_tx_initiator == rhs.num_tx_initiator && num_tx_recipient == rhs.num_tx_recipient;}
		bool operator!=(LinkProposal const& rhs) const { return !(*this == rhs);}

		int slot_offset = 0;
		SlotDuration slot_duration = twentyfour_ms;		
		int period = 0;
		int center_frequency = 0;
		int num_tx_initiator = 1;
		int num_tx_recipient = 1;
	};
}

#endif