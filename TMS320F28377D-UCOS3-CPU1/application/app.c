/**
 * @file app.c
 * @brief   电力电子基础软件架构示例项目
 *          CCS20 + Clang + Doxygen + Ucos3 + TIDSP28377D
 * @author hj
 * @version 1.0
 * @date 2024-08-31
 *
 * @copyright Copyright (c) 2024  XXXX
 *
 * @par Modification log:
 * <table>
 * <tr><th>Date           <th>Version   <th>Author      <th>Description
 * <tr><td>2024-08-31     <td>1.0       <td>hjroyal     <td>DEMO RUN
 * </table>
 */

#include "include.h"

//-------------------------------------------------//
// Task  macro definition                          //
//-------------------------------------------------//

#define APP_TASK_CONTROL_STK_SIZE    160u
#define APP_TASK_PROTECTION_STK_SIZE 160u
#define APP_TASK_COM_STK_SIZE        160u
#define APP_TASK_TICK_STK_SIZE       256u

#define APP_TASK_TICK_PRIO (1)
#define APP_TASK_CTRL_PRIO (3)
#define APP_TASK_PROT_PRIO (5)
#define APP_TASK_COM_PRIO  (7)

#pragma DATA_SECTION(AppTaskControlTCB, "APP_DATA_SARAM");
#pragma DATA_SECTION(AppTaskControlStk, "APP_DATA_SARAM");
#pragma DATA_SECTION(AppTaskProtectionTCB, "APP_DATA_SARAM");
#pragma DATA_SECTION(AppTaskProtectionStk, "APP_DATA_SARAM");
#pragma DATA_SECTION(AppTaskComTCB, "APP_DATA_SARAM");
#pragma DATA_SECTION(AppTaskComStk, "APP_DATA_SARAM");
#pragma DATA_SECTION(AppTaskTickTCB, "APP_DATA_SARAM");
#pragma DATA_SECTION(AppTaskTickStk, "APP_DATA_SARAM");

//-------------------------------------------------//
// Task  TCB                                       //
//-------------------------------------------------//

static OS_TCB AppTaskControlTCB;
static OS_TCB AppTaskProtectionTCB;
static OS_TCB AppTaskComTCB;
static OS_TCB AppTaskTickTCB;

//-------------------------------------------------//
// Task  stack                                     //
//-------------------------------------------------//
static CPU_STK AppTaskControlStk[APP_TASK_CONTROL_STK_SIZE];
static CPU_STK AppTaskProtectionStk[APP_TASK_PROTECTION_STK_SIZE];
static CPU_STK AppTaskComStk[APP_TASK_COM_STK_SIZE];
static CPU_STK AppTaskTickStk[APP_TASK_TICK_STK_SIZE];

//-------------------------------------------------//
// Task  function                                  //
//-------------------------------------------------//
static void AppTaskControl(void *p_arg);
static void AppTaskProtection(void *p_arg);
static void AppTaskCom(void *p_arg);
static void AppTaskTick(void *p_arg);

static void moduleDataLink(void);
static void moduleParaInit(void);

//-------------------------------------------------//
// Semaphore, Mutex, Flag                          //
//-------------------------------------------------//
OS_FLAG_GRP OSEventCtrl;
OS_FLAG_GRP OSEventProt;
OS_FLAG_GRP OSEventCom;


//-------------------------------------------------//
// Global variables                                //
//-------------------------------------------------//
SEG_DSIPLAY  gSegDisplay;
SEG_DSIPLAY *p_gSegDisplay = &gSegDisplay;

ADC_DATA  gAdcData;
ADC_DATA *p_gAdcData = &gAdcData;

STEP_MOTOR_CTRL  gStepMotorCtrl;
STEP_MOTOR_CTRL *p_gStepMotorCtrl = &gStepMotorCtrl;

APP_VERSION  gVersion;
APP_VERSION *p_gVersion = &gVersion;

// test tasks run flag
u32 flag         = 0;
u32 taskCtrlCnts = 0;
u32 taskProtCnts = 0;
u32 taskComCnts  = 0;
u32 taskTickCnts = 0;

