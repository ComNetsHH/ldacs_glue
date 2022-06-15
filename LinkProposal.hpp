#ifndef TUHH_INTAIRNET_MC_SOTDMA_LINKPROPOSAL_HPP
#define TUHH_INTAIRNET_MC_SOTDMA_LINKPROPOSAL_HPP

namespace TUHH_INTAIRNET_MCSOTDMA {
	class LinkProposal {
	public:
		enum SlotDuration {
			six_ms,
			twelve_ms,
			twentyfour_ms
		};

		int slot_offset = 0;
		SlotDuration slot_duration;
		int noise = 0;
		int period = 0;
		int center_frequency = 0;
	};
}

#endif