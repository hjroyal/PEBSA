/**
 * @file display_seg.h
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

#ifndef DISPLAY_SEG_H
#define DISPLAY_SEG_H


#define   SBIT3     GpioDataRegs.GPBDAT.bit.GPIO58
#define   SBIT2     GpioDataRegs.GPBDAT.bit.GPIO59
#define   SBIT1     GpioDataRegs.GPBDAT.bit.GPIO60
#define   SBIT0     GpioDataRegs.GPBDAT.bit.GPIO61



void SEG_DATA(unsigned char d);
void set_seg(unsigned char d[4]);

extern unsigned char const SEG_TABLE[]; //数码管显示0~9的编码
void display_temp(int16 temp);


#endif //__DISPLAY_SEG__

