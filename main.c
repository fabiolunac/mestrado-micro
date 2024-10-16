
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c1294ncpdt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

#define VECTOR_SIZE 100

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Blinky (blinky)</h1>
//!
//! A very simple example that blinks the on-board LED using direct register
//! access.
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif



uint32_t ui32SysClkFreq;

void UART0_ReceiveFloatVector(float* vector, uint32_t size)
{
    uint8_t* byte_ptr = (uint8_t*)vector;
    uint32_t i;
    for(i = 0; i < size * sizeof(float); i++)
    {
        while(!UARTCharsAvail(UART0_BASE));
        byte_ptr[i] = UARTCharGet(UART0_BASE); //Recebe o byte
    }
}

void UART0_SendFloatVector(float* vector, uint32_t size)
{
    uint8_t* byte_ptr = (uint8_t*)vector;
    uint32_t i;
    for(i = 0; i < size * sizeof(float); i++)
    {
        UARTCharPut(UART0_BASE, byte_ptr[i]);
    }
}

void Processing_Signal(float* vector, uint32_t size)
{
    uint32_t i;
    for (i = 0; i < size; i++)
    {
        vector[i] *= 2.0f;
    }
}

int main(void)
{
        // Clk do sistema
        ui32SysClkFreq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
    SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);


        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        UARTConfigSetExpClk(UART0_BASE, ui32SysClkFreq, 115200,(UART_CONFIG_WLEN_8 |
    UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

        UARTEnable(UART0_BASE);

        float signal_vector  [VECTOR_SIZE];

        while(1)
        {
            // Recebe o vetor do python
            UART0_ReceiveFloatVector(signal_vector, VECTOR_SIZE);

            // Processamento do Sinal
            Processing_Signal(signal_vector, VECTOR_SIZE);

            // Enviar o vetor para o python
            UART0_SendFloatVector(signal_vector, VECTOR_SIZE);
        }


}

