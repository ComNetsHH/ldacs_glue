//
// Created by Konrad Fuger on 10.12.20.
//

#include "PassThroughArq.hpp"
#include "IMac.hpp"
#include "IRlc.hpp"
#include <string>

using namespace TUHH_INTAIRNET_MCSOTDMA;

void PassThroughArq::notifyOutgoing(unsigned int num_bits, const MacId& mac_id) {
    debug("PassThroughArq::notifyOutgoing " + std::to_string(num_bits));
    IMac* mac = getLowerLayer();
    mac->notifyOutgoing(num_bits, mac_id);
}
L2Packet* PassThroughArq::requestSegment(unsigned int num_bits, const MacId& mac_id) {
    debug("PassThroughArq::requestSegment " + std::to_string(num_bits));
    IRlc * rlc = getUpperLayer();
    return rlc->requestSegment(num_bits, mac_id);
}

bool PassThroughArq::shouldLinkBeArqProtected(const MacId& mac_id) const {
    return false;
}
void PassThroughArq::injectIntoUpper(L2Packet* packet) {
    debug("PassThroughArq::injectIntoUpper");
    IRlc * rlc = getUpperLayer();
    return rlc->receiveInjectionFromLower(packet);
}
void PassThroughArq::receiveFromLower(L2Packet* packet) {
    debug("PassThroughArq::receiveFromLower");
    IRlc * rlc = getUpperLayer();
    return rlc->receiveFromLower(packet);
}
void PassThroughArq::notifyAboutNewLink(const MacId& id) {
    return;
}
void PassThroughArq::notifyAboutRemovedLink(const MacId& id) {
    return;
}

void PassThroughArq::processIncomingHeader(L2Packet* incoming_packet) {
    return;
}

void PassThroughArq::onEvent(double time) {

}
