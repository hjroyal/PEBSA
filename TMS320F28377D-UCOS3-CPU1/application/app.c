/**
 * @file app.c
 * @brief
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

/*
*************************************************************************
*                            USER STACK SIZE
*************************************************************************
*/
#define APP_TASK_CONTROL_STK_SIZE 160u
#define APP_TASK_PROTECTION_STK_SIZE 160u
#define APP_TASK_COM_STK_SIZE 160u
#define APP_TASK_TICK_STK_SIZE 256u

#pragma DATA_SECTION(AppTaskControlTCB, "TASK_STK_RAM");
#pragma DATA_SECTION(AppTaskControlStk, "TASK_STK_RAM");
#pragma DATA_SECTION(AppTaskProtectionTCB, "TASK_STK_RAM");
#pragma DATA_SECTION(AppTaskProtectionStk, "TASK_STK_RAM");
#pragma DATA_SECTION(AppTaskComTCB, "TASK_STK_RAM");
#pragma DATA_SECTION(AppTaskComStk, "TASK_STK_RAM");
#pragma DATA_SECTION(AppTaskTickTCB, "TASK_STK_RAM");
#pragma DATA_SECTION(AppTaskTickStk, "TASK_STK_RAM");
/*
**************************************************************
*                               TCB
**************************************************************
*/
#define APP_TASK_TICK_PRIO (1)
#define APP_TASK_CTRL_PRIO (3)
#define APP_TASK_PROT_PRIO (5)
#define APP_TASK_COM_PRIO (7)

static OS_TCB AppTaskControlTCB;
static OS_TCB AppTaskProtectionTCB;
static OS_TCB AppTaskComTCB;
static OS_TCB AppTaskTickTCB;
/*
*************************************************************************
*                            STACKS
*************************************************************************
*/

static CPU_STK AppTaskControlStk[APP_TASK_CONTROL_STK_SIZE];
static CPU_STK AppTaskProtectionStk[APP_TASK_PROTECTION_STK_SIZE];
static CPU_STK AppTaskComStk[APP_TASK_COM_STK_SIZE];
static CPU_STK AppTaskTickStk[APP_TASK_TICK_STK_SIZE];
/*
************************************************************************
*                          FUNCTION PROTOTYPES
*************************************************************************
*/

static void AppTaskControl(void *p_arg);
static void AppTaskProtection(void *p_arg);
static void AppTaskCom(void *p_arg);
static void AppTaskTick(void *p_arg);

static void ModuleDataLink(void);
static void ModuleParaInit(void);

/*
************************************************************************
*                          EVENT FLAG GROUP
*************************************************************************
*/
OS_FLAG_GRP OSEventCtrl;
OS_FLAG_GRP OSEventProt;
OS_FLAG_GRP OSEventCom;
/*
************************************************************************
*                          GLOBAL VARIABLES
*************************************************************************
*/
SEG_DSIPLAY gSegDisplay;
SEG_DSIPLAY *p_gSegDisplay = &gSegDisplay;

ADC_DATA gAdcData;
ADC_DATA *p_gAdcData = &gAdcData;

STEP_MOTOR_CTRL gStepMotorCtrl;
STEP_MOTOR_CTRL *p_gStepMotorCtrl = &gStepMotorCtrl;

// test tasks
u32 flag = 0;
u32 taskCtrlCnts = 0;
u32 taskProtCnts = 0;
u32 taskComCnts = 0;
u32 taskTickCnts = 0;

