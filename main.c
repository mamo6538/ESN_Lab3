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
//clock is about 48MHz

uint8 enc_left, enc_right, eLtemp, eRtemp;
int16 edge_countS, edge_countH;

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
    
    uint32 loop_count = 0;
    edge_countS = 0;
    edge_countH = 0;
    
    //rotary encoder values pulled high by default
    enc_left = 0xFF;
    enc_right = 0xFF;
    eLtemp = 0xFF;
    eRtemp = 0xFF;
    uint8 isEdge = 0;

    while(1)
    {
        loop_count++;
        
        //read values in encoder
        enc_left = (enc_left << 1) + P3_3_Read();
        enc_right = (enc_right << 1) + P3_5_Read();
        
        //find edge
        uint8 leftEdge = enc_left != 0xFF;
        uint8 rightEdge = enc_right != 0xFF;
        isEdge = leftEdge || rightEdge;
        
        //update counter upon an edge
        if(isEdge) {
            //filter out extra edges
            eLtemp = enc_left & 0x03;
            eRtemp = enc_right & 0x03;
            uint8 tempsum = (eRtemp << 2) |  eLtemp;
            
            //need to test this
            if( (tempsum == 0b1011) || (tempsum == 0b0010) || (tempsum == 0b0100) || (tempsum == 0b1101)){
                edge_countS++;
            }
            else if( (tempsum == 0b1110) || (tempsum == 0b1000) || (tempsum == 0b0001) || (tempsum == 0b0111)){
                edge_countS--;
            }
            
            //update counters
            edge_countH = QuadDec_GetCounter();
            
            //clear LCD position
            LCD_Position(1,0);
            LCD_PrintString("                    ");
            //print turn count
            LCD_Position(1,5);
            LCD_PrintNumber(edge_countS);
            LCD_Position(1,13);
            LCD_PrintNumber(edge_countH);
        }
        
    }
}

/* [] END OF FILE */
