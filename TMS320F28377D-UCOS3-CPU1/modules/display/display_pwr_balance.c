/**
 * @file display_pwr_balance.c
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

#include "include.h"

#define Ppv_MAX             10000
#define Pbat_CHAR_MAX       -10000
#define Pbat_DISCHAR_MAX    10000
#define Pgrid_GET_MAX       10000
#define Pgrid_FEED_MAX      0
#define Pload_MAX           10000

typedef struct{
  f32 Ppv;
  f32 Pbat;
  f32 BatSoc;
  f32 Pgrid;
  f32 Pload;
  f32 Pinv;

  u16 EmsMode;
  // u16 MeterType;
  u16 GridState;

} ST_HMI_UIP_DATA;

//修复显示数据功率不平衡问题
void FixHmiInstPwr(ST_HMI_UIP_DATA *p_hmi_data) {

  f32 DeltaPwr;
  f32 PloadRaw;

  // 负载非负处理
  p_hmi_data->Pload = (p_hmi_data->Pload < 0) ? 0 : p_hmi_data->Pload;

  PloadRaw = p_hmi_data->Pload;
  DeltaPwr = -(p_hmi_data->Pload) + (p_hmi_data->Ppv) + (p_hmi_data->Pbat) +
             (p_hmi_data->Pgrid);


  // 边界条件判断,放弃脏数据
  if ((p_hmi_data->Ppv > Ppv_MAX) || (p_hmi_data->Ppv < 0) ||
      (p_hmi_data->Pbat < Pbat_CHAR_MAX) ||
      (p_hmi_data->Pbat > Pbat_DISCHAR_MAX) ||
      (p_hmi_data->Pgrid < Pgrid_FEED_MAX) ||
      (fabs(DeltaPwr) > 1000) ||
      (p_hmi_data->Pgrid > Pgrid_GET_MAX) || (p_hmi_data->Pload > Pload_MAX)) {
    memset(p_hmi_data, 0, sizeof(ST_HMI_UIP_DATA));
    return;
  }



  ////自发自用模式功率流平衡////
  ////能量守恒////

    if (DeltaPwr > 0.1) {
      //1. 说明输入能量大于输出，计算总体正常
      if(p_hmi_data->Pload > 1){
        //1.1 有负载，补偿到负载
        p_hmi_data->Pload += DeltaPwr;
      }else if(p_hmi_data->Ppv > 1){
        //1.2无负载，或负载小于0被写为0
        //有PV
        if(p_hmi_data->Ppv - DeltaPwr >0){
          p_hmi_data->Ppv -= DeltaPwr;
        }else{
            if (p_hmi_data->GridState == 1) { 
                p_hmi_data->Pgrid = -(p_hmi_data->Ppv + p_hmi_data->Pbat);
            }else{
                //离网不补偿Pgrid
                if(p_hmi_data->Pbat <0){
                  p_hmi_data->Ppv = -p_hmi_data->Pbat;
                }else{
                  p_hmi_data->Pload = p_hmi_data->Ppv + p_hmi_data->Pbat;
                }
                p_hmi_data->Pgrid = 0;
            }
        }
      }else{
        //1.3无负载，或负载小于0被写为0
        //无PV
          if (p_hmi_data->GridState == 1) {
            p_hmi_data->Pgrid = - p_hmi_data->Pbat;
          } else {
            // 离网不补偿Pgrid
                if(p_hmi_data->Pbat <0){
                  p_hmi_data->Pload = 1000000; //故意设置大数字，清0脏数据
                }else{
                  p_hmi_data->Pload = p_hmi_data->Pbat;
                }
                p_hmi_data->Pgrid = 0;
            }
      }
    } else if (DeltaPwr < -0.1) {
      // 2. 误差较大
      if (p_hmi_data->Pload > 1) {
        // 2.1 有负载
        if(p_hmi_data->Pload + DeltaPwr>0){
          p_hmi_data->Pload += DeltaPwr;
        }else if(p_hmi_data->Ppv+DeltaPwr>0){
          p_hmi_data->Ppv += DeltaPwr;
        }else{
          if (p_hmi_data->GridState == 1) {
            p_hmi_data->Pgrid = -(p_hmi_data->Ppv+p_hmi_data->Pbat-p_hmi_data->Pload);
          } else {
            // 离网不补偿Pgrid
            if (p_hmi_data->Ppv > 1) {
              p_hmi_data->Ppv = p_hmi_data->Pload - p_hmi_data->Pbat; 
            } else {
                if(p_hmi_data->Pbat <0){
                  p_hmi_data->Pload = 1000000; //故意设置大数字，清0脏数据
                }else{
                  p_hmi_data->Pload = p_hmi_data->Pbat;
                }
            }
            p_hmi_data->Pgrid = 0;
          }
          
        }
        
      } else if (p_hmi_data->Ppv > 1) {
        // 2.2无负载
        // 有PV
        if (p_hmi_data->Ppv + DeltaPwr > 0) {
          p_hmi_data->Ppv += DeltaPwr;
        } else {

          if (p_hmi_data->GridState == 1) {
            p_hmi_data->Pgrid = -(p_hmi_data->Ppv + p_hmi_data->Pbat);
          } else {
            // 离网不补偿Pgrid
                if(p_hmi_data->Pbat <0){
                  p_hmi_data->Ppv = -p_hmi_data->Pbat; 
                }else{
                  p_hmi_data->Pload = p_hmi_data->Pbat+p_hmi_data->Ppv;
                }
                p_hmi_data->Pgrid = 0;
            }
        }
      } else {
        // 2.3无负载
        // 无PV
        if (p_hmi_data->GridState == 1) {
          p_hmi_data->Pgrid = -p_hmi_data->Pbat;
        } else {
          // 离网不补偿Pgrid
          if (p_hmi_data->Pbat < 0) {
            p_hmi_data->Pload = 1000000;
          } else {
            p_hmi_data->Pload = p_hmi_data->Pbat;
          }
          p_hmi_data->Pgrid = 0;
        }
      }
    }else {
      //pass 几乎无误差
    }

  // 边界条件判断,放弃脏数据
  if ((p_hmi_data->Ppv > Ppv_MAX) || (p_hmi_data->Ppv < 0) ||
      (p_hmi_data->Pbat < Pbat_CHAR_MAX) ||
      (p_hmi_data->Pbat > Pbat_DISCHAR_MAX) ||
      (p_hmi_data->Pgrid < Pgrid_FEED_MAX) ||
      (fabs(DeltaPwr) > 1000) ||
      (p_hmi_data->Pgrid > Pgrid_GET_MAX) || (p_hmi_data->Pload > Pload_MAX)) {
    memset(p_hmi_data, 0, sizeof(ST_HMI_UIP_DATA));
    return;
  }
}