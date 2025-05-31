#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>

#include "WirelessTempSensor.hpp"
#include "Logger.hpp"
#include "Display.hpp"
#include "Battery.hpp"

SC_MODULE(SystemTest)
{
    TemperatureSensor* sensor;
    ADC* adc;
    CommunicationInterface* comm;
    Display* display; 
    Battery* battery;

    sc_signal<double> temp_signal;
    sc_signal<int> adc_signal;
    sc_signal<bool> tx_signal;
    sc_signal<int> adc_signal_out;

    sc_signal<int> power_signal;
    sc_signal<int> display_temp_signal;

    sc_signal<bool> clock;

    void clock_gen()
    {
        while (true)
        {
            clock.write(true);
            wait(CLOCK_PERIOD, SC_NS);
            clock.write(false);
            wait(CLOCK_PERIOD, SC_NS);
        }
    }

    SC_CTOR(SystemTest)
    {
        battery = new Battery("Battery");
        battery->power_out(power_signal);

        sensor = new TemperatureSensor("Sensor");
        sensor->temp_out(temp_signal);
        sensor->clock(clock);

        adc = new ADC("ADC");
        adc->analog_in(temp_signal);
        adc->digital_out(adc_signal);
        adc->clock(clock);

        comm = new CommunicationInterface("Communication_interface");
        comm->data_in(adc_signal);
        comm->tx_active(tx_signal);
        comm->data_out(adc_signal_out);
        comm->clock(clock);

        display = new Display("Display");
        display->temp_data(adc_signal_out);
        display->clock(clock);
        display->battery_level(power_signal);

        SC_THREAD(clock_gen);
    }

    ~SystemTest() {
        delete sensor;
        delete adc;
        delete comm;
    }

};