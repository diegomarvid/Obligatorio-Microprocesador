////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//Archivo de cabecera para el uso del m�dulo USART PIC con el XC8///
///////////////////////////////////////////////////////////////////

#include <xc.h>
 
 #define _XTAL_FREQ 4000000
 #define CMD_SIZE 40
 
///////////////////////////////////////////////
//inicializaci�n del m�dulo USART PIC modo as�ncrono
//en una funci�n, a 8bits,a 9600 baudios
/////////////////////////////////////////////////////
void iniciar_usart(){
     TRISCbits.TRISC6 = 0;//pin TX como una salida digital
     TXSTA=0b00100110;// 8bits, transmisi�n habilitada, as�ncrono, alta velocidad
     RCSTA=0b10010000;//habilitado el USART PIC, recepci�n 8 bits,  habilitada, as�ncrono
     SPBRG=25;//para una velocidad de 9600baudios con un oscilador de 4Mhz
}
 
///////////////////////////////////////////////
//transmisi�n de datos del m�dulo USART PIC modo as�ncrono
///////////////////////////////////////////////
void envia_caracter_usart(unsigned char caracter){
    while(TXSTAbits.TRMT==0);// mientras el registro TSR est� lleno espera
	TXREG = caracter;//cuando el el registro TSR est� vacio se envia el caracter
}
 
 
///////////////////////////////////////////////
//transmisi�n de cadenas de caracteres con el m�dulo USART PIC modo as�ncrono
///////////////////////////////////////////////
void envia_cadena_usart(char* dato){//cadena de caracteres de tipo char
    for(int i = 0; i< 9; i++){
        envia_caracter_usart(*(dato + i));//transmite los caracteres de cadena       
    }
}

void cmd_printf(const char *s) {
   
   int i = 0;
   
   
   while(s[i] != 0 && i < CMD_SIZE) {
	 
      envia_caracter_usart(s[i]);
      i++;
      
   }
	 
   
}

//--------------------------------------GENERAR DATO-------------------------------------------------------//

#define CR 0x0D
#define LF 0x0A
char dato[9];

const char d1[9] = "01PILAS4\r";
const char d2[9] = "02JAMON1\r";
const char d3[9] = "03CARNE0\r";
const char d4[9] = "04VODKA3\r";
const char d5[9] = "05ARROZ9\r";

void generar_dato(int r){
   
	
   
        
        if(r % 5 == 0){

            for (int j = 0; j < 9; j++){
                
                dato[j] = d1[j];
            
            }
            
        }else if (r % 5 == 1){
            
            for (int j = 0; j < 9; j++){
                
                dato[j] = d2[j];
            
            }            
            
        }else if (r % 5 == 2){
            
            for (int j = 0; j < 9; j++){
                
                dato[j] = d3[j];
            
            }            
            
        }   else  if (r % 5 == 3){
            
            for (int j = 0; j < 9; j++){
                
                dato[j] = d4[j];
            
            }  
            
        } else if(r % 5 == 4){
	   
			for (int j = 0; j < 9; j++){
                
                dato[j] = d5[j];
            
            }  
	   
	   }
	
    
}    
//-----------------------------------------------------------------------------------------------------------------//

int r = 0;

int main(void) {
	
	
	TRISB = 0xFF;
	
	iniciar_usart();
	
	while(1) {
		
		if(RB1 == 1){
			while(RB1);
			
			generar_dato(r);
			
			envia_cadena_usart(dato);
			
			r++;
		}
		
		
		
	}
	
	
	
	return 0;
}