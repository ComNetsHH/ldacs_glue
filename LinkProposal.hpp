#ifndef TUHH_INTAIRNET_MC_SOTDMA_LINKPROPOSAL_HPP
#define TUHH_INTAIRNET_MC_SOTDMA_LINKPROPOSAL_HPP

namespace TUHH_INTAIRNET_MCSOTDMA {
	class LinkProposal {
	public:
		LinkProposal() {}
		LinkProposal(const LinkProposal &other) : slot_offset(other.slot_offset), slot_duration(other.slot_duration), noise(other.noise), period(other.period), center_frequency(other.center_frequency) {}		

		enum SlotDuration {
			six_ms,
			twelve_ms,
			twentyfour_ms
		};

		int slot_offset = 0;
		SlotDuration slot_duration = twentyfour_ms;
		int noise = 0;
		int period = 0;
		int center_frequency = 0;
	};
}

#endif