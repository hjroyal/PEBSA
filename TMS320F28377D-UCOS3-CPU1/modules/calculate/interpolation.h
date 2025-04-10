/**
 * @file interpolation.h
 * @brief Interpolation algorithm to achieve data consistency.
 * @details 基于循环计数的线性插值实现
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

 #ifndef INTERPOLATION_H
 #define INTERPOLATION_H
 
 
 
 /*
 *********************************************************************************************************
 *                                              DATA TYPES
 *                                         (Compiler Specific)
 *********************************************************************************************************
 */
// 数据点结构体
typedef struct {
    int cycle;   // 循环计数器
    float value;
} DataPoint;

// 环形缓冲区
typedef struct {
    DataPoint *buffer;
    int size;
    int head;
    int count;
} CircularBuffer;

// 数据管理器
typedef struct {
    CircularBuffer a_buffer;  // 保存最近两次A数据
    CircularBuffer b_buffer;  // 保存实时B数据
    // pthread_mutex_t mutex;
} DataManager;


 
 /*
 *********************************************************************************************************
 *                                              FUNCTION PROTOTYPES
 *                                        (Golbal function prototypes.   )
 *********************************************************************************************************
 */
 f32 linear_interpolation(f32 x_target, f32 x0, f32 y0, f32 x1, f32 y1);
 
 
 #endif /* INTERPOLATION_H */
 
 
 
//********************
// 使用线性插值公式
//********************
//y=y0 +(y1-y0)*(x-x0)/(x-x1)
//其中 x 是目标时间点，x0 和 x1 是相邻的已知时间点，y0 和 y1 是对应的数据值