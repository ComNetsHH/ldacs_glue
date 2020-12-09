//
// Created by Konrad Fuger on 06.12.20.
//

#ifndef INTAIRNET_LINKLAYER_GLUE_IOMNETPLUGGABLE_H
#define INTAIRNET_LINKLAYER_GLUE_IOMNETPLUGGABLE_H

#include <functional>
#include <string>

class IOmnetPluggable {
public:
     double getTime() {
        if(getTimeCallback) {
            return getTimeCallback();
        }
        return -1;
    }

    void scheduleAt(double time) {
        if(scheduleAtCallback) {
            scheduleAtCallback(time);
        }
    }

    void emit(std::string event_name, double value) {
         if(emitCallback) {
             emitCallback(event_name, value);
         }
     }

     void debug(std::string message) {
         if(debugCallback) {
             debugCallback(message);
         }
     }

    std::function<double()> getTimeCallback;
    std::function<void(double)> scheduleAtCallback;
    std::function<void(std::string, double)> emitCallback;
    std::function<void(std::string)> debugCallback;

    // will be called for every scheduleAt call
    virtual void onEvent(double time) = 0;

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

#endif //INTAIRNET_LINKLAYER_GLUE_IOMNETPLUGGABLE_H
