//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_CPRPOSITION_HPP
#define INTAIRNET_LINKLAYER_GLUE_CPRPOSITION_HPP

namespace TUHH_INTAIRNET_MCSOTDMA {
	
	/** The Compact Position Report-encoded position of latitude, longitude, altitude as ADS-B uses it. */
	class CPRPosition {
		public:
			CPRPosition(double latitude, double longitude, double altitude) : latitude(latitude), longitude(longitude), altitude(altitude) {
				// No actual computation is performed.
			}
			
			CPRPosition(const CPRPosition& other) = default;
			
			double latitude, longitude, altitude;
			
			/** Number of bits required to encode this position. */
			unsigned int getBits() const {
				return 12 /* latitude */ + 14 /* longitutde */ + 12 /* altitude */;
			}
	};
}

#endif //INTAIRNET_LINKLAYER_GLUE_CPRPOSITION_HPP
