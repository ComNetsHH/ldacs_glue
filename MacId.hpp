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

#ifndef INTAIRNET_LINKLAYER_GLUE_ICAOID_HPP
#define INTAIRNET_LINKLAYER_GLUE_ICAOID_HPP

#include <stdexcept>
#include <string>

namespace TUHH_INTAIRNET_MCSOTDMA {
	class MacId {
	public:
		MacId() : id(-1) {}

		explicit MacId(int id) : id(id) {}

		virtual ~MacId() = default;

		MacId(const MacId& other) = default;

		bool operator!=(const MacId& other) const {
			return !(*this == other);
		}

		static unsigned int getBits() {
			return 28; // ICAO ID is 28 bits.
		}

		const int& getId() const {
			return this->id;
		}

		bool operator==(const MacId& other) const {
			return this->id == other.id;
		}

		bool operator<(const MacId& other) const {
			return this->id < other.id;
		}

		bool operator<=(const MacId& other) const {
			return this->id <= other.id;
		}

		bool operator>(const MacId& other) const {
			return this->id > other.id;
		}

		bool operator>=(const MacId& other) const {
			return this->id >= other.id;
		}

	protected:
		int id;
	};

	/** Symbolic unset ID. */
	const MacId SYMBOLIC_ID_UNSET = MacId(-1);
	/** Symbolic broadcast ID. */
	const MacId SYMBOLIC_LINK_ID_BROADCAST = MacId(-2);
	/** Symbolic beacon ID (which is also a broadcast). */
	const MacId SYMBOLIC_LINK_ID_BEACON = MacId(-3);
	/** Symbolic DME ID. */
	const MacId SYMBOLIC_LINK_ID_DME = MacId(-4);

	inline std::ostream& operator<<(std::ostream& stream, const MacId& id) {
		std::string str;
		switch (id.getId()) {
			case -1: {str = "UNSET"; break;}
			case -2: {str = "SH"; break;}
			case -3: {str = "BEACON"; break;}
			case -4: {str = "DME"; break;}
			default: {str = std::to_string(id.getId()); break;}
		}
		return stream << str;
	}
}

#endif //INTAIRNET_LINKLAYER_GLUE_ICAOID_HPP
