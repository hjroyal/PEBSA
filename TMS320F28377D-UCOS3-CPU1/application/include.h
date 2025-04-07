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

//ucos3
#include <app_cfg.h>
#include <cpu_core.h>
#include <lib_def.h>
#include <os_cpu.h>
#include <os.h>

//f2837x
#include "F28x_Project.h" 
#include "driverlib.h"
#include "device.h"

//application
#include "module_inc.h"
#include "app.h"
#include "nr_micro_shell.h"

#define PEBSA_APP_VER  100      /* Version of app(Vx.yy mult. by 100)    */

#define PLX_ASSERT(x) do {\
   if(!(x)){\
      asm("        ESTOP0");\
      for(;;);\
   }\
} while(0)



#endif /* __INCLUDE__ */

