/**
 * @file software_filter.c
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

#include "software_filter.h"

/*
*********************************************************************************************************
*                                       Function Declaration
*                                 (函数声明，本文件使用加static修饰)
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       Function Realization
*                                 (函数实现，持续优化，接口设计可扩展)
*********************************************************************************************************
*/

/**
 * @brief 限幅滤波,限制所有数据在[min_val,max_val]之间
 * @param[in,out] signal         :待处理信号数组
 * @param[in,out] signal_length  :数组长度
 * @param[in,out] min_val        :最小值
 * @param[in,out] max_val        :最大值
 * @author hjroyal
 * @date 2025-04-19
 */
void amplitude_limit_filter(float* signal, uint16_t signal_length, float min_val, float max_val) {
    for (uint16_t i = 0; i < signal_length; i++) {
        if (signal[i] < min_val) {
            signal[i] = min_val;
        } else if (signal[i] > max_val) {
            signal[i] = max_val;
        }
    }
}


/**
 * @brief 限幅滤波,对相邻值之间的偏差进行限幅
 * @param[in,out] signal         :待处理信号数组
 * @param[in,out] signal_length  :数组长度
 * @param[in,out] min_diff       :最小偏差
 * @param[in,out] max_diff       :最大偏差
 * @author hjroyal
 * @date 2025-04-19
 */
void amplitude_diff_limit_filter(float* signal, uint16_t signal_length, float min_diff, float max_diff) {
    for (uint16_t i = 1; i < signal_length; i++) {
        float diff = signal[i] - signal[i-1];
        if (diff < min_diff) {
            signal[i] = signal[i-1] + min_diff;
        } else if (diff > max_diff) {
            signal[i] = signal[i-1] + max_diff;
        }
    }
}



/**
 * @brief 滞环滤波
 * @param[in,out] signal         :待处理信号数组
 * @param[in,out] signal_length  :数组长度
 * @param[in,out] upper_threshold:上限阈值
 * @param[in,out] lower_threshold:下限阈值
 * @param[in,out] hysteresis     :滞环值
 * @author hjroyal
 * @date 2025-04-19
 */
void hysteresis_filter(float* signal, uint16_t signal_length,float upper_threshold, float lower_threshold, float hysteresis) {
    float prev_value = signal[0]; // 初始化为信号的第一个值
    float hysteresis_upper = upper_threshold + hysteresis;
    float hysteresis_lower = lower_threshold - hysteresis;

    for ( uint16_t i = 1; i < signal_length; i++) {
        if (signal[i] > hysteresis_upper && prev_value <= upper_threshold) {
            // 信号超过上限阈值，进入滞环上限
            prev_value = upper_threshold;
        } else if (signal[i] < hysteresis_lower && prev_value >= lower_threshold) {
            // 信号低于下限阈值，进入滞环下限
            prev_value = lower_threshold;
        } else if (signal[i] <= hysteresis_upper && signal[i] >= hysteresis_lower) {
            // 信号在滞环内，跟随信号值
            prev_value = signal[i];
        }
        signal[i] = prev_value; // 更新信号值
    }
}

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
void kalman_init(KalmanFilter* kf, int n, int m, 
                float* init_x, float* init_P, 
                float* Q, float* R) {
    kf->state_dim = n;
    kf->measure_dim = m;
    kf->x = (float*)malloc(n * sizeof(float));
    kf->P = (float*)malloc(n * n * sizeof(float));
    kf->Q = (float*)malloc(n * n * sizeof(float));
    kf->R = (float*)malloc(m * m * sizeof(float));
    kf->K = (float*)malloc(n * m * sizeof(float));
    
    // 初始化参数
    memcpy(kf->x, init_x, n * sizeof(float));
    memcpy(kf->P, init_P, n * n * sizeof(float));
    memcpy(kf->Q, Q, n * n * sizeof(float));
    memcpy(kf->R, R, m * m * sizeof(float));
}

/**
 * @brief 卡尔曼滤波预测步骤
 * @param[in,out] kf             : 卡尔曼滤波器结构体指针
 * @param[in,out] F              :  状态转移矩阵
 * @param[in,out] B              :  控制输入矩阵
 * @param[in,out] u              :  控制输入向量
 * @author hjroyal
 * @date 2025-04-19
 */
void kalman_predict(KalmanFilter* kf, float* F, float* B, float* u) {
    // 状态预测: x = F * x + B * u
    matrix_multiply(F, kf->x, kf->x, kf->state_dim, kf->state_dim, 1);
    matrix_multiply_add(B, u, kf->x, kf->state_dim, 1, 1);
    
    // 协方差预测: P = F * P * F^T + Q
    float* FP = (float*)malloc(kf->state_dim * kf->state_dim * sizeof(float));
    matrix_multiply(F, kf->P, FP, kf->state_dim, kf->state_dim, kf->state_dim);
    matrix_transpose(F, kf->state_dim, kf->state_dim);
    matrix_multiply(FP, F, kf->P, kf->state_dim, kf->state_dim, kf->state_dim);
    matrix_add(kf->P, kf->Q, kf->P, kf->state_dim, kf->state_dim);
    
    free(FP);
}


/**
 * @brief  卡尔曼滤波更新步骤
 * @param[in,out] kf             : 卡尔曼滤波器结构体指针
 * @param[in,out] H              : 测量矩阵
 * @param[in,out] z              : 测量值
 * @author hjroyal
 * @date 2025-04-19
 */
void kalman_update(KalmanFilter* kf, float* H, float* z) {
    // 计算卡尔曼增益: K = P * H^T * (H * P * H^T + R)^-1
    float* H_T = (float*)malloc(kf->measure_dim * kf->state_dim * sizeof(float));
    matrix_transpose(H, kf->state_dim, kf->measure_dim, H_T);
    
    float* HP = (float*)malloc(kf->measure_dim * kf->state_dim * sizeof(float));
    matrix_multiply(H, kf->P, HP, kf->measure_dim, kf->state_dim, kf->state_dim);
    
    float* S = (float*)malloc(kf->measure_dim * kf->measure_dim * sizeof(float));
    matrix_multiply(HP, H_T, S, kf->measure_dim, kf->state_dim, kf->measure_dim);
    matrix_add(S, kf->R, S, kf->measure_dim, kf->measure_dim);
    
    matrix_inverse(S, kf->measure_dim);
    matrix_multiply(kf->P, H_T, kf->K, kf->state_dim, kf->state_dim, kf->measure_dim);
    matrix_multiply(kf->K, S, kf->K, kf->state_dim, kf->measure_dim, kf->measure_dim);
    
    // 状态更新: x = x + K * (z - H * x)
    float* Hx = (float*)malloc(kf->measure_dim * sizeof(float));
    matrix_multiply(H, kf->x, Hx, kf->measure_dim, kf->state_dim, 1);
    matrix_subtract(z, Hx, Hx, kf->measure_dim, 1);
    matrix_multiply_add(kf->K, Hx, kf->x, kf->state_dim, kf->measure_dim, 1);
    
    // 协方差更新: P = (I - K * H) * P
    float* KH = (float*)malloc(kf->state_dim * kf->state_dim * sizeof(float));
    matrix_multiply(kf->K, H, KH, kf->state_dim, kf->measure_dim, kf->state_dim);
    matrix_identity_subtract(KH, kf->state_dim);
    matrix_multiply(KH, kf->P, kf->P, kf->state_dim, kf->state_dim, kf->state_dim);
    
    free(H_T); free(HP); free(S); free(Hx); free(KH);
}
