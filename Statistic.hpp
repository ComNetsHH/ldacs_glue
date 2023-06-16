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

#ifndef TUHH_INTAIRNET_MC_SOTDMA_STATISTIC_HPP
#define TUHH_INTAIRNET_MC_SOTDMA_STATISTIC_HPP

#include <string>
#include "IOmnetPluggable.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {

	/** Handles statistic emission to the simulator. */
	class Statistic {
	public:
		Statistic(const std::string& name, IOmnetPluggable *creator);

		/** Capture a new value. */
		void capture(double new_value);
		/** Increment the current value. */
		void increment();
		void incrementBy(double incr_val);

		/** Called after every slot. Emits the value to the simulator if it has been changed. */
		void update();

		double get() const;

		/** Stop the emission of this statistic until the first report. If this is not called, then a first value will be emitted even before the first report has been made, s.t. the simulation result always contains values. */
		void dontEmitBeforeFirstReport();

		bool wasUpdated() const;

	protected:
		const std::string name;
		double value;
		bool was_updated;
		bool has_emitted_once = false;
		IOmnetPluggable *creator = nullptr;
	};
}

#endif //TUHH_INTAIRNET_MC_SOTDMA_STATISTIC_HPP
