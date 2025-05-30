#pragma once
#include "systemc.h"   
#include <iostream>

SC_MODULE(Hello) {
    SC_CTOR(Hello) {
        SC_METHOD(main);
    }

    void main() {
        std::cout << "SystemC works!" << std::endl;
    }
};