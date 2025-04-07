/**
 * @file adc_bsp.c
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




void InitADC_BSP(u32 adcBase) {
#if 0
  //
  // Set ADCDLK divider to /4
  //
  ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_4_0);
  //
  // Set resolution and signal mode (see #defines above) and load
  // corresponding trims.
  //
#if (EX_ADC_RESOLUTION == 12)
  ADC_setMode(adcBase, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
#elif (EX_ADC_RESOLUTION == 16)
  ADC_setMode(adcBase, ADC_RESOLUTION_16BIT, ADC_MODE_DIFFERENTIAL);
#endif

  //
  // Set pulse positions to late
  //
  ADC_setInterruptPulseMode(adcBase, ADC_PULSE_END_OF_CONV);

  //
  // Power up the ADCs and then delay for 1 ms
  //
  ADC_enableConverter(adcBase);
  DEVICE_DELAY_US(1000);
#endif

#if 1
  EALLOW;
  // write configurations
  AdcaRegs.ADCCTL2.bit.PRESCALE = 6; // set ADCCLK divider to /4
  AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

  // Set pulse positions to late
  AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

  // power up the ADC
  AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

  // delay for 1ms to allow ADC time to power up
  DEVICE_DELAY_US(1000);
  EDIS;

#endif
}

void InitADCSOC(void) {
#if 0
  uint16_t acqps;

  //
  // Determine minimum acquisition window (in SYSCLKS) based on resolution
  //
  if (EX_ADC_RESOLUTION == 12) {
    acqps = 14; // 75ns
  } else        // resolution is 16-bit
  {
    acqps = 63; // 320ns
  }
  //
  // - NOTE: A longer sampling window will be required if the ADC driving
  //   source is less than ideal (an ideal source would be a high bandwidth
  //   op-amp with a small series resistance). See TI application report
  //   SPRACT6 for guidance on ADC driver design.


  //
  // Select the channels to convert and the configure the ePWM trigger
  //
  ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA,
               ADC_CH_ADCIN0, acqps);
  ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA,
               ADC_CH_ADCIN1, acqps);

  //
  // Setup multiple SOCs to oversample A2
  //
  ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM1_SOCA,
               ADC_CH_ADCIN2, acqps);
  ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM1_SOCA,
               ADC_CH_ADCIN2, acqps);
  ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER4, ADC_TRIGGER_EPWM1_SOCA,
               ADC_CH_ADCIN2, acqps);
  ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER5, ADC_TRIGGER_EPWM1_SOCA,
               ADC_CH_ADCIN2, acqps);
  ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER6, ADC_TRIGGER_EPWM1_SOCA,
               ADC_CH_ADCIN2, acqps);
  ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER7, ADC_TRIGGER_EPWM1_SOCA,
               ADC_CH_ADCIN2, acqps);
  ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER8, ADC_TRIGGER_EPWM1_SOCA,
               ADC_CH_ADCIN2, acqps);
  ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER9, ADC_TRIGGER_EPWM1_SOCA,
               ADC_CH_ADCIN2, acqps);
  //
  // Selec SOC5 on ADCA as the interrupt source.
  //
  ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER9);
  ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
  ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

#endif

#if 1
    EALLOW;
    //Select the channels to convert and end of conversion flag
    //ADCA
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 13;      //SOC0 will convert internal connection A13 //temperature sensor
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = TEMP_SENSOR_ACQPS; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;    // 05h ADCTRIG5 - ePWM1, ADCSOCA

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;      //SOC1 will convert pin A3
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = cACQPS; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5;    // 05h ADCTRIG5 - ePWM1, ADCSOCA

    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2; //SOC2 will convert pin A5
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = cACQPS; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 5; // 05h ADCTRIG5 - ePWM1, ADCSOCA

    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 2; //SOC3 will convert pin A15
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = cACQPS; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = 5; // 05h ADCTRIG5 - ePWM1, ADCSOCA

    AdcaRegs.ADCSOC4CTL.bit.CHSEL = 2; //SOC4 will convert pin A0
    AdcaRegs.ADCSOC4CTL.bit.ACQPS = cACQPS; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL = 5; // 05h ADCTRIG5 - ePWM1, ADCSOCA

    AdcaRegs.ADCSOC5CTL.bit.CHSEL = 2; //SOC5 will convert pin A2
    AdcaRegs.ADCSOC5CTL.bit.ACQPS = cACQPS; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL = 5; // 05h ADCTRIG5 - ePWM1, ADCSOCA

    AdcaRegs.ADCSOC6CTL.bit.CHSEL = 2; //SOC6 will convert pin A4
    AdcaRegs.ADCSOC6CTL.bit.ACQPS = cACQPS; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC6CTL.bit.TRIGSEL = 5; // 05h ADCTRIG5 - ePWM1, ADCSOCA

    AdcaRegs.ADCSOC7CTL.bit.CHSEL = 2; //SOC7 will convert pin A14
    AdcaRegs.ADCSOC7CTL.bit.ACQPS = cACQPS; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC7CTL.bit.TRIGSEL = 5; // 05h ADCTRIG5 - ePWM1, ADCSOCA

    AdcaRegs.ADCSOC8CTL.bit.CHSEL = 2; //XXX SOC8 will convert pin ADCIN15 for DSP temp
    AdcaRegs.ADCSOC8CTL.bit.ACQPS = cACQPS; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC8CTL.bit.TRIGSEL = 5; // 05h ADCTRIG5 - ePWM1, ADCSOCA

    AdcaRegs.ADCSOC9CTL.bit.CHSEL = 2; //XXX SOC8 will convert pin ADCIN15 for DSP temp
    AdcaRegs.ADCSOC9CTL.bit.ACQPS = cACQPS; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC9CTL.bit.TRIGSEL = 5; // 05h ADCTRIG5 - ePWM1, ADCSOCA

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 9; // end of SOC8 will set INT1 flag,
    //only this one int scource
    // 1 int delay 1.57us ; 7 delay 3.41us; 8 delay 4.35us
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1; //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared

    CpuSysRegs.PCLKCR13.bit.ADC_A = 1; // 开启ADCA时钟
    (*Device_cal)();
    (*CalAdcaINL)();
    EDIS;
#endif

}
