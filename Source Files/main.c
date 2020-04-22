//Obligatorio 1 microprocesadores Diego Marvid y Valentin Otte.

#include <xc.h>
#include "constantes.h"



//chequea que el dato entrate cumpla con el checksum.
int check_sum(void){
    
    int suma = 0;
    
    for(int j = 0; j < 7; j++) {
            suma += dato[j] - 0x30;          
    }
    
    return (suma % 10 == dato[7] - 0x30);
    
    
}

//Limpia el dato actual en caso de error.
void limpiar_dato(void){
    
    for(int j = 0; j < 8; j++) {
        dato[j] = 0;          
    }
    
}


//---------  INTERRUPTION MANAGER------------//
void __interrupt () int_usart(void) {
    
 
 //Me fijo si la interrupcion es por Recepcion serial RX
 
 if(PIR1bits.RCIF == TRUE) {
  
    //Verifico si llego el final del dato
    if(RCREG == CR || RCREG == LF){
        
        //Si llego el final en la posicion correcta entonces el dato se recibio exitosamente
        if(i == 8) {
            
            i = 0;
            
            //Si cumple cheksum
            if(check_sum() == TRUE){
                dato_recibido = TRUE;
            //Sino limpio dato y dato_recibido = FALSE
            }else{              
                limpiar_dato(); 
            }    
            
            
            
        } else {         
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
        i = 0;
        //Limpiar dato
        limpiar_dato(); 
    }
    
     
 }
    
    
    
}

//Lectura de la EEPROM
unsigned char eeprom_read(unsigned char direccion){
    EEADR = direccion;      //direccion de la eeprom que se quiere leer
    EECON1bits.EEPGD = 0;   //para acceder a la memoria EEPROM PIC
    EECON1bits.RD = 1;      //habilita la lectura de la EEPROM PIC
    return EEDATA;        //en el registro EEDATA se encuentra el dato
                          //leido, la función retorna el dato leido
}

//Devuelve precio o 0 si error
//--------------------STOCK CHECK---------------------------//
int check_tt(Codigo *codigo) {
    
    
      
    for(unsigned char i = 0; i < MAX_ITEMS; i += 3) {
        
        char T1 = eeprom_read(i);
        char T2 = eeprom_read(i + 1);
        
        if(codigo ==  (Codigo*) NULL) {
            if (dato[0] == T1 && dato[1] == T2) {
                return (int) eeprom_read(i + 2);
            } 
        } else {
            if (codigo->dato[0] == T1 && codigo->dato[1] == T2) {
                return (int) eeprom_read(i + 2);
            } 
            
        }
            
        
    }
    
    return FALSE;
    
}

//----------------- ITEM REPETITION CHECK----------------------//
int check_s(void) {
    
    //Contador de elementos iguales
    int h = 0;
    
    //Me fijo si un item coincide en los 7 elementos que deberian coincidir
    //Si el contador da 7 entonces tiene 7 elementos iguales y significa que
    //el elemento se encuentra repetido.
    
    for(int i = 0; i < cantidad_items; i++){
        
        Codigo codigo = lista[i];
        
        for(int j = 0; j < 7; j++) {
            
            if( dato[j] == codigo.dato[j] ) {
                h++;   
            }
            
        }
        
        if(h == 7) {
            return TRUE;   
        } else {
            h = 0;   
        }
   
        
    }
    
    return FALSE;
    
}

//-----------------------ACTUALIZAR DATO--------------------------//
//Dado que primero verifico si hay lugar, agrego el dato y luego aumento el contador, cantidad de items no debe ser modificada
//para esta logica, pues para el primer items CI=0, para el segundo es CI=1, lo que hace el mapeo a la lisat mas simple.
void agregar_dato(void){     
    
        for (int j = 0; j < 8; j++){
            
            lista[cantidad_items].dato[j] = dato[j];
        
        }
   
}    

//------------ACTUALIZAR LISTA ITEMS-------------//
int agregar_lista() {
    
    if(cantidad_items < MAX_CART) {
        
        agregar_dato();
        
        //cantidad_items++;
        
        return TRUE;
        
    } else {
        return FALSE;
    }
    
}


//-------ACTUALIZAR DISPLAY--------//

//Dado un numero de dos digitos, muestra el mismo en dos display de 7 segmentos.
void display_numero(int numero){
    //Array con la representacion de los numeros en 7 segmentos.
    unsigned char seg[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
    
    //Calculo decenas y unidades.
    int decenas = numero / 100;
    int unidades = numero / 10;
    
    //Asigno la representacion del numero en 7 segmentos al puerto correspondiente.     
    PORTC = seg[decenas];
    PORTB = seg[unidades];      
}

//----------ELIMINAR EL ULTIMO ITEM---------------//

void eliminar_item(){
    
    int precio_item_actual = 0;
        
    if(cantidad_items > 0){
               
        precio_item_actual = check_tt(&lista[cantidad_items - 1]);
        precio_total -= precio_item_actual;
        cantidad_items--;
        //precio_actual=precio_item_actual;
        
        
    }else{ 
        //Led rojo de error
        RA4=1;
        __delay_ms(500);
        RA4=0;
    }    
    
}    

//-------ANIMACION AL PAGAR----------//
void animacion_pagar(){
   
   for(int j=0; j<3;j++){
    RA3=1;
    __delay_ms(500);
    RA3=0;
    RA4=1;
    __delay_ms(500);
    RA4=0;
   }
    
}    


    
//------------SETUPS---------//
void setup(void) {
    
    //---Displays----//
    
    //Decenas
    TRISB = 0x0;
    
    //Unidades
    TRISD = 0x0;
    
    //Botones
    TRISA = 0b0000110;
    
    
    //----Serial----//
    
    TRISCbits.TRISC7 = ENTRADA; // Pin RX entrada digital
    
    
    //SPEN && CREN
    RCSTA = 0b10010000;
    
    //Velocidad de transimision de baudios
    SPBRG = 25; //9615 baudios/s
    
    //----Interrupciones----//
    INTCON = 0b11000000; //Habilita todas las interrupciones y las perifericas
    
    PIE1bits.RCIE = 1; //Habilita interrupcion serial de recepcion en perisferico RX
    
    
}
   

int main(void) {
    
    setup();
    
    
    while(TRUE) {
        
        //-------ESCANEO-------//
        
        if(dato_recibido == TRUE) {
            
            precio_actual = check_tt(NULL);
                        
            if( precio_actual != 0 && check_s() ) {
                
                if(precio_total + precio_actual < 999) {
                    
                    if(agregar_lista() == TRUE) {
                        
                        
                        //Actualizo precio y display
                        precio_total += precio_actual;
                        display_numero(precio_total);
                        cantidad_items++;
                        
                        //Led verde.
                        
                        RA3 = 1;
                        __delay_ms(500);
                        RA3 = 0;
                        __delay_ms(500);
                        
                    } else {
                        
                        //Led rojo de error
                        RA4 = 1;
                        __delay_ms(500);
                        RA4 = 0;
                        __delay_ms(500);
                        
                    }
                
                    
                } else {
                    //Led rojo de error
                    RA4 = 1;
                    __delay_ms(500);
                    RA4 = 0;
                    
                }
                
                
            }else {
                //Led rojo de error
                RA4 = 1;
                __delay_ms(500);
                RA4 = 0;   
            }
            
            dato_recibido = FALSE;
        }
        
        //-------LIMPIAR-------//
        
        else if(RA1 == TRUE) {
            
            while(RA1) {
                
            }
            
            __delay_ms(50);
            
            eliminar_item();
            
        }
        
        //-------PAGAR-------//
        
        else if(RA2 == TRUE) {
            
            while(RA2) {
                
            }
            
            __delay_ms(50);
            
            //Reinicio la cuenta.
            precio_total = 0;
            cantidad_items = 0;
            //Rsetea el display.
            display_numero(precio_total);           
            animacion_pagar();
            
        }
        
        
        
    }
    

    
    
    return 0;
}
