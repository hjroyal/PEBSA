/**
 * @file display_seg.h
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

#ifndef DISPLAY_SEG_H
#define DISPLAY_SEG_H


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
    u16 *in_dsp_temp;       ///>hj 20250416 板载DSP温度
    u16 *in_dth11_temp;     ///>hj 20250416 DTH11温度
    u16 *in_rtc_time;      ///>hj 20250416 RTC时间
    u16 parm_sensor_sel;    ///>hj 20250411 温度传感器选择 0:板载DSP 1:DTH11
    u16 parm_en;            ///>hj 20250411 模块使能
} SEG_DSIPLAY;

void parmInitSegDisplay(SEG_DSIPLAY *dp);
void initSegDisplay    (SEG_DSIPLAY *dp);
void runSegDisplay     (SEG_DSIPLAY *dp);   //会降低实时性



#if defined(HOST_SIM)
// Simulation platform function
#endif

#ifdef __cplusplus
}
#endif







#endif //__DISPLAY_SEG__

