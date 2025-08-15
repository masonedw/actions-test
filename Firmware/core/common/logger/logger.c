#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "logger.h"

static t_Log_Sink sinks[LOGGER_MAX_SINKS] = {0};
static uint8_t u8_sink_counter = 0;

void LOGGER_Register_Sink(t_Log_Sink sink) {
    if (u8_sink_counter < LOGGER_MAX_SINKS && sink.log_message != NULL) {
        sinks[u8_sink_counter++] = sink;
    }
}

void Log_Message(LOG_LEVEL log_level, const char* format, ...) {
    // If nothing is registered then try to bail out as quick as possible.
    if (u8_sink_counter == 0) {
        return;
    }

    char log[LOGGER_MAX_LENGTH] = {0};
    const char* prefix;
    va_list args;

    switch (log_level) {
        case LOG_LEVEL_VERBOSE:
            prefix = "[VERBOSE] ";
            break;
        case LOG_LEVEL_DEBUG:
            prefix = "[DEBUG] ";
            break;
        case LOG_LEVEL_INFO:
            prefix = "[INFO] ";
            break;
        case LOG_LEVEL_WARNING:
            prefix = "[WARNING] ";
            break;
        case LOG_LEVEL_ERROR:
            prefix = "[ERROR] ";
            break;
        default:
            prefix = "[UNKNOWN] ";
            break;
    }

    va_start(args, format);

    int prefixLen = snprintf(log, sizeof (log), "%s", prefix);
    if (prefixLen > 0 && prefixLen < sizeof (log)) {
        vsnprintf(log + prefixLen, sizeof (log) - prefixLen, format, args);
    }

    va_end(args);

    for (int i = 0; i < u8_sink_counter; i++) {
        if (log_level >= sinks[i].minimum_log_level) {
            sinks[i].log_message(log);
        }
    }
}

