#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>                     // Defines NULL
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <math.h>
#include "fa_timer.h"
#include "fa_usart.h"
#include "framework.h"

#define _XTAL_FREQ                    20000000

// General purpose timer (T0)
#define  TICK_DURATION_US             100           /* based on 20mhz with no prescalar and T0 base of 65047 */ 

#define  INT_TIMER0_BASE            65047           /* Each tick represents 200ns.*/ 
                                                    /* 500 ticks represents 100us */
													/* If the processor were doing nothing else a base of 65035 would overflow after 100us */
                                                    /* However, using a 20mhz OSC the call to thr ISR and refreshing the interrupt flag takes 2.4002us */
                                                    /* increasing INT_TIMER0_BASE to 65047 compensates for 2.4us, leaving a T0 overflow of 100.0002us  */

#define  TICK_ADJUSTMENT              500           /* The remaining 200ns is factored out by adding 1 to the tick count every 500 cycles (equate to and extra 100us of time */

uint8_t calculateparity(uint16_t scancode);   
_Bool checkParity(uint16_t scancode);

extern uint32_t   u32_timer_tick_data;
extern void FA_SAM_USART_data_notification(uintptr_t context);

bool            bTimerOn = false;
uint16_t        u16_rx_error;
//uint16_t *      u16_target_buffer_ptr;
uintptr_t       context_value;
t_data_framing  eFraming_port_0;

// Baud rate calculation
static float                        fRealDBR = 0.0;
static uint8_t                      u8_brg16 = 0;
static uint8_t                      u8_brgh = 1;
static float                        fMul = 16.0;
//static int                        spbrgh = 0;
static float                        fSpbrg = 0.0;
static float                        fAdjDBR = 0.0;
static int                          s16_mulSwop = 0;
static float                        fBestSpbrg = 0.0;
static float                        fBestMul = 0.0;
static float                        fBestRealDBR = 0.0;
static _Bool                        bAdjUp = false;
static _Bool                        bAdjDown = false;
static _Bool                        bFinished = true;
static float                        fCalculating_Baud = 0.0;
static uint16_t                     u16_tick_adjustment_countdown = TICK_ADJUSTMENT;




/****************************************************************************
 * Interrupt service routine for the timer 
 * If the timer is off this will be called 
 * every 51.2uS 
*/

/**************************/
/* GENERAL PURPOSE TIMERS */
/**************************/
//type_systemTimer eRxPacketTimer;

static volatile _Bool               bBreakLatch = false;



//***************************************************************
// INTERRUPT HANDLER
//---------------------------------------------------------------
void  __interrupt (high_priority) Timer0_ISR(void) {
    //Restart timer quickly to maximise accuracy
    TMR0 = INT_TIMER0_BASE;
    INTCONbits.TMR0IF = 0;

    u32_timer_tick_data++;
    
    //Deal with the tick imprecision. at 100.2us 
    if (--u16_tick_adjustment_countdown == 0) {
        u16_tick_adjustment_countdown = TICK_ADJUSTMENT;
        u32_timer_tick_data++;
    }
}

void  __interrupt (low_priority) RCIF_low(void) {
    uint16_t u16_Data;

    PIR1bits.RCIF = 0;

    if (RCSTAbits.FERR) {
        u16_Data = RCREG;
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        u16_rx_error = RX_FRAMING_ERROR;
    }
    else if (RCSTAbits.OERR) {
        u16_Data = RCREG;
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        u16_rx_error = RX_OVERFLOW_ERROR;
    }
    else {
        u16_Data = RCREG;
        checkParity(u16_Data);
        u16_rx_error = RX_NO_ERROR;
    }
    FA_PIC_USART_data_notification(u16_Data, u16_rx_error);
}

//
//
// Set board rate 
//********************************************************
void setBaud_init(void){
    fRealDBR = 0.0;
    u8_brg16 = 0;
    u8_brgh = 1;
    fMul = 16.0;
    //spbrgh = 0;
    fSpbrg = 0.0;
    fAdjDBR = 0.0;
    s16_mulSwop = 0;
    fBestSpbrg = 0.0;
    fBestMul = 0.0;
    fBestRealDBR = 0.0;
    bAdjUp = false;
    bAdjDown = false;
    bFinished = true;
}

