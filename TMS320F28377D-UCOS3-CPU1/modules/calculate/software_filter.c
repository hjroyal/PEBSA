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

#include "include.h"

/**
 * @brief 限幅滤波,对当前值范围进行限幅
 * @param  input    : 输入值
 * @param  MAX  : 限幅值
 * @return float : 限幅后的值
 * @author hjroyal
 * @date 2025-04-08
 */
float maxFilter(float input, float MAX) {
    static float lastValue = 0;
    if ((input > MAX) || (input < -MAX)) {
        return lastValue;
    }
    lastValue = input;
    return input;
}

/**
 * @brief  限幅滤波,对相邻值之间的偏差进行限幅
 * @param  effective_value : 当前有效值
 * @param  new_value        : 新值
 * @param  delat_max        : 限幅值
 * @return filter_type      : 限幅后的值
 * @author hjroyal
 * @date 2025-04-08
 */
int filter(int effective_value, int new_value, int delat_max) {
    if ((new_value - effective_value > delat_max) || (effective_value - new_value > delat_max))
        return effective_value;
    return new_value;
}

/**
 * @brief 中值+滑动均值滤波,设定大小为windows的数组Data,sum计算去掉最大最小值的和值，算剩下数的平均值输出
 * @param  NewValue
 * @param  Data
 * @param  windows
 * @return float
 * @author hjroyal
 * @date 2025-04-08
 */
float GildeAverageValueFilter(float NewValue, float *Data, unsigned short int windows) {
    float max, min;
    float sum;
    unsigned char i;
    Data[0] = NewValue;
    max = Data[0];
    min = Data[0];
    sum = Data[0];
    for (i = windows - 1; i != 0; i--) // 循环四次，从后往前
    {
        if (Data[i] > max)
            max = Data[i];
        else if (Data[i] < min)
            min = Data[i];
        sum += Data[i];
        Data[i] = Data[i - 1]; // 数据右移
    }
    i = windows - 2;
    sum = sum - max - min;
    sum = sum / i;
    return (sum);
}

//---------------------Kalman Filter---------------------------------------------//
/**
 *卡尔曼滤波器
 *@param KFP *kfp 卡尔曼结构体参数
 *   float input 需要滤波的参数的测量值（即传感器的采集值）
 *@return 滤波后的参数（最优值）
 */
float kalmanFilter(KFP *kfp, float input) {
    // 预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
    kfp->Now_P = kfp->LastP + kfp->Q;

    // 卡尔曼增益方程：卡尔曼增益 = k1-1时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
    // 更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
    kfp->out = kfp->out + kfp->Kg * (input - kfp->out); // 因为这一次的预测值就是上一次的输出值
    // 更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
    kfp->LastP = (1 - kfp->Kg) * kfp->Now_P;

    return kfp->out;
}

/***调用卡尔曼滤波器 实践*/
// 以高度为例 定义卡尔曼结构体并初始化参数
// KFP KFP_height = {0.02, 0, 0, 0, 0.001, 0.543};
// int height;
// int kalman_height = 0;
// kalman_height = kalmanFilter(&KFP_height, (float)height);

//---------------------Kalman Filter---------------------------------------------//

bool average_filter_init(average_filter_t *af, float *pBuf, uint16_t size) {
    if (af == NULL || pBuf == NULL || size == 0) {
        return false;
    }
    af->pBuf = pBuf;
    af->size = size;
    average_filter_reset(af);
    return true;
}

float average_filter_update(average_filter_t *af, float value) {
    // Update the sum by subtracting the oldest value and adding the new value
    af->sum += value - af->pBuf[af->index];

    // Update the buffer with the new value
    af->pBuf[af->index] = value;

    // Update the index
    af->index = (af->index + 1) % af->size;

    // Update the count
    if (af->count < af->size) {
        af->count++;
    }

    // Calculate the average
    af->average = af->sum / af->count;

    return af->average;
}

void average_filter_reset(average_filter_t *af) {
    af->count = 0;
    af->index = 0;
    af->sum = 0;
    for (uint16_t i = 0; i < af->size; i++) {
        af->pBuf[i] = 0;
    }
    af->average = 0;
}

/////notes/////
//**  https://blog.csdn.net/qq_46280347/article/details/120743250
//