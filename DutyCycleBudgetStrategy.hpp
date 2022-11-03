#ifndef INTAIRNET_LINKLAYER_GLUE_DUTYCYCLEBUDGETSTRATEGIES_HPP
#define INTAIRNET_LINKLAYER_GLUE_DUTYCYCLEBUDGETSTRATEGIES_HPP

namespace TUHH_INTAIRNET_MCSOTDMA {	
	enum DutyCycleBudgetStrategy {
		/** Statically assigns a fraction of the maximum duty cycle to each link. */
		STATIC,
		/** Dynamically computes the available duty cycle based on the maximum duty cycle and the number of already established links. */
		DYNAMIC
	};
};	

#endif