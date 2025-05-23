/**
 * @file app.h
 * @brief
 * @author hjroyal
 * @version 1.0
 * @date 2025-03-01
 *
 * @copyright Copyright (c) 2024  XXXX
 *
 * @par Modification log:
 * <table>
 * <tr><th>Date           <th>Version   <th>Author      <th>Description
 * <tr><td>2025-03-01     <td>1.0       <td>hjroyal     <td>update
 * </table>
 */

#ifndef APP_H
#define APP_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
// Data interfaces, constants, etc.
//
//*****************************************************************************
// 常量定义



#define EventCtrl10ms (OS_FLAGS)0x0001 ///< 事件标志组宏定义
#define EventCtrl1    (OS_FLAGS)0x0002 ///< 事件标志组宏定义
#define EventCtrl2    (OS_FLAGS)0x0004 ///< 事件标志组宏定义
#define EventCtrl3    (OS_FLAGS)0x0008 ///< 事件标志组宏定义
#define EventCtrl4    (OS_FLAGS)0x0010 ///< 事件标志组宏定义
#define EventCtrl5    (OS_FLAGS)0x0020 ///< 事件标志组宏定义
#define EventCtrl6    (OS_FLAGS)0x0040 ///< 事件标志组宏定义
#define EventCtrl7    (OS_FLAGS)0x0080 ///< 事件标志组宏定义

#define EventProt20ms (OS_FLAGS)0x0001 ///< 事件标志组宏定义
#define EventProt1    (OS_FLAGS)0x0002 ///< 事件标志组宏定义
#define EventProt2    (OS_FLAGS)0x0004 ///< 事件标志组宏定义
#define EventProt3    (OS_FLAGS)0x0008 ///< 事件标志组宏定义
#define EventProt4    (OS_FLAGS)0x0010 ///< 事件标志组宏定义
#define EventProt5    (OS_FLAGS)0x0020 ///< 事件标志组宏定义
#define EventProt6    (OS_FLAGS)0x0040 ///< 事件标志组宏定义
#define EventProt7    (OS_FLAGS)0x0080 ///< 事件标志组宏定义

#define EventCom50ms (OS_FLAGS)0x0001 ///< 事件标志组宏定义
#define EventComScia (OS_FLAGS)0x0002 ///< 事件标志组宏定义
#define EventCom2    (OS_FLAGS)0x0004 ///< 事件标志组宏定义
#define EventCom3    (OS_FLAGS)0x0008 ///< 事件标志组宏定义
#define EventCom4    (OS_FLAGS)0x0010 ///< 事件标志组宏定义
#define EventCom5    (OS_FLAGS)0x0020 ///< 事件标志组宏定义
#define EventCom6    (OS_FLAGS)0x0040 ///< 事件标志组宏定义
#define EventCom7    (OS_FLAGS)0x0080 ///< 事件标志组宏定义

// 结构体定义

// 联合体定义

// 对外部文件开放全局变量声明

extern OS_FLAG_GRP OSEventCtrl;
extern OS_FLAG_GRP OSEventProt;
extern OS_FLAG_GRP OSEventCom;

extern OS_SEM SEM_SYNCH;     /* 用于同步 */
extern OS_SEM AppPrintfSemp; /* 用于printf互斥 */


//*****************************************************************************
//
// Prototypes for the APIs.
// 对外部文件开放函数声明。
//
//*****************************************************************************

interrupt void epwm1_isr(void); // 主中断函数

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* APP_H */
