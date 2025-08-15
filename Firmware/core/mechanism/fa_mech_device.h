/*
 * File:   fa_mech_device.h
 * Author: graeme.capes
 *
 * Created on 14 January 2025, 10:30
 */

#ifndef FA_MECH_DEVICE_H
#define	FA_MECH_DEVICE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "fa_mech_app.h"
#include "fa_mech_script.h"

    extern void DEVICE_Init(void);
    extern _Bool DEVICE_isMotorOn(t_motor_id eMotorId);
    extern uint16_t DEVICE_preprocessUserFunction(uint16_t u16_fn);
    extern uint16_t DEVICE_processSwitchFn(uint8_t u8_switch_id, uint8_t u8_level);
    extern t_motorAction DEVICE_targetScriptEntry(t_motorAction script_entry);
    extern void FA_DEVICE_motor_activation(t_motorAction thisMotorAction);
    extern void FA_DEVICE_deactivate_motor(t_motor_id eThisMotor);
    extern void FA_DEVICE_set_motor_direction(t_motor_id eThisMotor, uint16_t u16_this_fn);
    extern void FA_DEVICE_activate_motor(t_motor_id eThisMotor);
    extern uint16_t DEVICE_handle_counter(void);
    extern _Bool DEVICE_isAtReferencePoint(t_motor_id eMotorId);
    extern _Bool DEVICE_targetAtLimit(t_motorAction);



#ifdef	__cplusplus
}
#endif

#endif	/* FA_MECH_DEVICE_H */

