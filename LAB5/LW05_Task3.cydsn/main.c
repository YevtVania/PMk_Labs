#include "project.h"

static uint8_t LED_NUM[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
    0xBF, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0x7F,
    0xFF
};

static uint8_t display_data[8] = {
    1, 2, 3, 4, 5, 6, 7, 8
};

static volatile uint8 led_counter = 0;
static volatile uint16 shift_counter = 0;

static void FourDigit74HC595_sendData(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        Pin_DO_Write((data & (0x80 >> i)) ? 1 : 0);
        Pin_CLK_Write(1);
        Pin_CLK_Write(0);
    }
}

static void FourDigit74HC595_sendOneDigit(uint8_t position, uint8_t digit, uint8_t dot)
{
    if (position >= 8)
    {
        FourDigit74HC595_sendData(0xFF);
        FourDigit74HC595_sendData(0xFF);
        Pin_Latch_Write(1);
        Pin_Latch_Write(0);
        return;
    }

    FourDigit74HC595_sendData(0xFF & ~(1 << position));

    if (dot)
    {
        FourDigit74HC595_sendData(LED_NUM[digit] & 0x7F);
    }
    else
    {
        FourDigit74HC595_sendData(LED_NUM[digit]);
    }

    Pin_Latch_Write(1);
    Pin_Latch_Write(0);
}

static void shiftFirstFourRight(void)
{
    uint8_t temp = display_data[3];

    display_data[3] = display_data[2];
    display_data[2] = display_data[1];
    display_data[1] = display_data[0];
    display_data[0] = temp;
}

CY_ISR(Timer_Int_Handler2)
{
    Timer_1_ReadStatusRegister();

    FourDigit74HC595_sendOneDigit(led_counter, display_data[led_counter], 0);

    led_counter++;

    if (led_counter > 7)
    {
        led_counter = 0;
    }

    shift_counter++;

    if (shift_counter >= 500)
    {
        shift_counter = 0;
        shiftFirstFourRight();
    }
}

int main(void)
{
    CyGlobalIntEnable;

    Timer_Int_StartEx(Timer_Int_Handler2);
    Timer_1_Start();

    for (;;)
    {
    }
}