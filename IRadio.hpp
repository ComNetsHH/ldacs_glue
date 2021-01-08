//
// Created by Konrad Fuger on 14.12.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IRADIO_HPP
#define INTAIRNET_LINKLAYER_GLUE_IRADIO_HPP

#include "L2Packet.hpp"

namespace TUHH_INTAIRNET_MCSOTDMA {
    class IPhy;
    class IRadio {
    protected:
        IPhy *phy = nullptr;
    public:
        virtual void sendToChannel(L2Packet *packet, uint64_t center_frequency) = 0;
        virtual void receiveFromChannel(L2Packet *packet, uint64_t center_frequency) = 0;
        void setPhy(IPhy * phy);
        IPhy* getPhy();
    };
}


#endif //INTAIRNET_LINKLAYER_GLUE_IRADIO_HPP
