/**
 * @file module_inc.h
 * @brief 
 * @author hjroyal 
 * @version 1.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2024  XXXX
 * 
 * @par Modification log:
 * <table>
 * <tr><th>Date           <th>Version   <th>Author      <th>Description
 * <tr><td>2025-03-01     <td>1.0       <td>hjroyal     <td>update
 * </table>
 */

#ifndef MODULE_INC_H
#define MODULE_INC_H

#include <stdint.h>

#include "common.h"

//bsp
#include "f28377d_init.h"
#include "adc_bsp.h"
#include "gpio_bsp.h"
#include "pwm_bsp.h"
#include "sci_bsp.h"
#include "i2c_bsp.h"

//control
#include "control_pwm.h"
#include "control_step_motor.h"
#include "pid.h"
#include <PergaMOD.h>

//protection


//communication
#include "com_sci.h"
#include "com_ex_sram.h"
#include "com_ex_flash.h"
#include "com_modbus.h"


//sense
#include "sense_adc.h"


//display
#include "cot_param.h"
#include "cot_param_type.h"
#include "display_seg.h"
#include "control_parameter.h"
#include "ustdlib.h"
#include "app_version.h"

//calculate
#include "filter.h"
#include "filter_fo.h"
#include "filter_so.h"
#include "cbuffer.h"

#endif /* MODULE_INC_H */
