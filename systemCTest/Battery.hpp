#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>

#include "Logger.hpp"
using namespace std;

// Модуль датчика температуры

SC_MODULE(Battery) {
    sc_out<int> power_out; // Выход температуры в градусах Цельсия
    Logger* logger = new Logger("Battery");
    //sc_in<bool> clock;
    int power = 100;
    void generate_temp() {

        while (true) {
            wait(CLOCK_PERIOD * 10, SC_NS);
            power--;
            // Ограничиваем диапазон разумными значениями
            if (power <= 0) power = 0;

            power_out.write(power);
            *logger << "Battery: power = " << power << "%" << " @ " << sc_time_stamp() << endl;
        }
    }

    SC_CTOR(Battery) {
        SC_THREAD(generate_temp);
        //sensitive << clock;
    }
};