void calculateDBR(void){
    fSpbrg = ((((float)_XTAL_FREQ)/fCalculating_Baud)/fMul)-1;  //((FOSC-(DBR*mul))/(DBR*mul));
    fSpbrg = roundf(fSpbrg);
    fSpbrg = fSpbrg + fAdjDBR;
    fRealDBR = ((float)_XTAL_FREQ)/(fMul*(fSpbrg+1));
}

void RecalculateDBR(void){
    
    if (s16_mulSwop == 0){
        fMul = 64;
        u8_brgh = 0;
        u8_brg16 = 0;
        calculateDBR();
        s16_mulSwop++;
    }
    else if (s16_mulSwop == 1){
        // sync = 1;                                                         fix from table
         fMul = 4;
         u8_brgh = 1;
         u8_brg16 = 1;
         calculateDBR();
         s16_mulSwop++;
     }
    else {
         bFinished = false;
     }
}
void saveBest (void){
    fBestSpbrg = fSpbrg;
    fBestMul = fMul;
    fBestRealDBR = fRealDBR;
}

void FindBRG(void){
    
    if (fBestMul == 64.0){
        u8_brgh = 0;
        u8_brg16 = 0;
    }
    if (fBestMul == 16.0){
        u8_brgh = 1;
        u8_brg16 = 0;
    }
    if (fBestMul == 4.0){
        u8_brgh = 1;
        u8_brg16 = 1;
    }
}


void EUSART_setBaud(uint32_t Desired_Baud_Rate) {
    
    setBaud_init();
    
    fCalculating_Baud = ((float)Desired_Baud_Rate);
    calculateDBR();
    if (fSpbrg > 256 || fSpbrg < 0){
        RecalculateDBR();
    }    
    saveBest ();
    
    
    while (bFinished){///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       
        if (fRealDBR > fCalculating_Baud){
            fAdjDBR = fAdjDBR + ((float)1.0);
            calculateDBR();
            bAdjUp = true;
        }
        else{//if (realDBR < HW_DESIRED_BAUD_RATE){
            fAdjDBR = fAdjDBR - ((float)1.0);
            calculateDBR();
            bAdjDown = true;
        }
        if (abs(((int)fCalculating_Baud)-((int)fBestRealDBR)) > abs(((int)fCalculating_Baud)-((int)fRealDBR))){
            saveBest ();
        }
        /* if ((HW_DESIRED_BAUD_RATE-bestRealDBR) > (HW_DESIRED_BAUD_RATE - realDBR)){
            saveBest ();
        }*/
        if (bAdjUp && bAdjDown){
            bAdjUp = false;
            bAdjDown = false;
            RecalculateDBR();
        }
        if (fSpbrg > 256 || fSpbrg < 0){
            RecalculateDBR();
        } 
    }
    FindBRG();
    
    BAUDCONbits.BRG16 = u8_brg16;
    TXSTAbits.BRGH = u8_brgh;
    SPBRG = ((unsigned char)fBestSpbrg);
    SPBRGH = 0;
}



uint8_t calculateparity(uint16_t scancode)   
{   uint8_t iRetval = 0;   
	int8_t parity = 0; 
    int8_t i8_comparison;
	
    
	while(scancode > 0)          // if it is 0 there are no more 1's to count    
	{        
		if(scancode & 0x01)    //see if LSB is 1        
		{              
			parity++;                // why yes it is        
		}        
		scancode = scancode >> 1; //shift to next bit    
	}  
    i8_comparison = parity & 0x01;
    switch (eFraming_port_0) {
        case eight_bit_odd_parity:
            iRetval = i8_comparison ? 0 : 1;
            break;
        case eight_bit_even_parity:
            iRetval = i8_comparison ? 1 : 0;
            break;
        case eight_bit_no_parity:
        case nine_bit:
            /* Do nothing */
            break;

    }
	return (iRetval);  // only need the low bit to determine odd / even }      
}


