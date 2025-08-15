#include "framework.h"
#include "logger.h"
#include <string.h>

void serial_log_message(const char* log);

t_Log_Sink serial_sink = {
    .minimum_log_level = LOG_LEVEL_VERBOSE,
    .log_message = serial_log_message
};

uint32_t serial_port;

void SERIAL_SINK_Register(uint32_t port) {
    serial_port = port;
    LOGGER_Register_Sink(serial_sink);
}

/**
 * serial_log_message is the sink specific log implementation that writes the logs
 * to RS232.
 * @param log
 */
void serial_log_message(const char* log) {
    FA_USART_send(serial_port, (uint8_t*)log, strlen(log));
}
