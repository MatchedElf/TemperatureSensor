#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>
#include <queue>

#include "Logger.hpp"
using namespace std;

SC_MODULE(Memory) {
    sc_in<int> data_in;
    sc_in<bool> write_enable;

    queue<int> memory_buffer;

    Logger* logger = new Logger("Memory");

    void store_data() {
        if (write_enable.read()) {
            if (memory_buffer.size() >= 10) memory_buffer.pop();
            memory_buffer.push(data_in.read());
            *logger << "Memory: Wrote data. Buffer: ";
            queue<int> tmp = memory_buffer;
            for (int i = 0; i < memory_buffer.size(); i++)
            {
                *logger << tmp.front() << " ";
                tmp.pop();
            }
            *logger << "@ " << sc_time_stamp() << endl;
        }
    }
    SC_CTOR(Memory) { SC_METHOD(store_data); sensitive << write_enable; }
};