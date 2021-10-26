//
// Created by Konrad Fuger on 06.12.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IOMNETPLUGGABLE_HPP
#define INTAIRNET_LINKLAYER_GLUE_IOMNETPLUGGABLE_HPP

#include <functional>
#include <string>
#include "L2Packet.hpp"
#include "L3Packet.hpp"

class IOmnetPluggable {
public:
	double getTime() {
		if (getTimeCallback) {
			return getTimeCallback();
		}
		return -1;
	}

	void scheduleAt(double time) {
		if (scheduleAtCallback) {
			scheduleAtCallback(time);
		}
	}

	void emit(const std::string& event_name, double value) {
		if (emitCallback) {
			emitCallback(event_name, value);
		}
	}

	void emit(const std::string& event_name, size_t value) {
		this->emit(event_name, (double) value);
	}

	void debug(const std::string& message) {
		if (debugCallback) {
			debugCallback(message);
		}
	}

	void deletePacket(TUHH_INTAIRNET_MCSOTDMA::L2Packet* packet) {
        if (deleteL2Callback) {
            deleteL2Callback(packet);
        }
	}

    void deletePacket(L3Packet* packet) {
        if (deleteL3Callback) {
            deleteL3Callback(packet);
        }
    }

    SimulatorPosition getHostPosition() {
        if(getPositionCallback) {
            return getPositionCallback();
        }
        return SimulatorPosition(0,0,0);
    }

	std::function<double()> getTimeCallback;
	std::function<void(double)> scheduleAtCallback;
	std::function<void(std::string, double)> emitCallback;
	std::function<void(std::string)> debugCallback;
    std::function<void(TUHH_INTAIRNET_MCSOTDMA::L2Packet *)> deleteL2Callback;
    std::function<void(L3Packet *)> deleteL3Callback;
    std::function<SimulatorPosition()> getPositionCallback;

	/**
	 * To hook into OMNeT++'s scheduling mechanism.
	 * Override this function to make use of it.
	 * @param time
	 */
	virtual void onEvent(double time) {
		// Do nothing by default.
	}

	void registerGetTimeCallback(std::function<double()> callback) {
		getTimeCallback = callback;
	}

	void registerScheduleAtCallback(std::function<void(double)> callback) {
		scheduleAtCallback = callback;
	}

	void registerEmitEventCallback(std::function<void(std::string, double)> callback) {
		emitCallback = callback;
	}

	void registerDebugMessageCallback(std::function<void(std::string)> callback) {
		debugCallback = callback;
	}

    void registerDeleteL2Callback(std::function<void(TUHH_INTAIRNET_MCSOTDMA::L2Packet*)> callback) {
        deleteL2Callback = callback;
    }

    void registerDeleteL3Callback(std::function<void(L3Packet*)> callback) {
        deleteL3Callback = callback;
    }

    void registerGetPositionCallback(std::function<SimulatorPosition()> callback) {
	    getPositionCallback = callback;
	}
};

#endif //INTAIRNET_LINKLAYER_GLUE_IOMNETPLUGGABLE_HPP
