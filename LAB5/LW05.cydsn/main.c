#include "project.h"

#define DIGIT_2_CODE 0xA4
#define DIGIT_9_CODE 0x90

static void FourDigit74HC595_sendData(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        Pin_DO_Write((data & (0x80 >> i)) ? 1 : 0);
        Pin_CLK_Write(1);
        Pin_CLK_Write(0);
    }
}

static void FourDigit74HC595_sendOneCode(uint8_t position_code, uint8_t segment_code)
{
    FourDigit74HC595_sendData(position_code);
    FourDigit74HC595_sendData(segment_code);

    Pin_Latch_Write(1);
    Pin_Latch_Write(0);
}

int main(void)
{
    CyGlobalIntEnable;

    for (;;)
    {
        FourDigit74HC595_sendOneCode(0xFE, DIGIT_2_CODE);
        CyDelay(5);

        FourDigit74HC595_sendOneCode(0xFD, DIGIT_9_CODE);
        CyDelay(5);
    }
}