/**
 * @file com_ex_flash.c
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

Uint16 ChipErase(void);
Uint16 FLASHTEST(Uint16 Length);

void run_ex_flash_test(void);

  Uint16 erase_ok = 9;
  Uint16 flash_ok = 9;
void run_ex_flash_test(void) {

  erase_ok = ChipErase();
  asm(" ESTOP0");
  flash_ok =FLASHTEST(0xFFFF);
  asm(" ESTOP0");
  FLASHTEST(0xFFFF);
  asm(" ESTOP0");
}

Uint16 ChipErase(void) {
  Uint16 Data;
  Uint32 TimeOut, i;
  *(EX_FLASH_ADDR_START + 0x5555) = 0xAAAA;
  *(EX_FLASH_ADDR_START + 0x2AAA) = 0x5555;
  *(EX_FLASH_ADDR_START + 0x5555) = 0x8080;
  *(EX_FLASH_ADDR_START + 0x5555) = 0xAAAA;
  *(EX_FLASH_ADDR_START + 0x2AAA) = 0x5555;
  *(EX_FLASH_ADDR_START + 0x5555) = 0x1010;
  i = 0;
  TimeOut = 0;
  while (i < 5) {
    Data = *(EX_FLASH_ADDR_START + 0x3FFFF);
    if (Data == 0xFFFF)
      i++;
    else
      i = 0;
    if (++TimeOut > 0x1000000)
      return (TimeOutErr);
  }
  for (i = 0; i < 0x40000; i++) {
    Data = *(EX_FLASH_ADDR_START + i);
    if (Data != 0xFFFF)
      return (EraseErr);
  }
  return (EraseOK);
}


Uint16 FLASHTEST(Uint16 Length) {
  Uint32 i, TimeOut;
  Uint16 Data1, Data2, j, t;
  for (i = 0; i < Length; i++) // 写部分
  {
    *(EX_FLASH_ADDR_START + 0x5555) = 0x00AA;
    *(EX_FLASH_ADDR_START + 0x2AAA) = 0x0055;
    *(EX_FLASH_ADDR_START + 0x5555) = 0x00A0;
    *(EX_FLASH_ADDR_START + i) = i;
    TimeOut = 0;
    j = 0;
    while (j < 5) {
      Data1 = *(EX_FLASH_ADDR_START + i);
      Data2 = *(EX_FLASH_ADDR_START + i);
      if (Data1 == Data2)
        j++;
      else
        j = 0;
      if (++TimeOut > 0x1000000)
        return (TimeOutErr); // flash写自然数无响应
    }
  }

  for (i = 0; i < Length; i++) {
    t = *(EX_FLASH_ADDR_START + i);
    if (t != i)
      return (EraseErr);
  }
  return (WriteOK);
}