_Bool checkParity(uint16_t scancode){
    _Bool bRetval = false;   
	int8_t parity = 0;      
    int8_t i8_comparison;
	
    
	while(scancode > 0)          // if it is 0 there are no more 1's to count    
	{        
		if(scancode & 0x01)    //see if LSB is 1        
		{              
			parity++;                // why yes it is        
		}        
		scancode = scancode >> 1; //shift to next bit    
	}  
    i8_comparison = parity & 0x01;
    switch (eFraming_port_0) {
        case eight_bit_odd_parity:
            if (i8_comparison != RCSTAbits.RX9D) {
                bRetval = true;
            }
            break;
        case eight_bit_even_parity:
            if (i8_comparison == RCSTAbits.RX9D) {
                bRetval = true;
            }
        case eight_bit_no_parity:
        case nine_bit:
            bRetval = true;
            break;
    }
	return (bRetval);  // only need the low bit to determine odd / even } 
}     


void FRAMEWORK_MAIN_init_processor(void) {
    
    // Interrupts
        // 
    RCONbits.IPEN       = 1;    // Enable interrupt priority functionality
    INTCONbits.GIEH     = 1;    // Enable high priority interrupts
    INTCONbits.GIEL     = 1;    // Enable low priority interrupts
    IPR1bits.RCIP       = 0;    // Set USART RX interrupt to low priority
    
    INTCON2bits.RBPU    = 0;    // PortB weak pull ups enabled by individual port latch values
        
    //-----------------------------------------------------------------
    
    ADCON1 = 0x0F; // Set all analog pins as digital inputs
    
    //Port A
    //==============================
    TRISAbits.RA0 =   0; //RA0  OUT  Transceiver DE
    TRISAbits.RA1 =   0; // Switch 1 LED
    TRISAbits.RA2 =   0; // Switch 2 LED
    TRISAbits.RA3 =   0; // Switch 3 LED
    TRISAbits.RA4 =   0; // Switch 4 LED
    TRISAbits.RA5 =   1; // Switch 4
    
    
    //Port B
    //==============================
    TRISBbits.RB0 =   1; //RB0  IN   IR / external interrupt
    TRISBbits.RB1 =   1; //Switch 3
    TRISBbits.RB2 =   1; //Switch 2
    TRISBbits.RB3 =   1; //Switch 1
    TRISBbits.RB4 =   1; //Switch 11
    TRISBbits.RB5 =   0; //Switch 11 LED
    TRISBbits.RB6 =   1; //Switch 12
    TRISBbits.RB7 =   0; //Switch 12 LED

    //Port C
    //==============================
    TRISCbits.RC0 =   1; //Switch 7
    TRISCbits.RC1 =   0; //RC1  OUT  LED
    TRISCbits.RC2 =   0; //Switch 8 LED
    TRISCbits.RC3 =   0; //Switch 10 LED
    TRISCbits.RC4 =   1; //Switch 8
    //TRISCbits.RC5 =   0; //RC5  N/C
    TRISCbits.RC6 =   0; //RC6  OUT  EUSART TX
    TRISCbits.RC7 =   0; //RC7  IN  EUSART RX

    //Port D
    //==============================
    TRISDbits.RD0 =   1; //Switch 10
    //TRISDbits.RD1 =   1; //N/C
    TRISDbits.RD2 =   1; //Switch 9
    TRISDbits.RD3 =   0; //Switch 9 LED
    TRISDbits.RD4 =   0; //RD4  OUT  RS485 Dip Transistor 
    TRISDbits.RD5 =   0; //Switch 7 LED
    TRISDbits.RD6 =   1; //Switch 6
    TRISDbits.RD7 =   1; //Switch 5
    
    //Port E
    //==============================
    TRISEbits.RE0 =   0; //Switch 5 LED
    TRISEbits.RE1 =   0; //Switch 6 LED
    TRISEbits.RE2 =   0; //RE2  OUT  Transceiver RE
    
    
    TRANSISTOR_PIN = 0;    
}


void FRAMEWORK_TIMER_init (void) {
    // Initilaize T0 interrupt
    INTCONbits.T0IE     = 0;    // Timer 0 disabled (enabled when timers are active)
    T0CONbits.TMR0ON    = 0;    // Timer 0 off
    T0CONbits.T08BIT    = 0;    // Timer 0 16 bit
    T0CONbits.T0CS      = 0;    // Timer 0 internal clock source
    TMR0L                = 0;    // Clear timer 0 
    T0CONbits.TMR0ON     = 1;    // Timer 0 on
}


