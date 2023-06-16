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

#include <iostream>
#include "RngProvider.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

IntegerUniformRng::IntegerUniformRng() : random_device(), generator(random_device()) {}

int IntegerUniformRng::get(int min, int max) {
	dist = std::uniform_int_distribution<>(min, max - 1);
	return dist(generator);
}

IRng::IRng() {
	RngProvider::getInstance().signupInt(this);
}

int IRng::getRandomInt(int min, int max) {
	return RngProvider::getInstance().getInt(this, min, max);
}

int OmnetIntegerUniformRng::get(int min, int max) {
	return RngProvider::getInstance().omnetGetInt(min, max, this->k);
}
