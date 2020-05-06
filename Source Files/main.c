//Obligatorio 1 microprocesadores Diego Marvid y Valentin Otte.

#include <xc.h>
#include "constantes.h"

__EEPROM_DATA(0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x0A); //fill first 8 locations
__EEPROM_DATA(0x30,0x01,0x00,0x50,0x00,0x60,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x00,0x45,0x00,0x00,0x00,0x50); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x20,0x00,0x10,0x00,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x01,0x00,0x50,0x00,0x60,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x00,0x45,0x00,0x00,0x00,0x50); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x20,0x00,0x10,0x00,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x01,0x00,0x50,0x00,0x60,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x00,0x45,0x00,0x00,0x00,0x50); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x20,0x00,0x10,0x00,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x01,0x00,0x50,0x00,0x60,0x00,0x00); //fill first 8 locations
__EEPROM_DATA(0x30,0x00,0x00,0x45,0x00,0x00,0x00,0x50); //fill first 8 locations
//__EEPROM_DATA(0x30,0x00,0x00,0x45,0x00,0x00,0x00,0x50); //fill first 8 locations


//Obligatorio 1 microprocesadores Diego Marvid y Valentin Otte.

#include <xc.h>
#include "constantes.h"



//---------------------------CHECK SUM--------------------------------//
//Chequea que el dato entrate cumpla con el checksum.
int check_sum(void){
    
    int suma = 0;
    //Suma los primeros valores de ASCII->int
    for(int j = 0; j < 7; j++) {
            suma += dato[j] - 0x30;          
    }
    //Retorna el resultado de igualar logigamente la suma de los 7 valores con el check_sum
    return (suma % 10 == dato[7] - 0x30);
    
    
}

//-------------------CLEAR SERIAL DATA--------------------//
//Limpia el dato serial actual.
void limpiar_dato(void){
    
    for(int j = 0; j < 8; j++) {
        dato[j] = 0;          
    }
    
}