//-------------------------------------------------//
// Main                                            //
//-------------------------------------------------//
/**
 * @brief This is the standard entry point for C code.
 *        It is assumed that your code will call
 *        main() once you have performed all necessary initialization.
 */
void main(void) {
    OS_ERR os_err;

    initF28377d(); ///< main intrrrupt config

    moduleParaInit();
    moduleDataLink();

    OSInit(&os_err); /* Init uC/OS-III.*/

// #if OS_CFG_STAT_TASK_EN > 0u
//     OSStatTaskCPUUsageInit(&os_err);
// #endif

// #ifdef CPU_CFG_INT_DIS_MEAS_EN
//     CPU_IntDisMeasMaxCurReset();
// #endif

    // Mem_Init(); ///< 初始化内存管理组件（堆内存池和内存池表）

    /*  Control  */
    OSTaskCreate((OS_TCB *)&AppTaskControlTCB,                        ///< Task Control Block
                 (CPU_CHAR *)"App Task Control",                      ///< Task Name
                 (OS_TASK_PTR)AppTaskControl,                         ///< Task Function
                 (void *)0,                                           ///< Task Function Input Parameter
                 (OS_PRIO)APP_TASK_CTRL_PRIO,                         ///< Task Priority
                 (CPU_STK *)&AppTaskControlStk[0],                    ///< Task Stack Base Address
                 (CPU_STK_SIZE)APP_TASK_CONTROL_STK_SIZE / 10,        ///< Task Stack Limit
                 (CPU_STK_SIZE)APP_TASK_CONTROL_STK_SIZE,             ///< Task Stack Size
                 (OS_MSG_QTY)0u,                                      ///< Task Message Queue Size
                 (OS_TICK)0u,                                         ///< Task Time Slice
                 (void *)0,                                           ///< Task Initial Data
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), ///< Task Option
                 (OS_ERR *)&os_err);                                  ///< Task Creation Error Code
    /*  Protection  */
    OSTaskCreate((OS_TCB *)&AppTaskProtectionTCB,                     ///< Task Protection Block
                 (CPU_CHAR *)"App Task Protection",                   ///< Task Name
                 (OS_TASK_PTR)AppTaskProtection,                      ///< Task Function
                 (void *)0,                                           ///< Task Function Input Parameter
                 (OS_PRIO)APP_TASK_PROT_PRIO,                         ///< Task Priority
                 (CPU_STK *)&AppTaskProtectionStk[0],                 ///< Task Stack Base Address
                 (CPU_STK_SIZE)APP_TASK_PROTECTION_STK_SIZE / 10,     ///< Task Stack Limit
                 (CPU_STK_SIZE)APP_TASK_PROTECTION_STK_SIZE,          ///< Task Stack Size
                 (OS_MSG_QTY)0u,                                      ///< Task Message Queue Size
                 (OS_TICK)0u,                                         ///< Task Time Slice
                 (void *)0,                                           ///< Task Initial Data
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), ///< Task Option
                 (OS_ERR *)&os_err);                                  ///< Task Creation Error Code

    /*  Com */
    OSTaskCreate((OS_TCB *)&AppTaskComTCB,                            ///< Task Communication Block
                 (CPU_CHAR *)"App Task Com",                          ///< Task Name
                 (OS_TASK_PTR)AppTaskCom,                             ///< Task Function
                 (void *)0,                                           ///< Task Function Input Parameter
                 (OS_PRIO)APP_TASK_COM_PRIO,                          ///< Task Priority
                 (CPU_STK *)&AppTaskComStk[0],                        ///< Task Stack Base Address
                 (CPU_STK_SIZE)APP_TASK_COM_STK_SIZE / 10,            ///< Task Stack Limit
                 (CPU_STK_SIZE)APP_TASK_COM_STK_SIZE,                 ///< Task Stack Size
                 (OS_MSG_QTY)0u,                                      ///< Task Message Queue Size
                 (OS_TICK)0u,                                         ///< Task Time Slice
                 (void *)0,                                           ///< Task Initial Data
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), ///< Task Option
                 (OS_ERR *)&os_err);                                  ///< Task Creation Error Code

    /*  Tick */
    OSTaskCreate((OS_TCB *)&AppTaskTickTCB,                           ///< Task Tick Block
                 (CPU_CHAR *)"App Task Tick",                         ///< Task Name
                 (OS_TASK_PTR)AppTaskTick,                            ///< Task Function
                 (void *)0,                                           ///< Task Function Input Parameter
                 (OS_PRIO)APP_TASK_TICK_PRIO,                         ///< Task Priority
                 (CPU_STK *)&AppTaskTickStk[0],                       ///< Task Stack Base Address
                 (CPU_STK_SIZE)APP_TASK_TICK_STK_SIZE / 10,           ///< Task Stack Limit
                 (CPU_STK_SIZE)APP_TASK_TICK_STK_SIZE,                ///< Task Stack Size
                 (OS_MSG_QTY)0u,                                      ///< Task Message Queue Size
                 (OS_TICK)0u,                                         ///< Task Time Slice
                 (void *)0,                                           ///< Task Initial Data
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), ///< Task Option
                 (OS_ERR *)&os_err);                                  ///< Task Creation Error Code

    OSFlagCreate((OS_FLAG_GRP *)&OSEventCtrl, (CPU_CHAR *)"EventCtrl", (OS_FLAGS)0, (OS_ERR *)&os_err);
    OSFlagCreate((OS_FLAG_GRP *)&OSEventProt, (CPU_CHAR *)"EventProt", (OS_FLAGS)0, (OS_ERR *)&os_err);
    OSFlagCreate((OS_FLAG_GRP *)&OSEventCom, (CPU_CHAR *)"EventCom", (OS_FLAGS)0, (OS_ERR *)&os_err);


    OSStart(&os_err); /** Start multitasking(i.e.give control to uC/OS-III).*/

}