float FRAMEWORK_TIMER_get_resolution(void) {
    return TICK_DURATION_US;
}


void FRAMEWORK_TIMER_start(void) {
    u16_tick_adjustment_countdown = 500;
    INTCONbits.T0IE = 1;
}

void FRAMEWORK_TIMER_stop(void) {
    INTCONbits.T0IE = 0;
    INTCONbits.TMR0IF = 0;
}

bool FRAMEWORK_TIMER_running(void) {
    return INTCONbits.T0IE;    
}



// EEPROM
// ------
void FRAMEWORK_write_eeprom_8(uint16_t u16_addr, uint8_t u8_data) {
    EEADR = (uint8_t)(u16_addr & 0xFF);
    EEADRH = (uint8_t)((u16_addr >> 8) & 0xFF);
    EEDATA = u8_data;
    EECON1 &= 0x3F;
    EECON1 |= 4;

    INTCON &= 0x7F;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1 |= 2;
    while (EECON1 & 0x2) {};

    INTCON |= 0x80;
}



void FRAMEWORK_write_eeprom_16(uint16_t u16_addr, uint16_t u16_data) {
    FRAMEWORK_write_eeprom_8(u16_addr, (uint8_t)(u16_data & 0xFF));
    FRAMEWORK_write_eeprom_8(u16_addr+1, (uint8_t)((u16_data >> 8) & 0xFF));
}



void FRAMEWORK_write_eeprom_32(uint16_t u16_addr, uint32_t u32_data) {    
    FRAMEWORK_write_eeprom_8(u16_addr, (uint8_t)(u32_data & 0xFF));
    FRAMEWORK_write_eeprom_8(u16_addr+1, (uint8_t)((u32_data >> 8) & 0xFF));
    FRAMEWORK_write_eeprom_8(u16_addr+2, (uint8_t)((u32_data >> 16) & 0xFF));
    FRAMEWORK_write_eeprom_8(u16_addr+3, (uint8_t)((u32_data >> 24) & 0xFF));
}



void FRAMEWORK_write_eeprom_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length) {    
    uint16_t u16_index;
    
    for (u16_index = 0; u16_index < u16_length; u16_index++) {
        FRAMEWORK_write_eeprom_8(u16_addr+u16_index, u8_data[u16_index]);
    }
}


void FRAMEWORK_fill_eeprom_block(uint16_t u16_addr, uint8_t u8_data, uint16_t u16_length) {    
    uint16_t u16_index;
    
    for (u16_index = 0; u16_index < u16_length; u16_index++) {
        FRAMEWORK_write_eeprom_8(u16_addr+u16_index, u8_data);
    } 
}



uint8_t FRAMEWORK_read_eeprom_8(uint16_t u16_addr) {
    EEADR = (uint8_t)(u16_addr & 0xFF);
    EEADRH = (uint8_t)((u16_addr >> 8) & 0xFF);
    EECON1 &= 0x3F;
    EECON1 |= 1;
    return EEDATA;
}



uint16_t FRAMEWORK_read_eeprom_16(uint16_t u16_addr) {
    return  ( (uint16_t)FRAMEWORK_read_eeprom_8(u16_addr) +
              (uint16_t)(FRAMEWORK_read_eeprom_8(u16_addr + 1) << 8)
            );
}



uint32_t FRAMEWORK_read_eeprom_32(uint16_t u16_addr) {
    return  ( (uint32_t)FRAMEWORK_read_eeprom_8(u16_addr) +
              ((uint32_t)FRAMEWORK_read_eeprom_8(u16_addr + 1) << 8) +
              ((uint32_t)FRAMEWORK_read_eeprom_8(u16_addr + 2) << 16) +
              ((uint32_t)FRAMEWORK_read_eeprom_8(u16_addr + 3) << 24)
             );
}



void FRAMEWORK_read_eeprom_block(uint16_t u16_addr, uint8_t * u8_data, uint16_t u16_length) {
    uint16_t u16_index;

    for (u16_index = 0; u16_index < u16_length; u16_index++) {
        u8_data[u16_index] = FRAMEWORK_read_eeprom_8(u16_addr+u16_index);
    }
}



