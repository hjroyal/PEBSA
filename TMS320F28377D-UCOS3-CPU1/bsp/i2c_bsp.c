/**
 * @file i2c_bsp.c
 * @brief
 * @author hjroyal
 * @version 1.0
 * @date 2024-08-30
 *
 * @copyright Copyright (c) 2024  XXXX
 *
 * @par Modification log:
 * <table>
 * <tr><th>Date           <th>Version   <th>Author      <th>Description
 * <tr><td>2024-08-30     <td>1.0       <td>hjroyal     <td>
 * </table>
 */
#include "i2c_bsp.h"

// Prototype statements
void I2CA_Init(void);
void initI2cRTC(void);
Uint16  I2C_xrdy(void);
Uint16  I2C_rrdy(void);

// Global variables
u16 rtc_time[] = {0, 0, 0, 0, 0, 0, 0};
u16 rtc_init_time[] = {0x00, 0x50, 0x34, 0x17, 0x11, 0x05, 0x25}; // Uint16   rtc_init_time[]={ 0x00,0x22,0x23,0x01,0x09,0x01,0x00};
u16 rtc_sec, rtc_min, rtc_hour, rtc_day, rtc_month, rtc_year;
Uint16 WriteData(Uint16 *Wdata, Uint16 RomAddress, Uint16 number);
Uint16 ReadData(Uint16 *RamAddr, Uint16 RomAddress, Uint16 number);

void initI2cRTC(void) {
    I2CA_Init();
    WriteData(rtc_init_time, 0x00, 7);
    DELAY_US(10000);
    ReadData(rtc_time, 0x00, 7);
}

void getRtcTime(void) {
    ReadData(rtc_time, 0x0, 7);
    rtc_sec = (rtc_time[0] & 15) + ((rtc_time[0] >> 4) & 7) * 10;
    rtc_min = (rtc_time[1] & 15) + ((rtc_time[1] >> 4) & 7) * 10;
    rtc_hour = (rtc_time[2] & 15) + ((rtc_time[2] >> 4) & 3) * 10;
    rtc_day = (rtc_time[4] & 15) + ((rtc_time[4] >> 4) & 3) * 10;
    rtc_month = (rtc_time[5] & 15) + ((rtc_time[5] >> 4) & 1) * 10;
    rtc_year = (rtc_time[6] & 15) + ((rtc_time[6] >> 4) & 15) * 10;
}


Uint16  I2C_xrdy(void)
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.XRDY;
    return t;
}

Uint16  I2C_rrdy(void)
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.RRDY;
    return t;
}

Uint16 WriteData(Uint16 *Wdata, Uint16 RomAddress, Uint16 number) {
    Uint16 i;
    if (I2caRegs.I2CSTR.bit.BB == 1) {
        return I2C_BUS_BUSY_ERROR;
    }
    while (!I2C_xrdy());
    I2caRegs.I2CSAR.all = 0x68;
    I2caRegs.I2CCNT = number + 1;
    I2caRegs.I2CDXR.all = RomAddress;
    I2caRegs.I2CMDR.all = 0x6E20;
    for (i = 0; i < number; i++) {
        while (!I2C_xrdy());
        I2caRegs.I2CDXR.all = *Wdata;
        Wdata++;
        if (I2caRegs.I2CSTR.bit.NACK == 1)
            return I2C_BUS_BUSY_ERROR;
    }
    return I2C_SUCCESS;
}

Uint16 ReadData(Uint16 *RamAddr, Uint16 RomAddress, Uint16 number) {
    Uint16 i, Temp;

    if (I2caRegs.I2CSTR.bit.BB == 1) {
        return I2C_BUS_BUSY_ERROR;
    }
    while (!I2C_xrdy());
    I2caRegs.I2CSAR.all = 0x68;
    I2caRegs.I2CCNT = 1;
    I2caRegs.I2CDXR.all = RomAddress;
    I2caRegs.I2CMDR.all = 0x6620;
    if (I2caRegs.I2CSTR.bit.NACK == 1)
        return I2C_BUS_BUSY_ERROR;
    DELAY_US(50);
    while (!I2C_xrdy());
    I2caRegs.I2CSAR.all = 0x68;
    I2caRegs.I2CCNT = number;
    I2caRegs.I2CMDR.all = 0x6C20;
    if (I2caRegs.I2CSTR.bit.NACK == 1)
        return I2C_BUS_BUSY_ERROR;
    for (i = 0; i < number; i++) {
        while (!I2C_rrdy());
        Temp = I2caRegs.I2CDRR.all;
        if (I2caRegs.I2CSTR.bit.NACK == 1)
            return I2C_BUS_BUSY_ERROR;
        *RamAddr = Temp;
        RamAddr++;
    }
    return I2C_SUCCESS;
}

void I2CA_Init(void) {
    // Initialize I2C

    // For this example, only init the pins for the SCI-A port.
    // These functions are found in the F2837xS_Gpio.c file.
    //   GPIO_SetupPinMux(32, GPIO_MUX_CPU1, 1);
    //   GPIO_SetupPinMux(33, GPIO_MUX_CPU1, 1);
    EALLOW;
    /* Enable internal pull-up for the selected I2C pins */
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;

    /* Set qualification for the selected I2C pins */
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;

    /* Configure which of the possible GPIO pins will be I2C_A pins using GPIO regs*/

    GpioCtrlRegs.GPBGMUX1.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1; // SDAA
    GpioCtrlRegs.GPBGMUX1.bit.GPIO33 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1; // SCLA

    EDIS;

    //-------------------------------------------------------------
    I2caRegs.I2CMDR.bit.IRS = 0;
    //   I2caRegs.I2CSAR.all = 0x0050;      // Slave address - EEPROM control code

    I2caRegs.I2CPSC.all = 9;    // Prescaler - need 7-12 Mhz on module clk
    I2caRegs.I2CCLKL = 90;      // NOTE: must be non zero
    I2caRegs.I2CCLKH = 90;      // NOTE: must be non zero
    I2caRegs.I2CIER.all = 0x00; // Enable SCD & ARDY __interrupts

    I2caRegs.I2CIER.all = 0x24;
    I2caRegs.I2CFFTX.all = 0x0000;
    I2caRegs.I2CMDR.bit.IRS = 1;
    //   I2caRegs.I2CMDR.all = 0x0020;  // Take I2C out of reset
    // Stop I2C when suspended

    // I2caRegs.I2CFFTX.all = 0x6000; // Enable FIFO mode and TXFIFO
    // I2caRegs.I2CFFRX.all = 0x2040; // Enable RXFIFO, clear RXFFINT,

    return;
}