//-------------------------------------------------//
// Main interrupt                                  //
//-------------------------------------------------//
/**
 * @brief 主中断，产生OSTimeTick，ADC采样，PWM触发
 */
interrupt void epwm1_isr(void) {
    runtimeStart();

    runADCSense(p_gAdcData);

    osTimePWMTrigger(); ///< trigger os time tick 1ms

    runtimeStop();
}

//-------------------------------------------------//
// Task functions realization                      //
//-------------------------------------------------//

/**
 * @brief 控制任务
 *        优先级： 3
 * @param[in,out] p_arg          :
 */
static void AppTaskControl(void *p_arg) {
    OS_ERR os_err;
    CPU_TS ts;
    (void)p_arg;
    u16 luwFlag;

    taskCtrlCnts = 1000;

    while (DEF_TRUE) {
        /** 有事件则继续运行，无则挂起并切换 */
        OSFlagPend(&OSEventCtrl,                                                ///< 控制事件标志组
                   EventCtrl10ms,                                               ///< 事件标志
                   (OS_TICK)0,                                                  ///< 挂起时间
                   (OS_OPT)OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME, ///< 选项
                   &ts, &os_err);

        luwFlag = OSFlagPendGetFlagsRdy(&os_err); ///< 获取挂起后的事件标志

        if (luwFlag & EventCtrl10ms) {
            taskCtrlCnts++;
            // sciPrintf("taskComCnts=%d \r\n",taskCtrlCnts);
            runStepMotorCtrl(p_gStepMotorCtrl);
        }

        OSTimeDly(1, OS_OPT_TIME_DLY, &os_err);
    }
}

/**
 * @brief 保护任务
 *        优先级：5
 * @param[in,out] p_arg          :
 */
static void AppTaskProtection(void *p_arg) {
    OS_ERR os_err;
    CPU_TS ts;
    (void)p_arg;
    u16 luwFlag;

    taskProtCnts = 2000;

    while (DEF_TRUE) {
        OSFlagPend(&OSEventProt,                                                ///< 保护事件标志组
                   EventProt20ms,                                               ///< 事件标志
                   (OS_TICK)0,                                                  ///< 挂起时间
                   (OS_OPT)OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME, ///< 选项
                   &ts, &os_err);

        luwFlag = OSFlagPendGetFlagsRdy(&os_err);
        if (luwFlag & EventProt20ms) {
            taskProtCnts++;

            runSegDisplay(p_gSegDisplay);

            if (taskProtCnts > 50) {
                taskProtCnts = 0;
                BLINK(LED3);
                BLINK(LED4);
            }
        }

        OSTimeDly(1, OS_OPT_TIME_DLY, &os_err);
    }
}

