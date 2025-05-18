/**
 * @file control_parameter.c
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

#include "control_parameter.h"


// 0 使用一次性完成参数保存/加载  1 使用分次完成参数保存/加载
#define USE_PARAM_SAVE_LOAD_API   0

/******************************************  模拟储存设备操作  ******************************************************/
// 储存空间定义
u8 sg_buf[500];
u32 sg_length = 0;

#define HEX_PRINTF(str, hex, len)  \
    printf("%s: [%d] -> ", str, len);\
    for (s32 i = 0; i < (len); i++){printf("%02x ", hex[i]);}\
    printf("\n");

// 从储存空间读取数据


s32 OnLoadCallback(u8 *pBuf, u16 bufSize, u16 *pLength)
{
    u16 length;
    static u32 s_offset = 0;

    if (s_offset < sg_length)
    {
        if (s_offset + bufSize <= sg_length)
        {
            length = bufSize;
            memcpy(pBuf, &sg_buf[s_offset], length);
            s_offset += length;
        }
        else
        {
            length = sg_length - s_offset;
            memcpy(pBuf, &sg_buf[s_offset], length);
            s_offset += length;
        }
    }
    else
    {
        length = 0;
        s_offset = 0;
    }

    if (length > 0)
    {
        printf("************************************************************************************************\n");
        HEX_PRINTF("\tread", pBuf, length);
        printf("************************************************************************************************\n");
    }

    *pLength = length;
    return 0;
}

// 写数据至储存空间
s32 OnSaveCallback(const u8 *pBuf, u16 len)
{
    static u32 s_offset = 0;

    if (len > 0)
    {
        memcpy(&sg_buf[s_offset], pBuf, len);
        HEX_PRINTF("\twrite", pBuf, len);
        s_offset += len;
        sg_length = s_offset;
    }
    else
    {
        printf("************************************************************************************************\n");
        HEX_PRINTF("\tSave", sg_buf, sg_length);
        printf("************************************************************************************************\n");
        s_offset = 0;
    }

    return 0;
}

/******************************************  模拟储存设备操作  ******************************************************/

ParamDemo_t g_tTestVal = {
    .usValue = 20,
    .ucValue = 10,
    .uiValue = 1000,
    .fValue = 3.14,
    .szString_1 = "abcd",
    .dValue = 5.12,
    .sValue = -100,
    .cValue = -2,
    .iValue = 300,
    .szString_2 = "12234",
};

s8 g_cTest = 50;
s8 g_szString[10] = "qwer";

static s32 CheckSValue(const void *pCurParam);

cotParamInfo_t sg_ParamTable[] = {
    COT_PARAM_ITEM_BIND(1, g_tTestVal.usValue, COT_PARAM_UINT16, COT_PARAM_ATTR_WR),
    COT_PARAM_ITEM_BIND(2, g_tTestVal.ucValue, COT_PARAM_UINT8, COT_PARAM_ATTR_WR, 20),
    COT_PARAM_ITEM_BIND(3, g_tTestVal.uiValue, COT_PARAM_UINT32, COT_PARAM_ATTR_WR, 1000, 1000, 10000),
    COT_PARAM_ITEM_BIND(4, g_tTestVal.fValue, COT_PARAM_FLOAT, COT_PARAM_ATTR_WR, 10, -10.5, 10.5),
    COT_PARAM_ITEM_BIND(5, g_tTestVal.szString_1, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "abcd", 3, sizeof(g_tTestVal.szString_1)),
    COT_PARAM_ITEM_BIND(6, g_tTestVal.dValue, COT_PARAM_DOUBLE, COT_PARAM_ATTR_WR, 0, -90.10, 100.10),
    COT_PARAM_ITEM_BIND(7, g_tTestVal.sValue, COT_PARAM_INT16, COT_PARAM_ATTR_WR, 100, -200, 200, CheckSValue), // 添加自定义校验
    COT_PARAM_ITEM_BIND_WITH_NAME(8, "g_cTest", g_cTest, COT_PARAM_INT8, COT_PARAM_ATTR_WR, 50, -100, 100), // 另取参数名
    COT_PARAM_ITEM_BIND(9, g_szString, COT_PARAM_STRING, COT_PARAM_ATTR_WR, "XXX", 3, 6),
};

