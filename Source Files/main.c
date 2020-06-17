//Obligatorio 1 microprocesadores Diego Marvid y Valentin Otte.


#include <xc.h>
#include "constantes.h"
#include "cmd.h"	

#define SBIT_PS1 5
#define SBIT_PS0 4



__EEPROM_DATA(0xFF,0xFF,0xFF,0xFF,0xFF,0x1,0xFF,0x0A); //fill first 8 locations
__EEPROM_DATA(0x30,0x01,0xFF,0x50,0x00,0x60,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x00,0x45,0x00,0x00,0x00,0x50); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x20,0x00,0x10,0x00,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x01,0x00,0x50,0x00,0x60,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x00,0x45,0x00,0x00,0x00,0x50); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x20,0x00,0x10,0x00,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x01,0x00,0x50,0x00,0x60,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x00,0x45,0x00,0x00,0x00,0x50); //fill first 8 locations




//---------------------------------EEPROM READ-----------------------------------------//
unsigned char eeprom_read(unsigned char direccion){
    EEADR = direccion;      //direccion de la eeprom que se quiere leer
    EECON1bits.EEPGD = 0;   //para acceder a la memoria EEPROM PIC
    EECON1bits.RD = 1;      //habilita la lectura de la EEPROM PIC
    return EEDATA;        //en el registro EEDATA se encuentra el dato
}

//---------------------------------EEPROM WRITE-----------------------------------------//

void eeprom_write(char Address, char Data)
{
   while(EECON1bits.WR==1);  //mientras se esté realizando la escritura espera
   EECON1bits.WR==1;  //mientras se esté realizando alguan escritura
                                      //anterior espera
    EEADR=Address;          //se carga la dirección donde se quiere escribir
    EEDATA=Data;              //se carga el dato que se quiere escribir
    EECON1bits.EEPGD=0;       //para acceder a la memoria EEPROM PIC
    EECON1bits.WREN=1;        //habilita la escritura en la EEPROM PIC
    INTCONbits.GIE=0;         //deshabilita las interrupciones
    EECON2=0x55;              //cargar en EECON2 0x55
    EECON2=0xaa;              //cargar en EECON2 0xaa
    EECON1bits.WR=1;          //inicia la escritura en la EEPROM PIC
    INTCONbits.GIE=1;         //habilita las interrupciones
    EECON1bits.WREN=0;        //deshabilita la escritura en la EEPROM PIC
    while(EECON1bits.WR==1);  //mientras se esté realizando la escritura espera
   
      
}


//---------------------------CHECK SUM--------------------------------//
//Chequea que el dato entrate cumpla con el checksum.
int check_sum(char *s){
    
    int suma = 0;
    //Suma los primeros valores de ASCII->int
    for(int j = 0; j < 7; j++) {
            suma += s[j];          
    }
    //Retorna el resultado de igualar logigamente la suma de los 7 valores con el check_sum
    return (suma % 10 == s[7] - 0x30);
    
    
}

//-------------------CLEAR SERIAL DATA--------------------//
//Limpia el dato serial actual.
void limpiar_dato(void){
    
    for(int j = 0; j < 8; j++) {
        dato[j] = 0;          
    }
    
}

void limpiar_cmd(void){
    
    for(int j = 0; j < 8; j++) {
        cmd[j] = 0;          
    }
    
}


