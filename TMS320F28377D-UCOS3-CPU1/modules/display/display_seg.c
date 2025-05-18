/**
 * @file display_seg.c
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

#include "display_seg.h"

unsigned char const SEG_TABLE[] = { // 共阴极0~f数码管编码
    0xfc, 0x60, 0xda, 0xf2,         // 0~3
    0x66, 0xb6, 0xbe, 0xe0,         // 4~7
    0xfe, 0xf6};

static void SEG_DATA(unsigned char d);
static void set_seg(unsigned char d[4]);
static void display_temp(int16 temp);

/**
 * @brief Set the bit of the digital tube.
 * @param  d  : myParam
 * @author hjroyal
 * @date 2025-04-07
 */
static void SEG_DATA(unsigned char d) {
    Uint32 dat1 = 0, dat2 = 0;
    dat1 = d;
    dat1 <<= 13;
    dat2 = d;
    dat2 <<= 13;
    dat2 &= 0x100000;
    dat2 <<= 1;
    dat1 |= dat2;
    GpioDataRegs.GPCDAT.all = dat1;
}

/**
 * @brief Set the seg object
 * @param  d  : The number displayed by the digital tube
 * @author hjroyal
 * @date 2025-04-07
 */
static void set_seg(unsigned char d[4]) {
    SEG_DATA(SEG_TABLE[d[3]]);
    SBIT0 = 0;
    DELAY_US(1);
    SBIT2 = 1;
    DELAY_US(1);
    SBIT1 = 1;
    DELAY_US(1);
    SBIT3 = 1;
    DELAY_US(100);
    SBIT0 = 1;
    SEG_DATA(0);

    SEG_DATA(SEG_TABLE[d[2]]);
    SBIT1 = 0;
    DELAY_US(1);
    SBIT2 = 1;
    DELAY_US(1);
    SBIT3 = 1;
    DELAY_US(1);
    SBIT0 = 1;
    DELAY_US(100);
    SBIT1 = 1;
    SEG_DATA(0);

    SEG_DATA(SEG_TABLE[d[1]]);
    SBIT2 = 0;
    DELAY_US(1);
    SBIT1 = 1;
    DELAY_US(1);
    SBIT3 = 1;
    DELAY_US(1);
    SBIT0 = 1;
    DELAY_US(100);
    SBIT2 = 1;
    SEG_DATA(0);

    SEG_DATA(SEG_TABLE[d[0]]);
    SBIT3 = 0;
    DELAY_US(1);
    SBIT1 = 1;
    DELAY_US(1);
    SBIT2 = 1;
    DELAY_US(1);
    SBIT0 = 1;
    DELAY_US(100);
    SBIT3 = 1;
    SEG_DATA(0);
}

/**
 * @brief  Display temperature to digital tube
 * @param  temp : myParam
 * @author hjroyal
 * @date 2025-04-07
 */
static void display_temp(int16 temp) {
    unsigned char digits[4] = {0};
    int len = 0;
    int is_negative = 0;
    if (temp < 0)
        is_negative = 1; // 处理负数
    do {
        digits[len++] = temp % 10;
        temp /= 10;
    } while (temp > 0);

    if (is_negative) {
        digits[3] = 1;
    }

    set_seg(&digits);
}

///>hj 20250511 更新时间太慢
static void display_time(void) {
    unsigned char digits[4];
    digits[0] = rtc_sec % 10;
    rtc_sec /= 10;
    digits[1] = rtc_sec % 10;
    digits[2] = rtc_min % 10;
    rtc_min /= 10;
    digits[3] = rtc_min % 10;

    set_seg(&digits);
}

/**
 * @brief 显示模块参数初始化
 * @param[in,out] dp             :
 * @author hjroyal
 * @date 2025-04-16
 */
void parmInitSegDisplay(SEG_DSIPLAY *dp) {
    dp->parm_en = 1;
    dp->parm_sensor_sel = 0;
}

/**
 * @brief 显示初始化操作
 * @param[in,out] dp             :
 * @author hjroyal
 * @date 2025-04-16
 */
void initSegDisplay(SEG_DSIPLAY *dp) {
}

/**
 * @brief 显示更新逻辑
 * @param[in,out] dp             :
 * @author hjroyal
 * @date 2025-04-16
 */
void runSegDisplay(SEG_DSIPLAY *dp) {
    if (dp->parm_en) {
        if (dp->parm_sensor_sel == 1) {
            // display_temp((*(dp->in_dth11_temp)));
        } else if (dp->parm_sensor_sel == 2) {
            display_time();
        } else {
            display_temp((*(dp->in_dsp_temp)));
        }
    }
}
