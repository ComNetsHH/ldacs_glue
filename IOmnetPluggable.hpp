//
// Created by Konrad Fuger on 06.12.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IOMNETPLUGGABLE_HPP
#define INTAIRNET_LINKLAYER_GLUE_IOMNETPLUGGABLE_HPP

#include <functional>
#include <string>

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

	std::function<double()> getTimeCallback;
	std::function<void(double)> scheduleAtCallback;
	std::function<void(std::string, double)> emitCallback;
	std::function<void(std::string)> debugCallback;

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
};

#endif //INTAIRNET_LINKLAYER_GLUE_IOMNETPLUGGABLE_HPP
