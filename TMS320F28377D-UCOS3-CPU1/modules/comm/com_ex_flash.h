/**
 * @file com_ex_flash.h
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

/* NOTE!
************************************************************************************
28377D 核心板上外扩了一片 32M (2M*16bit ) 的 flash，起始地址为 0X100000
************************************************************************************
*/

#ifndef COM_EX_FLASH_H
#define COM_EX_FLASH_H


#define EX_FLASH_ADDR_START  ((Uint16 *)0x100000)

#define     TimeOutErr               1
#define     VerifyErr                2
#define     WriteOK                  0
#define     EraseErr                 3
#define     EraseOK                  0
#define     SectorSize               0x800
#define     BlockSize                0x8000

Uint16  ChipErase(void);
Uint16 FLASHTEST(Uint16 Length);

void run_ex_flash_test(void);
#endif


//note for debug
//memory browser
//16 bit hex TI style
