/**
 * @file sci_bsp.h
 * @brief 
 * @author hjroyal 
 * @version 1.0
 * @date 2024-08-30
 * 
 * @copyright Copyright (c) 2024  XXXX
 * 
 * @par Modification log:
 * <table>
 * <tr><th>Date           <th>Version   <th>Author      <th>Description
 * <tr><td>2024-08-30     <td>1.0       <td>hjroyal     <td>
 * </table>
 */

#ifndef SCI_BSP_H
#define SCI_BSP_H

#include "include.h"

/*
*********************************************************************************************************
*                                              DATA DEFINITION
*                                        (Data interfaces, constants, etc.)
*********************************************************************************************************
*/

//常量定义


//结构体定义


//联合体定义


//对外部文件开放全局变量声明


/*
*********************************************************************************************************
*                                              FUNCTION PROTOTYPES
*                                        (Golbal function prototypes.   )
*********************************************************************************************************
*/



void     initSCIAFIFO         ( uint16_t baud_rate           );
void     initSCIAGpio         ( void                         );
void     initSCIACBuffer      ( void                         );
uint16_t sciaBufferWrite      ( char* data, uint16_t len     );
void     sciPrintf            ( const char *pcString, ...    );

//-------------------------------------------------//
// SCIA bsp                                        //
//-------------------------------------------------//

void scia_msg(char *msg);
#endif   /* SCI_BSP_H */


