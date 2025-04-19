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

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HOST_SIM
/// Simulation platform head file
#else
/// Real platform head file
#endif
#include "include.h"
/*
*********************************************************************************************************
*                                              DATA API
*                                        (外部开放的数据接口)
*********************************************************************************************************
*/

/* 卡尔曼滤波数据结构 */
typedef struct {
    float *x;        // 状态向量（n维）
    float *P;        // 误差协方差矩阵（n×n）
    float *Q;        // 过程噪声协方差（n×n）
    float *R;        // 测量噪声协方差（m×m）
    float *K;        // 卡尔曼增益（n×m）
    int state_dim;   // 状态维度（n）
    int measure_dim; // 测量维度（m）
} KalmanFilter;

/*
*********************************************************************************************************
*                                              FUNCTION API
*                                         (外部开放的函数接口)
*********************************************************************************************************
*/

/**
 * @brief 限幅滤波,限制所有数据在[min_val,max_val]之间
 * @param[in,out] signal         :待处理信号数组
 * @param[in] signal_length  :数组长度
 * @param[in] min_val        :最小值
 * @param[in] max_val        :最大值
 * @author hjroyal
 * @date 2025-04-19
 */
void amplitude_limit_filter(float *signal, uint16_t signal_length, float min_val, float max_val);

/**
 * @brief 限幅滤波,对相邻值之间的偏差进行限幅
 * @param[in,out] signal         :待处理信号数组
 * @param[in] signal_length  :数组长度
 * @param[in] min_diff       :最小偏差
 * @param[in] max_diff       :最大偏差
 * @author hjroyal
 * @date 2025-04-19
 */
void amplitude_diff_limit_filter(float *signal, uint16_t signal_length, float min_diff, float max_diff);

/**
 * @brief 滞环滤波
 * @param[in,out] signal         :待处理信号数组
 * @param[in] signal_length  :数组长度
 * @param[in] upper_threshold:上限阈值
 * @param[in] lower_threshold:下限阈值
 * @param[in] hysteresis     :滞环值
 * @author hjroyal
 * @date 2025-04-19
 */
void hysteresis_filter(float *signal, uint16_t signal_length, float upper_threshold, float lower_threshold, float hysteresis);


/**
 * @brief 卡尔曼滤波初始化
 * @param[in,out] kf             : 卡尔曼滤波器结构体指针
 * @param[in,out] n              : 状态维度
 * @param[in,out] m              : 测量维度
 * @param[in,out] init_x         : 初始状态向量
 * @param[in,out] init_P         : 初始协方差矩阵
 * @param[in,out] Q              : 过程噪声协方差矩阵
 * @param[in,out] R              : 测量噪声协方差矩阵
 * @author hjroyal
 * @date 2025-04-19
 */
void kalman_init(KalmanFilter *kf, int n, int m,
                 float *init_x, float *init_P,
                 float *Q, float *R);


/**
 * @brief 卡尔曼滤波预测步骤
 * @param[in,out] kf             : 卡尔曼滤波器结构体指针
 * @param[in,out] F              :  状态转移矩阵
 * @param[in,out] B              :  控制输入矩阵
 * @param[in,out] u              :  控制输入向量
 * @author hjroyal
 * @date 2025-04-19
 */
void kalman_predict(KalmanFilter* kf, float* F, float* B, float* u);



/**
 * @brief  卡尔曼滤波更新步骤
 * @param[in,out] kf             : 卡尔曼滤波器结构体指针
 * @param[in,out] H              : 测量矩阵
 * @param[in,out] z              : 测量值
 * @author hjroyal
 * @date 2025-04-19
 */
void kalman_update(KalmanFilter* kf, float* H, float* z);



#if defined(HOST_SIM)
// Simulation platform function
#endif

#ifdef __cplusplus
}
#endif
#endif /* SOFTWARE_FILTER_H */
