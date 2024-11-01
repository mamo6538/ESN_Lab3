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

uint8 filtered_L, filtered_R;
int16 edge_countS, edge_countH;

#define BITMASK 0x03 //bitmask for # values to sample in filter
#define COMP 0x0000FFFF

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
     //inits
    QuadDec_Start();
    LCD_Start();
    Clock_1_Start();
    
    //start the LCD printout
    LCD_Position(0,0);
    LCD_PrintString("Lab3: Rotary Encoder");
    LCD_Position(1,0);
    LCD_PrintString("                    ");
    
    edge_countS = 0;
    edge_countH = 0;
    
    //rotary encoder values pulled high by default
    uint8 enc_left = 0xFF;
    uint8 enc_right = 0xFF;
    filtered_L = 1; //start at 1
    filtered_R = 1; //start at 1
    uint8 isEdge = 0;

    while(1)
    {
        //read values into "storage" from encoder
        enc_left = (enc_left << 1) + P3_3_Read();
        enc_right = (enc_right << 1) + P3_5_Read();
        
        //find edge
        isEdge = (enc_left != 0xFF) || (enc_right != 0xFF);
        
        //update counters upon an edge
        if(isEdge) {
            //capture last value
            uint8 last_Left = filtered_L;
            uint8 last_Right = filtered_R;
            
            //filter:
            // With the specified samples (BITMASK) to filter debouncing
            // If it is a falling edge, the filtered value should drop 
            // If it is a rising edge, the filtered value should rise
            // Do nothing if not enough samples same
            if((enc_left & BITMASK) == BITMASK)
                filtered_L = 1;
            else if((enc_left & BITMASK) == 0)
                filtered_L = 0;
            
            if((enc_right & BITMASK) == BITMASK)
                filtered_R = 1;
            else if((enc_right & BITMASK) == 0)
                filtered_R = 0;
            
            //update output filtered values
            P12_3_Write(filtered_L);
            P12_5_Write(filtered_R);
            
            //Determine direction
            //first edge only
            uint8 tempsum = (last_Right << 3) | (filtered_R << 2) | (last_Left << 1) | filtered_L;
            if(tempsum == 0b1011){ //clockwise Right falls
                edge_countS++;
            }
            else if(tempsum == 0b1110) {//counter Left falls
                edge_countS--;
            }
            //else no change to counter
            
            //update counters
            edge_countH = QuadDec_GetCounter();
            
            
            //clear LCD position
            LCD_Position(1,0);
            LCD_PrintString("                    ");
            
            //print counts
            LCD_Position(1,4);
            if(edge_countS < 0) {
                LCD_PrintString("-");
                uint32 tempedge = COMP - edge_countS + 1;
                LCD_PrintNumber(tempedge);
            }
            else
                LCD_PrintNumber(edge_countS);
                
            LCD_Position(1,10);
            if(edge_countS < 0) {
                LCD_PrintString("-");
                uint32 tempedge = COMP - edge_countH + 1;
                LCD_PrintNumber(tempedge);
            } 
            else
                LCD_PrintNumber(edge_countH);
        }
    }
}

/* [] END OF FILE */
