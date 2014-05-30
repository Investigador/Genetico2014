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
  - .travis.yml
  - Documentacion.md
  - LICENSE
  - Makefile
  - README.md
  - config.xml

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

Para construir todo el proyecto, basta con abrir un intérprete de órdenes, dirigirse a la raíz del proyecto y ejecutar la orden `make`. Con ello, estamos construyendo lo siguiente:

  - Ejecutables: (En la carpeta bin)
    - ag
    - test
    - generadorR
  - Librerías estáticas: (En la carpeta lib)
    - Individuo.a
    - Poblacion.a
  - Documentación de Doxygen: (En la carpeta doc/html)

Además, podemos ejecutar las órdenes `make clean` para borrar ficheros con código objeto y de edición (los acabados en ~), y `make mrproper` para borrar todo aquello que es construído con `make` (binarios, librerías, documentación y código objeto) además de los ficheros con resultados de las ejecuciones de los programas. De todas formas, podemos construir cualquiera de los binarios, librerías, códigos objeto y documentación por independiente con solo ejecutar:

  ```bash
  make <objeto a construir>
  ```

#####Test de la librería e integración continua:

El primer programa que deberíamos echar a funcionar sería el test. Éste se encarga de comprobar que los operadores principales de un algoritmo genético implementados en las librerías funcionan correctamente. Para ejecutar el programa nos situamos en la raíz del proyecto y ejecutamos:

  ```bash
  ./bin/test
  ```
  
Todos los parámetros de configuración, tales como el número de generaciones a generar, tamaño de la población y el número de cromosomas por individuo son leídos desde un fichero .xml situado en el directorio actual y contiene lo siguiente:

  ```xml
  <?xml version="1.0"?>
  <!DOCTYPE config PUBLIC "" "config.dtd">

  <config>
	<tamPob>100</tamPob>
	<numCro>1024</numCro>
	<nGen>3</numCro>
  </config>
  ```
  
