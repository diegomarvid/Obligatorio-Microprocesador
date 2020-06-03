////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//Archivo de cabecera para el uso del m�dulo USART PIC con el XC8///
///////////////////////////////////////////////////////////////////

#include <xc.h>
 #include <stdio.h>
 
 #define _XTAL_FREQ 4000000
 #define CMD_SIZE 30
 
///////////////////////////////////////////////
//inicializaci�n del m�dulo USART PIC modo as�ncrono
//en una funci�n, a 8bits,a 9600 baudios
/////////////////////////////////////////////////////
void iniciar_usart(){
     TRISCbits.TRISC6 = 0;//pin TX como una salida digital
     TRISCbits.TRISC7= 1;//pin RX como una entrada digital
     TXSTA=0b00100110;// 8bits, transmisi�n habilitada, as�ncrono, alta velocidad
     RCSTA=0b10010000;//habilitado el USART PIC, recepci�n 8 bits,  habilitada, as�ncrono
     SPBRG=25;//para una velocidad de 9600baudios con un oscilador de 4Mhz
   
     //----Interrupciones----//
     INTCON = 0b11000000; //Habilita todas las interrupciones y las perifericas
    
     PIE1bits.RCIE = 1; //Habilita interrupcion serial de recepcion en perisferico RX

   
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

enum { FALSE , TRUE };

int h = 0;
int llego_cierre = FALSE;

//----------------------------------INTERRUPCION---------------------------------//

void __interrupt () int_usart(void) {
    
 
    //Me fijo si la interrupcion es por Recepcion serial RX
    if(PIR1bits.RCIF == TRUE) {
       
       
	if(h == 0 && RCREG  == 'C' && FERR  != TRUE){
	    h++;
	} 	
	else if(h == 1 && RCREG  == 'i' && FERR  != TRUE){
	    h++;
	} 
	else if(h == 2 && RCREG  == 'e' && FERR  != TRUE){
	    h++;
	} 	
	else if(h == 3 && RCREG  == 'r' && FERR  != TRUE){
	    h++;
	}	
	else if(h == 4 && RCREG  == 'r' && FERR  != TRUE){
	    h++;
	} 	
	else if(h == 5 && RCREG  == 'e' && FERR  != TRUE){
	   llego_cierre = TRUE;
	   h = 0;
	} else{
	    h = 0;
	} 
       
       
       
    }
    
 }

 inicie_cierre = FALSE;
 
//----------------------------------------------------------------------------------------------------//

//--------------------------------------GENERAR DATO-------------------------------------------------------//

#define CR 0x0D
#define LF 0x0A

char cmd[CMD_SIZE];

void generar_cmd(int r){
   
	       if( r % 5 == 0 ) {
		  
		  sprintf( cmd, "+D\r" );
		 	  
	       } else if ( r % 5 == 1) {
		  
		    sprintf( cmd, "-D\r" );
		  // inicie_cierre = TRUE;
	       } else if ( r % 5 == 2) {
		  
		     sprintf( cmd, "+L\r" );
		  
	       } else if ( r % 5 == 3) {
		  
		    sprintf( cmd, "+L\r" );
		  
	       } else if ( r % 5 == 4) {
		  
		  sprintf( cmd , "-D\r" );
		  
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
			
			generar_cmd(r);
			
			cmd_printf(cmd);
			
			r++;
			
		} else if (RB2 == 1){
		   
		    __delay_ms(500);
			
			 cmd_printf("+L\r");
		}
		
		
		
		
		
		
		
		
		/*else if(llego_cierre == TRUE && inicie_cierre == TRUE) {
		   
		   
			 __delay_ms(1000);
			
			 cmd_printf("\r");
		   
			 llego_cierre = FALSE;
		         inicie_cierre = FALSE;
		   
		} else if(llego_cierre == TRUE && inicie_cierre == FALSE){
		   
			   __delay_ms(3000);
			
			 cmd_printf("+L\r");
		   
			 llego_cierre = FALSE;
		        inicie_cierre = TRUE;
		   
		   
		}*/
		
		
		
	}
	
	
	
	return 0;
}