//RS485
//--------
void FRAMEWORK_USART_init_port_0 ( t_data_framing eFraming) {
        // Enable serial port
    EUSART_setBaud(4800);      //for now the setting of the baud happens 
    RCSTAbits.SPEN = 1;    // Enable EUSART 1 
    TRISCbits.RC7 = 1;
    TRISCbits.RC6 = 1;
    eFraming_port_0 = eFraming;
    if (eFraming == eight_bit_no_parity) {
        RCSTAbits.RX9 = 0;     // 9 bit receive
        TXSTAbits.TX9 = 0;     // 9 bit transmit
    }
    else {
        RCSTAbits.RX9 = 1;     // 9 bit receive
        TXSTAbits.TX9 = 1;     // 9 bit transmit
    }
    RCSTAbits.CREN = 1;      // Enable continuous transmission
    TXSTAbits.TXEN = 1;    // Enable transmission
    // Wait for transmit shift register to flush
    while (TXSTAbits.TRMT == 0) {      
        asm("clrwdt");
    }

    PIE1bits.RCIE      = 1;    // Enable EUSART receive interrupt        
}

void FRAMEWORK_USART_init_port_1 (t_data_framing eFraming) {
    // Do nothing
}

void FRAMEWORK_USART_init_port_5 (t_data_framing eFraming) {
    // Do nothing
}


void FRAMEWORK_USART_re_set_port_0(void) {
    RE_PIN = 1;
}

void FRAMEWORK_USART_re_set_port_1(void) {
    // Do nothing
}


void FRAMEWORK_USART_re_set_port_5(void) {
    // Do nothing
}


void FRAMEWORK_USART_re_clear_port_0(void) {
    RE_PIN = 0;
}

void FRAMEWORK_USART_re_clear_port_1(void) {
    // Do nothing
}


void FRAMEWORK_USART_re_clear_port_5(void) {
    // Do nothing
}


void FRAMEWORK_USART_de_set_port_0(void) {
    DE_PIN = 1;
}

void FRAMEWORK_USART_de_set_port_1(void) {
    // Do nothing
}


void FRAMEWORK_USART_de_set_port_5(void) {
    // Do nothing
}


void FRAMEWORK_USART_de_clear_port_0(void) {
    DE_PIN = 0;
}

void FRAMEWORK_USART_de_clear_port_1(void) {
    // Do nothing
}


void FRAMEWORK_USART_de_clear_port_5(void) {
    // Do nothing
}


void FRAMEWORK_USART_break_set_port_0(void) {
    RE_PIN = 1;
    DE_PIN = 1;
    TXSTAbits.TXEN = 0;
    TXREG = 0;
    TXSTAbits.TXEN = 1;
    TXSTAbits.SENDB = 1;
    // load bits 1-8 and transmit
    while (TXSTAbits.TRMT == 0) {               // Wait until TSR is empty
        asm("clrwdt");
    }
    RE_PIN = 0;
    DE_PIN = 0;
}

void FRAMEWORK_USART_break_set_port_1(void) {
    // Do nothing
}

void FRAMEWORK_USART_break_set_port_5(void) {
    // Do nothing
}


void FRAMEWORK_USART_break_clear_port_0(void) {
    // Do nothing
}

void FRAMEWORK_USART_break_clear_port_1(void) {
    // Do nothing
}


void FRAMEWORK_USART_break_clear_port_5(void) {
    // Do nothing
}


uint16_t FRAMEWORK_USART_ErrorGet_port_0(void) {
    return u16_rx_error;
}


uint16_t FRAMEWORK_USART_ErrorGet_port_1(void) {
    return 0;
}


uint16_t FRAMEWORK_USART_ErrorGet_port_5(void) {
    return 0;
}


void FRAMEWORK_USART_TransmitterEnable_port_0(void) {
    TXSTAbits.TXEN = 1;
}


void FRAMEWORK_USART_TransmitterEnable_port_1(void) {
    // Do nothing
}


void FRAMEWORK_USART_TransmitterEnable_port_5(void) {
    // Do nothing
}


void FRAMEWORK_USART_TransmitterDisable_port_0(void) {
    TXSTAbits.TXEN = 0;
}


void FRAMEWORK_USART_TransmitterDisable_port_1(void) {
    // Do nothing
}


void FRAMEWORK_USART_TransmitterDisable_port_5(void) {
    // Do nothing
}


