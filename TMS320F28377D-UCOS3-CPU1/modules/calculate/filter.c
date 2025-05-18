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

#include "filter.h"

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
void amplitudeLimitFilter(float* signal, uint16_t signal_length, float min_val, float max_val) {
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
void amplitudeDiffLimitFilter(float* signal, uint16_t signal_length, float min_diff, float max_diff) {
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
void hysteresisFilter(float* signal, uint16_t signal_length,float upper_threshold, float lower_threshold, float hysteresis) {
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


void kalmanFilterInit(KalmanFilter *kf) {
  // Initialize Kalman Filter
  kf->x_est = 0;  // Initial estimate of state value
  kf->P_est = 1;  // Initial estimate of state covariance
  kf->Q = 0.0001; // Process noise covariance
  kf->R = 0.01;   // Measurement noise covariance
}

float kalmanFilter(KalmanFilter *kf, float z) {
  // Prediction step
  float x_pred = kf->x_est;
  float P_pred = kf->P_est + kf->Q;

  // Update step
  float K = P_pred / (P_pred + kf->R);
  kf->x_est = x_pred + K * (z - x_pred);
  kf->P_est = (1 - K) * P_pred;

  return kf->x_est;
}