//-------------------- INTERRUPTION MANAGER--------------------------//
void __interrupt () int_usart(void) {
    
 
 //Me fijo si la interrupcion es por Recepcion serial RX
 
 if(PIR1bits.RCIF == TRUE) {
  
    //Verifico si llego el final del dato
    if(RCREG == CR || RCREG == LF){
        
        //Si llego el final en la posicion correcta entonces el dato se recibio exitosamente
        if(i == 8) {
            
	   
	   //Reinicio el contador de posicion para la proxima lectura.
            i = 0;
	   
            //Si cumple cheksum
            if(check_sum() == TRUE){
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
      
 }
}


//---------------------------------EEPROM READ-----------------------------------------//
unsigned char eeprom_read(unsigned char direccion){
    EEADR = direccion;      //direccion de la eeprom que se quiere leer
    EECON1bits.EEPGD = 0;   //para acceder a la memoria EEPROM PIC
    EECON1bits.RD = 1;      //habilita la lectura de la EEPROM PIC
    return EEDATA;        //en el registro EEDATA se encuentra el dato
}


//----------------------------------------STOCK CHECK-----------------------------------------//
//Chequea que el item sea parte del stock, devuelve precio o 0 si error.
int check_tt(Codigo *codigo) {
   
    int TT = 0;
    //DATO = TTIIIIIC --> TT = (dato[1]-48) + (dato[0]-48)*10
   //Se resta 48 para convertir de ASCII a numero.
   
   //Si recibe NULL entonces la funcion actua con el dato obtenido de serial.
    if(codigo ==  (Codigo*) NULL) {
    
        TT=(dato[1]-48)+(dato[0]-48)*10;
    
    //De lo contrario realiza la operacion con el codigo que se le pasa a la funcion.
    } else{
        
        TT=(codigo->dato[1]-48)+(codigo->dato[0]-48)*10;
        
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
    for(int t= 0; t< cantidad_items; t++){
       
      //Comparo posicion a posicion el nuevo codigo con los codigos que estan ingresados en el carrito.
        for(int j = 0; j < 7; j++) {
            
            if( dato[j] == lista[t].dato[j]) {
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
    
    //Asigno la representacion del numero en 7 segmentos al puerto correspondiente.     
    PORTD = ~seg[unidades];
    PORTB = ~seg[decenas];      
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
       RA3 = 1;
       __delay_ms(500);
       RA3 = 0;
       __delay_ms(500);
        
    //Si no hay items da error.    
    }else{ 
        //Led rojo de error
        RA0=1;
        __delay_ms(500);
        RA0=0;
    }    
    
}    

//-------------CLOSE CART ANIMATION---------------//
//Genera una animacion al momento de cerrar el carrito.
void animacion_pagar(){
   
   for(int j=0; j<3;j++){
    RA3=1;
    __delay_ms(300);
    RA3=0;
    RA0=1;
    __delay_ms(300);
    RA0=0;
   }
    
}   

//--------------------------------------GENERAR DATO-------------------------------------------------------//
char d1[8] = {48,49,79,84,84,69,46,36};
char d2[8] = {48,50,86,73,73,67,46,36};
char d3[8] = {48,51,79,84,84,69,46,36};
char d4[8] = {48,52,86,73,73,67,46,36};
char d5[8] = {48,55,86,73,73,67,46,36};

void generar_dato(int r){
        
        if(r%5 == 0){

            for (int j = 0; j < 8; j++){
                
                dato[j]=d1[j];
            
            }
            
        }else if (r%5 == 1){
            
            for (int j = 0; j < 8; j++){
                
                dato[j]=d2[j];
            
            }            
            
        }else if (r%5 == 2){
            
            for (int j = 0; j < 8; j++){
                
                dato[j]=d3[j];
            
            }            
            
        }   else  if (r%5 == 3){
            
            for (int j = 0; j < 8; j++){
                
                dato[j]=d4[j];
            
            }  
            
        } else if(r%5 == 4){
	   
	   for (int j = 0; j < 8; j++){
                
                dato[j]=d5[j];
            
            }  
	   
	   }
	
    
}    
//-----------------------------------------------------------------------------------------------------------------//

    
//------------SETUPS---------//
void setup(void) {
    
    //---Displays----//
    
    //Decenas
    TRISB = 0x0;
    
    //Unidades
    TRISD = 0x0;
    
      ADCON1 = 0b0111;
   
    //Botones
    TRISA = 0b0100110;
    
    
    //----Serial----//
    
    TRISCbits.TRISC7 = ENTRADA; // Pin RX entrada digital
    
    
    //SPEN && CREN
    RCSTA = 0b10010000;
    
    //Velocidad de transimision de baudios
    SPBRG = 25; //9615 baudios/s
    
    //----Interrupciones----//
    INTCON = 0b11000000; //Habilita todas las interrupciones y las perifericas
    
    PIE1bits.RCIE = 1; //Habilita interrupcion serial de recepcion en perisferico RX
    
    display_numero(0);
    
    
}
   

int main(void) {
    
    setup();
    //AUXILIAR PARA GENERAR ITEMS
    int r=0;
    //----------//
     //Variables para almacenar el resultado de las condiciones. 
    int condicion_elemento_repetido = FALSE;
    int condicion_precio_max = FALSE;
    
    while(TRUE) {
        
       
       
	 //Si se obtiene un nuevo dato serial.
        if(RA4 == TRUE) {
	   //--------------AUXILIAR---------------------//
	   while(RA4) {
                
            }
            __delay_ms(50);
            generar_dato(r);
            //---------------------------------------------//
	    
	    //Chequea si el item es valido, no esta repetido y su precio al ser sumado no supera los 99.9$
            precio_actual = check_tt(NULL);
            condicion_elemento_repetido = precio_actual != 0 && check_rep() == FALSE;
            condicion_precio_max = precio_total + precio_actual < 999;
             
             //	Si cumplen todas las condiciones anteriores. 
            if(condicion_elemento_repetido && condicion_precio_max && agregar_lista()) {
             
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
                RA0 = 1;
                __delay_ms(500);
                RA0 = 0;
            }    
            
            dato_recibido = FALSE;
            r++;
               
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
            
            //Reinicio el carrito.
            precio_total = 0;
            cantidad_items = 0;
            //Resetea el display.
            display_numero(precio_total);           
            animacion_pagar();
            
        }
             
    }
    
    return 0;
}
