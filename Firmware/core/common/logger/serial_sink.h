/* 
 * File:   serial_sink.h
 * Author: mason.edwards
 *
 * Created on 22 July 2025, 07:59
 */

#ifndef SERIAL_SINK_H
#define	SERIAL_SINK_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * SERIAL_SINK_Register registers the RS232 sink to the logger.
 * @param port The RS232 port to write the logs to.
 */
void SERIAL_SINK_Register(uint32_t port);

#ifdef	__cplusplus
}
#endif

#endif	/* SERIAL_SINK_H */