/*
************************************************************************
*                                 MAIN
*************************************************************************
*/
void main(void) {
    OS_ERR os_err;

    BSPInit();

    ModuleParaInit();
    ModuleDataLink();

    OSInit(&os_err); /* Init uC/OS-III.*/

    /*  Control  */
    OSTaskCreate((OS_TCB *)&AppTaskControlTCB,
                 (CPU_CHAR *)"App Task Control",
                 (OS_TASK_PTR)AppTaskControl,
                 (void *)0,
                 (OS_PRIO)APP_TASK_CTRL_PRIO,
                 (CPU_STK *)&AppTaskControlStk[0],
                 (CPU_STK_SIZE)APP_TASK_CONTROL_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_CONTROL_STK_SIZE,
                 (OS_MSG_QTY)0u,
                 (OS_TICK)0u,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&os_err);
    /*  Protection  */
    OSTaskCreate((OS_TCB *)&AppTaskProtectionTCB,
                 (CPU_CHAR *)"App Task Protection",
                 (OS_TASK_PTR)AppTaskProtection,
                 (void *)0,
                 (OS_PRIO)APP_TASK_PROT_PRIO,
                 (CPU_STK *)&AppTaskProtectionStk[0],
                 (CPU_STK_SIZE)APP_TASK_PROTECTION_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_PROTECTION_STK_SIZE,
                 (OS_MSG_QTY)0u,
                 (OS_TICK)0u,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&os_err);

    /*  Com */
    OSTaskCreate((OS_TCB *)&AppTaskComTCB,
                 (CPU_CHAR *)"App Task Com",
                 (OS_TASK_PTR)AppTaskCom,
                 (void *)0,
                 (OS_PRIO)APP_TASK_COM_PRIO,
                 (CPU_STK *)&AppTaskComStk[0],
                 (CPU_STK_SIZE)APP_TASK_COM_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_COM_STK_SIZE,
                 (OS_MSG_QTY)0u,
                 (OS_TICK)0u,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&os_err);

    /*  Tick */
    OSTaskCreate((OS_TCB *)&AppTaskTickTCB,
                 (CPU_CHAR *)"App Task Tick",
                 (OS_TASK_PTR)AppTaskTick,
                 (void *)0,
                 (OS_PRIO)APP_TASK_TICK_PRIO,
                 (CPU_STK *)&AppTaskTickStk[0],
                 (CPU_STK_SIZE)APP_TASK_TICK_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_TICK_STK_SIZE,
                 (OS_MSG_QTY)0u,
                 (OS_TICK)0u,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&os_err);

    OSFlagCreate(&OSEventCtrl, "EventCtrl", (OS_FLAGS)0, &os_err);
    OSFlagCreate(&OSEventProt, "EventProt", (OS_FLAGS)0, &os_err);
    OSFlagCreate(&OSEventCom, "EventCom", (OS_FLAGS)0, &os_err);

    OSStart(&os_err); /* Start multitasking(i.e.give control to uC/OS-III).*/
}

/*
************************************************************************
*                                 MAIN INTERRUPT
*************************************************************************
*/
interrupt void epwm1_isr(void) {
    runADCSense(p_gAdcData);

    OSTimePWMTrigger(); // trigger os time tick 1ms
}

/*
************************************************************************
*                                 TASK CONTROL
*************************************************************************
*/

static void AppTaskControl(void *p_arg) {
    OS_ERR os_err;
    CPU_TS ts;
    (void)p_arg;
    u16 luwFlag;

    taskCtrlCnts = 1000;

    while (DEF_TRUE) {
        OSFlagPend(&OSEventCtrl, EventCtrl10ms, (OS_TICK)0,
                   (OS_OPT)OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME, &ts,
                   &os_err);

        luwFlag = OSFlagPendGetFlagsRdy(&os_err);

        if (luwFlag & EventCtrl10ms) {
            taskCtrlCnts++;

            runStepMotorCtrl(p_gStepMotorCtrl);
        }

        OSTimeDly(1, OS_OPT_TIME_DLY, &os_err);
    }
}

