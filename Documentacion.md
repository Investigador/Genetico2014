Documentación de op_mut&cross.cpp
=================================

Lo podeis encontrar en [op_mut&cross.cpp] (https://github.com/rotty11/MiRepositorio/blob/master/op_mut&cross.cpp). En este programa sólo se muestra la inicialización de los cromosomas a 1 ó 0 aleatoriamente y posteriormente se hará una mutación a uno o varios cromosomas. Después se realiza un crossover entre dos individuos cualesquiera usando la técnica del crossover en dos puntos. La estructura de datos usada será la mejor que se ha obtenido hasta ahora. Esto es:

  - Usando un unsigned char por cada 8 cromosomas

#####Mutación:
```cpp
// Realizo mutacion en cada individuo de la poblacion de 1 o varios bits
for(itPOB = POB.begin(); itPOB != POB.end(); ++itPOB) {
	int cromosoma = rand() % numCro8;
	(*itPOB)[cromosoma] = rand() % 256;
}
```
Lo que hacemos es mutar un unsigned char completo (0-255), aleatoriamente, lo que daría lugar a mutar en el mayor de los casos 8 bits, y por tanto 8 cromosomas. Así, en la representación binaria del unsigned char podría cambiar el estado de algunos bits, de uno sólo o de ninguno, según el número que le toque. Ejemplo:

	Unsigned char anterior(Dec / Bin): 26 / 00011010
	Unsigned char generado(Dec / Bin): 51 / 00110011
	
Como se ve, cambian 3 bits. Por tanto, se han mutado 3 cromosomas.

#####Crossover en dos puntos:
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
