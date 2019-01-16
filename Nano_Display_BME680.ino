/*

  GraphicsTest.ino
  
  Generate some example graphics

  Universal uC Color Graphics Library
  
  Copyright (c) 2014, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#include <SPI.h>
#include "Ucglib.h"
    

//Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 9, /*cs=*/ 10, /*reset=*/ 8);
Ucglib_ST7735_18x128x160_SWSPI ucg(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 9 , /*cs=*/ 10, /*reset=*/ 8);
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 4); //RX 4, TX 3

uint16_t temp1=0;
int16_t temp2=0;

unsigned char Re_buf[30],counter=0;
unsigned char sign=0;

float temp;
float hum;
float air;
float pres;
float air1;

void setup(void)
{
  delay(1000);
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.setFont(ucg_font_ncenR14_hr);
  ucg.clearScreen();
   Serial.begin(9600);  
  mySerial.begin(9600);
  mySerial.listen();  
  delay(4000);    
 
  mySerial.write(0XA5); 
  mySerial.write(0X55);    
  mySerial.write(0X3F);    
  mySerial.write(0X39); 
  delay(100); 

  mySerial.write(0XA5); 
  mySerial.write(0X56);    
  mySerial.write(0X02);    
  mySerial.write(0XFD);
  delay(100); 
}




