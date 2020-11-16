//
// Created by Sebastian Lindner on 16.11.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_ICAOID_HPP
#define INTAIRNET_LINKLAYER_GLUE_ICAOID_HPP

#include <stdexcept>

namespace TUHH_INTAIRNET_MCSOTDMA {
	class MacId {
		public:
			explicit MacId(int id) : id(id) {}
			virtual ~MacId() = default;
			
			MacId(const MacId& other)	= default;
			
			bool operator!=(const MacId& other) const {
				return !(*this == other);
			}
			
			unsigned int getBits() const {
				return 27; // ICAO ID is 27 bits.
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
	
	/** Symbolic link ID that represents an unset ICAO ID. */
	const MacId SYMBOLIC_ID_UNSET = MacId(-1);
	/** Symbolic link ID that represents a broadcast. */
	const MacId SYMBOLIC_LINK_ID_BROADCAST = MacId(-2);
	/** Symbolic link ID that represents a beacon (which is also a broadcast). */
	const MacId SYMBOLIC_LINK_ID_BEACON = MacId(-3);
}

#endif //INTAIRNET_LINKLAYER_GLUE_ICAOID_HPP
