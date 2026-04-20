#include "project.h"

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

uint8_t keys[4][3];

uint8_t key_map[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

void printMatrix()
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            SW_Tx_UART_PutChar(keys[r][c] + '0');
            SW_Tx_UART_PutChar(' ');
        }
        SW_Tx_UART_PutCRLF();
    }
    SW_Tx_UART_PutCRLF();
}

int main(void)
{
    CyGlobalIntEnable;
    SW_Tx_UART_Start();

    for (int i = 0; i < 3; i++)
        set_col_dm[i](COLUMN_0_DM_DIG_HIZ);

    uint8_t last_key = 0;

    for (;;)
    {
        uint8_t current_key = 0;

        for (int c = 0; c < 3; c++)
        {
            set_col_dm[c](COLUMN_0_DM_STRONG);
            write_col[c](0); 

            for (int r = 0; r < 4; r++)
            {
                keys[r][c] = read_row[r]();
                if (keys[r][c] == 0)
                    current_key = key_map[r][c];
            }

            write_col[c](1);
            set_col_dm[c](COLUMN_0_DM_DIG_HIZ);
        }

        if (current_key != last_key)
        {
            if (current_key != 0)
            {
                SW_Tx_UART_PutString("Button pressed: ");
                SW_Tx_UART_PutChar(current_key);
                SW_Tx_UART_PutCRLF();

                printMatrix();

                switch (current_key)
                {
                    case '1':
                    case '7':
                        LED_Write(0);
                        LED2_Write(1);
                        LED3_Write(1);
                        break;

                    case '2':
                    case '8':
                        LED_Write(1);
                        LED2_Write(0);
                        LED3_Write(1);
                        break;

                    case '3':
                    case '9':
                        LED_Write(1);
                        LED2_Write(1);
                        LED3_Write(0);
                        break;

                    case '4':
                    case '*':
                        LED_Write(0);
                        LED2_Write(0);
                        LED3_Write(1);
                        break;

                    case '5':
                    case '0':
                        LED_Write(0);
                        LED2_Write(1);
                        LED3_Write(0);
                        break;

                    case '6':
                    case '#':
                        LED_Write(1);
                        LED2_Write(0);
                        LED3_Write(0);
                        break;
                }
            }
            else
            {
                SW_Tx_UART_PutString("Button released");
                SW_Tx_UART_PutCRLF();

                LED_Write(1);
                LED2_Write(1);
                LED3_Write(1);
            }

            last_key = current_key;
        }

        CyDelay(50);
    }
}