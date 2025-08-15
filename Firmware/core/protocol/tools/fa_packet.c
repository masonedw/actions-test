#include <ctype.h>
#include <string.h>
#include "global-definitions.h"
#include "fn_codes.h"
#include "fa_packet.h"

bool FA_PACKET_check_header(uint8_t * u8_buffer_ptr) {
    char fa_tag[3] = "fa_";
    bool bRetVal = true;

    for (int i = 0; i < 3; i++) {
        if (tolower(u8_buffer_ptr[i]) != fa_tag[i]) {
            bRetVal = false;
        }
    }
    return bRetVal;
}

/*****************************************************************************/
//Take a FA_A to FA_STOP for FA RS232 / Keyad CMDS

/*****************************************************************************/
_Bool FA_PACKET_convert_fa_rs232(uint8_t * in_packet, uint16_t in_packet_len, t_fa_packet_data * command_packet) {
    uint8_t i;
    _Bool bRetVal = false;

    if ((in_packet_len > 0) && (in_packet_len <= FRAMEWORK_MAX_PACKET_SIZE)) {
        if ((in_packet[3] >= 0x41)&&(in_packet[3] <= 0x7A)) {
            if (in_packet[in_packet_len - 1] == '\r') {
                command_packet->length = in_packet_len - 1;
                for (i = 0; i < command_packet->length; i++) {
                    command_packet->data[i] = in_packet[i];
                }
                if (FA_PACKET_check_header(command_packet->data)) {
                    bRetVal = true;
                }
            }
        }
    }
    return bRetVal;
}

/*****************************************************************************/
//Take a FA_1 to 100 cmds for simulating Keypad inputs

/*****************************************************************************/
_Bool FA_PACKET_convert_sdn_button(uint8_t * in_packet, uint16_t in_packet_len, t_fa_packet_data * command_packet) {
    uint8_t i;
    _Bool bRetVal = false;
    if ((in_packet_len > 0) && (in_packet_len <= FRAMEWORK_MAX_PACKET_SIZE)) {
        //check first value is between 0 and 9
        if ((in_packet[3] >= 0x30)&&(in_packet[3] < 0x3A)) {
            //if carriage return then remove carriage return
            if (in_packet[in_packet_len - 1] == '\r') {
                command_packet->length = in_packet_len - 1;
            } else {
                command_packet->length = in_packet_len;
            }
            //convert to cmd packet
            for (i = 0; i < command_packet->length; i++) {
                command_packet->data[i] = in_packet[i];
            }
            //check for fa_
            if (FA_PACKET_check_header(command_packet->data)) {
                bRetVal = true;
            }
        }
    }
    return bRetVal;
}

uint8_t FA_PACKET_extract_fn(t_fa_packet_data * fn_packet_ptr) {
    uint8_t u8_retVal = FN_NULL;
    uint8_t i;

    char c8_packet[33];

    for (i = 0; i < fn_packet_ptr->length; i++) {
        c8_packet[i] = tolower(fn_packet_ptr->data[i]);
    }
    c8_packet[i] = 0;

    if (strcmp(c8_packet, "fa_in") == 0) {
        u8_retVal = FN_IN;
    } else if (strcmp(c8_packet, "fa_out") == 0) {
        u8_retVal = FN_OUT;
    } else if (strcmp(c8_packet, "fa_stop") == 0) {
        u8_retVal = FN_USER_STOP;
    } else if (strcmp(c8_packet, "fa_preset") == 0) {
        u8_retVal = FN_PRESET;
    } else if (strcmp(c8_packet, "fa_a") == 0) {
        u8_retVal = FN_A;
    } else if (strcmp(c8_packet, "fa_b") == 0) {
        u8_retVal = FN_B;
    } else if (strcmp(c8_packet, "fa_c") == 0) {
        u8_retVal = FN_C;
    } else if (strcmp(c8_packet, "fa_d") == 0) {
        u8_retVal = FN_D;
    } else if (strcmp(c8_packet, "fa_e") == 0) {
        u8_retVal = FN_E;
    } else if (strcmp(c8_packet, "fa_f") == 0) {
        u8_retVal = FN_F;
    } else if (strcmp(c8_packet, "fa_left") == 0) {
        u8_retVal = FN_LEFT;
    } else if (strcmp(c8_packet, "fa_right") == 0) {
        u8_retVal = FN_RIGHT;
    }
    return u8_retVal;
}

uint16_t FA_PACKET_extract_sdn_button(t_fa_packet_data * fn_packet_ptr) {
    uint16_t u16_retVal = GLB_NULL_INT;
    char b[3];
    uint8_t extractionLength = (fn_packet_ptr->length - 3);
    if (FA_PACKET_check_header(fn_packet_ptr->data)) {
        strncpy((char *) b, (/*const*/ char *) (fn_packet_ptr->data + 3), extractionLength);
        // Manually insert NUL terminator as strncpy doesnt ensure that what is copied
        // is a correctly formed C string. As the string that is copied may be truncated
        // due to the "b" buffer being small, the NUL terminator may be omitted.
        b[2] = '\0';
        u16_retVal = atoi(b) - 1;
    }
    return u16_retVal;
}

