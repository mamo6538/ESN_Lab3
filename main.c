/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
     //inits
    QuadDec_Start();
    LCD_Start();
    
    //start the LCD printout
    LCD_Position(0,0);
    LCD_PrintString("Lab3: Rotary Encoder");
    //LCD_Position(0,0);
    //LCD_PrintString("                    ");
    
    uint32 loop_count = 0;

    while(1)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
