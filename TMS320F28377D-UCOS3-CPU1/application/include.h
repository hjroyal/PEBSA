/**
 * @file include.h
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

#ifndef INCLUDE_H
#define INCLUDE_H

#if !defined(PEBSA) || !defined(CPU1)
#error Incorrect project settings!
#endif

#define DEBUG_PRINT_EN 1

//ucos3
#include <app_cfg.h>
#include <cpu_core.h>
#include <lib_def.h>
#include <os_cpu.h>
#include <os.h>

//f2837x
#include "F28x_Project.h"  //外设库
#include "driverlib.h"     //驱动库
#include "device.h"



//application
#include "modules.h"
#include "app.h"
#include "stdint.h"



#define PLX_ASSERT(x) do {\
   if(!(x)){\
      asm("        ESTOP0");\
      for(;;);\
   }\
} while(0)



#endif /* __INCLUDE__ */

