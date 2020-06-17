
#ifndef CMD_H
#define CMD_H

#include "constantes.h"
char help[5];


// -----Funcion para convertir un int a varios char------//
void separar_int(unsigned int numero){
   
   
     help[4] = numero / 10000;
     numero = numero % 10000;
   
     help[3] = numero / 1000;
     numero = numero % 1000;  
   
     help[2] = numero / 100;
     numero = numero % 100;
   
     help[1] = numero / 10;
     numero = numero % 10; 
   
     help[0] = numero;
   
}
 //-----------------------LEDS----------------------------------//
  void encender_verde(){
    RA3 = 1;
    __delay_ms(LED_TIME);
    RA3 = 0;
    
 }
 
  void encender_rojo(){
     
     RE0 = 1;
    __delay_ms(LED_TIME);
    RE0 = 0;
    
 }

 
   void encender_azul(){
     
     RB1 = 1;
    __delay_ms(LED_TIME);
    RB1 = 0;
    
 }
 //----------------------------------------------------------------------//


//---------------------DEBUG MODE-----------------------//

void cmd_etiqueta(){
      
   respuesta[0] = 'E';
   respuesta[1] = '=';
   
   for(int j = 2; j < 11; j++) {
      
      respuesta[j] = dato[j-2];
      
   }
   
   respuesta[10] = 0;
   
}

void cmd_inicio_debug() {
   

   respuesta[0] = 'S';
   respuesta[1] = 'e';
   respuesta[2] = ' ';
   respuesta[3] = 'i';
   respuesta[4] = 'n';
   respuesta[5] = 'i';
   respuesta[6] = 'c';
   respuesta[7] = 'i';
   respuesta[8] = 'o';
   respuesta[9] = ' ';
   respuesta[10] = 'm';
   respuesta[11] = 'o';
   respuesta[12] = 'd';
   respuesta[13] = 'o';
   respuesta[14] = ' ';
   respuesta[15] = 'd';
   respuesta[16] = 'e';
   respuesta[17] = 'b';
   respuesta[18] = 'u';
   respuesta[19] = 'g';
   respuesta[20] = 0;
   

   
}

void cmd_fin_debug(){
   

   respuesta[0] = 'D';
   respuesta[1] = '=';
   respuesta[2] = '0';
   respuesta[3] = 0;
   
}

//-------------------------ESTADO------------------------------//
void cmd_estado(){
      
      if(estado == ACTIVO) {
	    

	 respuesta[0] = 'S';
	 respuesta[1] = 'e';
	 respuesta[2] = 's';
	 respuesta[3] = 'i';
	 respuesta[4] = 'o';
	 respuesta[5] = 'n';
	 respuesta[6] = ' ';
	 respuesta[7] = 'a';
	 respuesta[8] = 'c';
	 respuesta[9] = 't';
	 respuesta[10] = 'i';
	 respuesta[11] = 'v';
	 respuesta[12] = 'a';
	 respuesta[13] = 0;
	    
      } else {
	 respuesta[0] = 'S';
	 respuesta[1] = 'e';
	 respuesta[2] = 's';
	 respuesta[3] = 'i';
	 respuesta[4] = 'o';
	 respuesta[5] = 'n';
	 respuesta[6] = ' ';
	 respuesta[7] = 'e';
	 respuesta[8] = 'n';
	 respuesta[9] = ' ';
	 respuesta[10] = 'e';
	 respuesta[11] = 's';
	 respuesta[12] = 'p';
	 respuesta[13] = 'e';
	 respuesta[14] = 'r';
	 respuesta[15] = 'a';
	 respuesta[16] = 0; 
	 

	 
      }
   
}

