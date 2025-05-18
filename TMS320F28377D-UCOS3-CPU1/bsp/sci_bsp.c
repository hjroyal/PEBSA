/**
 * @file sci_bsp.c
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
#include "sci_bsp.h"

uint16_t  wBuffPool[MAX_WCBUFFER_POOL + 1];
cBuffer_t wBuff;

uint16_t SCI_writeCharArrayNoneBlocking(uint32_t base, const uint16_t *const array, uint16_t length) {
    uint16_t i;
    for (i = 0U; i < length; i++) HWREGH(base + SCI_O_TXBUF) = array[i];
    return i;
}

void SCI_readCharArrayNoneBlocking(uint32_t base, uint16_t *const array, uint16_t length) {
    uint16_t i;
    for (i = 0U; i < length; i++) array[i] = (uint16_t)(HWREGH(base + SCI_O_RXBUF) & SCI_RXBUF_SAR_M);
}

uint16_t       wData[SCI_FIFO_TX16];
interrupt void sciaTXFIFOISR(void) {
    uint16_t fifoSpace  = SCI_FIFO_TX16 - SCI_getTxFIFOStatus(SCIA_BASE);
    uint16_t cBufferLen = readCBufferArray(&wBuff, wData, fifoSpace);
    SCI_writeCharArrayNoneBlocking(SCIA_BASE, wData, cBufferLen);
    if (cBufferLen < fifoSpace)
        SCI_disableTxModule(SCIA_BASE);
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXFF);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

uint16_t       rData[SCI_FIFO_RX16];
interrupt void sciaRXFIFOISR(void) {
    uint16_t len = SCI_getRxFIFOStatus(SCIA_BASE);
    SCI_readCharArrayNoneBlocking(SCIA_BASE, rData, len);

    SCI_clearOverflowStatus(SCIA_BASE);
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_RXFF);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

///<hj 20250517 ok
void initSCIAGpio(void) {
    // GPIO64 is the SCI Rx pin.
    GPIO_setMasterCore(64, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_64_SCIRXDA);
    GPIO_setDirectionMode(64, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(64, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(64, GPIO_QUAL_ASYNC);

    // GPIO65 is the SCI Tx pin.
    GPIO_setMasterCore(65, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_65_SCITXDA);
    GPIO_setDirectionMode(65, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(65, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(65, GPIO_QUAL_ASYNC);
}

///<hj 20250517 ok
void initSCIAFIFO(uint16_t baud_rate) {
    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, baud_rate,
                  (SCI_CONFIG_WLEN_8 | SCI_CONFIG_STOP_ONE | SCI_CONFIG_PAR_NONE));
    SCI_enableModule(SCIA_BASE);
    SCI_resetChannels(SCIA_BASE);
    SCI_enableFIFO(SCIA_BASE);
    SCI_enableInterrupt(SCIA_BASE, SCI_INT_RXFF);
    SCI_disableInterrupt(SCIA_BASE, SCI_INT_RXERR | SCI_INT_FE | SCI_INT_OE | SCI_INT_PE);
    SCI_setFIFOInterruptLevel(SCIA_BASE, SCI_FIFO_TX2, SCI_FIFO_RX1);
    SCI_performSoftwareReset(SCIA_BASE);
    SCI_resetTxFIFO(SCIA_BASE);
    SCI_resetRxFIFO(SCIA_BASE);
    Interrupt_register(INT_SCIA_RX, sciaRXFIFOISR);
    Interrupt_register(INT_SCIA_TX, sciaTXFIFOISR);
    Interrupt_enable(INT_SCIA_RX);
    Interrupt_enable(INT_SCIA_TX);
    initSCIACBuffer();
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

uint16_t sciaBufferWrite(char *data, uint16_t len) {
    uint16_t ans;
    SCI_disableTxModule(SCIA_BASE);
    ans = writeCBufferArray(&wBuff, data, len);
    SCI_enableTxModule(SCIA_BASE);
    return ans;
}

void initSCIACBuffer(void) {
    wBuff.pool     = wBuffPool;
    wBuff.dataSize = sizeof(wBuffPool[0]);
    wBuff.poolSize = MAX_WCBUFFER_POOL;
    wBuff.rIndex   = 0;
    wBuff.wIndex   = 0;
}

char Buff[APP_OUT_BUF_SIZE];
void sciPrintf(const char *pcString, ...) {
    // va_list vaArgP;
    // int     len;
    // va_start(vaArgP, pcString);
    // len = uvsnprintf(Buff, sizeof(Buff), pcString, vaArgP);
    // sciaBufferWrite(Buff, len);
    // va_end(vaArgP);

    scia_msg(pcString);
}

// Transmit a character from the SCI
void scia_xmit(int a) {
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {} // 0h (R/W) = Transmit FIFO is empty
    SciaRegs.SCITXBUF.all = a;
}

void scia_msg(char *msg) {
    int i;
    i = 0;
    while (msg[i] != '\0') {
        scia_xmit(msg[i]);
        i++;
    }
}