Si al ejecutarse el programa no aparece ningún mensaje en pantalla es que todo es satisfactorio y podremos proceder con confianza a usar las librerías para nuestros propios programas. De todas formas, mediante la integración continua, podemos realizar este test justo en el momento en que realizamos `git push` en nuestro repositorio. Para ello, basta con seguir las instrucciones de [este enlace] (http://docs.travis-ci.com/user/getting-started/). Nos pedirá hacer login en la página de `travis-ci`, aceptar los permisos para leer el repositorio y manipulación del mismo. Al final, lo que tendremos que hacer es crear el fichero `.travis.yml` en la raíz de nuestro proyecto y configurarlo para que haga lo que queramos justo antes del `git push`. Por ejemplo, mi fichero `.travis.yml` contiene:

  ```yml
  language: cpp
  compiler:
    - gcc
  install: 
    - sudo wget http://sourceforge.net/projects/boost/files/boost/1.55.0/boost_1_55_0.tar.bz2/download -O /usr/include/boost_1_55_0.tar.bz2
    - sudo tar --bzip2 -xf /usr/include/boost_1_55_0.tar.bz2 -C /usr/include
    #Decomentar las siguientes lineas y comentar la ultima para compilar absolutamente todo  
    #- sudo apt-get install doxygen
  #script: make && ./bin/test
  script: make test && ./bin/test
  ```

Lo que estamos indicando es que usamos lenguaje `c++` y compilador `gcc`. Dado que necesito la librería `boost` para el tratamiento de XML, indico con `install` que el servidor de travis debe descargarla y colocarla en `/usr/include` para posteriormente descomprimirla en el mismo directorio. Con `script` ordeno lo que quiero que haga en último lugar y, como es normal, construir y ejecutar el test. En la página de travis podemos ver el estado de la compilación. Si todo es correcto, aparecerá en verde el texto "passed", de lo contrario, "failed".

#####Ejecutando nuestro algoritmo genético

Ahora procedemos a ejecutar nuestro algoritmo genético. Para ello, sin movernos del directorio actual ejecutamos:

  ```bash
  ./bin/ag
  ```

Se habrá creado en la carpeta `res` un fichero `.xml` con la estadística relacionada. El nombre de dicho archivo estará estructurado y mostrará los parámetros usados en la ejecución del algoritmo. Así, el fichero `genetico2014-3-100-1024-24545` significa:

  - `genetico2014` es el nombre del evento (Así lo he querido yo).
  - `3` es el número de generaciones a generar
  - `100` es el número de individuos que componen una población
  - `1024` es el número de cromosomas que tiene un individuo
  - `24545` es la hora de finalización de la ejecución en el formato `min(2)-hora(45)-seg(45)`
  
El contenido de dicho fichero estará compuesto por los parámetros usados (Nº de generaciones, Nº de individuos y Nº de cromosomas), estádistica de fitness de la población para cada generación (sumatoria, media y máximo) y por último el tiempo (en segundos) empleado en la ejecución del algoritmo. A continuación se muestra el fichero generado para el fichero de configuración anterior:

  ```xml
  <?xml version="1.0" encoding="utf-8"?>
  <resultados>
  	<tamPob>100</tamPob>
  	<numCro>1024</numCro>
  	<nGen>3</nGen>
  	<gen0>
  		<totalFitness>51252</totalFitness>
  		<fitMedia>512.52</fitMedia>
  		<mejorFitness>550</mejorFitness>
  	</gen0>
  	<gen1>
  		<totalFitness>52409</totalFitness>
  		<fitMedia>524.09</fitMedia>
  		<mejorFitness>547</mejorFitness>
  	</gen1>
  	<gen2>
  	<totalFitness>53358</totalFitness>
  		<fitMedia>533.58</fitMedia>
  		<mejorFitness>559</mejorFitness>
  	</gen2>
  	<tiempo>0.003101110458374023</tiempo>
  </resultados>
  ```

Este fichero `.xml` resultante puede ser analizado con el tercer programa incluído en el repositorio, llamado `generadorR`. Este programa lee como argumento un fichero `.xml` estructurado de la forma anterior y genera, en la carpeta `/res`, otro fichero llamado `tablaR.r` listo para ser leído por un intérprete del lenguaje R y generando una tabla estructurada con la anterior información para un posible tratamiento más personalizado. La forma de invocarlo es:

  ```bash
  ./bin/generadorR <fichero XML>
  ```

Profiling del algoritmo genético
--------------------------------

Llegados a este punto, falta evaluar el rendimiento de nuestro algoritmo genético. Por supuesto, todo es mejorable. El hecho de usar, por ejemplo, modularización, conlleva una pérdida de rendimiento en llamadas a funciones, copia de parámetros (otros van por referencia), declaraciones de variables y demás. Estamos cambiando, por así decirlo, el rendimiento por modularización, reutilización, mantenimiento, abstracción, entre otros. Para el uso del profiling, lo que vamos a hacer es realizar mediciones cambiando los parámetros disponibles en el programa, a saber; Nº de generaciones y Nº de individuos por población, manteniendo el Nº de cromosomas por individuo. Además, haremos uso del profiler `gprof` para realizar una traza del algoritmo y ver qué funciones son las que más consumen recursos. ¡¡Podríamos llevarnos una sorpresa!!. En la siguiente tabla se muestra la comparativa de tiempos para nuestro algoritmo genético:

 Tam. población | Nº cromosomas | Nº generaciones | Mejor media de fitness | Tiempo de ejecución
----------------|---------------|-----------------|------------------------|--------------------
       128      |      128      |        32       |       98.3828125       |     0.013034 s
       128      |      512      |        32       |       364.3203125      |     0.020687 s
       128      |      2048     |        32       |      1255.7421875      |     0.052391 s
       128      |      128      |       128       |       106.8828125      |     0.049417 s
       128      |      512      |       128       |        425.53125       |     0.080253 s
       128      |      2048     |       128       |        1480.8125       |     0.202097 s
       128      |      128      |       512       |        113.15625       |     0.199811 s
       128      |      512      |       512       |        467.46875       |     0.323740 s
       128      |      2048     |       512       |       1732.046875      |     0.798970 s

Podemos observar que al mantener el tamaño de la población y para un mismo número de cromosomas, el número de generaciones influye en la calidad del fitness dado que en cada generación se está mejorando el fitness de cada individuo y un nuevo cruce provoca hijos de mayor calidad. Variar cualquier parámetro tiene un impacto lineal en los tiempos de ejecución, siendo, hasta la fecha, lo mejor que se ha podido implementar. Aún así, vamos a hacer uso de `gprof` a ver qué dice. Para ello, lo primero que debemos hacer es añadir la opción `-pg` al compilar con g++ para añadir información de estadística y ejecutar nuestro programa (se generará el fichero `gmon.out`). Acto seguido, debemos ejecutar `gprof` pasándole el nombre de nuestro programa. En resumen:

  - Compilar: g++ -pg ..... -o programa
  - Ejecutar normalmente: ./programa <tus parámetros si es que los tiene>
  - Gprof: gprof programa

De esta forma, nos saldrá en pantalla información del porcentaje de tiempo consumido en las distintas llamadas a funciones, el número de llamadas (si puede) y otros parámetros. Me interesan los siguientes parámetros y sólo muestro las 7 primeras llamadas (Las que más consumen). Los resultados se han tomado con la `población = 128`, `cromosomas = 2048` y `generaciones = 512`:

 % tiempo | Nº llamadas |                 Función         
----------|-------------|------------------------------------------
  22.39   |  40188003   |      Vector<unsigned char>::size()
  10.45   |      -      |           Individuo::Evaluar()
   8.96   |      -      |    Vector<unsigned char>::operator[]
   6.72   |      -      |           Individuo::setCrom()
   5.97   |      -      |          Individuo::operator[]
   5.22   |   198019    |         Individuo::GetCrom(int)
   4.48   |    2563     | _Vector_base::_M_allocate(unsigned long)

Si observamos la tabla, ¡¡SORPRESA!!, casi la cuarta parte del tiempo es consumido llamando a la función que nos devuelve el tamaño del vector que almacena los cromosomas. Esto es una aberración. Se solucionaría fácilmente incluyendo este valor como variable privada en la clase `Individuo`. De esta forma el acceso sería directo y cuyo coste se vería reducido considerablemente. El resto de tiempos son bastantes normales teniendo en cuenta que la evaluación de todos los individuos es pesada.
