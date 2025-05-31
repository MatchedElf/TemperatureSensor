#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>

#include "Logger.hpp"
using namespace std;

SC_MODULE(Controller) {
    sc_in<int> adc_data;
    sc_in<int> power_in;
    sc_out<int> mem_data;
    sc_out<bool> mem_write;
    sc_out<int> display_temp;
    sc_out<int> display_battery;

    Logger* logger = new Logger("Controller");


    void control() {
        while (true) {
            wait();
            int power = power_in.read();
            int dataTmp = adc_data.read();
            double data = dataTmp * 60.0 / 1023.0 - 10.0;
            mem_data.write(data);      // Отправка в RF
            mem_write.write(true);    // Запись в память
            display_temp.write(data);  // На экран
            display_battery.write(power);
            wait(SC_ZERO_TIME);
            mem_write.write(false);
            *logger << "Controller: Send data to memory: " << data << " @ " << sc_time_stamp() << endl;
        }
    }
    SC_CTOR(Controller) { 
        SC_THREAD(control);
        sensitive << adc_data;
    }
};