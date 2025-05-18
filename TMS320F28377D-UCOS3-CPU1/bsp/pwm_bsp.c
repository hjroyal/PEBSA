/**
 * @file pwm_bsp.c
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

#include "pwm_bsp.h"

u16 g_os1ms_cnts = 0;


//函数声明



void initEpwmBSP(void) {
  EALLOW;
  ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = TB_DIV1;
  // Setup TBCLK
  EPwm1Regs.TBPRD = 6250;   // Set period to 62.5 us
  EPwm1Regs.TBCTR = 0x0000; // Clear counter

  // Setup counter mode
  EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
  EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
  EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
  EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;
  EPwm1Regs.TBPHS.bit.TBPHS = 0; // Phase is 0
  EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
  EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE; // Disable phase    loading

  // Setup compare shadowing
  EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
  EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
  EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD; // Load on zero and period
  EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
  EPwm1Regs.CMPA.bit.CMPA = 3000; // Set compare A value
  EPwm1Regs.CMPB.bit.CMPB = 3000; // Set compare A value

  // Set actions
  EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR; // Clear PWM1A on event A, up count
  EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;   // Set  PWM1A on event A, down count
  EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR; // Clear PWM1A on event A, up count
  EPwm1Regs.AQCTLB.bit.CBD = AQ_SET;   // Set  PWM1A on event A, down count

  EPwm1Regs.AQSFRC.bit.RLDCSF = AQ_ZERO; // Force load on at zero
  EPwm1Regs.AQCSFRC.bit.CSFA = AQ_CLEAR; // AQ out A set to 0
  EPwm1Regs.AQCSFRC.bit.CSFB = AQ_SET;   // AQ out B set to 0

  // DB Set
  EPwm1Regs.DBCTL.bit.IN_MODE = DBA_RED_DBB_FED; // PWMA & PWMB
  EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;      // active high
  EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // PWM A&B All output 0
  EPwm1Regs.DBRED.bit.DBRED = 400;            //  1us/(1/200M) = 200
  EPwm1Regs.DBFED.bit.DBFED = 400;            //  1us/(1/200M) = 200

  // TZ Set
  EPwm1Regs.TZSEL.bit.CBC1 =
      TZ_ENABLE; // enables TZ1 as a cycle by cycle event source
  EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO;         // Force Low
  EPwm1Regs.TZCTL.bit.TZB = TZ_NO_CHANGE;        // Force Low
  EPwm1Regs.TZCLR.bit.CBCPULSE = TZ_CLR_PRDZERO; // CBC clear at zero or period
  EPwm1Regs.TZCLR.bit.CBC = 1;                   // clear CBC
  EPwm1Regs.TZCBCCLR.bit.CBC1 = 1;               // clear CBC1

  // ADC SOC
  EPwm1Regs.ETSEL.bit.SOCAEN = 1; // Enable SOC on A group
  EPwm1Regs.ETSEL.bit.SOCASEL =
      ET_CTR_PRDZERO; // ET_CTR_ZERO;//ET_CTR_PRD;// Select SOC on 1 for 0; 2
                      // for period
  EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST; // Generate pulse on 1st event
  EPwm1Regs.ETCLR.bit.SOCA = 1;

  // INT Set
  EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD; // Select INT on Period event
  EPwm1Regs.ETSEL.bit.INTEN = 1;
  EPwm1Regs.ETPS.bit.INTPRD = ET_1ST; // Generate INT on 1nd event
  EPwm1Regs.ETCLR.bit.INT = 1;

  CpuSysRegs.PCLKCR2.all = 0x0FFF;	// Enable Pwm1~12 clock

  EDIS;
}


void osTimePWMTrigger(void){
    //确保系统可以正确响应中断并继续运行
    EPwm1Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // generate 1ms ostimetick
    g_os1ms_cnts++;
    if (g_os1ms_cnts >= 16) {
        g_os1ms_cnts = 0;
        OSIntEnter();
        OSTimeTick();
        OSIntExit();
    }
}

