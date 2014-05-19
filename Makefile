# JUAN JOSÉ ESCOBAR PÉREZ
# 	15473138-A

SRC = src
INC = include
OBJ = obj
BIN = bin
LIB = lib
DOC = doc
RES = res

CXX = g++
CPPFLAGS = -c -Iinclude
LIBR = ar rsv
XMLFLAGS = -I/usr/include/boost_1_55_0

# ************ Objetivos principales ************

all: $(LIB)/Poblacion.a $(LIB)/Individuo.a $(BIN)/ag $(BIN)/test $(BIN)/generadorR documentacion

# ************ Generación de documentación ******************

documentacion:
	doxygen $(DOC)/Doxyfile

# ************ Compilación de módulos y librerias ************

$(OBJ)/Individuo.o: $(SRC)/Individuo.cpp $(INC)/Individuo.h
	$(CXX) $(CPPFLAGS) $(SRC)/Individuo.cpp -o $(OBJ)/Individuo.o
$(OBJ)/Poblacion.o: $(SRC)/Poblacion.cpp $(INC)/Poblacion.h
	$(CXX) $(CPPFLAGS) $(SRC)/Poblacion.cpp -o $(OBJ)/Poblacion.o
$(OBJ)/ag.o : $(SRC)/ag.cpp
	$(CXX) $(CPPFLAGS) $(XMLFLAGS) $(SRC)/ag.cpp -o $(OBJ)/ag.o
$(OBJ)/test.o: $(SRC)/test.cpp
	$(CXX) $(CPPFLAGS) $(XMLFLAGS) $(SRC)/test.cpp -o $(OBJ)/test.o
$(OBJ)/generadorR.o: $(SRC)/generadorR.cpp
	$(CXX) $(CPPFLAGS) $(XMLFLAGS) -std=c++11 $(SRC)/generadorR.cpp -o $(OBJ)/generadorR.o

# ************ Bibliotecas ************

$(LIB)/Individuo.a: $(OBJ)/Individuo.o
	$(LIBR) $(LIB)/Individuo.a $(OBJ)/Individuo.o
$(LIB)/Poblacion.a: $(OBJ)/Poblacion.o
	$(LIBR) $(LIB)/Poblacion.a $(OBJ)/Poblacion.o

# ************ Enlazado y creación de ejecutables ************

$(BIN)/ag: $(OBJ)/ag.o $(OBJ)/Individuo.o $(OBJ)/Poblacion.o
	$(CXX) $(OBJ)/ag.o $(OBJ)/Individuo.o $(OBJ)/Poblacion.o -o $(BIN)/ag
$(BIN)/test: $(OBJ)/test.o $(OBJ)/Individuo.o $(OBJ)/Poblacion.o
	$(CXX) $(OBJ)/test.o $(OBJ)/Individuo.o $(OBJ)/Poblacion.o -o $(BIN)/test
$(BIN)/generadorR: $(OBJ)/generadorR.o
	$(CXX) $(OBJ)/generadorR.o -o $(BIN)/generadorR

# ************ Limpieza ***************

clean:
	@echo "Limpiando..."
	@\rm -rf $(OBJ)/* $(SRC)/*~ $(INC)/*~ $(DOC)/*~ ./*~

mrproper: clean
	@echo "Mrproper..."
	@\rm -rf $(BIN)/*
	@\rm -rf $(DOC)/html/*
	@\rm -rf $(LIB)/*
	@\rm -rf $(RES)/*
