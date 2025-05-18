/**
 * @file f28377d_init.C
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

#include "f28377d_init.h"

// 全局变量定义
f32 runtime = 0; // 单位us

// 函数声明
void initF28377d(void);

interrupt void cpuTimer0ISR(void);
interrupt void cpuTimer1ISR(void);
interrupt void cpuTimer2ISR(void);
void runtimeStart(void);
void runtimeStop(void);

// 函数定义
interrupt void cpuTimer0ISR(void) {
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    CpuTimer0.InterruptCount++; // test
}

interrupt void cpuTimer1ISR(void) {
    CpuTimer1.InterruptCount++; // test

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpuTimer2ISR(void) {
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    CpuTimer2.InterruptCount++; // test

    // if (OSRunning == OS_STATE_OS_RUNNING) {
    // OSIntEnter();
    // OSTimeTick();
    // OSIntExit();
    // }
}

void runtimeStart(void) {
    runtime = 0;
    CpuTimer1Regs.TCR.bit.TRB = 1; // 周期计时器重装载
    CpuTimer1Regs.TCR.bit.TSS = 0; // 开始计数
}

void runtimeStop(void) {
    runtime = (f32)(CpuTimer1Regs.PRD.all - CpuTimer1Regs.TIM.all) * 0.005; // 等价于 / 200， 单位us
    CpuTimer1Regs.TCR.bit.TSS = 1;                                          // 停止计数
}

void initF28377d(void) {
    Device_init();

#ifdef _STANDALONE
#ifdef _FLASH
    // Send boot command to allow the CPU2 application to begin execution
    Device_bootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
#else
    // Send boot command to allow the CPU2 application to begin execution
    Device_bootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_RAM);
#endif // _FLASH
#endif // _STANDALONE

// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
#ifdef _FLASH
    InitFlash();
#endif

    Device_initGPIO();

    initGpioBSP(); // IO初始化

    DHT11_Start();

    Interrupt_initModule();

    Interrupt_initVectorTable();

    // EMIF 接口 （高速的并行总线接口） 连接到外部扩展 flash sram
    // InitEmif1(); // 与seg IO 冲突,

    Interrupt_register(INT_TIMER1, &cpuTimer1ISR);
    Interrupt_register(INT_TIMER2, &cpuTimer2ISR);
    Interrupt_register(INT_RTOS, &RTOS_ISR);
    Interrupt_register(INT_EPWM1, &epwm1_isr);

    ConfigCpuTimer(&CpuTimer1, 200, 10000000);
    ConfigCpuTimer(&CpuTimer2, 200, 1000);

    initI2cRTC();
    
    InitTempSensor(3.0);

    initAdcBSP(ADCA_BASE);
    initEpwmBSP();

    initAdcSoc();

    initSCIAFIFO(9600);
    // InitSCIBSP();

    // CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    CPUTimer_enableInterrupt(CPUTIMER1_BASE);
    CPUTimer_enableInterrupt(CPUTIMER2_BASE);

    // Interrupt_enable(INT_TIMER0);
    Interrupt_enable(INT_TIMER1);
    Interrupt_enable(INT_TIMER2);
    Interrupt_enable(INT_RTOS);
    Interrupt_enable(INT_EPWM1);

    // CPUTimer_startTimer(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER1_BASE);
    CPUTimer_startTimer(CPUTIMER2_BASE);

    EINT; // Enable Global interrupt INTM
    ERTM; // Enable Global realtime interrupt DBGM
}
