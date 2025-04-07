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

#include "include.h"


//全局变量定义



//函数声明
void BSPInit(void);

#if 0
interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);
interrupt void cpu_timer2_isr(void);



//函数定义
interrupt void cpu_timer0_isr(void) {
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

  CpuTimer0.InterruptCount++; // test
}

interrupt void cpu_timer1_isr(void) {
  CpuTimer1.InterruptCount++; // test

  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer2_isr(void) {
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

  CpuTimer2.InterruptCount++; // test

  // if (OSRunning == OS_STATE_OS_RUNNING) {
    // OSIntEnter();
    // OSTimeTick();
    // OSIntExit();
  // }
}

#endif


void BSPInit(void) {
  // Step 1. Initialize System Control:-------------------------------------------------------------------
  // PLL, WatchDog, enable Peripheral Clocks
  InitSysCtrl();

  //    #ifdef _STANDALONE
  //    #ifdef _FLASH
  //    // Send boot command to allow the CPU2 application to begin execution
  //    IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
  //    #else
  //    // Send boot command to allow the CPU2 application to begin execution
  //    IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_RAM);
  //    #endif
  //    #endif

  //    // Call Flash Initialization to setup flash waitstates
  //    // This function must reside in RAM
  //    #ifdef _FLASH
  //    InitFlash();
  //    #endif


  // Step 2. Initalize GPIO:-------------------------------------------------------------------------------

  InitGPIO_BSP();


  // Step 3. Clear all interrupts and initialize PIE vector table:-----------------------------------------
  // Disable CPU interrupts
  DINT;
  // Initialize the PIE control registers to their default state.
  // The default state is all PIE interrupts disabled and flags are cleared.
  InitPieCtrl();

  // Disable CPU interrupts and clear all CPU interrupt flags:  
  IER = 0x0000;
  IFR = 0x0000;

  // Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
  // This will populate the entire table, even if the interrupt
  // is not used in this example.  This is useful for debug purposes.
  InitPieVectTable();

  // 外部 flash sram
  // InitEmif1(); // 与seg IO 冲突

  EALLOW;
  // PieVectTable.TIMER0_INT = &cpu_timer0_isr;
  // PieVectTable.TIMER1_INT = &cpu_timer1_isr;
  // PieVectTable.TIMER2_INT = &cpu_timer2_isr;
  PieVectTable.RTOS_INT = &RTOS_ISR; // RTOS
  // PieVectTable.ADCA1_INT = &adca1_isr; // function for ADCA interrupt 1
	PieVectTable.EPWM1_INT = &epwm1_isr;
	// PieVectTable.EPWM2_INT = &epwm2_isr;
	// PieVectTable.EPWM3_INT = &epwm3_isr;
  EDIS;
  // Interrupt_register(INT_ADCA1, &adcA1ISR);

  // Step 4. Initialize all the Device Peripherals: ---------------------------------------------------------
  // InitCpuTimers();
  // ConfigCpuTimer(&CpuTimer2, 200, 1000); // 1ms // 200MHz, INTERRUPT_period = 1e6 us
  // StartCpuTimer2(); // start timer2

  // Initialize the temperature sensor
  // Note: The argument needs to change if using a VREFHI voltage other than 3.0V
  InitTempSensor(3.0);
  
  InitADC_BSP(ADCA_BASE);
  InitEPWM_BSP();
  InitSCI_BSP();

  InitADCSOC();

  // Step 5. User specific code, enable interrupts:-------------------------------------------------------------
  IER |= M_INT1; // Enable group 1 interrupts  ADC
  IER |= M_INT2; // Enable group 1 interrupts  ADC
  IER |= M_INT3; // Enable group 3 interrupts  EPWM

  // IER |= M_INT13; //For Timer1
  // IER |= M_INT14; //For Timer2


  //Interrupt_enable(INT_ADCA1);
  // PieCtrlRegs.PIEIER1.bit.INTx1 = 1; // ADCA1
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1; // Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
	PieCtrlRegs.PIEIER3.bit.INTx2 = 1; // Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
	PieCtrlRegs.PIEIER3.bit.INTx3 = 1;  // Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
  PieCtrlRegs.PIEIER1.bit.INTx7 = 1; // Enable CPU-Timer 0 Interrupt
  // Enable global Interrupts and higher priority real-time debug events:
  EINT;   // Enable Global interrupt INTM
  ERTM;   // Enable Global realtime interrupt DBGM

  //
  // Start ePWM1, enabling SOCA and putting the counter in up-count mode
  //
  // EPWM_enableADCTrigger(EPWM1_BASE, EPWM_SOC_A);
  // EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP);
}