char x;
//-------------------- INTERRUPTION MANAGER--------------------------//
void __interrupt () int_usart(void) {
    
  //Se genera una interrupcion debido a baja en tension de la fuente. 
   if( INTF == 1 ) {
      
      //Activa la bandera de finalizacion del programa.
      finalizar = TRUE;
      //Baja la bandera de la interrupcion.
      INTF = 0;
      //Guardo el precio total
      PORTB=0xFF;
      PORTD=0xFF;
      
      //Guarda en memoria el precio del lote actual
     int  precio_lote = lote.precio_total;
     eeprom_write(104, (char) (precio_lote/10000));
     precio_lote=precio_lote%10000;
     eeprom_write(103, (char) (precio_lote/1000));
     precio_lote=precio_lote%1000;
     eeprom_write(102, (char) (precio_lote/100));
     precio_lote=precio_lote%100;
     eeprom_write(101, (char) (precio_lote/10));
     precio_lote=precio_lote%10;
     eeprom_write(100, (char) precio_lote);
 
      //Guardo numero de lote.
       eeprom_write(106, lote.numero);
      //Guardo cant sesiones
       eeprom_write(107, lote.cant_ventas);
       //Activo flag para levantar datos
       eeprom_write(108, TRUE);
     

      
   }
   
   //Ocurre una interrupcion por el timer
   if ( TMR1IF == TRUE ){
      
      //Limpia el valor del timer
      TMR1H = 0;
      TMR1L = 0;
     
      //Si ya sucedio un overflow dos veces, entonces paso el tiempo suficiente.
      if(timer_overflow == 2){
	    //Se activa la bandera indicando que se cronometro el tiempo deseado
	    paso_segundo = TRUE;
	    //Se limpia el contador de overflows
	    timer_overflow = 0;
      } else {
	 //Si no sucedeio la cantidad necesaria, aumento el overlfow counter.
	  timer_overflow++;	 
      }
      
      //Bajo la bandera de interrupcion por timer.
      TMR1IF = FALSE;
      
   }
 
    //Me fijo si la interrupcion es por Recepcion serial RX
    if(PIR1bits.RCIF) {
     
    if(FERR  != TRUE ) {
      
       //Para el primer dato diferencio el caso numerico de las letras
       if (i == 0){
	  
	  //Si es numerico indico que el dato debe ser procesado como codigo de barras.
	  if( RCREG > 47 && RCREG < 58 ){
	       es_cmd = FALSE;
	  //De lo contrario se procesa como comando.
	  } else {
	       es_cmd = TRUE;
		
	  }
	  
       }
       
       //Caso codigo de barras
       if (es_cmd == FALSE){
	  
	    //Verifico si llego el final del dato
	     if(RCREG == CR || RCREG == LF){
		 
		 //Si llego el final en la posicion correcta entonces el dato se recibio exitosamente
		 if(i == 8) {
		     
		    
		    //Reinicio el contador de posicion para la proxima lectura.
		     i = 0;
		    
		     //Si cumple cheksum
		     if(check_sum(dato) == TRUE){
			 dato_recibido = TRUE;
		     //Sino limpio dato y dato_recibido = FALSE
		     }else{              
			 limpiar_dato(); 
		     }    
			   
		 } else {    
		     //Reinicio el contador de posicion para la proxima lectura.
		     i = 0;
		     //Limpiar dato
		     limpiar_dato(); 
		 }
		 
	     }  
	     //Agrego el dato
	     else if( i < 8 ) {
		 
		 dato[i] = RCREG;
		 i++;
		 
	     //No es un enter en la posicion correcta ni un numero dentro de los 8 bytes    
	     } else {
		//Reinicio el contador de posicion para la proxima lectura.
		 i = 0;
		 //Limpiar dato
		 limpiar_dato(); 
	     }
		   

       }else{
	  //Mientras que haya espacio en el buffer
	  if (i < 8){
	     
	     //Si se termina el comando.
	       if(RCREG == CR || RCREG == LF){
		  
		 // if(TRUE == TRUE){
		  //Guardo el enter y activo la bandera de nuevo codigo.
		     cmd[i] = RCREG;
		     cmd_recibido = TRUE;
		  //Reinicio el contador de datos.
		     i = 0;
		
	    //Si no llego el final se guarda el dato.
	       }else{
		  cmd[i] = RCREG;
		  i++;
	       }  
	
	//Si el supuesto comando posee mas de 8 caracteres  
	  }else{
	  //Se limpia el comando y se reinicia el contador.
	    i  = 0;
	   limpiar_cmd(); 
	  }  
      }
       
    }else{
        x=RCREG;
	es_cmd = FALSE;
        dato_recibido = FALSE;
        i = 0;
    }
 }

} 
  
//------------------------------------------------------------------------------------------------------------//

