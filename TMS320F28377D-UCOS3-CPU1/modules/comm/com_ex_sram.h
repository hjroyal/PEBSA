/**
 * @file com_ex_sram.h
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

#ifndef COM_EX_SRAM_H
#define COM_EX_SRAM_H


#define EX_RAM_ADDR_START ((Uint16 *)0x300000)


extern void InitEmif1(void);

void run_ex_sram_test(void);
#endif


