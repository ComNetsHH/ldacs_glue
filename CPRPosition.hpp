//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_CPRPOSITION_HPP
#define INTAIRNET_LINKLAYER_GLUE_CPRPOSITION_HPP

namespace TUHH_INTAIRNET_MCSOTDMA {
	
	/** The Compact Position Report-encoded position of latitude, longitude, altitude as ADS-B uses it. */
	class CPRPosition {
		public:
			CPRPosition(double latitude, double longitude, double altitude, bool odd) : latitude(latitude), longitude(longitude), altitude(altitude), odd(odd) {
				// No actual computation is performed.
			}
			CPRPosition() : CPRPosition(0, 0, 0, false) {}
			
			CPRPosition(const CPRPosition& other) = default;
			
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
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_CPRPOSITION_HPP
