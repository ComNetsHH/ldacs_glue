//
// Created by Konrad Fuger on 10.12.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_DELAYMAC_HPP
#define INTAIRNET_LINKLAYER_GLUE_DELAYMAC_HPP

#include "IMac.hpp"
#include "IOmnetPluggable.hpp"
namespace TUHH_INTAIRNET_MCSOTDMA {

class DelayMac: public IMac, public IOmnetPluggable {
public:
    void notifyOutgoing(unsigned long num_bits, const MacId& mac_id);
    void passToLower(L2Packet* packet, unsigned int center_frequency);
    void passToUpper(L2Packet* packet);
    void injectIntoUpper(L2Packet* packet);
};

}


#endif //INTAIRNET_LINKLAYER_GLUE_DELAYMAC_HPP