//----------------------------------------STOCK CHECK-----------------------------------------//
//Chequea que el item sea parte del stock, devuelve precio o 0 si error.
int check_tt(Codigo *codigo) {
   
    int TT = 0;
    //DATO = TTIIIIIC --> TT = (dato[1]-48) + (dato[0]-48)*10
   //Se resta 48 para convertir de ASCII a numero.
   
   //Si recibe NULL entonces la funcion actua con el dato obtenido de serial.
    if(codigo ==  (Codigo*) NULL) {
    
        TT = (dato[1]-48) + (dato[0]-48) * 10;
    
    //De lo contrario realiza la operacion con el codigo que se le pasa a la funcion.
    } else{
        
        TT = (codigo->dato[1]-48) + (codigo->dato[0]-48) * 10;
        
    }    
    
    //retorna el precio encontrado en la EEPROM
    return (int) eeprom_read(TT);
        
}


//----------------------------------- ITEM REPETITION CHECK---------------------------------------//
int check_rep(void) {
    
    //Contador de elementos iguales
    int h = 0;
    
    //Me fijo si un item coincide en los 7 elementos que deberian coincidir
    //Si el contador da 7 entonces tiene 7 elementos iguales y significa que
    //el elemento se encuentra repetido.
    
   //Recorro el carrito
    for(int t = 0; t < cantidad_items; t++){
       
      //Comparo posicion a posicion el nuevo codigo con los codigos que estan ingresados en el carrito.
        for(int j = 0; j < 7; j++) {
            
            if( dato[j] == lista[t].dato[j] ) {
                h++;   
            }
            
        }
	
	//Si son iguales entonces retorno verdadero.
        if(h == 7) {
            return TRUE;   
	   
	 //De lo contrario sigo buscando.  
        } else {
            h = 0;   
        }
   
        
    }
    
    //Si recorri todo el carrito y ninguno coincide entonces retorno falso.
    return FALSE;
    
}


//-----------------------------------------REFRESH ITEM---------------------------------------------//
//Dado que primero verifico si hay lugar, agrego el dato y luego aumento el contador, cantidad de items no debe ser modificada
//para esta logica, pues para el primer items C.I=0, para el segundo es C.I=1, lo que hace el mapeo a la lista mas simple. (C.I=cantidad items)
void agregar_dato(void){     
    
        for (int j = 0; j < 8; j++){
            
            lista[cantidad_items].dato[j] = dato[j];
	 
        }
   
}    

//---------------------------REFRESH LIST---------------------------------//

int agregar_lista() {
    //Si hay espacio disponible agrega el item al carrito y retorna true.
    if(cantidad_items < MAX_CART) {
        
        agregar_dato();
                
        return TRUE;
     
    //De lo contrario retorna false.   
    } else {
        return FALSE;
    }
    
}


//-------------------------------------REFRESH DISPLAY--------------------------------------//

