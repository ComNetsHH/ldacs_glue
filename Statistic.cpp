// The L-Band Digital Aeronautical Communications System (LDACS) Link Layer Glue Library provides interfaces and common classes necessary for the LDACS Air-Air Medium Access Control simulator.
// Copyright (C) 2023  Sebastian Lindner, Konrad Fuger, Musab Ahmed Eltayeb Ahmed, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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