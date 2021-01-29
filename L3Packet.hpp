//
// Created by Konrad Fuger on 07.12.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_L3PACKET_HPP
#define INTAIRNET_LINKLAYER_GLUE_L3PACKET_HPP

#include "MacId.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;
namespace inet {
	class Packet;
}

class L3Packet {
public:
	/** default constructor **/
	L3Packet();

	/** Size of the contained inet packet **/
	int size = 0;

	/** Offset shows how much data was already fragmented and put into a L2Packet **/
	int offset = 0;

	/** The MacId resolved from the NW layer**/
	MacId dest;

	/** Pointer to the original data **/
	inet::Packet* original;
};

#endif //INTAIRNET_LINKLAYER_GLUE_L3PACKET_HPP
