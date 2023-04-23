#include <Wire.h>


#define ID_Slave 0x01

Adafruit_ADS1115 ads(0x48);         //Define i2c address 
Adafruit_MCP4725 dac;
 
int address[]={1,2,3,7,5};
float factorEscala = 0.1875F;
char data;  ////0x0 3 -> 0000 0011     ///0x 0 4 -> 0000 0100
char VChar[30];
bool finTrans = false;
double ref;
int i=0;
int j=0;
int k=0;
int n=0;
int t=0;
int time_ = 815;                 //used to change the delay needed depending on the command sent to the EZO Class pH Circuit.
String readString="";
const float multiplier = 0.0001827;     //Multiplier used for "current" read between ADC0 and ADC1 of the ADS1115 
const float multiplier_A2 = 0.0020645;
char computerdata={'r'};
byte code;
byte in_char = 0;
char ec_data[32];
float sensor[]={0,0,0,0,0};

void setup() {  // put your setup code here, to run once:
  Serial.begin(115200); 
  ads.setGain(GAIN_TWOTHIRDS);
  dac.begin(0x60);
  dac.setVoltage(0, false);

}

void loop() {  
  int k=0;  // put your main code here, to run repeatedly:  
  if(finTrans && VChar[0] == ID_Slave)  {   
    t=i;
    switch(VChar[1])  
    {
      case 'm': /////medicion        
        medicion();
      break;      
      ////////////////////////////////////////////////    
      case 's': /////settings
        settings();
      break;      
      ////////////////////////////////////////////////       
      case 'p': ////pruebas   
        test();      
      break;      
      ////////////////////////////////////////////////      
      default:        
        Serial.println(":X") ;
        readString="";
        i=0;
      break;
    }
    for(k=0;k<t;k++)        
    {          
      VChar[k]=0;
    }    
    finTrans=false;
    i=0;
    readString="";
  }
}

void medicion()
{
          for(k=1;k<i-1;k++)        
          {          
            readString += VChar[k];
          }  
          k=0;        
          ref= atof(readString.c_str()); 
  
}

void settings()
{
  for(k=2;k<i-1;k++)        
  {          
    ec_data[k-2]= tolower(VChar[k]);
  } 
  k=0;  
  int ts=(VChar[1])-48;
  if (VChar[2] == 'c' || VChar[2] == 'r')time_ = 815;             //if a command has been sent to calibrate or take a reading we wait 815ms so that the circuit has time to take the reading.
  else time_ = 250;                                                             //if any other command has been sent we wait only 250ms.
  
  Wire.beginTransmission(ts);                                            //call the circuit by its ID number.
  Wire.write(ec_data);                                                   //transmit the command that was sent through the serial port.
  Wire.endTransmission();                                                     //end the I2C data transmission.
  delay(time_);  
  Wire.requestFrom(ts, 32, 1);                                         //call the circuit and request 32 bytes (this could be too small, but it is the max i2c buffer size for an Arduino)
  code = Wire.read();                                                       //the first byte is the response code, we read this separately.
  switch (code) {                     //switch case based on what the response code is.
    case 1:                           //decimal 1.
      Serial.println("1");            //means the command was successful.
    break;                            //exits the switch case.
    case 2:                           //decimal 2.
      Serial.println("0");            //means the command has failed.
    break;                            //exits the switch case.
    case 254:                         //decimal 254.
      Serial.println("-1");           //means the command has not yet been finished calculating.
    break;                            //exits the switch case.
    case 255:                         //decimal 255.
      Serial.println("-2");           //means there is no further data to send.
    break;                            //exits the switch case.
  }
  while (Wire.available()) {          //are there bytes to receive.
    in_char = Wire.read();            //receive a byte.
    ec_data[k] = in_char;             //load this byte into our array.
    k += 1;                           //incur the counter for the array element.
    if (in_char == 0) 
    {                                 //if we see that we have been sent a null command.
      n=k;
      k=0;                            //reset the counter i to 0.
      break;                          //exit the while loop.
    }
  }
  
  Serial.println(ec_data);            //print the data.
  Serial.println();  
  for(k=0;k<32;k++)        
  {          
    ec_data[k]=0;
  }    
}

void test()
{
  Serial.println("Prueba de recepcion...");
}

void serialEvent(){  //Recepción de datos Seriales
    VChar[i]=(Serial.read());   ///¿Qué dato es?
    i++;    
    if(VChar[i-1]=='$')
    {
      finTrans=true;    
    }
        if(VChar[i-1]=='\n')
    {
      i=0;
    }
  }
