#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>

#include "WirelessTempSensor.hpp"
#include "Communication.hpp"
#include "Logger.hpp"
#include "Display.hpp"
#include "Battery.hpp"
#include "Controller.hpp"
#include "Memory.hpp"

SC_MODULE(SystemTest)
{
    TemperatureSensor* sensor;
    ADC* adc;
    CommunicationInterface* comm;
    Display* display; 
    Battery* battery;
    Controller* control;
    Memory* memory;

    sc_signal<double> temp_signal;
    sc_signal<int> adc_signal;
    sc_signal<bool> tx_signal;
    sc_signal<int> adc_signal_out;

    sc_signal<int> power_signal_to_control;
    sc_signal<int> power_signal_from_control;
    sc_signal<int> display_temp_signal;

    sc_signal<int> data_to_memory;
    sc_signal<bool> write_enable;

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
        battery->power_out(power_signal_to_control);

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
        display->temp_data(display_temp_signal);
        display->clock(clock);
        display->battery_level(power_signal_from_control);

        control = new Controller("Controller");
        control->power_in(power_signal_to_control);
        control->display_battery(power_signal_from_control);
        control->adc_data(adc_signal_out);
        control->display_temp(display_temp_signal);
        control->mem_data(data_to_memory);
        control->mem_write(write_enable);

        memory = new Memory("Memory");
        memory->data_in(data_to_memory);
        memory->write_enable(write_enable);
        SC_THREAD(clock_gen);
    }

    ~SystemTest() {
        delete sensor;
        delete adc;
        delete comm;
    }

};