/*
************************************************************************
*                                 TASK PROTECTION
*************************************************************************
*/
static void AppTaskProtection(void *p_arg) {
    OS_ERR os_err;
    CPU_TS ts;
    (void)p_arg;
    u16 luwFlag;

    taskProtCnts = 2000;

    while (DEF_TRUE) {
        OSFlagPend(&OSEventProt, EventProt20ms, (OS_TICK)0,
                   (OS_OPT)OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME, &ts,
                   &os_err);

        luwFlag = OSFlagPendGetFlagsRdy(&os_err);
        if (luwFlag & EventProt20ms) {
            taskProtCnts++;

            runSegDisplay(p_gSegDisplay);

            if (taskProtCnts > 50) {
                taskProtCnts = 0;
                BLINK(LED3);
            }
        }

        OSTimeDly(1, OS_OPT_TIME_DLY, &os_err);
    }
}

/*
************************************************************************
*                                 TASK COMMUNICATION
*************************************************************************
*/
static void AppTaskCom(void *p_arg) {
    OS_ERR os_err;
    (void)p_arg;

    unsigned int i = 0;
    // 匹配好结束符配置 NR_SHELL_END_OF_LINE 0
    char test_line[] = "test 1 2 3\n";
    /* 初始化 */
    shell_init();

    /* 初步测试代码 */
    for (i = 0; i < sizeof(test_line) - 1; i++) {
        shell(test_line[i]);
    }

    taskComCnts = 0;

    while (DEF_TRUE) {
        BLINK(LED5);
        BLINK(LED6);

        taskComCnts = (++taskComCnts > 9) ? 0 : taskComCnts;

        OSTimeDly(3000, OS_OPT_TIME_DLY, &os_err);
    }
}

/*
************************************************************************
*                                 TASK TICK
*************************************************************************
*/

static void AppTaskTick(void *p_arg) {
    OS_ERR os_err;
    (void)p_arg;
    CPU_TS ts;

    taskTickCnts = 2000;

    static u16 luw10MsCnt1 = 1;
    static u16 luw20MsCnt1 = 3;
    static u16 luw50MsCnt1 = 5;

    while (DEF_TRUE) {
        taskTickCnts++;

        // 当前任务等待自身任务信号量 //TODO 中断还没配
        //  (void)OSTaskSemPend((OS_TICK  )0,
        //                      (OS_OPT   )OS_OPT_PEND_BLOCKING,
        //                      (CPU_TS  *)&ts,
        //                      (OS_ERR  *)&os_err);   /* Wait for signal from tick
        //                      interrupt */

        if (os_err == OS_ERR_NONE) {
            luw10MsCnt1++;
            if (luw10MsCnt1 > 10) {
                luw10MsCnt1 = 0;
                OSFlagPost(&OSEventCtrl, EventCtrl10ms,
                           (OS_OPT)OS_OPT_POST_FLAG_SET | OS_OPT_POST_NO_SCHED,
                           &os_err);
            }

            luw20MsCnt1++;
            if (luw20MsCnt1 > 20) {
                luw20MsCnt1 = 0;
                OSFlagPost(&OSEventProt, EventProt20ms,
                           (OS_OPT)OS_OPT_POST_FLAG_SET | OS_OPT_POST_NO_SCHED,
                           &os_err);
            }

            luw50MsCnt1++;
            if (luw50MsCnt1 > 50) {
                luw50MsCnt1 = 0;
                OSFlagPost(&OSEventCom, EventCom50ms,
                           (OS_OPT)OS_OPT_POST_FLAG_SET | OS_OPT_POST_NO_SCHED,
                           &os_err);
            }
        }

        OSTimeDly(1, OS_OPT_TIME_DLY, &os_err);
    }
}

/*
************************************************************************
*                                 APP DATA LINK
*************************************************************************
*/
static void ModuleParaInit(void) {
    parmInitSegDisplay(p_gSegDisplay);
    parmInitADCSense(p_gAdcData);
    parmInitStepMotorCtrl(p_gStepMotorCtrl);
}

static void ModuleDataLink(void) {
    p_gSegDisplay->in_dsp_temp   = &p_gAdcData->out_dsp_temp;
    p_gSegDisplay->in_dth11_temp = &p_gAdcData->out_adc_a2;
}
