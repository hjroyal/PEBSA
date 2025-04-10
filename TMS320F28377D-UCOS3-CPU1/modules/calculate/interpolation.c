/**
 * @file interpolation.c
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


//线性插值算法实现
 f32 linear_interpolation(f32 x_target, f32 x0, f32 y0, f32 x1, f32 y1) {
    return y0 + (y1 - y0) * (x_target - x0) / (x1 - x0);
}

//初始化缓冲区
DataBuffer create_buffer(int size) {
    DataBuffer buf;
    buf.timestamps = (float*)malloc(size * sizeof(float));
    buf.values = (float*)malloc(size * sizeof(float));
    buf.size = size;
    buf.head = -1;  // 初始为空
    return buf;
}

//数据插入与更新
void push_data(DataBuffer *buf, float timestamp, float value) {
    buf->head = (buf->head + 1) % buf->size;
    buf->timestamps[buf->head] = timestamp;
    buf->values[buf->head] = value;
}

//插值查询
float get_interpolated_value(DataBuffer *buf, float target_time) {
    // 查找最近的相邻点（可优化为二分查找）
    int i = buf->head;
    while (i >= 0 && buf->timestamps[i] > target_time) {
        i = (i - 1 + buf->size) % buf->size;
    }
    if (i < 0) return NAN;  // 数据不足

    int next_i = (i + 1) % buf->size;
    return linear_interpolation(target_time, 
                                buf->timestamps[i], buf->values[i],
                                buf->timestamps[next_i], buf->values[next_i]);
}
