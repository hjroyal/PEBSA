/**
 * @file com_sram.c
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

#include "include.h"

void run_ex_sram_test(void) {
    Uint16 i = 0;
    for (i = 0; i < 0xFFFF; i++) {
        *(EX_RAM_ADDR_START + i) = 0x5555;
        if (*(EX_RAM_ADDR_START + i) != 0x5555) {
            printf("EX SRAM ERR!"); ////一旦读出的值与写入的值不相等,说明 SRAM 有问题
        }
    }
    asm(" ESTOP0");
    for (i = 0; i < 0xFFFF; i++) {
        *(EX_RAM_ADDR_START + i) = 0xAAAA;
        if (*(EX_RAM_ADDR_START + i) != 0xAAAA) {
            printf("EX SRAM ERR!"); ////检测 SRAM 的读写是否正确
        }
    }
    asm(" ESTOP0");
    for (i = 0; i < 0xFFFF; i++) {
        *(EX_RAM_ADDR_START + i) = i;
        if (*(EX_RAM_ADDR_START + i) != i) {
            printf("EX SRAM ERR!"); ////检测 SRAM 的读写是否正确
        }
        if (*EX_RAM_ADDR_START == 0x4000) {
            printf("EX SRAM ERR!"); ////检测 SRAM 的读写是否正确
        }
    }
}