void loop(void)
{
  float Temperature ,Humidity;
  unsigned char i=0,sum=0;
  uint32_t Gas;
  uint32_t Pressure;
  uint16_t IAQ;
  int16_t  Altitude;
  uint8_t IAQ_accuracy; 
  while (mySerial.available()) {   
    Re_buf[counter]=(unsigned char)mySerial.read();
    
    if(counter==0&&Re_buf[0]!=0x5A) return;         
    if(counter==1&&Re_buf[1]!=0x5A)
  {
    counter=0;
     return;
   };           
    counter++;       
    if(counter==20)               
    {    
       counter=0;                  
       sign=1;
    }      
  }
  if(sign)
  {  
     sign=0;
     
     if(Re_buf[0]==0x5A&&Re_buf[1]==0x5A )        
     {    
       
            for(i=0;i<19;i++)
               sum+=Re_buf[i]; 
             if(sum==Re_buf[i] ) 
             {
                     temp2=(Re_buf[4]<<8|Re_buf[5]);   
                     Temperature=(float)temp2/100;
                     temp1=(Re_buf[6]<<8|Re_buf[7]);
                     Humidity=(float)temp1/100; 
                     Pressure=((uint32_t)Re_buf[8]<<16)|((uint16_t)Re_buf[9]<<8)|Re_buf[10];
                     IAQ_accuracy= (Re_buf[11]&0xf0)>>4;
                     IAQ=((Re_buf[11]&0x0F)<<8)|Re_buf[12];
                     Gas=((uint32_t)Re_buf[13]<<24)|((uint32_t)Re_buf[14]<<16)|((uint16_t)Re_buf[15]<<8)|Re_buf[16];
                     Altitude=(Re_buf[17]<<8)|Re_buf[18]; 
                

//                     Serial.print(" ,Gas:"); 
//                     Serial.print(Gas );
//                     Serial.print(" Ohm"); 
//                     Serial.print("  ,Altitude:"); 
//                     Serial.print(Altitude);  
//                     Serial.print(" m");                      
//                     Serial.print("  ,IAQ_accuracy:"); 
//                     Serial.println(IAQ_accuracy);  


//                        
  ucg.setRotate90();
   ucg.setColor(255, 255, 255);
   ucg.drawFrame(2, 2, 95, 40);
   ucg.drawFrame(97, 2, 60, 40);
   ucg.drawFrame(2, 42, 75, 40);
   ucg.drawFrame(77, 42, 80, 40);
   ucg.drawFrame(2, 82, 155, 40);

  ucg.setFont(ucg_font_helvB10_hr);
  ucg.setPrintPos(7,17);
//  ucg.setColor(0, 80, 40, 0);
//  ucg.setColor(1, 60, 0, 40);
//  ucg.setColor(2, 20, 0, 20);
//  ucg.setColor(3, 60, 0, 60);
  ucg.setColor(0, 100, 10, 100);
  ucg.print("Temperature");
  if (Temperature != temp )
   {
    ucg.setColor(0, 0, 0, 0);
    ucg.drawBox(20, 22, 40, 15);
    ucg.setFont(ucg_font_helvB10_hr);
    ucg.setColor(0, 255, 255, 255);
    ucg.setPrintPos(24,35);
    temp = Temperature;
    ucg.print(Temperature); 
   }
  ucg.setColor(0, 255, 255, 255);
  ucg.setPrintPos(60,35);
  ucg.print(" 'C");

  ucg.setFont(ucg_font_helvB10_hr);
  ucg.setPrintPos(7,60);
  ucg.setColor(0, 255, 0, 0);
  ucg.print("Humidity");
  if (Humidity != hum )
   {
    ucg.setColor(0, 0, 0, 0);
    ucg.drawBox(10, 62, 40, 15);
    ucg.setFont(ucg_font_helvB10_hr);
    ucg.setColor(0, 255, 255, 255);
    ucg.setPrintPos(10,77);
    hum = Humidity;
    ucg.print(Humidity); 
   }
  ucg.setPrintPos(50,77);
  ucg.setColor(0, 255, 255, 255);
  ucg.print(" %");


  ucg.setFont(ucg_font_helvB10_hr);
  ucg.setPrintPos(100,60);
  ucg.setColor(0, 0, 0, 255);
  ucg.print("Gas");
  if (Gas != air1 )
   {
    ucg.setColor(0, 0, 0, 0);
    ucg.drawBox(80, 62, 50, 15);
    ucg.setFont(ucg_font_helvB10_hr);
    ucg.setColor(0, 255, 255, 255);
    ucg.setPrintPos(80,77);
    air1 = Gas;
    ucg.print(Gas); 
   }
  ucg.setPrintPos(130,77);
  ucg.setColor(0, 255, 255, 255);
  ucg.setFont(ucg_font_helvB08_hr);
  ucg.print(" ohm");
  
   ucg.setFont(ucg_font_helvB10_hr);
  ucg.setPrintPos(115,17);
  ucg.setColor(1, 150, 220, 255);
  ucg.setColor(0, 0, 100, 50);
  ucg.print("IAQ");
  if (IAQ != air )
   {
    ucg.setColor(0, 0, 0, 0);
    ucg.drawBox(109, 22, 27, 15);
    ucg.setFont(ucg_font_helvB10_hr);
    ucg.setColor(0, 255, 255, 255);
    ucg.setPrintPos(110,35);
    air = IAQ;
    ucg.print(IAQ); 
   }
  if (IAQ <= 50)
  {
  ucg.setColor(0, 0, 255, 0);
  ucg.drawDisc(142, 29, 5, UCG_DRAW_ALL);
  }
   if ((IAQ > 50) && (IAQ <= 100))
  {
  ucg.setColor(0, 0, 100, 100);
  ucg.drawDisc(142, 29, 5, UCG_DRAW_ALL);
  }
     if ((IAQ > 100) && (IAQ <= 150))
  {
  ucg.setColor(0, 80, 40, 0);
  ucg.setColor(1, 60, 0, 40);
  ucg.setColor(2, 20, 0, 20);
  ucg.setColor(3, 60, 0, 60);
  ucg.drawDisc(142, 29, 5, UCG_DRAW_ALL);
  }
   if ((IAQ > 150) && (IAQ <= 500))
  {
  ucg.setColor(0, 255, 0, 0);
  ucg.drawDisc(142, 29, 5, UCG_DRAW_ALL);
  }

  ucg.setFont(ucg_font_helvB10_hr);
  ucg.setPrintPos(50,100);
  ucg.setColor(0, 0, 255, 0);
  ucg.print("Pressure");
  if (Pressure != pres )
   {
    ucg.setColor(0, 0, 0, 0);
    ucg.drawBox(40, 102, 55, 15);
    ucg.setFont(ucg_font_helvB10_hr);
    ucg.setColor(0, 255, 255, 255);
    ucg.setPrintPos(40,115);
    pres = Pressure;
    ucg.print(Pressure/100.0F); 
   }
  ucg.setPrintPos(100,115);
  ucg.setColor(0, 255, 255, 255);
  ucg.print(" hPa");       
            
            
            
            }            
           delay(1000);           
   }
  } 

}
