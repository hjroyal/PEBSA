/**
 * @file software_filter.h
 * @brief 
 * @author hjroyal
 * @version 1.0.0
 * @date 2025-04-08
 * 
 * @copyright Copyright (c) 2025  XXXX
 * 
 * @par modification log:
 * <table>
 * <tr><th>Date           <th>Version   <th>Author      <th>Description
 * <tr><td>2025-04-08     <td>1.0       <td>hjroyal     <td>First creation
 * </table>
 */

#ifndef SOFTWARE_FILTER_H
#define SOFTWARE_FILTER_H



/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/
typedef struct
{
    float LastP; // 上次估算协方差 初始化值为0.02		--e(ESTk-1)  上次协方差
    float Now_P; // 当前估算协方差 初始化值为0		--预测e(ESTk)	当前估算协方差
    float out;   // 卡尔曼滤波器输出 初始化值为0
    float Kg;    // 卡尔曼增益 初始化值为0				--Kk
    float Q;     // 过程噪声协方差 初始化值为0.001
    float R;     // 观测噪声协方差 初始化值为0.543		--e(MEAk)  测量误差
} KFP;          // Kalman Filter parameter


/*
*********************************************************************************************************
*                                              FUNCTION PROTOTYPES
*                                        (Golbal function prototypes.   )
*********************************************************************************************************
*/
float maxFilter(float input, float MAX);
int filter(int effective_value, int new_value, int delat_max);
float GildeAverageValueFilter(float NewValue, float *Data, unsigned short int windows);
float kalmanFilter(KFP *kfp, float input);


#endif /* SOFTWARE_FILTER_H */


