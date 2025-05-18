/**
 * @file sense_adc.h
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

#ifndef SENSE_ADC_H
#define SENSE_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HOST_SIM
/// Simulation platform head file
#else
/// Real platform head file
#endif

#include "include.h"

typedef struct {
    u16 out_dsp_temp;
    u16 out_adc_a1;
    u16 out_adc_a2;
} ADC_DATA;

void parmInitADCSense(ADC_DATA *dp);
void initADCSense(ADC_DATA *dp);
void runADCSense(ADC_DATA *dp);

#if defined(HOST_SIM)
// Simulation platform function
#endif

#ifdef __cplusplus
}
#endif

#endif /* SENSE_ADC_H */


