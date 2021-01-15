//
// Created by Konrad Fuger on 10.12.20.
//

#include "DelayMac.hpp"
#include "MacId.hpp"
#include "IArq.hpp"
#include "IPhy.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

DelayMac::DelayMac(const MacId& id): IMac(id), nextMacId(-1)  {

}

void DelayMac::notifyOutgoing(unsigned long num_bits, const MacId& mac_id) {
    double time = getTime();
    double delay = 2;
    nextPktSize = num_bits;
    nextMacId = mac_id;
    scheduleAt(time + delay);
}

void DelayMac::onEvent(double time) {
    IArq *arq = getUpperLayer();
    auto pkt = arq->requestSegment(nextPktSize, nextMacId);
    passToLower(pkt, 0);
}

void DelayMac::passToLower(L2Packet* packet, unsigned int center_frequency) {
    debug("PASSING DOWN");
    IPhy *phy = getLowerLayer();
    phy->receiveFromUpper(packet, center_frequency);
}

void DelayMac::passToUpper(L2Packet* packet) {
    IArq *arq = getUpperLayer();
    arq->receiveFromLower(packet);
}
void DelayMac::injectIntoUpper(L2Packet* packet) {

}

void DelayMac::receiveFromLower(L2Packet* packet) {
	IArq *arq = getUpperLayer();
	arq->receiveFromLower(packet);
}
