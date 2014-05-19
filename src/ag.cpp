/*
«Copyright 2014 Juan José Escobar Pérez»

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
	ptree ptin, ptout;

	// Datos de entrada
	read_xml("config.xml", ptin);
	int tamPob = ptin.get<int>("config.tamPob");
	int numCro = ptin.get<int>("config.numCro");
	int nGen = ptin.get<int>("config.nGen");

	// Datos de salida
	ptout.put("resultados.tamPob", tamPob);
	ptout.put("resultados.numCro", numCro);
	ptout.put("resultados.nGen", nGen);

	struct timeval timPOB;
	
	Poblacion POB(tamPob, numCro);
	int numCro8 = POB.GetInd(0).Size();

	gettimeofday(&timPOB, NULL);
	double dTime1 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);

	// Inicializo aleatoriamente los cromosomas
	srand(time(NULL));
	POB.Inicializar();

	Poblacion HIJOS;
	// El algoritmo acaba cuando se generan nGen generaciones
	for(int i = 0; i < nGen; ++i) {
		char NumG[100];
		sprintf(NumG, "resultados.gen%d", i);

		// Realizamos cruce (Internamente realiza torneo, selección de candidatos y ruleta)
		long int Tfitness;
		vector< pair<int, long int> > fitness;
		HIJOS = POB.Cruzar(fitness, Tfitness);
		char aux[100];
		sprintf(aux, "%s.totalFitness", NumG);
		ptout.put(aux, Tfitness);
		sprintf(aux, "%s.fitMedia", NumG);
		ptout.put(aux, ((double) Tfitness) / tamPob);
		sprintf(aux, "%s.mejorFitness", NumG);
		ptout.put(aux, fitness[0].second);

		// Realizo mutacion en cada hijo de 1 o varios bits
		for(int i = 0; i < tamPob; ++i) {
			int cromosoma = rand() % numCro8;
			HIJOS.Mutar_ind(i, cromosoma);
		}

		// Ahora establecemos qué HIJOS sustituirá a la anterior población
		HIJOS.Swap(POB);
	}

	gettimeofday(&timPOB, NULL);
	double dTime2 = timPOB.tv_sec+(timPOB.tv_usec/1000000.0);
	ptout.put("resultados.tiempo", dTime2 - dTime1);

	time_t tSac = time(NULL); // instante actual
	struct tm *tmP = localtime(&tSac);

	char nombre[100];
	sprintf(nombre, "res/genetico2014-%d-%d-%d-%d%d%d", nGen, tamPob, numCro, tmP->tm_hour, tmP->tm_min, tmP->tm_sec);
	write_xml(nombre, ptout);

	return 0;
}
