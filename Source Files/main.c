/*
 * File:   main.c
 * Author: diegomarvid99@gmail.com
 *
 * Created on 4/17/2020 7:02:56 PM UTC
 * "Created in MPLAB Xpress"
 */


#include <xc.h>
#include "constantes.h"

unsigned char eeprom_read(unsigned char direccion){
    EEADR = direccion;      //direccion de la eeprom que se quiere leer
    EECON1bits.EEPGD = 0;   //para acceder a la memoria EEPROM PIC
    EECON1bits.RD = 1;      //habilita la lectura de la EEPROM PIC
    return EEDATA;        //en el registro EEDATA se encuentra el dato
                          //leido, la función retorna el dato leido
}

//Devuelve precio o 0 si error
int check_tt() {
    
      
    for(unsigned char i = 0; i < MAX_ITEMS; i += 3) {
        
        char T1 = eeprom_read(i);
        char T2 = eeprom_read(i + 1);
        
        if (dato[0] == T1 && dato[1] == T2) {
            return (int) eeprom_read(i + 2);
        }    
        
        
    }
    
    return FALSE;
    
}

int check_s() {
    
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

int agregar_lista() {
    
    if(cantidad_items < MAX_CART) {
        
        lista[cantidad_items].dato = dato;
        
        cantidad_items++;
        
    } else {
        return FALSE;
    }
    
}



void __interrupt () int_usart(void) {
    
 
 //Me fijo si la interrupcion es por Recepcion serial RX
 
 if(PIR1bits.RCIF == TRUE) {
  
    //Verifico si llego el final del dato
    if(RCREG == CR || RCREG == LF){
        
        //Si llego el final en la posicion correcta entonces el dato se recibio exitosamente
        if(i == 8) {
            i = 0;
            //Si cumple cheksum
            dato_recibido = TRUE;
            //Sino limpio dato y dato_recibido = FALSE
            
            
        } else {         
            i = 0;
            //Limpiar dato
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
    }
    
     
 }
    
    
    
}




void setup(void) {
    
    //---Displays----//
    
    //Decenas
    TRISB = 0x0;
    
    //Unidades
    TRISD = 0x0;
    
    //Botones
    TRISA = 0x0F;
    
    
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
            
            precio_actual = check_tt();
                        
            if( precio_actual != 0 && check_s() ) {
                
                if(precio_total + precio_actual < 999) {
                    
                    if(agregar_lista() == TRUE) {
                        
                    } else {
                        
                    }
                
                    
                } else {
                    
                    
                }
                
                //if(agregar_lista() == TRUE) {
                    
                    //actualizar_display();
                    //prender_led_verde();
                    
                //} else{
               //     prender_led_rojo();
               // }
                
            }
            
            dato_recibido = FALSE;
        }
        
        //-------LIMPIAR-------//
        
        else if(RA1 == TRUE) {
            
            while(RA1) {
                
            }
            
            __delay_ms(50);
            
            
        }
        
        //-------PAGAR-------//
        
        else if(RA2 == TRUE) {
            
            while(RA2) {
                
            }
            
            __delay_ms(50);
            
            cantidad_items = 0;
            //resetar_display();
           // limpiar_lista();
           // animacion_pagar();
            
        }
        
        
        
    }
    

    
    
    return 0;
}
