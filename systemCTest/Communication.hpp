#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>
#include "Logger.hpp"

using namespace std;

// Модуль интерфейса связи
SC_MODULE(CommunicationInterface) {
    sc_in<int> data_in; // Вход данных от АЦП
    sc_out<int> data_out; // Выходные данные из периферии
    sc_out<bool> tx_active; // Сигнал передачи данных

    Logger* logger = new Logger("Communication");
    
    sc_in<bool> clock;


    void transmit() {
        std::default_random_engine rand_engine;
        std::bernoulli_distribution error_dist(0.05); // 5% ошибок
        while (true) {
            wait();
            int data = data_in.read();
            tx_active.write(true);
            *logger << "Communication Interface: Start of transmit: " << data << " @ " << sc_time_stamp() << endl;

            // Имитация времени передачи
            wait(CLOCK_PERIOD / 2, SC_NS);

            if (!error_dist(rand_engine))
            {
                *logger << "Communication Interface: End of transmit: " << data << " @ " << sc_time_stamp() << endl;
                data_out.write(data);
            }
            else {
                *logger << "Communication Interface: Packet lost: " << -999 << " @ " << sc_time_stamp() << endl;
            }
            
            tx_active.write(false);

        }
    }

    SC_CTOR(CommunicationInterface) {
        SC_THREAD(transmit);
        sensitive << data_in;
    }
};