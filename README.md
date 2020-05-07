# Obligatorio Microprocesador
 Obligatorio para la materia Microprocesadores y Microcontroladores

 Autores: Diego Marvid y Valentin Otte
 
 Se presenta el codigo C de una pistola de codigo de barras simulada en un PIC 16F877A. Para poder usarla se debe conectar la pata TX de este mismo PIC a la pata RX del PIC en el sistema. Dentro de la pistola se contienen 5 productos de prueba para usar en el escaneo dado.
 
 Estos son strings de la forma: TTIIIIIC\r
 
 Ejemplos de productos que vienen por defecto:
 
 * 01PILAS4\r
 * 02JAMON1\r
 * 03CARNE0\r
 * 04VODKA3\r
 * 05ARROZ9\r

Todos vienen con el checksum calculado correctamente (suma del valor ASCII de todos los caracteres modulo 10).

Para poder calcular el checksum de un string de forma rapida se puede utilizar el navegador de Chrome como IDE de programacion y utilizar JavaScript. Para eso en una ventana del navegador se debe apretar F12 y se abrira por defecto una consola. En ella se puede pegar el siguiente codigo:

```javascript

function checksum(cadena) {

	let ret = 0;

	for(let i = 0; i < cadena.length; i++){
	
		ret += cadena.charCodeAt(i);
	
	}
	
	return ret % 10;

}
```

Se pega el codigo y se apreta enter para cargar la funcion. Posteriormente para poder usarla se debe ejecutar lo siguiente:

```javascript
checksum("05EJEMPLO")
```
Y el navegador devolvera el checksum adecuado a la cadena de texto ingresada como atributo a la funcion.

Para poder ejecutar la pistola se dispone de un registro en el puerto B denominado RB1 en el cual se puede adjuntar un Boton para poder funcionar como disparador. Cada vez que se apriete el boton se va a oscilar de forma ciclica entre los 5 codigos que vienen por defecto.
