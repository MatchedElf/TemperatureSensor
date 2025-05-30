#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>

#include "WirelessTempSensor.hpp"
#include "Logger.hpp"

using namespace std;

int sc_main(int argc, char* argv[]) {
    WirelessTempSensor wireless_sensor("WirelessTempSensorSystem");

    // Запуск симуляции на 5 секунд
    sc_start(5, SC_SEC);

    return 0;
}