//-----------------------INFO LOTE----------------------------//
void cmd_lote(){

   	 respuesta[0] = 'L';
	 respuesta[1] = '=';
	 //Numero de lote
	 separar_int((int) lote.numero);
   
	 respuesta[2] = help[2]+48;
	 respuesta[3] = help[1]+48;
	 respuesta[4] = help[0]+48;
	 respuesta[5] = ';';
	 respuesta[6] = 'N';
	 respuesta[7] = '=';
   		
	 //Cantidad de ventas
   	 separar_int((int) lote.cant_ventas);
   
   	 respuesta[8] = help[2]+48;
	 respuesta[9] = help[1]+48;
	 respuesta[10] = help[0]+48;
	 respuesta[11] = ';';
	 respuesta[12] = 'T';
	 respuesta[13] = '=';
	
	 //Precio de lote
         separar_int(lote.precio_total);
	 
	 respuesta[14] =  help[4]+48;
	 respuesta[15] =  help[3]+48;
	 respuesta[16] =  help[2]+48;
	 respuesta[17] =  help[1]+48;
	 respuesta[18] = help[0]+48;
	 respuesta[19] = 0;
   
}

//---------------OBTENER PRECIO ITEM-----------------------//
void cmd_precio(int tt, char T1, char T2){
      separar_int((int) eeprom_read(tt));
   
         respuesta[0] = 'P';
	 respuesta[1] = T1;
	 respuesta[2] = T2;
	 respuesta[3] = '=';
	 respuesta[4] = help[2]+48;
	 respuesta[5] = help[1]+48;
	 respuesta[6] = help[0]+48;
   	 respuesta[7] = 0;
   
   
}

//---------------OBTENER VALOR FUENTE-----------------------//
void cmd_fuente(int v){
   separar_int(v);
      	  respuesta[0] = 'V';
	 respuesta[1] = '=';
   	 respuesta[2] = help[2]+48;
   respuesta[3] = '.';
      	 respuesta[4] = help[1]+48;
         respuesta[5] = help[0]+48;
   respuesta[6] = 0;

   
}

//-----------------------CIERRE LOTE----------------------------//
void cmd_cierre_lote(){
   
   	 respuesta[0] = 'C';
	 respuesta[1] = 'i';
	 respuesta[2] = 'e';
	 respuesta[3] = 'r';
	 respuesta[4] = 'r';
	 respuesta[5] = 'e';
	 respuesta[6] = ',';
	 respuesta[7] = 'L';
	 respuesta[8] = '=';
   
   	 separar_int((int) lote.numero);
   
	 respuesta[9] = help[2]+48;
	 respuesta[10] = help[1]+48;
	 respuesta[11] = help[0]+48;
	 respuesta[12] = ';';
	 respuesta[13] = 'N';
	 respuesta[14] = '=';
   
   	 separar_int((int) lote.cant_ventas);
   
   	 respuesta[15] = help[2]+48;
	 respuesta[16] = help[1]+48;
	 respuesta[17] = help[0]+48;
	 respuesta[18] = ';';
	 respuesta[19] = 'T';
	 respuesta[20] = '=';
	 
         separar_int(lote.precio_total);
	 
	 respuesta[21] =  help[4]+48;
	respuesta[22] =  help[3]+48;
	 respuesta[23] =  help[2]+48;
	 respuesta[24] =  help[1]+48;
	 respuesta[25] = help[0]+48;
	 respuesta[26] = 0;
   
}

//----------------------VACIAR RESPUESTA-----------------//
void vaciar_respuesta(){
   
   for (int f = 0 ;f < CMD_SIZE; f++){
      respuesta[f] = 0;
   } 
   
}

//-----------------------IS NUMBER-----------------------//
int es_numero(char c){
   return(c < 58 && c > 47);
}

//----------------------I END ------------------------//

int es_fin(char c){
   
   return c == CR || c == LF ;
   
}

//-------------------------------------ANALOG READ-----------------------------------------------//


void ADC_Initialize()
{
  ADCON0 = 0b01000001; //ADC ON and Fosc/16 is selected
  ADCON1 = 0b11001110; // Internal reference voltage is selected
}

