#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>

#include "WirelessTempSensor.hpp"
#include "Logger.hpp"
#include "Display.hpp"

using namespace std;

int sc_main(int argc, char* argv[]) {
    TemperatureSensor sensor("Sensor");
    ADC adc("ADC");
    CommunicationInterface comm("Communication interface");;
    Display display("Display");

    sc_signal<double> temp_signal;
    sc_signal<int> adc_signal;
    sc_signal<bool> tx_signal;
    sc_signal<int> adc_signal_out;

    sc_signal<int> display_temp_signal;

    sensor.temp_out(temp_signal);

    adc.analog_in(temp_signal);
    adc.digital_out(adc_signal);

    comm.data_in(adc_signal);
    comm.tx_active(tx_signal);
    comm.data_out(adc_signal_out);

    display.temp_data(adc_signal_out);

    // Запуск симуляции на 5 секунд
    sc_start(5, SC_SEC);

    return 0;
}