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

#ifndef INTAIRNET_LINKLAYER_GLUE_SEQUENCENUMBER_HPP
#define INTAIRNET_LINKLAYER_GLUE_SEQUENCENUMBER_HPP

#include <cstdint>

namespace TUHH_INTAIRNET_MCSOTDMA {
	class SequenceNumber {
	public:
		SequenceNumber();

		SequenceNumber(uint8_t sequence_number);

		SequenceNumber(const SequenceNumber& other);

		bool operator==(const SequenceNumber& other) const;

		bool operator>(const SequenceNumber& other) const;

		bool operator<(const SequenceNumber& other) const;

		SequenceNumber operator+(uint8_t increment);

		SequenceNumber operator-(uint8_t increment);

		bool isHigherThan(SequenceNumber other, uint8_t window_size);

		bool isLowerThan(SequenceNumber other, uint8_t window_size);

		bool isLowerThanEqual(SequenceNumber other, uint8_t window_size);

		uint8_t get() const;

		void increment();

		void decrement();

        SequenceNumber next();

        SequenceNumber prev();

	protected:
		uint8_t sequence_number;
	};

	static const uint8_t SEQNO_MAX = 255;
	static const uint8_t SEQNO_UNSET = 0;
	static const uint8_t SEQNO_FIRST = 1;
	static const uint8_t NUM_BYTES = 1;
}

#endif //INTAIRNET_LINKLAYER_GLUE_SEQUENCENUMBER_HPP
