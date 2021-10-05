
#include <16F1789.h>
#device ADC=12;
#fuses WDT,NOMCLR
#use delay(internal=16MHz)
#include <PIC16F1789_registers.h>
#include <define_func.c>

//#use rs232(baud=9600,parity=N,xmit=PIN_B7,rcv=PIN_B6,bits=8,stream=PORT1)
#use rs232(UART1,baud=9600,parity=N,bits=8,stream=MPIC)


#use spi(MASTER, CLK = PIN_C3, DI = PIN_C4, DO = PIN_C5,ENABLE = PIN_C2, BAUD = 9600, BITS = 16, STREAM = PORT2, MODE = 2 )

int8 fabData[39] = {0};
unsigned int16 SRC_current = 0;
unsigned int32 bat_current = 0;

#include <ADC_reading_func.c>

BYTE rx_chars[8] = {0};

#int_rda
void serial_isr(){
 
  rx_chars[0] = fgetc(MPIC);
} 

void main()
{
  
   fabData[0] = 0x33;
   //RD0 = 1;
   //fabData[37] = 1;
  
   TRISD7 = 0; LATD7 = 1; //enabling buffer for voltage measurements
   
   TRISD0 = 0; RD0 = 0; //disabling battery heater
   
   check=read_eeprom(check_memory);
   kill_flag = read_eeprom(memory);
   
//!   kill_fab_forward(); //kill switch forward mode
//!   kill_OBC_forward();  

   if ((check!=8)&&(kill_flag!=5))
      {
        kill_fab_forward(); //kill switch forward mode
        kill_OBC_forward(); //kill switch OBC forward mode
      }
   else
      {     
        kill_fab_reverse(); //kill switch reverse mode
        kill_OBC_reverse(); //kill switch OBC reverse mode
      }
      
   
    SETUP_ADC(ADC_CLOCK_INTERNAL);
    SETUP_ADC_PORTS(ALL_ANALOG);  //setting all analog ports
   
    enable_interrupts(INT_RDA);
    enable_interrupts(global);   
    
    
   while(True)
   
   {     
         battery_heater();
         kill_status();

         if(rx_chars[0] == 0x61)//receiving sensor data request interrupt
         {             
           temperature();
           voltages();
           adc_FAB();
           
           rx_chars[0]=0;
            
           delay_ms(10);
                
           for (int i=0;i<39;i++)
           {
                  fputc(fabData[i],MPIC);                              
           }
         
          }
         
         
         else if(rx_chars[0] == 0x17){            //receiving Kill switch interrupt
            
            rx_chars[0]=0;
            
            delay_ms(10);
            
            kill_fab_reverse();
            kill_OBC_reverse();
            
            write_eeprom(check_memory,8);
            check=8;
            write_eeprom(memory,5);
            kill_flag=5;
            
         }
         
         else if(rx_chars[0] == 0x18){            //receiving Kill switch interrupt
            
            rx_chars[0]=0;
            
            delay_ms(10);
            
            kill_fab_forward();
            kill_OBC_forward();
            
            write_eeprom(check_memory,1);
            check=1;
            write_eeprom(memory,1);
            kill_flag=1;
            
         }
                               
  }   
}
  
