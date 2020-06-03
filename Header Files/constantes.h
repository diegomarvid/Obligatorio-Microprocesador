
/* 
 * File:   constantes.h
 * Author: diegomarvid99@gmail.com
 *
 * Created on 4/17/2020 9:07:53 PM UTC
 * "Created in MPLAB Xpress"
 */

#ifndef CONSTANTES_H
#define	CONSTANTES_H

#define NULL (void*) 0

#define _XTAL_FREQ 4000000

#define CR 0x0D
#define LF 0x0A

#define MAX_CART 10
#define MAX_BILL 999

#define LED_TIME 250


//-----EEPROM-----//

//TT$TT$TT$TT$...

#define MAX_ITEMS 99
#define MAX_MEMORY_BYTES MAX_ITEMS * 3

//----------CMD------//
#define CMD_SIZE 30

//----STRUCT----//

enum { SALIDA, ENTRADA };
enum { FALSE, TRUE };

typedef struct {

   char dato[8];

} Codigo;

Codigo lista[MAX_CART];

typedef struct {

   char cant_ventas;
   char numero;
   int precio_total; 

} Lote;

Lote lote;

//Informacion serial

//TTIIIIIC
//Se usan en ASCII chars por lo cual 01 ocupa dos chars

//Informacion que se transmite por el lector de codigo de barras
unsigned char dato[8];

//Contador para ver posicion actual del char en dato.
int i = 0;

//Flag para recepcion exitosa de dato
int dato_recibido = FALSE;

int cantidad_items = 0;
int precio_total = 0;
int precio_actual = 0;
 
 unsigned char cmd[8];
 int cmd_recibido = FALSE;
 int es_cmd = FALSE;
 
char respuesta[CMD_SIZE];

//Estado
enum {ACTIVO, ESPERA};
//Modo
enum {NO_DEBUG, DEBUG};

int estado = ESPERA;
int modo = NO_DEBUG;

float v = 9.0f;

int cerro_lote = FALSE;
int finalizar_cierre = FALSE;

int timer_overflow = 0;
int espero_respuesta = FALSE;
int paso_segundo = FALSE;

int finalizar = FALSE;


#endif	/* CONSTANTES_H */