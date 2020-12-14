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
    void notifyOutgoing(unsigned int num_bits, const MacId& mac_id) override;
    L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id) override;
    bool shouldLinkBeArqProtected(const MacId& mac_id) const override;
    void injectIntoUpper(L2Packet* packet) override;
    void receiveFromLower(L2Packet* packet) override;
    void notifyAboutNewLink(const MacId& id) override;
    void notifyAboutRemovedLink(const MacId& id) override;
    void onEvent(double time) override;

protected:
    void processIncomingHeader(L2Packet* incoming_packet) override;
};

}


#endif //INTAIRNET_LINKLAYER_GLUE_PASSTHROUGHARQ_HPP
