#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>

#include "WirelessTempSensor.hpp"
#include "Logger.hpp"
#include "Display.hpp"
#include "System.hpp"

using namespace std;

int sc_main(int argc, char* argv[]) {
    SystemTest test("Test");
    // Запуск симуляции на 5 секунд
    sc_start(CLOCK_PERIOD * 110, SC_NS);

    return 0;
}