unsigned int ADC_Read(unsigned char channel)
{
  ADCON0  &= 0x11000101; //Clearing the Channel Selection Bits
  ADCON0  |= channel<<3; //Setting the required Bits
  __delay_ms(2); //Acquisition time to charge hold capacitor
  GO_nDONE = 1; //Initializes A/D Conversion
  while(GO_nDONE); //Wait for A/D Conversion to complete
  return ((ADRESH<<8)+ADRESL); //Returns Result
}




//-----------------------------------------------------------------------------------------------------------------//



//------------------------------------------- CMD ADMIN----------------------------------------------------------//
void administrar_cmd(){
   
   if(cmd[0] == '?'){
      
      //?
      if(es_fin(cmd[1])){
	 
	 cmd_estado();
	 
      // ?L
      }else if(cmd[1] == 'L' && es_fin(cmd[2])){
	 
	 cmd_lote();
	 
      //?V
      }else if(cmd[1] == 'V' && es_fin(cmd[2])){
	 
	 v = (ADC_Read(0) * 5) / 1023.0f; //Convertir bits a voltaje
	 v *= 2; //Divisor de tension 1/2 1/2
	 cmd_fuente((int)(v*100)); //Se multiplica por 100 para obtener 3 cifras en la conversion a char
	 
      //?26
      }else if(es_numero(cmd[1]) && es_numero(cmd[2]) && es_fin(cmd[3])){
	 
	 int tt =  (int) ( (cmd[1] - 48) * 10 + (cmd[2] - 48) );
	 
	 cmd_precio(tt, cmd[1] , cmd[2] );
	 
	 
      }else{
	 	 //LED ROJO; 
		 encender_rojo();
	         vaciar_respuesta();
      }	 
      
      
   } else if(cmd[0] == '+'){
      
            
      
      
      //+L
      if(cmd[1] == 'L'  && es_fin(cmd[2])){
	 
	 //Si viene del boton de cierre forzado, no mandar mensaje al admin porque el +L es su respuesta ya
	 if (espero_respuesta == TRUE){
	    vaciar_respuesta();
	    encender_verde();
	 }else{
	    cmd_cierre_lote();
	
	 }
	 
	 if (lote.numero < 255){
	    
	     lote.numero++;
	    
	    
	 } else if(lote.numero == 255){
	    
	    lote.numero = 0;

	 } 
	 

	 lote.cant_ventas = 0;
	 lote.precio_total = 0;
	 espero_respuesta = FALSE;
	 TMR1H = 0;
	 TMR1L = 0;
	 TMR1ON = FALSE;
	 
	 
	//+D 
      }else if(cmd[1] == 'D' && es_fin(cmd[2])){
	 
	 modo = DEBUG;
	 cmd_inicio_debug();
	 
      //+29=205
      }else if(cmd[3]== '=' && es_numero(cmd[1]) && es_numero(cmd[2]) && es_numero(cmd[4]) && es_numero(cmd[5]) && es_numero(cmd[6]) && es_fin(cmd[7])){
	 
	 int precio = (cmd[4] - 48) * 100 + (cmd[5] - 48) * 10 + (cmd[6] - 48);
	 int tt = (cmd[1] - 48) * 10+ ( cmd[2] - 48 );

	 if (precio > 255){
	    //LED ROJO;  
	    encender_rojo();
	    vaciar_respuesta();
	 }else{
	    eeprom_write((char) tt, (char) precio);    
	    cmd_precio(tt,cmd[1] ,cmd[2]);
         }
	 
      }else{
		 //LED ROJO;  
	  encender_rojo();
	  vaciar_respuesta();
      }
      
      
   }else if(cmd[0] == '-'){
      
      if(cmd[1] == 'D' && es_fin(cmd[2])){
	 
	 modo = NO_DEBUG;
	 cmd_fin_debug();
	 
      }else{
	 //LED ROJO; 
	 encender_rojo();
	 vaciar_respuesta();
      }
      
   }else{
     	 //LED ROJO;  
	 encender_rojo();
      vaciar_respuesta();
   }   
   
   
}



#endif	//MCD
