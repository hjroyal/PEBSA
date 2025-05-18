/**
 * @file com_sci.h
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

#ifndef COM_SCI_H
#define COM_SCI_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HOST_SIM
/// Simulation platform head file
#else
    #include "include.h" /// Real platform head file

#endif

/*
*********************************************************************************************************
*                                              DATA API
*                                        (外部开放的数据接口)
*********************************************************************************************************
*/


#if defined(HOST_SIM)
// Simulation platform function
#endif

#ifdef __cplusplus
}
#endif

#endif /* COM_SCI_H */
