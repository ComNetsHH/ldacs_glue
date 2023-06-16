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

#ifndef TUHH_INTAIRNET_MC_SOTDMA_TIMESTAMP_HPP
#define TUHH_INTAIRNET_MC_SOTDMA_TIMESTAMP_HPP

#include <cstdint>

namespace TUHH_INTAIRNET_MCSOTDMA {

	/**
	 * Implements a notion of time just as OMNeT++ does. Could be derived from for a different format.
	 */
	class Timestamp {
	public:
		int64_t t;

		Timestamp() : t(0) {}

		explicit Timestamp(int64_t t) : t(t) {}

		bool operator==(const Timestamp& x) const { return t == x.t; }

		bool operator!=(const Timestamp& x) const { return t != x.t; }

		bool operator<(const Timestamp& x) const { return t < x.t; }

		bool operator>(const Timestamp& x) const { return t > x.t; }

		bool operator<=(const Timestamp& x) const { return t <= x.t; }

		bool operator>=(const Timestamp& x) const { return t >= x.t; }

		Timestamp& operator=(const Timestamp& x) {
			t = x.t;
			return *this;
		}

		Timestamp& operator=(const int64_t x) {
			t = x;
			return *this;
		}

		Timestamp operator+(const Timestamp& x) { return Timestamp(t + x.t); }

		Timestamp operator+(int64_t x) { return Timestamp(t + x); }

		Timestamp& operator+=(const Timestamp& x) {
			t += x.t;
			return *this;
		}

		Timestamp& operator+=(int64_t x) {
			t += x;
			return *this;
		}

		Timestamp operator-(const Timestamp& x) { return Timestamp(t - x.t); }

		Timestamp operator-(int64_t x) { return Timestamp(t - x); }

		Timestamp& operator-=(const Timestamp& x) {
			t -= x.t;
			return *this;
		}

		Timestamp& operator-=(int64_t x) {
			t -= x;
			return *this;
		}
	};
}

#endif //TUHH_INTAIRNET_MC_SOTDMA_TIMESTAMP_HPP
