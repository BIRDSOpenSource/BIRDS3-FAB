
//________________________READ VOLTAGE CHANNELS OF ADC_______________________________________________________________________________________//

unsigned int16 READ_vol_CHANNEL(int16 Address)
{
  
    RC2 = 0;
    
    spi_xfer(PORT2, Address);
                
    unsigned int16 vol_X = spi_xfer(PORT2);
         
    unsigned int16 adc_res16 = vol_X & 0x0FFF; //removing the address bits
                                
    RC2 = 1;
      
    delay_us(20);
    
    return adc_res16;
}

//________________________READ TEMPERATURE CHANNEL OF ADC_________________________________________________________________________________________________________//


int16 READ_temp_CHANNEL(int16 Address)
{
  
   // RC2 = 0;
                      
   spi_xfer(PORT2, Address);
   
   unsigned int16 temp_X = spi_xfer(PORT2);
    
   unsigned int16 adc_temp16 = temp_X & 0x0FFF; //removing address bits
               
    //RC2 = 1;
    
    delay_us(20);
    
    return adc_temp16;
}
//________________________CALIBRATE TEMPERATURE CHANNEL OF ADC_________________________________________________________________________________________________________//

void temperature(void) {
                  
         
          temp_pX = READ_temp_CHANNEL(pX_temp);
          
          fabData[1] = (temp_pX >> 8) & 0xFF;
          fabData[2] = temp_pX & 0x00FF;                                      
    //-------------------------------------------------------//      
          temp_mY = READ_temp_CHANNEL(mY_temp);
          
          fabData[3] = (temp_mY >> 8) & 0xFF;
          fabData[4] = temp_mY & 0x00FF;       
     //-------------------------------------------------------//       
          temp_mZ = READ_temp_CHANNEL(mZ_temp);
          
          fabData[5] = (temp_mZ >> 8) & 0xFF;
          fabData[6] = temp_mZ & 0x00FF;       
    //-------------------------------------------------------//     
          temp_pY = READ_temp_CHANNEL(pY_temp);
          
          fabData[7] = (temp_pY >> 8) & 0xFF;
          fabData[8] = temp_pY & 0x00FF;       

    //-------------------------------------------------------//              
//!          temp_mX = READ_temp_CHANNEL(mX_temp);
//!          
//!          fabData[9] = (temp_mX >> 8) & 0xFF;
//!          fabData[10] = temp_mX & 0x00FF;       
    //-------------------------------------------------------//         
          temp_CPLD = READ_temp_CHANNEL(CPLD_temp);
          
          fabData[9] = (temp_CPLD >> 8) & 0xFF;
          fabData[10] = temp_CPLD & 0x00FF;       
    //-------------------------------------------------------//         
//!          temp_pZ1 = READ_temp_CHANNEL(pZ_temp1);
//!          
//!          fabData[13] = (temp_pZ1 >> 8) & 0xFF;
//!          fabData[14] = temp_pZ1 & 0x00FF;       

    //-------------------------------------------------------//         
          temp_pZ2 = READ_temp_CHANNEL(pZ_temp2);
          
          fabData[11] = (temp_pZ2 >> 8) & 0xFF;
          fabData[12] = temp_pZ2 & 0x00FF;  
          
          return;
}

//________________________CALIBRATE VOLTAGE CHANNEL OF ADC_________________________________________________________________________________________________________//

void voltages(void) {

          vol_pX = READ_vol_CHANNEL(pX_vol);
          
          fabData[13] = (vol_pX >> 8) & 0xFF;
          fabData[14] = vol_pX & 0x00FF;                             
    //-------------------------------------------------------//   
          vol_mY = READ_vol_CHANNEL(mY_vol);
          
          fabData[15] = (vol_mY >> 8) & 0xFF;
          fabData[16] = vol_mY & 0x00FF;       
     //-------------------------------------------------------//       
          vol_mZ = READ_vol_CHANNEL(mZ_vol);
          
          fabData[17] = (vol_mZ >> 8) & 0xFF;
          fabData[18] = vol_mZ & 0x00FF;       
          
    //-------------------------------------------------------//     
          vol_pY = READ_vol_CHANNEL(pY_vol);
          
          fabData[19] = (vol_pY >> 8) & 0xFF;
          fabData[20] = vol_pY & 0x00FF;       
                    
    //-------------------------------------------------------//      
//!          vol_mX = READ_vol_CHANNEL(mX_vol);
//!          
//!          fabData[25] = (vol_mX >> 8) & 0xFF;
//!          fabData[26] = vol_mX & 0x00FF;       
//!
    //-------------------------------------------------------//         
          vol_pZ = READ_vol_CHANNEL(pZ_vol);
          
          fabData[21] = (vol_pZ >> 8) & 0xFF;
          fabData[22] = vol_pZ & 0x00FF;     
          
          return;
}