/**
 * @brief 通信任务
 *        优先级：7
 * @param[in,out] p_arg          :
 */
static void AppTaskCom(void *p_arg) {
    OS_ERR os_err;
    CPU_TS ts;
    (void)p_arg;
    u16 luwFlag;


    taskComCnts = 0;


    while (DEF_TRUE) {
        OSFlagPend(&OSEventCom,                                                 ///< 通信事件标志组
                   EventCom50ms,                                                ///< 事件标志
                   (OS_TICK)0,                                                  ///< 挂起时间
                   (OS_OPT)OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME, ///< 选项
                   &ts, &os_err);

        luwFlag = OSFlagPendGetFlagsRdy(&os_err);

        if (luwFlag & EventCom50ms) {
            taskComCnts = (++taskComCnts > 40) ? 0 : taskComCnts;
            if (++taskComCnts > 40) {
                taskComCnts = 0;
                BLINK(LED5);
                BLINK(LED6);

                getRtcTime();

            }
        }

        OSTimeDly(1, OS_OPT_TIME_DLY, &os_err);
    }
}

/**
 * @brief 滴答时刻任务，产生时间事件
 *        优先级：7
 * @param[in,out] p_arg          :
 */
static void AppTaskTick(void *p_arg) {
    OS_ERR os_err;
    (void)p_arg;
    // CPU_TS ts;

    char *msg;

    taskTickCnts = 2000;


    static u16 luw10MsCnt1 = 1;
    static u16 luw20MsCnt1 = 3;
    static u16 luw50MsCnt1 = 5;

    while (DEF_TRUE) {
        taskTickCnts++;

        if (os_err == OS_ERR_NONE) {
            luw10MsCnt1++;
            if (luw10MsCnt1 > 10) {
                luw10MsCnt1 = 0;
                OSFlagPost(&OSEventCtrl,                                        ///< 事件标志组
                           EventCtrl10ms,                                       ///< 事件标志
                           (OS_OPT)OS_OPT_POST_FLAG_SET | OS_OPT_POST_NO_SCHED, ///< 选项
                           &os_err);
            }

            luw20MsCnt1++;
            if (luw20MsCnt1 > 20) {
                luw20MsCnt1 = 0;
                OSFlagPost(&OSEventProt,                                        ///< 事件标志组
                           EventProt20ms,                                       ///< 事件标志
                           (OS_OPT)OS_OPT_POST_FLAG_SET | OS_OPT_POST_NO_SCHED, ///< 选项
                           &os_err);
            }

            luw50MsCnt1++;
            if (luw50MsCnt1 > 50) {
                luw50MsCnt1 = 0;
                OSFlagPost(&OSEventCom,                                         ///< 事件标志组
                           EventCom50ms,                                        ///< 事件标志
                           (OS_OPT)OS_OPT_POST_FLAG_SET | OS_OPT_POST_NO_SCHED, ///< 选项
                           &os_err);
            }
        }
        // DHT11_REC_Data(); //接收温度和湿度的数据

        /** 将任务延时一段时间并执行一次任务调度*/
        OSTimeDly(1, OS_OPT_TIME_DLY, &os_err);
    }
}

//-------------------------------------------------//
// Module in/out data link, initialize parameters  //
//-------------------------------------------------//

/**
 * @brief 初始化模块参数
 */
static void moduleParaInit(void) {
    parmInitSegDisplay(p_gSegDisplay);
    parmInitADCSense(p_gAdcData);
    parmInitStepMotorCtrl(p_gStepMotorCtrl);
}

/**
 * @brief 关联模块输入输出
 */
static void moduleDataLink(void) {
    p_gSegDisplay->in_dsp_temp = &p_gAdcData->out_dsp_temp;
}
