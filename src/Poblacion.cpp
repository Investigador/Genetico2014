/**
  * @file Poblacion.cpp
  * @brief Fichero con definiciones para la gestión de una población
	* @author Juan José Escobar Pérez
	* @date 13/05/2014
  *
  * Implementa la interfaz de Poblacion.h
  *
  */

/*
«Copyright 2014 Juan José Escobar Pérez»

Poblacion.cpp is part of ag.

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

#include "Poblacion.h"
#include <cstdlib>
#include <cassert>
#include <cstdio>

/* ***************************************** */

// Funcion usada por qsort para ordenar por fitness a los individuos en el torneo
int compare(const void *ind1, const void *ind2) {

	return ( ((pair<int, long int>*)ind2)->second - ((pair<int, long int>*)ind1)->second );
}

/* ***************************************** */

Poblacion::Poblacion() {}

/* ***************************************** */

Poblacion::Poblacion( const int tamPob, const int numCro ) {

	assert(tamPob > 0 && numCro > 0);

	this->pob = vector<Individuo> (tamPob, Individuo(numCro));
}

/* ***************************************** */

void Poblacion::Inicializar() {

	assert(this->pob.size() > 0);
	vector<Individuo>::iterator itPOB;

	// Voy rellenando aleatoriamente con numeros entre 0 y 255 el vector de unsigned char
	for(itPOB = this->pob.begin(); itPOB != this->pob.end(); ++itPOB)
		(*itPOB).Inicializar();
}

/* ***************************************** */

int Poblacion::Size() {

	return this->pob.size();
}

/* ***************************************** */

long int Poblacion::Evaluar() {

	assert(this->pob.size() > 0);

	// Evaluo el fitness de cada individuo
	long int Tfitness = 0;
	for(int i = 0; i < this->pob.size(); ++i)
		Tfitness += this->pob[i].Evaluar();
	return (Tfitness);
}

/* ***************************************** */

Individuo Poblacion::GetInd( const int ind ) {

	assert(this->pob.size() > 0 && ind > -1 && ind < this->pob.size());
	return this->pob[ind];
}

/* ***************************************** */

void Poblacion::SetInd( const int pos, Individuo ind ) {

	assert(this->pob.size() > 0 && pos > -1 && pos < this->pob[0].Size());
	assert(ind.Size() == this->pob[0].Size());
	this->pob[pos] = ind;
}

/* ***************************************** */

Poblacion Poblacion::Cruzar( vector< pair<int, long int> > &fitness, long int &Tfitness ) {

	assert(this->pob.size() > 0);

	// Creo el torneo y obtengo el fitness total de la población
	//vector< pair<int, long int> > TORNEO; // Almaceno indice de individuo y su fitness (unos)
	Tfitness = Torneo(fitness);

	// Ruleta donde almaceno los índices de los participantes en proporción a su fitness
	vector<int> RULETA;
	RULETA = Ruleta(fitness, Tfitness);

	// Realizo crossover entre dos individuos cogidos al azar de entre la RULETA
	// Utilizo el crossover entre dos puntos tambien elegidos al azar
	// Cada hijo lo almaceno en un nuevo vector
	int cruces = this->pob.size() >> 1;
	Poblacion HIJOS(this->pob.size(), this->pob[0].Size() * 8);
	//printf("\nRealizo %d cruces:\n", cruces);
	for(int i = 0; i < cruces; ++i) {
		int ind1 = rand() % this->pob.size();
		int ind2 = rand() % this->pob.size();
		int p1 = rand() % this->pob[0].Size();
		int p2 = rand() % this->pob[0].Size();
		int menor = (p1<p2) ? p1 : p2;
		int mayor = (p1>p2) ? p1 : p2;
		pair<Individuo,Individuo> aux = CruzarPareja(RULETA[ind1], RULETA[ind2], menor, mayor);
		HIJOS.pob[i<<1] = aux.first;
		HIJOS.pob[(i<<1)+1] = aux.second;
		/*printf("\nCruce %d: entre padres %d y %d con cruce entre los puntos %d y %d\n", i, RULETA[ind1], RULETA[ind2], menor, mayor);
		printf("PADRE %d: ", RULETA[ind1]);
		for(int k = 0; k < this->pob[0].Size(); ++k)
			printf("%d ", this->pob[RULETA[ind1]].GetCrom(k));
		printf("\n");
		printf("PADRE %d: ", RULETA[ind2]);
		for(int k = 0; k < this->pob[0].Size(); ++k)
			printf("%d ", this->pob[RULETA[ind2]].GetCrom(k));
		printf("\n");
		printf("HIJO %d: ", i<<1);
		for(int k = 0; k < this->pob[0].Size(); ++k)
			printf("%d ", HIJOS.pob[i<<1].GetCrom(k));
		printf("\n");
		printf("HIJO %d: ", (i<<1)+1);
		for(int k = 0; k < this->pob[0].Size(); ++k)
			printf("%d ", HIJOS.pob[(i<<1)+1].GetCrom(k));
		printf("\n\n");*/
	}

	// Si la poblacion es impar el numero de hijos generados es uno menos: Agregamos entonces al padre con mas fitness
	// Ya tenemos la nueva población generada
	if(this->pob.size() % 2 == 1)
		HIJOS.pob[this->pob.size() - 1] = this->pob[fitness[0].first];
	return HIJOS;
}

