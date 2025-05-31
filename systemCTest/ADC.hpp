#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>

#include "Logger.hpp"

using namespace std;

// Модуль АЦП (аналого-цифрового преобразователя)
SC_MODULE(ADC) {
    sc_in<double> analog_in;  // Аналоговый вход (температура)
    sc_out<int> digital_out;  // Цифровой выход (квантованное значение)
    Logger* logger = new Logger("ADC");
    sc_in<bool> clock;

    void convert() {
        while (true) {
            double temp = analog_in.read();
            // Преобразуем температуру в цифровое значение (0-1023)
            int digital_value = static_cast<int>((temp + 10.0) * (1023.0 / 60.0));
            if (digital_value < 0) digital_value = 0;
            if (digital_value > 1023) digital_value = 1023;

            digital_out.write(digital_value);
            *logger << "ADC: Value = " << digital_value << " (from temp " << temp << "°C)" << " @ " << sc_time_stamp() << endl;
            //wait(SC_ZERO_TIME); // Небольшая задержка для синхронизации
            wait();
        }
    }

    SC_CTOR(ADC) {
        SC_THREAD(convert);
        sensitive << analog_in;
    }
};