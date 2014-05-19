/**
  * @file Poblacion.h
	* @author Juan José Escobar Pérez
	* @date 13/05/2014
  * @brief Fichero cabecera para la gestión de una Población
  *
  * Ofrece una interfaz simple para crear una Población y sus correspondientes individuos
  *
  */

/*
«Copyright 2014 Juan José Escobar Pérez»

Poblacion.h is part of ag.

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

#ifndef __POBLACION__H__
#define __POBLACION__H__

/* ***************************************** */

#include "Individuo.h"

/* ***************************************** */

/**
  * @brief Clase que permite la creación de un nuevo tipo de dato llamado Poblacion
  *
  * Almacena individuos
 */
class Poblacion {

	private:

		/**
  	  * @brief Almacenamiento de los individuos
  	  *
  	  * Cada individuo está compuesto de diversos cromosomas
  	*/
		vector<Individuo> pob;

		/**
  	  * @brief Crea un torneo entre los individuos de la población
			* @param torneo Donde se almacenará el torneo
			* @return El fitness total de la población como long int
			* @see Ruleta
  	*/
		long int Torneo( vector< pair<int, long int> > &torneo );

		/**
  	  * @brief Crea una ruleta con los individuos seleccionados para reproducirse
			* @param torneo El torneo entre los individuos
			* @param tfitness El fitness total de la población
			* @return Ruleta con X copias de los índices de cada individuo como probabilidad de reproducirse
			* @see Torneo
  	*/
		vector<int> Ruleta( const vector< pair<int,long int> > &torneo, const long int tfitness );

	public:

		/**
  	  * @brief Constructor vacío, crea una variable de tipo Poblacion
  	*/
		Poblacion();

		/**
  	  * @brief Constructor con parámetros, crea una variable de tipo Poblacion con tamaño especificado
			* @param tamPob El número de individuos que compondrá la población
			* @param numCro El número de cromosomas de cada individuo
			* @pre @tamPob y @numCro deben ser mayor que cero
			* @post El número de cromosomas reales de cada individuo será @numCro entre 8
			* @post Si @numCro no es divisible por 8, se redondea hacia arriba
  	*/
		Poblacion( const int tamPob, const int numCro );

		/**
  	  * @brief Inicializa de forma aleatoria los cromosomas de cada individuo
			* @pre La población debe tener al menos un individuo
			* @pre Se debería usar srand() para establecer la semilla a usar
  	*/
		void Inicializar();

		/**
  	  * @brief Obtiene la cantidad de individuos que compone la población
			* @return La cantidad de individuos en decimal
  	*/
		int Size();

		/**
  	  * @brief Evalua el fitness de todos los individuos mediante COUNT-ONES
			* @pre La población debe tener al menos un individuo
			* @return El fitness total (número de unos) de la población como long int
  	*/
		long int Evaluar();

		/**
  	  * @brief Obtiene un individuo determinado
			* @param ind El índice del individuo a obtener
			* @pre La población debe tener al menos un individuo y @ind debe ser válido
			* @return Una copia del individuo completo
			* @see SetInd
  	*/
		Individuo GetInd( const int ind );

		/**
  	  * @brief Establece un individuo
			* @param pos El índice del individuo a establecer/modificar
			* @param ind El individuo que se establecerá en la posición de @pos
			* @pre La población debe tener al menos un individuo y @pos debe ser válida
			* @pre El individuo a establecer debe tener la misma cantidad de cromosomas
			* @see GetInd
  	*/
		void SetInd( const int pos, Individuo ind );

		/**
  	  * @brief Realiza los cruces entre la población acorde a sus fitness
			* @param fitness vector por referencia donde se guardará ordenadamente los individuos y su fitness
			* @param Tfitness Referencia al total de fitness de la población actual
			* @pre La población debe tener al menos un individuo
			* @return Una nueva población con los hijos generados
			* @post Si el tamaño de la población es impar, se agrega el padre con más fitness
  	*/
		Poblacion Cruzar( vector< pair<int, long int> > &fitness, long int &Tfitness );

		/**
  	  * @brief Realiza los cruces entre dos individuos seleccionados
			* @param ind1 El índice del primer individuo a cruzar
			* @param ind2 El índice del segundo individuo a cruzar
			* @param p1 Primer punto de corte en la cadena de cromosomas
			* @param p2 SEgundo punto de corte en la cadena de cromosomas
			* @pre Los individuos deben tener la misma cantidad de cromosomas
			* @return Una pareja de individuos resultante del cruce
  	*/
		pair<Individuo,Individuo> CruzarPareja( int ind1, int ind2, const int p1, const int p2 );

		/**
  	  * @brief Realiza la mutación a un individuo
			* @param ind El índice del individuo a mutar
			* @param pos Posición del grupo de 8 cromosomas que se mutarán
			* @pre La población debe tener al menos un individuo y @pos debe ser válida
  	*/
		void Mutar_ind( const int ind, const int pos );

		/**
  	  * @brief Intercambia los individuos de dos poblaciones
			* @param p La población con la que se intercambiará
			* @pre Las poblaciones deben tener el mismo número de individuos y cromosomas y no estar vacías
  	*/
		void Swap(Poblacion &p);
};

#endif
