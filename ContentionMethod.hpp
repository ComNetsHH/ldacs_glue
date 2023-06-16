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
	/** just pick a random slot out of the next seven slots */
	naive_random_access,	
};

#endif //TUHH_INTAIRNET_MC_SOTDMA_CONTENTIONMETHOD_HPP
