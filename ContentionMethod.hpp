//
// Created by seba on 9/7/21.
//

#ifndef TUHH_INTAIRNET_MC_SOTDMA_CONTENTIONMETHOD_HPP
#define TUHH_INTAIRNET_MC_SOTDMA_CONTENTIONMETHOD_HPP

/** Different modes for collision avoidance. These make use of contention estimation at the user in different ways to configure its random-access-based broadcast channel access. */
enum ContentionMethod {
	/** estimate no. of channel accesses from average neighbor broadcast rate */
	binomial_estimate,
	/** each neighbor's channel access is an independent Bernoulli trial with its own probability */
	poisson_binomial_estimate,
	/** calculate the number of candidate slots required to meet a certain collision probability */
	randomized_slotted_aloha,
	/** don't utilize contention estimation in any way, just pick a random slot out of the next 100 slots */
	naive_random_access,	
};

#endif //TUHH_INTAIRNET_MC_SOTDMA_CONTENTIONMETHOD_HPP
