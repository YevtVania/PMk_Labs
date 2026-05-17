#include "project.h"

static uint8_t LED_NUM[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
    0xBF, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0x7F,
    0xFF
};

static uint8_t password[4] = {1, 2, 3, 4};
static uint8_t input[4] = {18, 18, 18, 18};

static uint8_t display_data[8] = {
    18, 18, 18, 18, 18, 18, 18, 18
};

static uint8_t key_map[4][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
    {16, 0, 17}
};

void (*write_col[3])(uint8) = {
    COLUMN_0_Write,
    COLUMN_1_Write,
    COLUMN_2_Write
};

void (*set_col_dm[3])(uint8) = {
    COLUMN_0_SetDriveMode,
    COLUMN_1_SetDriveMode,
    COLUMN_2_SetDriveMode
};

uint8 (*read_row[4])(void) = {
    ROW_0_Read,
    ROW_1_Read,
    ROW_2_Read,
    ROW_3_Read
};

static volatile uint8 led_counter = 0;
static uint8 input_counter = 0;

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

CY_ISR(Timer_Int_Handler2)
{
    Timer_1_ReadStatusRegister();

    FourDigit74HC595_sendOneDigit(led_counter, display_data[led_counter], 0);

    led_counter++;

    if (led_counter > 7)
    {
        led_counter = 0;
    }
}

static uint8_t getKey(void)
{
    for (uint8_t c = 0; c < 3; c++)
    {
        for (uint8_t i = 0; i < 3; i++)
        {
            set_col_dm[i](CY_SYS_PINS_DM_DIG_HIZ);
        }

        set_col_dm[c](CY_SYS_PINS_DM_STRONG);
        write_col[c](0);

        for (uint8_t r = 0; r < 4; r++)
        {
            if (read_row[r]() == 0)
            {
                CyDelay(20);

                while (read_row[r]() == 0)
                {
                }

                return key_map[r][c];
            }
        }
    }

    return 255;
}

static uint8_t checkPassword(void)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        if (input[i] != password[i])
        {
            return 0;
        }
    }

    return 1;
}

static void clearDisplay(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        display_data[i] = 18;
    }
}

static void correctPassword(void)
{
    for (uint8_t j = 0; j < 3; j++)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            display_data[i] = 8;
        }

        CyDelay(250);

        clearDisplay();

        CyDelay(250);
    }
}

static void wrongPassword(void)
{
    for (uint8_t j = 0; j < 3; j++)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            display_data[i] = 0;
        }

        CyDelay(150);

        clearDisplay();

        CyDelay(150);
    }
}

int main(void)
{
    CyGlobalIntEnable;

    Timer_Int_StartEx(Timer_Int_Handler2);
    Timer_1_Start();

    for (;;)
    {
        uint8_t key = getKey();

        if (key != 255)
        {
            if (key <= 9)
            {
                if (input_counter < 4)
                {
                    input[input_counter] = key;
                    display_data[input_counter] = key;
                    input_counter++;
                }
            }

            if (key == 17)
            {
                if (input_counter == 4)
                {
                    if (checkPassword())
                    {
                        correctPassword();
                    }
                    else
                    {
                        wrongPassword();
                    }
                }

                input_counter = 0;

                for (uint8_t i = 0; i < 4; i++)
                {
                    input[i] = 18;
                }

                clearDisplay();
            }

            if (key == 16)
            {
                input_counter = 0;

                for (uint8_t i = 0; i < 4; i++)
                {
                    input[i] = 18;
                }

                clearDisplay();
            }
        }
    }
}