#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>

#include "Logger.hpp"

using namespace std;

// Модуль датчика температуры

SC_MODULE(TemperatureSensor) {
    sc_out<double> temp_out; // Выход температуры в градусах Цельсия
    Logger* logger = new Logger("Sensor");

    void generate_temp() {
        std::default_random_engine generator;
        std::normal_distribution<double> distribution(25.0, 5.0); // Средняя температура 25°C, отклонение 5°C

        while (true) {
            double temperature = distribution(generator);
            // Ограничиваем диапазон разумными значениями
            if (temperature < -10.0) temperature = -10.0;
            if (temperature > 50.0) temperature = 50.0;

            temp_out.write(temperature);
            *logger << "Sensor: temperature = " << temperature << "°C" << " @ " << sc_time_stamp() << endl;
            wait(0.5, SC_SEC); // Измеряем температуру каждую секунду
        }
    }

    SC_CTOR(TemperatureSensor) {
        SC_THREAD(generate_temp);
    }
};

// Модуль АЦП (аналого-цифрового преобразователя)
SC_MODULE(ADC) {
    sc_in<double> analog_in;  // Аналоговый вход (температура)
    sc_out<int> digital_out;  // Цифровой выход (квантованное значение)
    Logger* logger = new Logger("ADC");

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

// Модуль интерфейса связи
SC_MODULE(CommunicationInterface) {
    sc_in<int> data_in; // Вход данных от АЦП
    sc_out<int> data_out; // Выходные данные из периферии
    sc_out<bool> tx_active; // Сигнал передачи данных
    Logger* logger = new Logger("Communication");

    void transmit() {
        while (true) {
            wait();
            int data = data_in.read();
            tx_active.write(true);
            *logger << "Communication Interface: Start of transmit: " << data << " @ " << sc_time_stamp() << endl;

            // Имитация времени передачи
            wait(0.1, SC_SEC);

            *logger << "Communication Interface: End of transmit: " << data << " @ " << sc_time_stamp() << endl;
            data_out.write(data);
            tx_active.write(false);

        }
    }

    SC_CTOR(CommunicationInterface) {
        SC_THREAD(transmit);
        sensitive << data_in;
    }
};

// Верхний модуль системы
SC_MODULE(WirelessTempSensor) {
    TemperatureSensor* sensor;
    ADC* adc;
    CommunicationInterface* comm;


    sc_signal<double> temp_signal;
    sc_signal<int> adc_signal;
    sc_signal<bool> tx_signal;
    sc_signal<int> adc_signal_out;

    SC_CTOR(WirelessTempSensor) {
        sensor = new TemperatureSensor("TempSensor");
        sensor->temp_out(temp_signal);

        adc = new ADC("ADC_Module");
        adc->analog_in(temp_signal);
        adc->digital_out(adc_signal);

        comm = new CommunicationInterface("CommInterface");
        comm->data_in(adc_signal);
        comm->tx_active(tx_signal);
        comm->data_out(adc_signal_out);
    }

    ~WirelessTempSensor() {
        delete sensor;
        delete adc;
        delete comm;
    }
};