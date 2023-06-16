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

#ifndef INTAIRNET_LINKLAYER_GLUE_CPRPOSITION_HPP
#define INTAIRNET_LINKLAYER_GLUE_CPRPOSITION_HPP

#include "SimulatorPosition.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {

	/** The Compact Position Report-encoded position of latitude, longitude, altitude as ADS-B uses it. */
	class CPRPosition {
	public:
		enum PositionQuality {
			low,
			med,
			hi
		};

		CPRPosition(double latitude, double longitude, double altitude, bool odd) : latitude(latitude), longitude(longitude), altitude(altitude), odd(odd) {
			// No actual computation is performed.
		}

		CPRPosition() : CPRPosition(0, 0, 0, false) {}

		CPRPosition(const CPRPosition& other) = default;

		CPRPosition(SimulatorPosition simulatorPosition): encodedPosition(simulatorPosition) {};

		/** Number of bits required to encode this position. */
		unsigned int getBits() const {
			return 12 /* latitude */ + 14 /* longitutde */ + 12 /* altitude */;
		}

		bool operator==(const CPRPosition& other) const {
			return latitude == other.latitude
			       && longitude == other.longitude
			       && altitude == other.altitude;
		}

		bool operator!=(const CPRPosition& other) const {
			return !((*this) == other);
		}

		double latitude, longitude, altitude;
		bool odd;

		// holds the xyz position from the simulator that is encoded in this
		SimulatorPosition encodedPosition;
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_CPRPOSITION_HPP
