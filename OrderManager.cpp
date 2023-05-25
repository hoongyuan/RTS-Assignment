#include <iostream>
#include "portmacro.h"

void OrderManagerTask(void* parameter) {
    // Order Manager
    while (true) {
        // Execute transactions based on received orders
        std::cout << "Executing transaction..." << std::endl;

        // Delay to simulate real-time behavior
        /*vTaskDelay(1000 / portTICK_PERIOD_MS);*/
    }
}
