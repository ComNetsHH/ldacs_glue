//
// Created by seba on 8/5/21.
//

#include "Statistic.hpp"

TUHH_INTAIRNET_MCSOTDMA::Statistic::Statistic(const std::string& name, IOmnetPluggable *creator) : name(name), value(0.0), was_updated(false), creator(creator) {}

void TUHH_INTAIRNET_MCSOTDMA::Statistic::capture(double new_value) {
	if (this->value != new_value) {
		this->value = new_value;
		was_updated = true;
	}
}

void TUHH_INTAIRNET_MCSOTDMA::Statistic::update() {
	// Only emit to the simulator if the value has changed.
	if (was_updated || !has_emitted_once) {
		creator->emit(name, value);
		was_updated = false;
		has_emitted_once = true;
	}
}

void TUHH_INTAIRNET_MCSOTDMA::Statistic::increment() {
	capture(this->value + 1);
}

void TUHH_INTAIRNET_MCSOTDMA::Statistic::incrementBy(double incr_val) {
	capture(this->value + incr_val);
}

double TUHH_INTAIRNET_MCSOTDMA::Statistic::get() const {
	return this->value;
}
