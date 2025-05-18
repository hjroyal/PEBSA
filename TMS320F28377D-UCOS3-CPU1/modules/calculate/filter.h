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
typedef struct {
  float x_est; // Estimated state value
  float P_est; // Estimated state covariance
  float Q;     // Process noise covariance
  float R;     // Measurement noise covariance
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
void amplitudeLimitFilter(float *signal, uint16_t signal_length, float min_val, float max_val);

/**
 * @brief 限幅滤波,对相邻值之间的偏差进行限幅
 * @param[in,out] signal         :待处理信号数组
 * @param[in] signal_length  :数组长度
 * @param[in] min_diff       :最小偏差
 * @param[in] max_diff       :最大偏差
 * @author hjroyal
 * @date 2025-04-19
 */
void amplitudeDiffLimitFilter(float *signal, uint16_t signal_length, float min_diff, float max_diff);

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
void hysteresisFilter(float *signal, uint16_t signal_length, float upper_threshold, float lower_threshold, float hysteresis);



void kalmanFilterInit(KalmanFilter *kf);
float kalmanFilter(KalmanFilter *kf, float z);

#if defined(HOST_SIM)
// Simulation platform function
#endif

#ifdef __cplusplus
}
#endif
#endif /* SOFTWARE_FILTER_H */
