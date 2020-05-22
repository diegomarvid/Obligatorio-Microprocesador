
#ifndef CMD_H
#define CMD_H

#include "constantes.h"
#include <stdio.h>

//Modo debug

void cmd_etiqueta(){
      
   respuesta[0] = 'E';
   respuesta[1] = '=';
   
   for(int j = 2; j < 11; j++) {
      
      respuesta[j] = dato[j-2];
      
   }
   
   respuesta[10] = 0;
   
   
   
}



void cmd_estado(){
      
      if(estado == ACTIVO) {
	    
	    sprintf(respuesta, "Sesion activa");
	    
      } else {
	 
	    sprintf(respuesta, "Sesion en espera");
	 
      }
   
}

void cmd_lote(){
   
      sprintf( respuesta, "L=%i;N=%i;T=%i", (int) lote.numero, (int) lote.cant_ventas, lote.precio_total );
   
}

void cmd_precio(int tt){

      sprintf( respuesta, "P%i=%i", tt, (int) eeprom_read(tt) );
   
}

void cmd_fuente(){
   
      sprintf( respuesta, "Fuente=%.2f Volts", v );
   
}

void cmd_cierre_lote(){
   
      sprintf( respuesta, "Cierre,L=%i;N=%i;T=%i", (int) lote.numero, (int) lote.cant_ventas, lote.precio_total );
   
}

void cmd_inicio_debug() {
   
      sprintf( respuesta, "Se inicio modo Debug" );
   
}

void cmd_fin_debug(){
   
      sprintf( respuesta, "D=0" );
   
}


//-----------------------IS NUMBER-----------------------//
int es_numero(char c){
   return(c < 58 && c > 47);
}


//------------------------------------------- CMD ADMIN----------------------------------------------------------//
void administrar_cmd(){
   
   if(cmd[0] == '?'){
      
      //?
      if(cmd[1] == CR || cmd[1] == LF ){
	 
	 cmd_estado();
	 
      // ?L
      }else if(cmd[1] == 'L'){
	 
	 cmd_lote();
	 
      //?V
      }else if(cmd[1] == 'V'){
	 
	 cmd_fuente();
	 
      //?26
      }else if(es_numero(cmd[1]) && es_numero(cmd[2]) ){
	 
	 int tt =  (int) ( (cmd[1] - 48) * 10 + (cmd[2] - 48) );
	 
	 cmd_precio(tt);
	 
      }else{
	 	 //LED ROJO; 
      }	 
      
      
   } else if(cmd[0] == '+'){
      
      //+L
      if(cmd[1] == 'L'){
	 
	 cmd_cierre_lote();
	 
	 //Esperar?
	 
	 lote.numero++;
	 lote.cant_ventas = 0;
	 lote.precio_total = 0;
	 
	//+D 
      }else if(cmd[1] == 'D'){
	 
	 modo = DEBUG;
	 cmd_inicio_debug();
	 
      //+29=205CH
      }else if(cmd[3]== '=' && es_numero(cmd[1]) && es_numero(cmd[2]) && es_numero(cmd[4]) && es_numero(cmd[5]) && es_numero(cmd[6])){
	 
	 int precio = (cmd[4] - 48) * 100 + (cmd[5] - 48) * 10 + (cmd[6] - 48);
	 int tt = (cmd[1] - 48) * 10+ ( cmd[2] - 48 );

	 if (precio > 255){
	    //LED ROJO;  
	 }else{
	    eeprom_write((char) tt, (char) precio);    
	    cmd_precio(tt);
         }
	 
      }else{
		 //LED ROJO;  
      }
      
      
   }else if(cmd[0] == '-'){
      
      if(cmd[1] == 'D'){
	 
	 modo = NO_DEBUG;
	 cmd_fin_debug();
	 
      }else{
	 //LED ROJO; 
      }
      
   }else{
     	 //LED ROJO;  
   }   
   
   
}



#endif	//MCD