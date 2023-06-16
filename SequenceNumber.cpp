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

#include <assert.h>
#include "SequenceNumber.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

SequenceNumber::SequenceNumber(uint8_t sequence_number) : sequence_number(sequence_number) {
	assert(SEQNO_MAX >= sequence_number);
}

SequenceNumber::SequenceNumber() : sequence_number(SEQNO_UNSET) {}

SequenceNumber::SequenceNumber(const SequenceNumber& other) : SequenceNumber(
		other.sequence_number) {}

uint8_t SequenceNumber::get() const {
	return this->sequence_number;
}

void SequenceNumber::increment() {
	this->sequence_number = (uint8_t) ((this->sequence_number + 1) % SEQNO_MAX);
	// Skip over UNSET.
	if (this->get() == SEQNO_UNSET)
		this->increment();
}

void SequenceNumber::decrement() {
	this->sequence_number = (uint8_t) ((this->sequence_number -1) % SEQNO_MAX);
	// Skip over UNSET.
	if (this->get() == SEQNO_UNSET)
		this->decrement();
}

bool SequenceNumber::operator==(const SequenceNumber& other) const {
	return other.get() == this->get();
}

SequenceNumber SequenceNumber::operator+(uint8_t increment) {
	assert(increment > 0);
	SequenceNumber ret = SequenceNumber(this->get());
	for (uint8_t i = 0; i< increment; i++) {
		ret.increment();
	}

	return ret;
}

SequenceNumber SequenceNumber::operator-(uint8_t decrement) {
	assert(decrement > 0);
	SequenceNumber ret = SequenceNumber(this->get());
	for (uint8_t i = 0; i< decrement; i++) {
		ret.decrement();
	}

	return ret;
}

bool SequenceNumber::operator>(const SequenceNumber& other) const {
	throw "Deprecated, Sequencenumbers dont have a strict notion of bigger,smaller";
}

bool SequenceNumber::operator<(const SequenceNumber& other) const {
	throw "Deprecated, Sequencenumbers dont have a strict notion of bigger,smaller";
}

bool SequenceNumber::isHigherThan(SequenceNumber other, uint8_t windowSize) {
	uint8_t raw = this->get();
	uint8_t otherRaw = other.get();

	// A sequence number A is higher than an other one  B if
	// I) The raw value of A is higher than the raw value of B but the distance is smaller than the window Size
	if (raw > otherRaw) {
		uint8_t dist = raw - otherRaw;
		if (dist < windowSize) {
			return true;
		}
	}

	// 2) The raw value of A is lower than the raw value of B there was a wrap around
	if (otherRaw > raw) {
		uint8_t dist = (SEQNO_MAX - otherRaw) + (raw - SEQNO_FIRST);

		if (dist < windowSize) {
			return true;
		}
	}
	return false;
}

bool SequenceNumber::isLowerThan(SequenceNumber other, uint8_t windowSize) {
	return other.isHigherThan(*this, windowSize);
}

bool SequenceNumber::isLowerThanEqual(SequenceNumber other, uint8_t windowSize) {
	return isLowerThan(other, windowSize) || *this == other;
}

SequenceNumber SequenceNumber::next() {
    auto result = SequenceNumber(this->get());
    result.increment();
    return result;
}

SequenceNumber SequenceNumber::prev() {
    auto result = SequenceNumber(this->get());
    result = result -1;
    return result;
}
