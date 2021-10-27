//
// Created by Konrad Fuger on 26.10.21.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_SIMULATORPOSITION_HPP
#define INTAIRNET_LINKLAYER_GLUE_SIMULATORPOSITION_HPP

namespace TUHH_INTAIRNET_MCSOTDMA {
    class SimulatorPosition {
    public:
        double x = 0.0, y = 0.0, z= 0.0;
        SimulatorPosition(double x, double y, double z): x(x), y(y), z(z){};
        SimulatorPosition(){};
    };
}

#endif //INTAIRNET_LINKLAYER_GLUE_SIMULATORPOSITION_HPP
