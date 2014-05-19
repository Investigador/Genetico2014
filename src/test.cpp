/*
«Copyright 2014 Juan José Escobar Pérez»

test is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Foobar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with test.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <sys/time.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "Poblacion.h"

int main(int argc, char *argv[]) {

	// Crea un objeto árbol para el xml y leo los parametros
	using boost::property_tree::ptree;
	ptree pt;
	read_xml("config.xml", pt);
	int tamPob = pt.get<int>("config.tamPob");
	int numCro = pt.get<int>("config.numCro");

	// Inicializo aleatoriamente los cromosomas
	srand(time(NULL));
	Poblacion POB(tamPob, numCro);
	POB.Inicializar();
	int numCro8 = POB.GetInd(0).Size();

	/////////////////////////////////////////////////////////////////
	// Realizamos un test al operador de fitness en cada individuo //
	/////////////////////////////////////////////////////////////////
	for(int i = 0; i < tamPob; ++i) {

		// Evaluamos el fitness según la API
		long int fitnessAPI = POB.GetInd(i).Evaluar();

		// Evaluamos el fitness según el test
		long int fitnessTEST = 0;
		for(int j = 0; j < numCro8; ++j)
			fitnessTEST += __builtin_popcountll(POB.GetInd(i).GetCrom(j));

		// Comprobamos que el resultado coincide y pasa parcialmente el test
		if(fitnessAPI != fitnessTEST) {
			printf("El operador de fitness es erroneo: Se aborta el programa\n");
			return -1;
		}
	}

	/////////////////////////////////////////////
	// Realizamos 10 test al operador de cruce //
	/////////////////////////////////////////////
	for(int i = 0; i < 10; ++i) {
		Poblacion AUX(2, numCro);
		AUX.Inicializar();

		// Creamos puntos de corte
		int p1 = rand() % numCro8;
		int p2 = rand() % numCro8;
		int menor = (p1<p2) ? p1 : p2;
		int mayor = (p1>p2) ? p1 : p2;

		// Realizamos el cruce según la API
		pair<Individuo,Individuo> hijosAPI = AUX.CruzarPareja(0, 1, menor, mayor);

		// Realizamos el cruce según el test
		pair<Individuo,Individuo> hijosTEST(AUX.GetInd(0), AUX.GetInd(1));
	
		for(int j = menor; j <= mayor; ++j) {
			hijosTEST.first.SetCrom(j, AUX.GetInd(1).GetCrom(j));
			hijosTEST.second.SetCrom(j, AUX.GetInd(0).GetCrom(j));
		}

		// Comprobamos que el cruce coincide y pasa parcialmente el test
		for(int j = 0; j < numCro8; ++j) {
			if(hijosAPI.first.GetCrom(j) != hijosTEST.first.GetCrom(j) || hijosAPI.second.GetCrom(j) != hijosTEST.second.GetCrom(j)) {
				printf("El operador de cruce es erroneo: Se aborta el programa\n");
				return -1;
			}
		}
	}

	//////////////////////////////////////////////////////////////////
	// Realizamos un test al operador de mutación en cada individuo //
	//////////////////////////////////////////////////////////////////
	for(int i = 0; i < tamPob; ++i) {
		int cromosoma = rand() % numCro8;
		unsigned char sinMutar = POB.GetInd(i).GetCrom(cromosoma);
		POB.Mutar_ind(i, cromosoma);
		if(sinMutar == POB.GetInd(i).GetCrom(cromosoma)) {
			printf("El operador de mutacion es erroneo: Se aborta el programa\n");
			return -1;
		}
	}

	return 0;
}
