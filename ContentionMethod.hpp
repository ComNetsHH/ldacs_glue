//
// Created by seba on 9/7/21.
//

#ifndef TUHH_INTAIRNET_MC_SOTDMA_CONTENTIONMETHOD_HPP
#define TUHH_INTAIRNET_MC_SOTDMA_CONTENTIONMETHOD_HPP

enum ContentionMethod {
	/** estimate no. of channel accesses from average neighbor broadcast rate */
	binomial_estimate,
	/** each neighbor's channel access is an independent Bernoulli trial with its own probability */
	poisson_binomial_estimate,
	/** assume that all users that have been active recently will again be active */
	all_active_again_assumption
};

#endif //TUHH_INTAIRNET_MC_SOTDMA_CONTENTIONMETHOD_HPP
