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



// 基于循环计数器的线性插值
float linear_interpolate(DataPoint p1, DataPoint p2, int target_cycle) {
    if (p1.cycle == p2.cycle) return p1.value; // 避免除零
    float ratio = (float)(target_cycle - p1.cycle) / (p2.cycle - p1.cycle);
    return p1.value + (p2.value - p1.value) * ratio;
}


// 更新A数据（每N次循环调用）
void update_a(DataManager *manager, float value, int cycle) {
    // pthread_mutex_lock(&manager->mutex);
    CircularBuffer *buf = &manager->a_buffer;
    buf->head = (buf->head + 1) % buf->size;
    buf->buffer[buf->head].value = value;
    buf->buffer[buf->head].cycle = cycle;
    if (buf->count < buf->size) buf->count++;
    // pthread_mutex_unlock(&manager->mutex);
}

// 更新B数据（每次循环调用）
void update_b(DataManager *manager, float value, int cycle) {
    // pthread_mutex_lock(&manager->mutex);
    CircularBuffer *buf = &manager->b_buffer;
    buf->head = (buf->head + 1) % buf->size;
    buf->buffer[buf->head].value = value;
    buf->buffer[buf->head].cycle = cycle;
    if (buf->count < buf->size) buf->count++;
    // pthread_mutex_unlock(&manager->mutex);
}

// 获取对齐后的A、B数据
int get_aligned_data(DataManager *manager, float *a_out, float *b_out) {
    // pthread_mutex_lock(&manager->mutex);
    
    // 获取最新B数据
    if (manager->b_buffer.count == 0) {
        // pthread_mutex_unlock(&manager->mutex);
        return -1; // B数据为空
    }
    DataPoint b_latest = manager->b_buffer.buffer[manager->b_buffer.head];
    
    // 获取最近两次A数据
    if (manager->a_buffer.count < 2) {
        // pthread_mutex_unlock(&manager->mutex);
        return -1; // A数据不足
    }
    DataPoint a_prev = manager->a_buffer.buffer[(manager->a_buffer.head - 1) % manager->a_buffer.size];
    DataPoint a_curr = manager->a_buffer.buffer[manager->a_buffer.head];
    
    // 检查B的cycle是否在A数据范围内
    if (b_latest.cycle < a_prev.cycle || b_latest.cycle > a_curr.cycle) {
        // pthread_mutex_unlock(&manager->mutex);
        return -1; // 数据未对齐
    }
    
    // 计算插值并输出
    *a_out = linear_interpolate(a_prev, a_curr, b_latest.cycle);
    *b_out = b_latest.value;
    
    // pthread_mutex_unlock(&manager->mutex);
    return 0;
}