static s32 CheckSValue(const void *pCurParam)
{
    const s16 *p_sValue = (const s16 *)pCurParam;

    if ((*p_sValue) % 2 != 0)
    {
        return -1;
    }

    return 0;
}


static cotParamManager_t sg_tParamManager;

static void ShowSingleParam(const cotParamInfo_t *paramInfo);

// 数据校验出错时恢复默认处理
s32 OnCheckErrorResetHandle(const cotParamInfo_t *pParamInfo, cotParamCheckRet_e eCheckResult)
{
    printf("\n--------error list start ----------\n");
    printf("%4s    %-24s %-8s %-8s %-8s %-10s %-10s %-10s %-10s\n", "id", "name", "type", "length", "attr", "val", "def", "min", "max");
    ShowSingleParam(pParamInfo);
    printf("--------error list end ------------\n");
    cotParam_SingleParamResetDefValue(pParamInfo);
    return 0;
}

void InitParam(bool isReset)
{
    cotParam_Init(&sg_tParamManager, sg_ParamTable, COT_PARAM_TABLE_SIZE(sg_ParamTable));

    if (sg_length == 0) // 储存设备中没有储存过参数则首次进行储存
    {
        printf("frist save param: %d\n", cotParam_GetSerializeSize(&sg_tParamManager));
        SaveParam(false);
    }

    ReloadParam(isReset);
}

void ReloadParam(bool isReset)
{
    u8 buf[300];
    u16 length;

    printf("\n============================ load param start ==========================\n");

    if (isReset)
    {
        cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
    }

#if USE_PARAM_SAVE_LOAD_API
    cotParam_Load(&sg_tParamManager, OnLoadCallback);
#else
    // OnLoadCallback(sg_buf, sg_length, true);
    cotParam_Deserialization(&sg_tParamManager, sg_buf, sg_length);
#endif
    printf("============================ load param end ============================\n\n");
}

void SaveParam(bool isReset)
{
    u8 buf[300];
    u32 bufLength = 0;
    printf("\n============================ save param start ==========================\n");
    if (isReset)
    {
        cotParam_Check(&sg_tParamManager, OnCheckErrorResetHandle);
    }

#if USE_PARAM_SAVE_LOAD_API
    cotParam_Save(&sg_tParamManager, OnSaveCallback);
#else
    bufLength = cotParam_Serialize(&sg_tParamManager, buf);
    OnSaveCallback(buf, bufLength);
    OnSaveCallback(buf, 0);
#endif
    printf("============================ save param end ============================\n\n");
}

void ResetParam(void)
{
    printf("reset param\n");
    cotParam_ResetDefault(&sg_tParamManager);
}

#define ATTR(x)   ((x & (PARAM_ATTR_READ | COT_PARAM_ATTR_WRITE)) == (PARAM_ATTR_READ | COT_PARAM_ATTR_WRITE) ? \
                    "wr" : (x & (PARAM_ATTR_READ) ? "r" : ((x & (PARAM_ATTR_WRITE) ? "w" : ""))))

char *Attr(u8 attr)
{
    static char buf[10];
    char *p = buf;

    if (attr & COT_PARAM_ATTR_READ)
    {
        p += sprintf(p, "r");
    }

    if (attr & COT_PARAM_ATTR_WRITE)
    {
        p += sprintf(p, "w");
    }

    if (attr & COT_PARAM_ATTR_RESET)
    {
        p += sprintf(p, "s");
    }

    if (attr & COT_PARAM_ATTR_RANGE)
    {
        p += sprintf(p, "m");
    }

    return buf;
}

