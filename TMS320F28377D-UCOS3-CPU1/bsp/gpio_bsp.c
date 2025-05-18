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

#include "gpio_bsp.h"

// dth11数据
unsigned int rec_data[4];

void initGpioBSP(void) {
    InitGpio();
    initSCIAGpio();
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
    GPIO_SetupPinMux(4, GPIO_MUX_CPU2, 0);
    GPIO_SetupPinOptions(4, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(5, GPIO_MUX_CPU2, 0);
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

    EDIS;
}

// 对于28377d来说，是输出
void DH11_GPIO_Init_OUT(void) {
    GPIO_SetupPinMux(50, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(50, GPIO_OUTPUT, GPIO_PUSHPULL);
}

// 对于s28377d来说，是输入
void DH11_GPIO_Init_IN(void) {
    GPIO_SetupPinMux(50, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(50, GPIO_INPUT, GPIO_PUSHPULL);
}

// 主机发送开始信号
void DHT11_Start(void) {
    DH11_GPIO_Init_OUT(); // 输出模式

    DTH11_HIGH; // 先拉高
    DELAY_US(30);

    DTH11_LOW; // 拉低电平至少18us
    DELAY_US(20000);

    DTH11_HIGH; // 拉高电平20~40us
    DELAY_US(30);

    DH11_GPIO_Init_IN(); // 输入模式
}

// 获取一个字节
char DHT11_Rec_Byte(void) {
    unsigned char i = 0;
    unsigned char data;

    for (i = 0; i < 8; i++) // 1个数据就是1个字节byte，1个字节byte有8位bit
    {
        while (Read_Data == 0) {} // 从1bit开始，低电平变高电平，等待低电平结束
        DELAY_US(30);             // 延迟30us是为了区别数据0和数据1，0只有26~28us

        data <<= 1; // 左移

        if (Read_Data == 1) // 如果过了30us还是高电平的话就是数据1
        {
            data |= 1; // 数据+1
        }

        while (Read_Data == 1) {} // 高电平变低电平，等待高电平结束
    }

    return data;
}

// 获取数据

void DHT11_REC_Data(void) {
    unsigned int R_H, R_L, T_H, T_L;
    unsigned char RH, RL, TH, TL, CHECK;

    DHT11_Start(); // 主机发送信号
    DTH11_HIGH;    // 拉高电平

    if (Read_Data == 0) // 判断DHT11是否响应
    {
        while (Read_Data == 0); // 低电平变高电平，等待低电平结束
        while (Read_Data == 1); // 高电平变低电平，等待高电平结束

        R_H = DHT11_Rec_Byte();
        R_L = DHT11_Rec_Byte();
        T_H = DHT11_Rec_Byte();
        T_L = DHT11_Rec_Byte();
        CHECK = DHT11_Rec_Byte(); // 接收5个数据

        DTH11_LOW;    // 当最后一bit数据传送完毕后，DHT11拉低总线 50us
        DELAY_US(55); // 这里延时55us
        DTH11_HIGH;   // 随后总线由上拉电阻拉高进入空闲状态。

        if (R_H + R_L + T_H + T_L == CHECK) // 和检验位对比，判断校验接收到的数据是否正确
        {
            RH = R_H;
            RL = R_L;
            TH = T_H;
            TL = T_L;
        }
    }

    rec_data[0] = RH;
    rec_data[1] = RL;
    rec_data[2] = TH;
    rec_data[3] = TL;
}
