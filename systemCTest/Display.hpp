#pragma once

#include <systemc.h>
#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <map>
#include <string>

using namespace std; 

void printDigit(int num) {
    // Каждая цифра представлена как 5 строк по 3 символа
    const map<int, vector<string>> digits = {
        {0, {
            "#####",
            "#   #",
            "#   #",
            "#   #",
            "#   #",
            "#   #",
            "#####"
        }},
        {1, {
            "  #  ",
            " ##  ",
            "# #  ",
            "  #  ",
            "  #  ",
            "  #  ",
            "#####"
        }},
        {2, {
            "#####",
            "    #",
            "    #",
            "#####",
            "#    ",
            "#    ",
            "#####"
        }},
        {3, {
            "#####",
            "    #",
            "    #",
            "#####",
            "    #",
            "    #",
            "#####"
        }},
        {4, {
            "#   #",
            "#   #",
            "#   #",
            "#####",
            "    #",
            "    #",
            "    #"
        }},
        {5, {
            "#####",
            "#    ",
            "#    ",
            "#####",
            "    #",
            "    #",
            "#####"
        }},
        {6, {
            "#####",
            "#    ",
            "#    ",
            "#####",
            "#   #",
            "#   #",
            "#####"
        }},
        {7, {
            "#####",
            "    #",
            "    #",
            "   # ",
            "  #  ",
            " #   ",
            "#    "
        }},
        {8, {
            "#####",
            "#   #",
            "#   #",
            "#####",
            "#   #",
            "#   #",
            "#####"
        }},
        {9, {
            "#####",
            "#   #",
            "#   #",
            "#####",
            "    #",
            "    #",
            "#####"
        }}
    };

    // Проверяем, что число в диапазоне 0-9
    if (num < 0 || num > 9) {
        cerr << "Error: Number must be between 0 and 9" << endl;
        return;
    }

    // Печатаем каждую строку цифры
    for (const auto& line : digits.at(num)) {
        cout << line << endl;
    }
}

void printNumber(int number) {
    string numStr = to_string(number);

    const map<int, vector<string>> digits = {
                {0, {
            "#####",
            "#   #",
            "#   #",
            "#   #",
            "#   #",
            "#   #",
            "#####"
        }},
        {1, {
            "  #  ",
            " ##  ",
            "# #  ",
            "  #  ",
            "  #  ",
            "  #  ",
            "#####"
        }},
        {2, {
            "#####",
            "    #",
            "    #",
            "#####",
            "#    ",
            "#    ",
            "#####"
        }},
        {3, {
            "#####",
            "    #",
            "    #",
            "#####",
            "    #",
            "    #",
            "#####"
        }},
        {4, {
            "#   #",
            "#   #",
            "#   #",
            "#####",
            "    #",
            "    #",
            "    #"
        }},
        {5, {
            "#####",
            "#    ",
            "#    ",
            "#####",
            "    #",
            "    #",
            "#####"
        }},
        {6, {
            "#####",
            "#    ",
            "#    ",
            "#####",
            "#   #",
            "#   #",
            "#####"
        }},
        {7, {
            "#####",
            "    #",
            "    #",
            "   # ",
            "  #  ",
            " #   ",
            "#    "
        }},
        {8, {
            "#####",
            "#   #",
            "#   #",
            "#####",
            "#   #",
            "#   #",
            "#####"
        }},
        {9, {
            "#####",
            "#   #",
            "#   #",
            "#####",
            "    #",
            "    #",
            "#####"
        }}
    };

    for (int i = 0; i < 7; ++i) { // Каждая цифра состоит из 5 строк
        for (char c : numStr) {
            int digit = c - '0';

            // Получаем соответствующую строку для текущей цифры
            

            if (digits.count(digit)) {
                cout << " \033[32m" << digits.at(digit)[i] << "\033[0m ";
            }
        }
        vector<string> letter = {
            "       ",
            "## ####",
            "## #   ",
            "   #   ",
            "   #   ",
            "   #   ",
            "   ####"
        };
        cout << "  \033[32m" << letter[i] << "\033[0m ";
        cout << endl;
    }
}

SC_MODULE(Display) {
    sc_in<int> temp_data;
    sc_in<int> battery_level;
    sc_in<bool> clock;

    void update() {
        while (true)
        {
            wait();
            //cout << "Display: " << temp_data.read() << "C | Battery: " /*<< battery_level.read() << "%"*/ << endl;
            cout << "-----------------------------------------------------" << endl;
            cout << " " << sc_time_stamp() << " " << battery_level.read() << "%\n" << endl;
            printNumber(temp_data.read());
            cout << "-----------------------------------------------------\n" << endl;
        }
    }
    SC_CTOR(Display) 
    { 
        SC_THREAD(update); 
        sensitive << temp_data /*<< battery_level*/; 
    }
};