void ShowSingleParam(const cotParamInfo_t *paramInfo)
{
    if (paramInfo != NULL)
    {
        switch (paramInfo->type)
        {
        case COT_PARAM_INT8:
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->name.pTextString, 
                "s8", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt8);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10d ", *paramInfo->unDefValuePtr.pInt8);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10d ", *paramInfo->unMinValuePtr.pInt8);
                printf("%-10d ", *paramInfo->unMaxValuePtr.pInt8);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_INT16:
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->name.pTextString, 
                "s16", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt16);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10d ", *paramInfo->unDefValuePtr.pInt16);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10d ", *paramInfo->unMinValuePtr.pInt16);
                printf("%-10d ", *paramInfo->unMaxValuePtr.pInt16);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_INT32:
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10d ", paramInfo->id, paramInfo->name.pTextString, 
                "s32", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt32);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10d ", *paramInfo->unDefValuePtr.pInt32);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10d ", *paramInfo->unMinValuePtr.pInt32);
                printf("%-10d ", *paramInfo->unMaxValuePtr.pInt32);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_INT64:
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10ld ", paramInfo->id, paramInfo->name.pTextString, 
                "s64", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pInt64);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10ld ", *paramInfo->unDefValuePtr.pInt64);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10ld ", *paramInfo->unMinValuePtr.pInt64);
                printf("%-10ld ", *paramInfo->unMaxValuePtr.pInt64);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_UINT8:
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->name.pTextString, 
                "u8", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint8);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10u ", *paramInfo->unDefValuePtr.pUint8);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10u ", *paramInfo->unMinValuePtr.pUint8);
                printf("%-10u ", *paramInfo->unMaxValuePtr.pUint8);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_UINT16:
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->name.pTextString, 
                "u16", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint16);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10u ", *paramInfo->unDefValuePtr.pUint16);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10u ", *paramInfo->unMinValuePtr.pUint16);
                printf("%-10u ", *paramInfo->unMaxValuePtr.pUint16);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_UINT32:
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10u ", paramInfo->id, paramInfo->name.pTextString, 
                "u32", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint32);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10u ", *paramInfo->unDefValuePtr.pUint32);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10u ", *paramInfo->unMinValuePtr.pUint32);
                printf("%-10u ", *paramInfo->unMaxValuePtr.pUint32);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_UINT64:
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10lu ", paramInfo->id, paramInfo->name.pTextString, 
                "u64", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pUint64);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10lu ", *paramInfo->unDefValuePtr.pUint64);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10lu ", *paramInfo->unMinValuePtr.pUint64);
                printf("%-10lu ", *paramInfo->unMaxValuePtr.pUint64);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_FLOAT:
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10f ", paramInfo->id, paramInfo->name.pTextString, 
                "f32", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pFloat);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10f ", *paramInfo->unDefValuePtr.pFloat);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10f ", *paramInfo->unMinValuePtr.pFloat);
                printf("%-10f ", *paramInfo->unMaxValuePtr.pFloat);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
        case COT_PARAM_DOUBLE:
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10f ", paramInfo->id, paramInfo->name.pTextString, 
                "f64", paramInfo->length, Attr(paramInfo->attr), *paramInfo->unCurValuePtr.pDouble);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10f ", *paramInfo->unDefValuePtr.pDouble);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10f ", *paramInfo->unMinValuePtr.pDouble);
                printf("%-10f ", *paramInfo->unMaxValuePtr.pDouble);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
#if COT_PARAM_USE_STRING_TYPE
        case COT_PARAM_STRING:
            printf(" %-4d   %-24s %-10s %-6d %-8s %-10s ", paramInfo->id, paramInfo->name.pTextString, 
                "string", paramInfo->length, Attr(paramInfo->attr), paramInfo->unCurValuePtr.pString);

            if (paramInfo->attr & COT_PARAM_ATTR_RESET)
                printf("%-10s ", paramInfo->unDefValuePtr.pString);
            else
                printf("%-10s ", "-");

            if (paramInfo->attr & COT_PARAM_ATTR_RANGE)
            {
                printf("%-10u ", *paramInfo->unMinValuePtr.pStringLength);
                printf("%-10u ", *paramInfo->unMaxValuePtr.pStringLength);
            }
            else
            {
                printf("%-10s ", "-");
                printf("%-10s ", "-");
            }
            break;