//________________________READ ADC CHANNEL OF FAB CURRENT_________________________________________________________________________________________________________//

int8 READ_PIC_ADC(int8 port_name)
{
   SET_ADC_CHANNEL(port_name); //  routing nth channel to adc
   delay_us(20);
   unsigned int16 ADC_VALUE = READ_ADC();   
   
   int8 bit8value = ADC_VALUE/16;
   
   return bit8value;
}

//=================================16bit value reading ADC===================================//

int16 READ_PIC_ADC16(int8 port_name)
{
   SET_ADC_CHANNEL(port_name); //  routing nth channel to adc
   delay_us(20);
   unsigned int16 ADC_VALUE16 = READ_ADC();   
     
   return ADC_VALUE16;
}

//________________________CREATE ARRAY OF FAB DATA_________________________________________________________________________________________________________//

void adc_FAB(void)
{

        fabData[23] = READ_PIC_ADC(0);         //I out plus X 
             
        fabData[24] = READ_PIC_ADC(1);         //I out minus Y

        fabData[25] = READ_PIC_ADC(2);         //I out minus Z 
      
        fabData[26] = READ_PIC_ADC(3);         //I out plus Y

        fabData[27] = READ_PIC_ADC(5);         //I out plus Z
 
        fabData[28] = READ_PIC_ADC(6);         // I_raw

        fabData[29] = READ_PIC_ADC(8);       // SRC_voltage

        fabData[30] = READ_PIC_ADC(9);         //Raw_voltage
        
 //============================ reading the SRC current ==================================// 
       
       for(int k=0; k<100; k++)
       {
       SET_ADC_CHANNEL(11); //  routing nth channel to adc
       delay_us(10);
        SRC_current = SRC_current + READ_ADC(); 
        delay_ms(1);
        }
        
       SRC_current= SRC_current/100;
       
       fabData[31] = (SRC_current >> 8)& 0xff;
       fabData[32] = SRC_current & 0x00ff;
       
       SRC_current = 0;
       
//==================================end of reading SRC Current==============================//
      
        fabData[33] = READ_PIC_ADC(10);  //battery voltage
      
//============================ reading the Battery current ==================================// 
       for(int j=0; j<100; j++)
       {
        SET_ADC_CHANNEL(12); //  routing nth channel to adc
        delay_us(10);
        bat_current = bat_current + READ_ADC(); 
        delay_ms(1);      
        }

       bat_current= bat_current/100;

       fabData[34] = (bat_current >> 8)& 0xff;
       fabData[35] = bat_current & 0x00ff;
       
       bat_current = 0; 
       
//==================================end of reading battery Current==============================//     
        
       fabData[36] = READ_PIC_ADC(13); //battery temperature
         
}

void battery_heater(){

if ((fabData[36]> 0xB0) && (fabData[33]> 0x93))
   {
   RD0 = 1;
   fabData[37] = 0x69;
   }
   
else
  {
   RD0 = 0;
   fabData[37] = 0;
  }

}

void kill_status(void)
{
   if(RD3 == 1 && RC1 == 0){
    
    fabData[38] = 0x10;
    
   }
   
    else if(RD3 == 1 && RC1 == 1){
    
    fabData[38] = 0x11;
    
    }
   
   else if(RD3 == 0 && RC1 == 1){
    
    fabData[38] = 0x01;

   }
   
   else{
    
    fabData[38] = 0x00;

   }
   
   return;
}
