/**
 * @file interpolation.h
 * @brief Interpolation algorithm to achieve data consistency.
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

 typedef struct {
    f32 *timestamps;  // 时间戳数组
    f32 *values;      // 数据值数组
    u32 size;           // 缓冲区大小
    u32 head;           // 最新数据位置
} DataBuffer;


 
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