#endif
        default:
            break;
        }

        printf("\n");

    }
}

s32 SingleParamCheckProcess(const void *pCurParam, cotParamResetOpt_e eResetOpt)
{
    return cotParam_SingleParamCheckProcess(cotParam_FindParamByParamPtr(&sg_tParamManager, pCurParam), eResetOpt);
}

cotParamCheckRet_e SingleParamCheck(const void *pCurParam, const void *pCheckValue)
{
    cotParamCheckRet_e eCheckResult;

    cotParam_SingleParamCheckInput(cotParam_FindParamByParamPtr(&sg_tParamManager, pCurParam), pCheckValue, &eCheckResult);

    return eCheckResult;
}

cotParamCheckRet_e SingleParamSelfCheck(const void *pCurParam)
{
    cotParamCheckRet_e eCheckResult;

    cotParam_SingleParamSelfCheck(cotParam_FindParamByParamPtr(&sg_tParamManager, pCurParam), &eCheckResult);

    return eCheckResult;
}

void SingleParamResetResetDefValue(const void *pCurParam)
{
    cotParam_SingleParamResetDefValue(cotParam_FindParamByParamPtr(&sg_tParamManager, pCurParam));
}

void ShowAllParam(void)
{
    size_t idx = 0;
    cotParamInfo_t *paramInfo;

    printf("%4s    %-24s %-8s %-8s %-8s %-10s %-10s %-10s %-10s\n", "id", "name", "type", "length", "attr", "val", "def", "min", "max");

    do
    {
        paramInfo = cotParam_IterateList(&sg_tParamManager, &idx);

        ShowSingleParam(paramInfo);
    } while (paramInfo != NULL);
    printf("\n");
}

void SingleParamChange(const void *pCurParam, ...)
{
    va_list paramList;

    va_start(paramList, pCurParam);

    cotParam_SingleParamChangeImpl(&sg_tParamManager, pCurParam, paramList);

    va_end(paramList);
}

void test_param(void) {
  InitParam(true);

  ShowAllParam();

  g_tTestVal.fValue = 20.05;

  g_tTestVal.uiValue = 50;
  SingleParamCheckProcess(
      &g_tTestVal.uiValue,
      COT_PARAM_RESET_MIN_MAX); // 修改后检查并处理：如果小于最小值则恢复最小值，大于最大值则恢复最大值

  g_tTestVal.uiValue = 50;
  if (SingleParamSelfCheck(&g_tTestVal.uiValue) !=
      COT_PARAM_CHECK_OK) // 修改后检查
  {
    SingleParamResetResetDefValue(
        &g_tTestVal.uiValue); // 如果校验失败，则恢复为默认值
  }

  uint32_t tmp = 50;

  if (SingleParamCheck(&g_tTestVal.uiValue, &tmp) ==
      COT_PARAM_CHECK_OK) // 修改前检查（参数和被检查变量值类型需要一样）
  {
    g_tTestVal.uiValue = tmp; // 如果校验成功，则修改
  }

  // g_test = 80;
  // g_test_3 = -20.5;
  sprintf(g_szString, "sd");

  SingleParamChange(&g_tTestVal.uiValue, 1500);

  SingleParamChange(&g_tTestVal.uiValue, 800); // 修改无效

  SingleParamChange(g_tTestVal.szString_1, "wwww.bau");
  SingleParamChange(g_tTestVal.szString_1, "ww"); // 修改无效

  SaveParam(true);
  ReloadParam(true);

  ShowAllParam();
}
