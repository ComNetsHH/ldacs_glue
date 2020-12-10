//
// Created by Konrad Fuger on 10.12.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_PASSTHROUGHARQ_HPP
#define INTAIRNET_LINKLAYER_GLUE_PASSTHROUGHARQ_HPP

#include "IArq.hpp"
#include "IOmnetPluggable.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {

class PassThroughArq : public IArq, public IOmnetPluggable {
public:
    void notifyOutgoing(unsigned int num_bits, const MacId& mac_id);
    L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id);
    bool shouldLinkBeArqProtected(const MacId& mac_id);
    void injectIntoUpper(L2Packet* packet);
    void receiveFromLower(L2Packet* packet);
    void notifyAboutNewLink(const MacId& id);
    void notifyAboutRemovedLink(const MacId& id);

protected:
    void processIncomingHeader(L2Packet* incoming_packet);
};

}


#endif //INTAIRNET_LINKLAYER_GLUE_PASSTHROUGHARQ_HPP
