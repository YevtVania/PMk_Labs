#include "project.h"

int main(void)
{
    CyGlobalIntEnable; 
    SW_Tx_UART_Start();

    for(;;)
    {
        // 1. ПЕРЕВІРКА: Кнопку затиснуто (Button == 0)
        if(Button_Read() == 0) 
        {
            // Виводимо повідомлення в термінал Putty
            SW_Tx_UART_PutString("Ya vykonuyu laboratornu robotu #2\r\n");

            // САЛАТОВИЙ колір (R=0, G=0, B=1)
            LED_Write(0);   
            LED2_Write(0);  
            LED3_Write(1);  
            
            CyDelay(250); // Пауза для блимання (світло)

            // ВИМИКАЄМО (для мерцання)
            LED_Write(1); LED2_Write(1); LED3_Write(1);
            
            CyDelay(250); // Пауза для блимання (темрява)
        }
        // 2. Кнопка відпущена (Button == 1)
        else 
        {
            // Виводимо повідомлення в термінал Putty
            SW_Tx_UART_PutString("Knopka OFF\r\n");

            // ФІОЛЕТОВИЙ колір (R=0, G=1, B=0)
            LED_Write(0);   
            LED2_Write(1);  
            LED3_Write(0);  
            
            CyDelay(250); // Пауза для блимання (світло)

            // ВИМИКАЄМО (для мерцання)
            LED_Write(1); LED2_Write(1); LED3_Write(1);
            
            CyDelay(250); // Пауза для блимання (темрява)
        }
    }
}