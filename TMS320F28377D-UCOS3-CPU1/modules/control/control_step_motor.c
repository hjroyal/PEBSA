/**
 * @file control_step_motor.c
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

#include "control_step_motor.h"

/*
*********************************************************************************************************
*                                              FUNCTIONS DECLARATIONS
*
*********************************************************************************************************
*/
void parmInitStepMotorCtrl(STEP_MOTOR_CTRL *dp);
void initStepMotorCtrl(STEP_MOTOR_CTRL *dp);
void runStepMotorCtrl(STEP_MOTOR_CTRL *dp);

static void InitStepMotorIO(void);

/*
*********************************************************************************************************
*                                              FUNCTIONS IMPLEMENTATIONS
*
*********************************************************************************************************
*/

/**
 * @brief  步进机IO初始化
 * @author hjroyal
 * @date 2025-04-12
 */
static void InitStepMotorIO(void) {
    StepA_ON;
    DELAY_US(10);
    StepB_ON;
    DELAY_US(10);
    StepC_ON;
    DELAY_US(10);
    StepD_ON;
    DELAY_US(10);
}

/**
 * @brief 初始化模块参数
 * @param[in,out] dp             : 模块结构体指针
 * @author hjroyal
 * @date 2025-04-12
 */
void parmInitStepMotorCtrl(STEP_MOTOR_CTRL *dp) {
    dp->parm_poweron_seq = 1;  /// 步进机四相通电分配顺序
    dp->parm_rotation_dir = 1; // 步进机转动方向标志direct_st，1正向，0反向
    dp->parm_en = 0;      // 步进机控制使能标志ctrl_en，1使能，0禁止
}

/**
 * @brief 初始化模块相关操作
 * @param[in,out] dp             : 模块结构体指针
 * @author hjroyal
 * @date 2025-04-12
 */
void initStepMotorCtrl(STEP_MOTOR_CTRL *dp) {
}

/**
 * @brief 模块运行函数
 * @param[in,out] dp             :
 * @author hjroyal
 * @date 2025-04-12
 */
void runStepMotorCtrl(STEP_MOTOR_CTRL *dp) {
    if (dp->parm_en) {
        InitStepMotorIO();
        switch (dp->parm_poweron_seq) {
        case 1:
            StepA_OFF;
            StepB_ON;
            StepC_ON;
            StepD_ON;
            break; // 分配顺序1
        case 2:
            StepA_OFF;
            StepB_OFF;
            StepC_ON;
            StepD_ON;
            break; // 分配顺序2
        case 3:
            StepA_ON;
            StepB_OFF;
            StepC_ON;
            StepD_ON;
            break; // 分配顺序3
        case 4:
            StepA_ON;
            StepB_OFF;
            StepC_OFF;
            StepD_ON;
            break; // 分配顺序4
        case 5:
            StepA_ON;
            StepB_ON;
            StepC_OFF;
            StepD_ON;
            break; // 分配顺序5
        case 6:
            StepA_ON;
            StepB_ON;
            StepC_OFF;
            StepD_OFF;
            break; // 分配顺序6
        case 7:
            StepA_ON;
            StepB_ON;
            StepC_ON;
            StepD_OFF;
            break; // 分配顺序7
        case 8:
            StepA_OFF;
            StepB_ON;
            StepC_ON;
            StepD_OFF;
            break; // 分配顺序8
        }

        switch (dp->parm_rotation_dir) // 判断步进机转动方向
        {
        case 1:
            if (dp->parm_poweron_seq == 8) // 正向
                dp->parm_poweron_seq = 1;
            else
                dp->parm_poweron_seq++;
            break;
        case 0:
            if (dp->parm_poweron_seq == 1) // 反向
                dp->parm_poweron_seq = 8;
            else
                dp->parm_poweron_seq--;
            break;
        }
    }
}