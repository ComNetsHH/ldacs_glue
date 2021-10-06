//
// Created by seba on 8/5/21.
//

#include "Statistic.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

Statistic::Statistic(const std::string& name, IOmnetPluggable *creator) : name(name), value(0.0), was_updated(false), creator(creator) {}

void Statistic::capture(double new_value) {	
	this->value = new_value;
	was_updated = true;	
}

void Statistic::update() {
	// Only emit to the simulator if the value has changed.
	if (was_updated || !has_emitted_once) {		
		creator->emit(name, value);
		was_updated = false;
		has_emitted_once = true;
	}
}

void Statistic::increment() {
	capture(this->value + 1);
}

void Statistic::incrementBy(double incr_val) {
	capture(this->value + incr_val);
}

double Statistic::get() const {
	return this->value;
}

void Statistic::dontEmitBeforeFirstReport() {
	this->has_emitted_once = true;
}

bool Statistic::wasUpdated() const {
	return was_updated;
}