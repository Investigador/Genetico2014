/**
  * @file Individuo.h
	* @author Juan José Escobar Pérez
	* @date 13/05/2014
  * @brief Fichero cabecera para la gestión de un Individuo
  *
  * Ofrece una interfaz simple para crear un individuo y sus correspondientes cromosomas
  *
  */

/*
«Copyright 2014 Juan José Escobar Pérez»

Individuo.h is part of ag.

ag is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Foobar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ag.  If not, see <http://www.gnu.org/licenses/>.
*/

/* ***************************************** */

#ifndef __INDIVIDUO__H__
#define __INDIVIDUO__H__

/* ***************************************** */

#include <vector>

/* ***************************************** */

using namespace std;

/* ***************************************** */

/**
  * @brief Clase que permite la creación de un nuevo tipo de dato llamado Individuo
  *
  * Almacena cromosomas
 */
class Individuo {

	private:

		/**
  	  * @brief Almacenamiento de los cromosomas en la forma de unsigned char
  	  *
  	  * Cada unsigned char representa a 8 cromosomas
  	*/
		vector<unsigned char> ind;

	public:

		/**
  	  * @brief Constructor vacío, crea una variable de tipo Individuo 
  	*/
		Individuo();

		/**
  	  * @brief Constructor con parámetros, crea una variable de tipo Individuo con tamaño especificado
			* @param numCro El número de cromosomas del individuo
			* @pre @numCro debe ser mayor que cero
			* @post El tamaño real será @numCro entre 8
			* @post Si @numCro no es divisible por 8, se redondea hacia arriba
  	*/
		Individuo( const int numCro );

		/**
  	  * @brief Inicializa de forma aleatoria al individuo
			* @pre El individuo debe tener al menos un cromosoma
			* @pre Se debería usar srand() para establecer la semilla a usar
  	*/
		void Inicializar();

		/**
  	  * @brief Obtiene la cantidad de cromosomas del individuo
			* @return La cantidad de cromosomas del individuo en decimal
  	*/
		int Size();

		/**
  	  * @brief Evalua el fitness del individuo mediante COUNT-ONES
			* @pre El individuo debe tener al menos un cromosoma
			* @return El fitness (número de unos) del individuo como long int
  	*/
		long int Evaluar();

		/**
  	  * @brief Obtiene el valor que representa a 8 cromosomas
			* @param pos La posición que quiere ser leída
			* @pre El individuo debe tener al menos un cromosoma y @pos debe ser válida
			* @return El valor 0-255 del unsigned char que representa a 8 cromosomas
			* @see SetCrom
  	*/
		unsigned char GetCrom( const int pos );

		/**
  	  * @brief Establece el valor que representa a 8 cromosomas
			* @param pos La posición que quiere ser establecida
			* @param valor El valor de los 8 cromosomas que quiere establecerse
			* @pre El individuo debe tener al menos un cromosoma y @pos debe ser válida
			* @see GetCrom
  	*/
		void SetCrom( const int pos, const unsigned char valor );

		/**
  	  * @brief Realiza una mutación sobre un conjunto de 8 cromosomas
			* @param pos La posición que quiere ser establecida
			* @pre El individuo debe tener al menos un cromosoma y @pos debe ser válida
  	*/
		void Mutar( const int pos );
};

#endif