/* ***************************************** */

pair<Individuo,Individuo> Poblacion::CruzarPareja( int ind1, int ind2, const int p1, const int p2 ) {

	assert(this->pob[ind1].Size() == this->pob[ind2].Size());
	pair<Individuo,Individuo> HIJOS(this->pob[ind1], this->pob[ind2]);
	
	for(int j = p1; j <= p2; ++j) {
		HIJOS.first.SetCrom(j, this->pob[ind2].GetCrom(j));
		HIJOS.second.SetCrom(j, this->pob[ind1].GetCrom(j));
	}
	return HIJOS;
}

/* ***************************************** */

long int Poblacion::Torneo( vector< pair<int, long int> > &torneo ) {

	// Evaluo el fitness de cada individuo y lo almaceno en torneo
	torneo = vector< pair<int, long int> >(this->pob.size());
	long int Tfitness = 0;
	for(int i = 0; i < this->pob.size(); ++i) {
		long int fitness = this->pob[i].Evaluar();
		Tfitness += fitness;
		torneo[i] = make_pair(i, fitness);
	}

	// Ordeno los individuos por fitness
	qsort(&torneo[0], this->pob.size(), sizeof(pair<int, long int>), compare);

	/*printf("Individuos ordenados por fitness:\n");
	for(int i = 0; i < this->pob.size(); ++i)
		printf("Num %d, fitness: %ld\n", torneo[i].first, torneo[i].second);
	printf("Total fitness: %ld / %ld\n\n", Tfitness, this->pob[0].Size() * 8 * this->pob.size());*/
	return Tfitness;
}

/* ***************************************** */

vector<int> Poblacion::Ruleta( const vector< pair<int, long int> > &torneo, const long int tfitness ) {

	vector<int> RULETA(this->pob.size());
	int generados = 0;
	for(int i = 0; i < this->pob.size() && generados < this->pob.size(); ++i) {
		int copias = ((double) torneo[i].second / (double) tfitness) * (double) this->pob.size();
		if(copias + generados > this->pob.size())
			copias = this->pob.size() - generados;
		//printf("Copias de %d: %d\n", torneo[i].first, copias);
		for(int j = generados; j < copias + generados; ++j)
			RULETA[j] = torneo[i].first;
		generados += copias;
	}

	if(generados < this->pob.size())
		for(int i = generados, aux = 0; i < this->pob.size(); ++i, ++aux)
			RULETA[i] = torneo[aux].first;

	//printf("Indices de individuos en la ruleta: ");
	for(int i = 0; i < this->pob.size(); ++i)
		//printf("%d ", RULETA[i]);
	//printf("\n");
	return RULETA;
}

/* ***************************************** */

void Poblacion::Mutar_ind( const int ind, const int pos ) {

	assert(this->pob.size() > 0 && ind > -1 && ind < this->pob.size());
	this->pob[ind].Mutar(pos);
}

/* ***************************************** */

void Poblacion::Swap( Poblacion &p ) {

	assert(this->pob.size() > 0 && this->pob.size() == p.pob.size());
	assert(this->pob[0].Size() == p.pob[0].Size());
	this->pob.swap(p.pob);
}
