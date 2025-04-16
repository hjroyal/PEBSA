/**
 * @file control_step_motor.h
 * @brief
 * @author hjroyal
 * @version 1.0.0
 * @date 2025-04-11
 *
 * @copyright Copyright (c) 2025  XXXX
 *
 * @par modification log:
 * <table>
 * <tr><th>Date           <th>Version   <th>Author      <th>Description
 * <tr><td>2025-04-11     <td>1.0       <td>hjroyal     <td>First creation
 * </table>
 */

#ifndef CONTROL_STEP_MOTOR_H
#define CONTROL_STEP_MOTOR_H

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
    u16 parm_poweron_seq;  ///>hj 20250411 电机上电启动顺序
    u16 parm_rotation_dir; ///>hj 20250411 电机旋转方向
    u16 parm_en;      ///>hj 20250411 电机控制使能
} STEP_MOTOR_CTRL;

void parmInitStepMotorCtrl(STEP_MOTOR_CTRL *dp);
void initStepMotorCtrl(STEP_MOTOR_CTRL *dp);
void runStepMotorCtrl(STEP_MOTOR_CTRL *dp);

#if defined(HOST_SIM)
// Simulation platform function
#endif

#ifdef __cplusplus
}
#endif

#endif /* CONTROL_STEP_MOTOR_H */
