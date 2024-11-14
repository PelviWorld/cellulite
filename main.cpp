#include <modbus/modbus.h>
#include <iostream>

int main() {
    // Create a new RTU context
    modbus_t *ctx = modbus_new_rtu("/dev/ttyUSB0", 57600, 'N', 8, 2);
    if (ctx == nullptr) {
        std::cerr << "Unable to create the libmodbus context" << std::endl;
        return -1;
    }

    // Set the Modbus slave ID
    modbus_set_slave(ctx, 1);

    // Connect to the Modbus server
    if (modbus_connect(ctx) == -1) {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return -1;
    }

    // Read 5 holding registers starting from address 0
    uint16_t tab_reg[5];
    int rc = modbus_read_registers(ctx, 101, 5, tab_reg);
    if (rc == -1) {
        std::cerr << "Failed to read registers: " << modbus_strerror(errno) << std::endl;
        modbus_close(ctx);
        modbus_free(ctx);
        return -1;
    }

    // Print the read registers
    for (int i = 0; i < 5; ++i) {
        std::cout << "Register " << i << ": " << tab_reg[i] << std::endl;
    }

    // Close the connection and free the context
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}