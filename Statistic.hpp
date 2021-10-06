//
// Created by seba on 8/5/21.
//

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
