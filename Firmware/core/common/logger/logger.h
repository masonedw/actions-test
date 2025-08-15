/* 
 * File:   logger.h
 * Author: mason.edwards
 *
 * Created on 21 July 2025, 14:48
 */

#ifndef LOGGER_H
#define	LOGGER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define LOGGER_MAX_SINKS 5
#define LOGGER_MAX_LENGTH 1024

typedef enum {
    LOG_LEVEL_VERBOSE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
} LOG_LEVEL;

#define LOG_VERBOSE(format, ...) Log_Message(LOG_LEVEL_VERBOSE, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) Log_Message(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) Log_Message(LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) Log_Message(LOG_LEVEL_WARNING, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Log_Message(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

/**
 * t_Log_Sink
 * 
 * Struct representing a logging sink.
 * log_message is a function pointer to the sink specific log implementation.
 * minimum_log_level is the minimum logging level for the sink. This allows sinks to
 * choose what they want to log. For example, an EEPROM sink might only want to log ERRORs
 * to EEPROM. 
 * 
 */
typedef struct {
    void (*log_message)(const char* log_message);
    LOG_LEVEL minimum_log_level;
} t_Log_Sink;

/**
 * Log_Message will log a message to the given sinks that have registered with the logging module.
 * 
 * @param log_level The level assigned to the log.
 * @param format The format string for the log.
 * @param ... The arguments to place in the format string. 
 */
void Log_Message(LOG_LEVEL log_level, const char* format, ...);

/**
 * LOGGER_Register_Sink Will register the sink to the logging module.
 * Many different sinks can register, meaning you can log many
 * different outputs (232, EEPROM, etc.).
 * @param sink
 */
void LOGGER_Register_Sink(t_Log_Sink sink);

#ifdef	__cplusplus
}
#endif

#endif	/* LOGGER_H */

