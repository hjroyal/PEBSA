/**
 * @file sense_adc.c
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

#include "sense_adc.h"

void parmInitADCSense(ADC_DATA *dp);
void initADCSense(ADC_DATA *dp);
void runADCSense(ADC_DATA *dp);

void parmInitADCSense(ADC_DATA *dp) {
}

void initADCSense(ADC_DATA *dp) {
    dp->out_dsp_temp = 0;
    dp->out_adc_a1 = 0;
    dp->out_adc_a2 = 0;
}

void runADCSense(ADC_DATA *dp) {
    u32 adcAResult0;
    u32 adcAResult1;
    u32 adcAResult2;

    adcAResult0 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
    dp->out_dsp_temp = GetTemperatureC(adcAResult0);

    dp->out_adc_a1 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);
    adcAResult2 = ((ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER2) + ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER3) + ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER4) + ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER5) + ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER6) + ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER7) + ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER8) + ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER9)) >> 3);
    dp->out_adc_a2 = GetTemperatureC(adcAResult2);
}