bool FRAMEWORK_USART_WriteIsBusy_port_0( void ) {
    //return SERCOM1_USART_WriteIsBusy();
    return false;
}

bool FRAMEWORK_USART_WriteIsBusy_port_1( void ) {
    return false;
}


bool FRAMEWORK_USART_WriteIsBusy_port_5( void ) {
    return false;
}


bool FRAMEWORK_USART_Write_port_0( void *buffer, const size_t size ) {
    uint16_t u16_txData;
    
    uint8_t  * buffer_8;
    uint16_t * buffer_16;
    
    uint8_t u8_index;
    uint8_t u8_tx_data;
    
    
    buffer_8 = buffer;
    buffer_16 = buffer;                

    for (u8_index = 0; u8_index < size; u8_index++) {
        switch (eFraming_port_0) {
            case eight_bit_odd_parity:
            case eight_bit_even_parity:
                TXSTAbits.TX9D = calculateparity(buffer_8[u8_index]);
                u8_tx_data = buffer_8[u8_index];
                break;
            case eight_bit_no_parity:
                TXSTAbits.TX9D = 0;
                u8_tx_data = buffer_8[u8_index];
                break;
            case nine_bit:
                TXSTAbits.TX9D = (((buffer_16[u8_index]) & 0x100) >> 8);
                u8_tx_data = (uint8_t)(buffer_16[u8_index] & 0xFF);
                break;
        }
        TXREG1 = u8_tx_data;                     // load bits 1-8 and transmit
        while (TXSTAbits.TRMT == 0) {               // Wait until TSR is empty
            asm("clrwdt");
        }
    }

    return false;
}


bool FRAMEWORK_USART_Write_port_1( void *buffer, const size_t size ) {
    return false;
}


bool FRAMEWORK_USART_Write_port_5( void *buffer, const size_t size ) {
    return false;
}


void FRAMEWORK_USART_ReceiverEnable_port_0( void ) {
    RCSTAbits.CREN = 1;
}

void FRAMEWORK_USART_ReceiverEnable_port_1( void ) {
    // Do nothing
}

void FRAMEWORK_USART_ReceiverEnable_port_5( void ) {
    // Do nothing
}

void FRAMEWORK_USART_ReceiverDisable_port_0( void ) {
    RCSTAbits.CREN = 0;
}

void FRAMEWORK_USART_ReceiverDisable_port_1( void ) {
    // Do nothing
}

void FRAMEWORK_USART_ReceiverDisable_port_5( void ) {
    // Do nothing
}


bool FRAMEWORK_USART_ReadAbort_port_0(void) {
    uint8_t u8_Waste;
    
    u8_Waste = RCREG;
    RCSTAbits.CREN = 0;
    RCSTAbits.CREN = 1;
    return true;
}

bool FRAMEWORK_USART_ReadAbort_port_1(void) {
    return false;
}

bool FRAMEWORK_USART_ReadAbort_port_5(void) {
    return false;
}


bool FRAMEWORK_USART_Read_port_0( void *buffer, const size_t size ) {
    u16_rx_error = RX_NO_ERROR;
    return true;
}

bool FRAMEWORK_USART_Read_port_1( void *buffer, const size_t size ) {
    return false;
}

bool FRAMEWORK_USART_Read_port_5( void *buffer, const size_t size ) {
    return false;
}


void FRAMEWORK_USART_RegisterReadCallback_port_0 (void * callback_func, uintptr_t context) {
    context_value = context;
}

void FRAMEWORK_USART_RegisterReadCallback_port_1 (void * callback_func, uintptr_t context) {
    // Do nothing
}

void FRAMEWORK_USART_RegisterReadCallback_port_5 (void * callback_func, uintptr_t context) {
    // Do nothing
}


uint32_t FRAMEWORK_get_rx_timeout_port_0(void) {
    return 5000;
}

uint32_t FRAMEWORK_get_rx_timeout_port_1(void) {
    return 0;
}

uint32_t FRAMEWORK_get_rx_timeout_port_5(void) {
    return 0;
}

uint8_t FRAMEWORK_read_RJ45(uint8_t u8_pin_index) {
    return 0;
}


_Bool FRAMEWORK_get_mac(uint8_t * dest) {
    return false;
}