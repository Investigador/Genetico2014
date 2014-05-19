/**
  * @file Individuo.cpp
  * @brief Fichero con definiciones para la gestión de un individuo
	* @author Juan José Escobar Pérez
	* @date 13/05/2014
  *
  * Implementa la interfaz de Individuo.h
  *
  */

/*
«Copyright 2014 Juan José Escobar Pérez»

Individuo.cpp is part of ag.

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

#include "Individuo.h"
#include <cstdlib>
#include <cassert>
#include <cstdio>

/* ***************************************** */

Individuo::Individuo() {}

/* ***************************************** */

Individuo::Individuo( const int numCro ) {

	assert(numCro > 0);

	int numCro8 = numCro / 8;
	int rest8 = numCro % 8;
	bool exacto = (rest8 == 0) ? true : false;
	if(!exacto)
		++numCro8;
	this->ind = vector<unsigned char> (numCro8);
}

/* ***************************************** */

void Individuo::Inicializar() {

	assert(this->ind.size() > 0);
	vector<unsigned char>::iterator itGEN;

	// Voy rellenando aleatoriamente con numeros entre 0 y 255 el vector de unsigned char
	for(itGEN = this->ind.begin(); itGEN != this->ind.end(); ++itGEN)
		*itGEN = rand() % 256;
}

/* ***************************************** */

int Individuo::Size() {

	return this->ind.size();
}

/* ***************************************** */

long int Individuo::Evaluar() {

	assert(this->ind.size() > 0);

	long int cont = 0;
	for(int i = 0; i < this->ind.size(); ++i)
		cont += __builtin_popcountll(this->ind[i]);
	return cont;
}

/* ***************************************** */

unsigned char Individuo::GetCrom( const int pos ) {

	assert(this->ind.size() > 0 && pos > -1 && pos < this->ind.size() );
	return this->ind[pos];	
}

/* ***************************************** */

void Individuo::SetCrom( const int pos, const unsigned char valor ) {

	assert(this->ind.size() > 0 && pos > -1 && pos < this->ind.size() );
	this->ind[pos] = valor;
}

/* ***************************************** */

void Individuo::Mutar( const int pos ) {

	assert(this->ind.size() > 0);
	assert(pos > -1 && pos < this->ind.size());

	// Realizo mutacion de 1 o varios bits
	unsigned char aux;
	do {
		aux = rand() % 256;
	} while(aux == this->ind[pos]);
	this->ind[pos] = aux;
}
