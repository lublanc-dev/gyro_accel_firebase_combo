#include <EEPROM.h>                  // EEPROM library

int adreca;                          
byte valor;                         

void setup()
{
  Serial.begin(115200);             
  while (!Serial) {
    ;                               
  }
  EEPROM.begin(512);                 
  delay(3000);                       

  ax();                            
  ay();                              
  az();                              
  gx();                              
  gy();                              
  gz();                              

}

void loop()
{
      // No need loop code                             
}


void ax(){

  adreca = 1;                        
  
  Serial.println("Ax value");    
  
  while(adreca < 505){               
   
  valor = EEPROM.read(adreca);       
  Serial.print(valor, DEC);          
  Serial.println();

  adreca = adreca + 7;               // addres + 7, because there's six different variables saved (6) and one value that organize the row (1)
  
  delay(10);

  }
  }



void ay(){

  adreca = 2;                        
  
  Serial.println("Ay value");
  
  while(adreca < 506){                 
  
  valor = EEPROM.read(adreca);      
  Serial.print(valor, DEC);            
  Serial.println();

  adreca = adreca + 7;                 
  
  delay(10);

  }
  }


void az(){

  adreca = 3;   
  
  Serial.println("Az value");
  
  while(adreca < 507){     
  
  valor = EEPROM.read(adreca);   
  Serial.print(valor, DEC);     
  Serial.println();

  adreca = adreca + 7;      
  
  delay(10);

  }
  }



void gx(){

  adreca = 4;  
  
  Serial.println("Gx value");
  
  while(adreca < 508){    
  
  valor = EEPROM.read(adreca);   
  Serial.print(valor, DEC);    
  Serial.println();

  adreca = adreca + 7;   
  
  delay(10);

  }
  }


void gy(){

  adreca = 5;    
  
  Serial.println("Gy value");
  
  while(adreca < 509){      
  
  valor = EEPROM.read(adreca);   
  Serial.print(valor, DEC);     
  Serial.println();

  adreca = adreca + 7;      
  
  delay(10);

  }
  }


void gz(){

  adreca = 6;   
  
  Serial.println("Gz value");
  
  while(adreca < 510){       

  valor = EEPROM.read(adreca);   
  Serial.print(valor, DEC);     
  Serial.println();

  adreca = adreca + 7;      
  
  delay(10);

  }
  }
 
