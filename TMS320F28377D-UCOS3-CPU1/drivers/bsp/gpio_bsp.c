/**
 * @file gpio_bsp.c
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

#include "include.h"

void InitGPIO_BSP(void) {
    InitGpio();

    // Set by hardware !!
    EALLOW;

    // epwm1
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;
    // led3~6
    // GPIO_SetupPinMux(0, GPIO_MUX_CPU1, 0);
    // GPIO_SetupPinOptions(0, GPIO_OUTPUT, GPIO_PUSHPULL);
    // GPIO_SetupPinMux(1, GPIO_MUX_CPU1, 0);
    // GPIO_SetupPinOptions(1, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(2, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(2, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(3, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(3, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(4, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(4, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(5, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(5, GPIO_OUTPUT, GPIO_PUSHPULL);

    // step motor
    GPIO_SetupPinMux(16, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(16, GPIO_OUTPUT, GPIO_INVERT);
    GPIO_SetupPinMux(17, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(17, GPIO_OUTPUT, GPIO_INVERT);
    GPIO_SetupPinMux(18, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(18, GPIO_OUTPUT, GPIO_INVERT);
    GPIO_SetupPinMux(19, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(19, GPIO_OUTPUT, GPIO_INVERT);

    // beep
    GPIO_SetupPinMux(30, GPIO_MUX_CPU1, 0);
    // GPIO_SetupPinOptions(30, GPIO_OUTPUT, GPIO_PUSHPULL);

    // relay
    GPIO_SetupPinMux(67, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(67, GPIO_OUTPUT, GPIO_PUSHPULL);

    // seg
    // 位选
    GPIO_SetupPinMux(58, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(58, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(59, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(59, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(60, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(60, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(61, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(61, GPIO_OUTPUT, GPIO_PUSHPULL);
    // 段选
    GPIO_SetupPinMux(77, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(77, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(78, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(78, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(79, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(79, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(80, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(80, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(81, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(81, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(82, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(82, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(83, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(83, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(85, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(85, GPIO_OUTPUT, GPIO_PUSHPULL);

    // SCIA
    GPIO_SetupPinMux(64, GPIO_MUX_CPU1, 1); // SCIRXDA
    GPIO_SetupPinOptions(64, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(65, GPIO_MUX_CPU1, 1); // SCITXDA
    GPIO_SetupPinOptions(65, GPIO_INPUT, GPIO_PUSHPULL);
    // SCIB
    GPIO_SetupPinMux(54, GPIO_MUX_CPU1, 1); // SCITXDB
    GPIO_SetupPinOptions(54, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(55, GPIO_MUX_CPU1, 1); // SCIRXDB
    GPIO_SetupPinOptions(55, GPIO_INPUT, GPIO_PUSHPULL);
    // SCIC
    GPIO_SetupPinMux(56, GPIO_MUX_CPU1, 1); // SCITXDC
    GPIO_SetupPinOptions(56, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(57, GPIO_MUX_CPU1, 1); // SCIRXDC
    GPIO_SetupPinOptions(57, GPIO_INPUT, GPIO_PUSHPULL);

    EDIS;
}