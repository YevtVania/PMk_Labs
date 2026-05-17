#include "project.h"
#include <string.h>
void (*write_col[3])(uint8) = {COLUMN_0_Write, COLUMN_1_Write,
COLUMN_2_Write};
void (*set_col_dm[3])(uint8) = {COLUMN_0_SetDriveMode,
COLUMN_1_SetDriveMode, COLUMN_2_SetDriveMode};
uint8 (*read_row[4])(void) = {ROW_0_Read, ROW_1_Read, ROW_2_Read,
ROW_3_Read};
uint8_t key_map[4][3] =
{{'1','2','3'},{'4','5','6'},{'7','8','9'},{'*','0','#'}};
char password[] = "123456";
char input[10];
uint8_t input_idx = 0;
int main(void)
{
CyGlobalIntEnable;
SW_Tx_UART_Start();
for(int i = 0; i < 3; i++) set_col_dm[i](COLUMN_0_DM_DIG_HIZ);
uint8_t last_key = 0;
for(;;)
{
uint8_t current_key = 0;
for(int c = 0; c < 3; c++) {
set_col_dm[c](COLUMN_0_DM_STRONG);
write_col[c](0);
for(int r = 0; r < 4; r++) if(read_row[r]() == 0)
current_key = key_map[r][c];
write_col[c](1);
set_col_dm[c](COLUMN_0_DM_DIG_HIZ);
}
if(current_key != last_key && current_key != 0) {
if(current_key == '#') {
input[input_idx] = '\0';
if(strcmp(input, password) == 0) {
SW_Tx_UART_PutString("\nAccess allowed\r\n");
for(int i=0; i<999; i++) { LED_Write(1);
LED3_Write(1); }
} else {
SW_Tx_UART_PutString("\nAccess denied\r\n");
for(int i=0; i<999; i++) { LED2_Write(1);
LED3_Write(1); LED_Write(0); }
}
input_idx = 0;
} else if(input_idx < 9) {
input[input_idx++] = current_key;
SW_Tx_UART_PutChar('*');
}
}
last_key = current_key;
CyDelay(50);
}
}
