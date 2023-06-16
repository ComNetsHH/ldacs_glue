// The L-Band Digital Aeronautical Communications System (LDACS) Link Layer Glue Library provides interfaces and common classes necessary for the LDACS Air-Air Medium Access Control simulator.
// Copyright (C) 2023  Sebastian Lindner, Konrad Fuger, Musab Ahmed Eltayeb Ahmed, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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