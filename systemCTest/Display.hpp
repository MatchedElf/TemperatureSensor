#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>

SC_MODULE(Display) {
    sc_in<int> temp_data;
    //sc_in<double> battery_level;

    void update() {
        while (true)
        {
            wait();
            cout << "Display: " << temp_data.read() << "C | Battery: "
                /*<< battery_level.read() << "%"*/ << endl;
        }
    }
    SC_CTOR(Display) 
    { 
        SC_THREAD(update); 
        sensitive << temp_data /*<< battery_level*/; 
    }
};