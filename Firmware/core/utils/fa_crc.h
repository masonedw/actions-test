/*
 * File:   fa_crc.h
 * Author: mason.edwards
 *
 * Created on 07 May 2025, 09:33
 */

#ifndef FA_CRC_H
#define	FA_CRC_H

#ifdef	__cplusplus
extern "C" {
#endif

    uint16_t crc16_ccitt(const uint8_t *data, size_t length);


#ifdef	__cplusplus
}
#endif

#endif	/* FA_CRC_H */

