// The user must define CLA_C in the project linker settings if using the
// CLA C compiler
// Project Properties -> C2000 Linker -> Advanced Options -> Command File
// Preprocessing -> --define
#ifdef CLA_C
// Define a size for the CLA scratchpad area that will be used
// by the CLA compiler for local symbols and temps
// Also force references to the special symbols that mark the
// scratchpad are.
CLA_SCRATCHPAD_SIZE = 0x100;
--undef_sym=__cla_scratchpad_end
--undef_sym=__cla_scratchpad_start
#endif //CLA_C

/*
   看手册
   https://www.ti.com.cn/cn/lit/ds/symlink/tms320f28377d.pdf
   7.3节
*/

MEMORY
{
//程序存储空间
PAGE 0 :
   /* BEGIN is used for the "boot to SARAM" bootloader mode   */

   BEGIN           	    : origin = 0x080000,   length = 0x000002

   CLA_PROG_SARAM        : origin = 0x008000, length = 0x00001FFF    //CLA1 Local Shared RAM, LS0-LS3, 8K
   APP_FAST_SARAM        : origin = 0x00B000, length = 0x00002FFF    //Local  Shared RAM, D0-D1, 4K 
   APP_PROG_SARAM        : origin = 0x018000, length = 0x00004FFF    //Golbal Shared RAM,GS12-GS15, 16K 

   /* Flash sectors */
   SYS_BOOT_FLASH           : origin = 0x080002,   length = 0x007FFD	//FLASH0-FLASH3,32K
   APP_START_FLASH          : origin = 0x088000,   length = 0x000002	//FLASH4, codestart
   APP_CLA_FLASH            : origin = 0x088002,   length = 0x007FFE	//FLASH4, 31K
   APP_PROG_FLASH           : origin = 0x090000,   length = 0x070000	//448K

   RESET           	  : origin = 0x3FFFC0,   length = 0x000002
//数据存储空间
PAGE 1 :

   BOOT_RSVD         : origin = 0x000002, length = 0x000121     /* Part of M0, BOOT rom will use this for stack */
   APP_STK_SARAM     : origin = 0x000123, length = 0x0006DD     /* on-chip RAM block M1 */  //1K
   //RAMM1_RSVD      : origin = 0x0007F8, length = 0x000008     /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   CLA_DATA_SARAM    : origin = 0x00A000, length = 0x000FFF    //CLA1 Local Shared RAM, LS4-LS5, 4K
   APP_DATA_SARAM    : origin = 0x00C000, length = 0x00BFF8     //GSO-GS11,48K
   //RAMGS11_RSVD    : origin = 0x017FF8, length = 0x000008    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */


   CLA1_MSGRAMLOW   : origin = 0x001480,   length = 0x000080
   CLA1_MSGRAMHIGH  : origin = 0x001500,   length = 0x000080
}


SECTIONS
{
   /* Allocate program areas: */
   .cinit           : > APP_PROG_FLASH            PAGE = 0, ALIGN(8)
   .text            : > APP_PROG_FLASH            PAGE = 0, ALIGN(8)
   .stack           : > APP_STK_SARAM             PAGE = 1              ///>hj 20250510 必须显式分配到低64KB的RAM区域
   .switch          : > APP_PROG_FLASH            PAGE = 0, ALIGN(8)
   codestart        : > BEGIN                     PAGE = 0, ALIGN(8)


   /* Allocate uninitalized data sections: */

   .pinit              : > APP_PROG_FLASH,       PAGE = 0, ALIGN(8)
   .ebss               : > APP_DATA_SARAM,		 PAGE = 1  //段用于存储未初始化的全局变量和静态变量。在程序启动时，这些变量会被自动初始化为零
   .esysmem            : > APP_DATA_SARAM,       PAGE = 1 //段通常用于存储系统级的数据或特定的内存管理信息
   .econst             : > APP_PROG_FLASH   	 PAGE = 0, ALIGN(8)


   .reset              : > RESET,     PAGE = 0, TYPE = DSECT /* not used, */


    /* CLA specific sections */

   Cla1Prog          : LOAD = APP_CLA_FLASH,
                      RUN = CLA_PROG_SARAM,
                      LOAD_START(_Cla1funcsLoadStart),
                      LOAD_END(_Cla1funcsLoadEnd),
                      RUN_START(_Cla1funcsRunStart),
                      LOAD_SIZE(_Cla1funcsLoadSize),
                      PAGE = 0, ALIGN(8)


   CLADataSARAM		  : > CLA_DATA_SARAM,   PAGE=1

   Cla1ToCpuMsgRAM  : > CLA1_MSGRAMLOW,   PAGE = 1
   CpuToCla1MsgRAM  : > CLA1_MSGRAMHIGH,  PAGE = 1


	.TI.ramfunc :  {-l F021_API_F2837xD_FPU32.lib}
                     LOAD = APP_PROG_FLASH,
					      RUN = APP_PROG_SARAM,
          	         LOAD_START(_RamfuncsLoadStart),
           	         LOAD_SIZE(_RamfuncsLoadSize),
           	         LOAD_END(_RamfuncsLoadEnd),
            	         RUN_START(_RamfuncsRunStart),
            	         RUN_SIZE(_RamfuncsRunSize),
             	         RUN_END(_RamfuncsRunEnd),
					      PAGE = 0, ALIGN(8)

#ifdef CLA_C
   /* CLA C compiler sections */
   //
   // Must be allocated to memory the CLA has write access to
   //
   CLAscratch       :
                     { *.obj(CLAscratch)
                     . += CLA_SCRATCHPAD_SIZE;
                     *.obj(CLAscratch_end) } >  CLA_DATA_SARAM,  PAGE = 0

   .scratchpad     : > CLA_DATA_SARAM,      PAGE = 1
   .bss_cla		    : > CLA_DATA_SARAM,       PAGE = 1
   .const_cla      :  LOAD = APP_CLA_FLASH,
                      RUN  = CLA_DATA_SARAM,
                      RUN_START(_Cla1ConstRunStart),
                      LOAD_START(_Cla1ConstLoadStart),
                      LOAD_SIZE(_Cla1ConstLoadSize),
                      PAGE = 1,
                      ALIGN(8)
#endif //CLA_C
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
