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

//QUESTIONS:
//1) What to set the PGA?
//2) How to null? 
//3) what is "ADC_delta"? or "ADC_value"? A: ADC_delta is ADC Value meas - NULL value
//4) where do I print the g value? A: however I want

#include "project.h"
#include "stdio.h" //for string funcs

#define C_DAC_START 10
#define C_mV_V 0.6
#define C_STRAIN_MAX 100
#define C_VDD 5 //scope shows 4.74V
#define C_mV_MAX 3 //C_mV_V*C_VDD

int16 mV_ADC = -1; //init to be a negative saturation
uint8 dac_in = C_DAC_START;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    //starts
    UART_1_Start();
    PGA_1_Start();
    DAC_Start();
    OPAmp_Start();
    ADC_DelSig_1_Start();
    
    //setup up LCD just to keep wiring in place
    LCD_Char_1_Start();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("        ");
    
    //starter variables
    uint32 loop_count = 0;
    
    
    //Setup Headings with UART
    UART_1_PutCRLF('\0'); //This will create an empty line and set you to beginning of next line
    UART_1_PutCRLF('\0');
    UART_1_PutCRLF('\0');
    UART_1_PutString("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    UART_1_PutCRLF('\0');
    UART_1_PutString("       Strain Gauge (load cell) Lab       ");
    UART_1_PutCRLF('\0');
    UART_1_PutString("     dac       ADC_value      ADC_delta   ");
    UART_1_PutCRLF('\0');
    
    
    //set DAC to ?small? value
    DAC_SetValue(dac_in);
    
    //read ADC
    ADC_DelSig_1_StartConvert();
    ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT);
    int16 result = ADC_DelSig_1_GetResult16();
    mV_ADC = ADC_DelSig_1_CountsTo_mVolts(result);
    
    //TODO: see signal from load cell on the scope to see how this compares. 
    
    char printstring[42];
    sprintf(printstring, "     %d       %d      %d", dac_in,result,mV_ADC);
    UART_1_PutString(printstring);
    UART_1_PutCRLF('\0');
    
    //do NULLing
    do {
        //set DAC to ?small? value
        DAC_SetValue(dac_in);
        
        //read ADC
        ADC_DelSig_1_StartConvert();
        ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT);
        int16 result = ADC_DelSig_1_GetResult16();
        mV_ADC = ADC_DelSig_1_CountsTo_mVolts(result);
        
        //increase dac    
        dac_in++;
    } while(mV_ADC < 0);
    
    //do Reading + Calculations
    while(1)
    {
        loop_count++;
        /*
        //equation: mV_meas = C_mV_V*C_VDD (at max 100g)
        // so max mV_meas ~=5*.6 = 3mV  (meas/max = g/100)
        // g = mV_meas/3mV*100
        
        //read ADC
        ADC_DelSig_1_StartConvert();
        ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT);
        result = ADC_DelSig_1_GetResult16();
        mV_meas = ADC_DelSig_1_CountsTo_mVolts(result);
        
        //calculate strain weight
        float g = (float)mV_meas*C_STRAIN_MAX;
        g = g/C_mV_MAX;
        
        //print to terminal
        if(loop_count > 100) {
            sprintf(printstring, "g= %f, ADC_result=%d ", g,mV_meas);
            UART_1_PutString(printstring);
            UART_1_PutCRLF('\0');
            loop_count = 0;
        }
        */
    }
}

/* [] END OF FILE */