//Dado un numero de dos digitos, muestra el mismo en dos display de 7 segmentos.
void display_numero(int numero){
    //Array con la representacion de los numeros en 7 segmentos.
    unsigned char seg[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
    
    //Calculo decenas y unidades.
    int decenas = numero / 100;
    int unidades = numero / 10;
    unidades = unidades % 10;
    
    int decimas=numero-decenas*100-unidades*10;
    
    //Asigno la representacion del numero en 7 segmentos al puerto correspondiente.     
    PORTD = decimas * 16 + unidades;
    PORTB = decenas * 16;
}

//----------REMOVE LAST ITEM---------------//
void eliminar_item(){
    
    int precio_item_actual = 0;
     
    // Si hay items para eliminar.  
    if(cantidad_items > 0){
         
	 //Obtiene el precio del ultimo item, sustrae el precio al total y disminuye la cantidad de items en uno.
        precio_item_actual = check_tt(&lista[cantidad_items - 1]);
        precio_total -= precio_item_actual;
	display_numero(precio_total);
        cantidad_items--;
        precio_actual=precio_item_actual;
       
	//Led verde.
       encender_verde();
        
    //Si no hay items da error.    
    }else{ 
        //Led rojo de error
        encender_rojo();
    }    
    
}    

//-------------CLOSE CART ANIMATION---------------//
//Genera una animacion al momento de cerrar el carrito.
void animacion_pagar(){
   
   for(int j=0; j<3;j++){
    encender_verde();
    encender_rojo();
   }
    
}   


//------------------------------LOTES---------------------------------//
int actualizar_lote( int precio){
   
   if(lote.cant_ventas < 60){
      lote.precio_total += precio;
      lote.cant_ventas++;
      return TRUE;
   }else {
      return FALSE;
   }  
      
   
 }

 //-----------------------------DATA TRANSMISION------------------------------------//
 
 //Envio de un caracter.
 void envia_caracter_usart(unsigned char caracter){
    while(TXSTAbits.TRMT==0);// mientras el registro TSR esté lleno espera
	TXREG = caracter;//cuando el el registro TSR está vacio se envia el caracter
}

//Transmision de un string.
void cmd_printf(const char *s) {
   
   int h = 0;
   
   while(s[h] != 0 && h < CMD_SIZE) {
	 
      envia_caracter_usart(s[h]);
      h++;
      
   }
	 
}


//---------------TIMER INITIALIZER----------------//
void setup_timer(void){
   
    T1CON = (1<<SBIT_PS1) | (1<<SBIT_PS0); // Timer0 with external freq and 8 as prescalar
    TMR1H=0x0;     // Load the time value(0xBDC) for 100ms delay
    TMR1L=0x0;       
    TMR1IE=1;       //Enable timer interrupt bit in PIE1 register
    GIE=1;          //Enable Global Interrupt
    PEIE=1;         //Enable the Peripheral Interrupt       
  
}


//--------------OBTENER PRECIO EEPROM-----------------------//
int obtener_precio_EEPROM(){
   

      int precio = 0;
      int aux = 1;
      
      for (char p=0; p < 5 ; p++){
	 
	 precio += ((int) eeprom_read(100+p))*aux;
	 aux*=10;
	  
      }
      
   
   return precio;
   
} 


//------------SETUPS---------//
void setup(void) {
    
    //---Displays----//
    
    //Decenas
    //TRISB = 0x0F;
   TRISB=0b1;
    
    //Unidades
    TRISD = 0x0;
   
    //Botones
    TRISA = 0b100111;
   
     ADC_Initialize();
   
     TRISE = 0x0;
    
    //----Serial----//
    
    TRISCbits.TRISC7 = ENTRADA; // Pin RX entrada digital
    TRISCbits.TRISC6 = 0;//pin TX como una salida digital
    
    //SPEN && CREN
    RCSTA = 0b10010000;
    TXSTA = 0b00100110;
    
    //Velocidad de transimision de baudios
    SPBRG = 25; //9615 baudios/s
    
    //----Interrupciones----//
    INTCON = 0b11000000; //Habilita todas las interrupciones y las perifericas
    
    PIE1bits.RCIE = 1; //Habilita interrupcion serial de recepcion en perisferico RX
    
    INTE = 1;       //Enable RB0 as external Interrupt pin
    
    //Si no hubo una falla de voltaje
    if ( eeprom_read(108) == FALSE){
       //Inicializo en 0 los valores del lote.
       lote.numero = 0;
       lote.cant_ventas = 0;
       lote.precio_total = 0;
    //Si hubo una falla de voltaje
    }else{
       //Levanto de memoria los valores del lote.
       eeprom_write(108, FALSE);
       lote.numero = eeprom_read(106) ;
       lote.cant_ventas = eeprom_read(107) ;
       lote.precio_total = obtener_precio_EEPROM();
          
    }  

    //Siempre se inicia una nueva compra, no guarda el carrito.
    display_numero(0);
   
    setup_timer();
    
       RE0 = 0;
    
}
   

int main(void) {
   
    setup();
    //----------//
     //Variables para almacenar el resultado de las condiciones. 
    int condicion_elemento_repetido = FALSE;
    int condicion_precio_max = FALSE;
  
 
   
   
    while(finalizar == FALSE) {
       
       if (OERR == TRUE ){
	  CREN=0;
	  CREN=1;
       }
 
	 //Si se obtiene un nuevo dato serial.
      if(dato_recibido == TRUE) {
	 
	     estado = ACTIVO;
	     
           	    
	    //Chequea si el item es valido, no esta repetido y su precio al ser sumado no supera los 99.9$
            precio_actual = check_tt(NULL);
            condicion_elemento_repetido = precio_actual != 0 && check_rep() == FALSE;
            condicion_precio_max = precio_total + precio_actual < 999;
             
             //	Si cumplen todas las condiciones anteriores. 
            if(condicion_elemento_repetido && condicion_precio_max && agregar_lista()) {
             
		  //Si se esta en modo debug, se avisa al administrador la lectura de un nuevo codigo.
		  if(modo == DEBUG) {
		     cmd_etiqueta();
		     cmd_printf(respuesta);
		  }
	              
                //Actualizo precio y display.
                precio_total += precio_actual;
                display_numero(precio_total);
                cantidad_items++;
	        
                
                //Led verde.
                RA3 = 1;
                __delay_ms(500);
                RA3 = 0;
                __delay_ms(500);

                
            }else {
                
                //Led rojo de error
                RE0 = 1;
                __delay_ms(500);
                RE0 = 0;
            }    
            
            dato_recibido = FALSE;
               
        }        
        
        //Si se desea eliminar el ultimo item.
        else if(RA1 == TRUE) {
            
            while(RA1) {
                
            }
            
            __delay_ms(50);
            
            eliminar_item();
	    
	      
        }
        
        //Si se desea cerrar el carrito actual.   
        else if(RA2 == TRUE) {
            
            while(RA2) {
                
            }
            
            __delay_ms(50);
	    
	    //Si cuando se actualiza el lote se sobrepasan las 255 sesiones
	    if (actualizar_lote(precio_total) == FALSE){
	       //Enciende led rojo avisando al usuario.
	       encender_rojo();
	       
	    //Si la cantidad de sesiones esta dentro del limita    
	    } else {
	       //Si se esta en modo debug se avisa el cierre de carrito.
	       if(modo == DEBUG){
		  cmd_lote();
		  cmd_printf(respuesta);
	       }
	       //Se indica al usuario el exito de la transaccion 
	        animacion_pagar();
	    }
            
            //Reinicio el carrito.
            precio_total = 0;
            cantidad_items = 0;
            //Resetea el display.
            display_numero(precio_total);
	     estado = ESPERA;
       
	//Si se recibe un comando debe de ser administrado.
        }else if(cmd_recibido == TRUE){  
	        //Se crea la respuesta apropiada para el pedido y es enviada al admin
	       encender_azul();
	       administrar_cmd();
	       cmd_printf(respuesta);	
	       //Se baja la bandera, indicando que ya fue procesado el comando.
	       cmd_recibido = FALSE;
	
	 //Si se cierra el lote debe de ser administrado.   
	} else if(RA5 == TRUE) {
	   
	   while(RA5) {
                
            }
            
            __delay_ms(50);
	    
	    //Genera el mensaje de cierre de lote.
	    if (lote.numero < 255){
	       cmd_cierre_lote();
	      
	       //Transmite el mismo.
	       cmd_printf(respuesta);
	      

	       //Inicializa el conteo de tiempo.
	       if(TMR1ON == FALSE){
		   espero_respuesta = TRUE;
		  //Activa el timer y la flag de esperar respuesta.
		  TMR1ON = TRUE;
		 
	       }
	       
	    }else{
	       	   encender_rojo();	
	    }
	
	//Si se esta esperando respuesta y paso el tiempo limite se muestra la falla.    
	} else if( espero_respuesta == TRUE  && paso_segundo == TRUE ){
	   
	   //Desactivo las banderas de esperar respuesta.
	   espero_respuesta = FALSE;
	   //Reinicio el valor de control de tiempo.
	   paso_segundo = FALSE;
	   //Apago y reinicio los valores del timer.
	   TMR1H = 0;
	   TMR1L = 0;
	   TMR1ON = FALSE;
	   //avisa al usuario de error.
	   encender_rojo();	
	   
	}  
             
    }

    
    return 0;
}
