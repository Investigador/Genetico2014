/*
«Copyright 2014 Juan José Escobar Pérez»

generadorR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Foobar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with generadorR.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cstdio>

using namespace std;

int main(int argc, char *argv[]) {

	// El segundo argumento es el nombre del .xml que contiene los datos
	if(argc < 2)
		exit(-1);

	// Crea un objeto árbol para el xml y leo los parametros
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(argv[1], pt);
	int nGen = pt.get<int>("resultados.nGen");

	// Empiezo a escribir el código en R
	string fitTotal = "fitTotal = c(";
	string fitMedia = "fitMedia = c(";
	string fitMejor = "fitMejor = c(";
	char NumG[100], aux[100];

	// Leo los datos
	for(int i =  1; i < nGen; ++i) {
		sprintf(NumG, "resultados.gen%d", i);
		sprintf(aux, "%s.totalFitness", NumG);
		fitTotal.append(pt.get<string>(aux));
		fitTotal.append(",");
		sprintf(aux, "%s.fitMedia", NumG);
		fitMedia.append(pt.get<string>(aux));
		fitMedia.append(",");
		sprintf(aux, "%s.mejorFitness", NumG);
		fitMejor.append(pt.get<string>(aux));
		fitMejor.append(",");
	}

	// Elimino la última cobra sobrante y cierro el conjunto con un paréntesis
	fitTotal.pop_back();
	fitMedia.pop_back();
	fitMejor.pop_back();
	fitTotal.append(")");
	fitMedia.append(")");
	fitMejor.append(")");

	// Abro el fichero .r para echar los datos en formato que R reconoce
	FILE *r = fopen("res/tablaR.r", "w");
	if(r != NULL) {
		fputs(fitTotal.c_str(), r);
		fputs("\n", r);
		fputs(fitMedia.c_str(), r);
		fputs("\n", r);
		fputs(fitMejor.c_str(), r);
		fputs("\n", r);
		fputs("tabla = data.frame(cbind(fitTotal,fitMedia,fitMejor))", r);
		fputs("\n", r);
		fputs("tabla", r);
		fclose(r);
	}

	return 0;
}
