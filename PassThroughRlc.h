//
// Created by Konrad Fuger on 06.12.20.
//

#include "IRlc.hpp"
#include "IOmnetPluggable.h"

#ifndef INTAIRNET_LINKLAYER_GLUE_PASSTHROUGHRLC_H
#define INTAIRNET_LINKLAYER_GLUE_PASSTHROUGHRLC_H

namespace TUHH_INTAIRNET_MCSOTDMA {

class PassThroughRlc: public IRlc, public IOmnetPluggable {
public:
    void receiveFromUpper(L3Packet* data, MacId dest, PacketPriority priority = PRIORITY_DEFAULT) override;
    void receiveFromLower(L2Packet* packet) override;
    void receiveInjectionFromLower(L2Packet* packet, PacketPriority priority = PRIORITY_LINK_MANAGEMENT) override;
    virtual L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id) override;
    void init();

private:
    void onEvent(double time) override;

};

}

#endif //INTAIRNET_LINKLAYER_GLUE_PASSTHROUGHRLC_H
