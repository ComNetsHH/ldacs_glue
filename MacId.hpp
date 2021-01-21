//
// Created by Sebastian Lindner on 16.11.20.
//

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
	
	/** Symbolic unset ID. */
	const MacId SYMBOLIC_ID_UNSET = MacId(-1);
	/** Symbolic broadcast ID. */
	const MacId SYMBOLIC_LINK_ID_BROADCAST = MacId(-2);
	/** Symbolic beacon ID (which is also a broadcast). */
	const MacId SYMBOLIC_LINK_ID_BEACON = MacId(-3);
	
	inline std::ostream & operator<<(std::ostream & stream, const MacId& id) {
		std::string str;
		switch (id.getId()) {
			case -1:
				str = "UNSET";
				break;
			case -2:
				str = "BROADCAST";
				break;
			case -3:
				str = "BEACON";
				break;
			default:
				str = std::to_string(id.getId());
				break;
		}
		return stream << str;
	}
}

#endif //INTAIRNET_LINKLAYER_GLUE_ICAOID_HPP
