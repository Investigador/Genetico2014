Documentación del proyecto
==========================

Lo podeis encontrar en mi repositorio [Genetico2014] (https://github.com/rotty11/Genetico2014). En este proyecto, distribuído bajo licencia GPLv3, se ha realizado una librería para la gestión de individuos y poblaciones necesaria para la creación de un algoritmo genético base. En la raíz del proyecto os debéis encontrar con una estructura similar a esta:

  - bin
  - doc
  - include
  - lib
  - obj
  - res
  - src
  - config.xml
  - LICENSE
  - Makefile

#####Creación del programa:

Teníamos 3 opciones base para la gestión de las poblaciones, individuos y cromosomas. A saber:

  - Usando booleanos
  - Usando un unsigned char por cada cromosoma
  - Usando un unsigned char por cada 8 cromosomas

Pasándolo a código, respectivamente, sería:

  ```c++
  vector< vector<bool> > POB1(tamPob, vector<bool>(numCro));
  vector< vector<unsigned char> > POB2(tamPob, vector<unsigned char>(numCro));
  vector< vector<unsigned char> > POB3(tamPob, vector<unsigned char>(numCro8));
  ```
Donde `tamPob` es el tamaño de la población, `numCro` es el número de cromosomas por individuo y `numCro8` es el número de cromosomas por individuo entre 8.

La explicación es sencilla:

  - En el primero, cada cromosoma toma false o true. Un booleano ocupa 1 byte de memoria
  - En el segundo, cada cromosoma toma 0 o 1. Un unsigned char ocupa 1 byte de memoria
  - En el tercero, cada unsigned char es un 1 byte = 8 bits, y cada bit será un cromosoma. De esta forma el vector se reduce a la octava parte.

Nos quedamos definitivamente con la tercera debido a que mejora mucho el tiempo de ejecución y consumo de memoria. Para más detalles se puede consultar esta tabla comparativa realizada con anterioridad: [Comparativa entre estructuras de datos] (https://github.com/rotty11/MiRepositorio/blob/master/ev_crom_explicacion.md). Por otro lado, para aumentar la eficiencia del algoritmo COUNT_ONES, existían dos posibilidades:

  - Método de la Lookup Table: descrito y analizado [aquí] (https://github.com/rotty11/MiRepositorio/blob/master/ev_fitness_explicacion.md)
  - Llamada a una subrutina que usa la instrucción `pop` propia del procesador

Nos hemos decidido por esta última dado que es más eficiente que la anterior al hacer uso directo de instrucciones ensamblador.

#####Las librerías:

Lo que se ha hecho es crear dos clases: La clase Poblacion y la clase Individuo. Sus nombres ya dicen implícitamente lo que hacen. De esta forma, se ofrece una API con sus correspondientes interfaces que abstraen al usuario de la implementación interna de la misma. Por este motivo, se han creado funciones para la gestión de las librerías logrando la creación de un algoritmo genético con sólo llamadas a las mismas. También se ha tenido en cuenta que la interfaz de las funciones deben servir por igual tanto a un programa creado por un usuario como a los programas que realizan tests sobre estas funciones y que son tenidos en cuenta a la hora de realizar la integración continua. Más información sobre cómo funcionan estas funciones se pueden encontrar en el directorio `doc` del proyecto, concretamente en la carpeta `html`. Para ello, deberá compilar el proyecto usando el `Makefile` incluído y se generarán los ficheros `.html` correspondientes. A continuación se explica algunas opciones del Makefile.

#####Compilación de los ejecutables que conforman el proyecto:

Para construir todo el proyecto, basta abrir un intérprete de órdenes, dirigirse a la raíz del proyecto y ejecutar la orden `make`. Con ello, estamos construyendo lo siguiente:

  - Ejecutables: (En la carpeta bin)
    - ag
    - test
    - generadorR
  - Librerías estáticas: (En la carpeta lib)
    - Individuo.a
    - Poblacion.a
  - Documentación de Doxygen: (En la carpeta doc/html)

Además, podemos ejecutar las órdenes `make clean` para borrar ficheros con código objeto y de edición (los acabados en ~), y `make mrproper` para borrar todo aquello que es construído con `make` (binarios, librerías, documentación y código objeto) además de los ficheros con resultados de las ejecuciones de los programas.

#####Funcionamiento y ejecución de los programas:

El primer programa que deberíamos echar a funcionar sería el test. Éste se encarga de comprobar que los operadores principales de un algoritmo genético implementados en las librerías funcionan correctamente. Para ejecutar el programa nos situamos en la raíz del proyecto y ejecutamos:

  ```bash
  ./bin/test
  ```
  
Todos los parámetros de configuración, tales como el número de generaciones a generar, tamaño de la población y el número de cromosomas por individuo son leídos desde un fichero .xml situado en el directorio actual y que contiene lo siguiente:

  ```xml
  <?xml version="1.0"?>
  <!DOCTYPE config PUBLIC "" "config.dtd">

  <config>
	<tamPob>100</tamPob>
	<numCro>1024</numCro>
	<nGen>75</numCro>
  </config>
  ```

```cpp
// Realizo crossover entre dos individuos cogidos al azar (No me fijo en fitness ni nada)
// Utilizo el crossover entre dos puntos tambien elegidos al azar
// El resultado (dos hijos) lo almaceno en un nuevo vector de tamaño 2
int ind1 = rand() % tamPob;
int ind2 = rand() % tamPob;
int p1 = rand() % numCro8;
int p2 = rand() % numCro8;
int menor = (p1<p2) ? p1 : p2;
int mayor = (p1>p2) ? p1 : p2;
vector< vector<unsigned char> > HIJOS;
HIJOS.push_back(POB[ind1]);
HIJOS.push_back(POB[ind2]);
for(int i = menor; i <= mayor; ++i) {
	HIJOS[0][i] = POB[ind2][i];
	HIJOS[1][i] = POB[ind1][i];
}
```
Para empezar, en `ind1` e `ind2` se almacena la posición de los individuos que van a reproducirse elegidos al azar. En `p1` y `p2` se almacena los puntos inicial y final del trozo de cadena a usar en el crossover (rango de cromosomas) también elegidos al azar. Como son aleatorios, compruebo quién es mayor y quién es menor y lo guardo en `menor` y `mayor`. Acto seguido copio en el vector `HIJOS` (de tamaño 2) los padres. Por último, recorro el trozo de cadena a copiar y la voy asignando a su correspondiente hijo: La cadena del padre1 se copia en su lugar correspondiente en hijo2 y la cadena del padre2 se copia en el hijo1.
De esta forma hemos creado dos nuevos hijos a partir de los padres. Para aclarar el proceso, aquí un ejemplo, donde los padres tienen cadenas de tamaño 24 y el rango de cromosomas a copiar es del 8 al 15:

Padres `pad0` y `pad1`:

	10100100`00110010`00010111        01111100`10101001`10010101

Se copian los padres en `HIJOS[0]` e `HIJOS[1]`:

	10100100`00110010`00010111        01111100`10101001`10010101
	
Se cruzan los cromosomas marcados quedando `HIJOS[0]` e `HIJOS[1]` de la siguiente forma:

	10100100`10101001`00010111        01111100`00110010`10010101

Ejecución del programa
----------------------

  ```bash
  ./op_mut\&cross <Tamaño de la población> <Número de cromosomas>
  ```
Viendo la ventaja en tiempos de ejecución que esta implementación me ha proporcionado hasta ahora, y la facilidad para realizar mutaciones y crossover hace que me parezca una implementación adecuada para un algoritmo genético.
