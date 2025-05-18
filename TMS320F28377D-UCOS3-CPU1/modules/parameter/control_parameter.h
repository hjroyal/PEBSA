/**
 * @file control_parameter.h
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

#ifndef CONTROL_PARAMETER_H
#define CONTROL_PARAMETER_H

#include "include.h"
#include "cot_param_type.h"


typedef struct
{
    u16 usValue;
    u8  ucValue;
    u32 uiValue;
    f32 fValue;
    s8 szString_1[12];
    f64 dValue;
    s16 sValue;
    s8 cValue;
    s32 iValue;
    s8 szString_2[10];
}ParamDemo_t;

extern ParamDemo_t g_tTestVal;
extern s8 g_cTest;
extern s8 g_szString[10];


void InitParam(bool isReset);
void ReloadParam(bool isReset);

void ResetParam(void);
void SaveParam(bool isReset);

s32 SingleParamCheckProcess(const void *pCurParam, cotParamResetOpt_e eResetOpt);

cotParamCheckRet_e SingleParamCheck(const void *pCurParam, const void *pCheckValue);

cotParamCheckRet_e SingleParamSelfCheck(const void *pCurParam);

void SingleParamResetResetDefValue(const void *pCurParam);
void SingleParamChange(const void *pCurParam, ...);

void ShowAllParam(void);

typedef struct
{

} CONTROL_PARAMETER;

void test_param(void);

#endif // CONTROL_PARAMETER_H
