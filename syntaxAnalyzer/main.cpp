//
//  main.cpp
//  COMP4998
//
//  Created by Roberto Rivera, George Perez, Andres Garcia on 4/25/19.
//  Copyright © 2019 Roberto Rivera, George Perez, Andres Garcia. All rights reserved.
//

#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <array>
using namespace std;

// Abre archivo de salida del analizador lexico
void abrirArchivoEntrada(ifstream &fin, string archivo);

// Devuelve cierto si el string es un token
bool esToken(string token);

// Verifica si token == "LINE", de ser asi almacena la linea actual a currentLine
void setCurrentLine(ifstream &fin, string &token, int &currentLine);

// Imprime la linea actual
void getCurrentLine(int &currentLine);


// Devuleve cierto si el primer token es igual a "<palabraReservada:inicio>", seguido por una secuencia de instruciones y
// el ultimo token es igual a "<palabraReservada:final>"
bool esPrograma(ifstream &fin, string &token, int &currentLine);

// Devuelve cierto si encuentra una instrucción seguida por una secuencia de instrucciones o una instrucción
bool esSecuenciaInst(ifstream &fin, string &token, int &currentLine);

// Devuelve cierto si alguna de las funciones esAsignacion(), esSi(), esEscribe(), o esMientras() devuelven cierto
bool esInstruccion(ifstream &fin, string &token, int &currentLine);

// Devuelve cierto si encuentra un identificador seguido por el operador de asignación luego seguido por una expresión
bool esAsignacion(ifstream &fin, string &token, int &currentLine);

// Devuelve cierto si el token es igual a "<palabraReservada:Si> seguido por una expresión de parentesis, seguido por
// una secuencia de instrucciones [alt]
// alt1) seguida por el token de "<palabraReservada:finsi>"
// alt2) seguida por el token de "<palabraReservada:sino>" seguido por una secuencia de instrucciones y seguido por
// el token "<palabraReservada:finsi"
bool esSi(ifstream &fin, string &token, int &currentLine);

// Llamada por esSi() devuelve cierto si el token es igual a "<palabraReservada:sino", es seguido por una secuencia de
// instrucciones y seguido por el token "<palabraReservada:finsi>"
bool esSino(ifstream &fin, string &token, int &currentLine);

// Devuelve cierto si token es igual a "<palabraReservada:Escribe>" seguido por una secuencia de instruciones y seguida
// por un ';'
bool esEscribe(ifstream &fin, string &token, int &currentLine);

// Devuelve cierto si el token es igual a "<palabraReservada:Mientras", es seguido por una secuencia de instrucciones y
// el seguido por el token "<palabraReservada:finmientras>"
bool esMientras(ifstream &fin, string &token, int &currentLine);

// Devuelve cierto si token es igual a "<parentesisIzquierdo>" seguido por una expreción seguida por el token "<parentesisDerecho>"
bool esExpParentesis(ifstream &fin, string &token, int &currentLine);

// Devuelve cierto si encuentra un factor o la secuencia de un factor seguido por un operador binerio seguido por una expresión
bool esExpresion(ifstream &fin, string &token, int &currentLine);

// Devuelve cierto si encuentra una expresión de parentesis, un numero, o un identificador
bool esFactor (ifstream &fin, string &token, int &currentLine);

// Devuelve cierto si el token es un operador relacional o aritmetico
bool esOpBinario(ifstream &fin, string &token, int &currentLine);

int main(){

	ifstream fin;
	ofstream fout;

	string token, next;
	int currentLine = 0;

	abrirArchivoEntrada(fin, "lex_output.txt");
	fout.open("lista_tokens.txt");

	// Extrae lista de tokens de lex_output.txt, luego la inserta a lista_tokens.txt
	while (!fin.eof()){
		fin >> token;

		if (token == "LINE"){
			/*fin >> next;
			fout << token << " " << next << endl;*/

			fout << token << endl;
		}

		else if (esToken(token)){
			// Para evitar que al final haya una linea vacia que causa que el ultimo token este duplicado
			if (token == "<palabraReservada:final>"){
				fout << token;
			}
			else{
				fout << token << endl;
			}
		}
	}

	fin.close();
	fout.close();

	// Abre archivo que contiene una lista de los tokens del programa fuente
	abrirArchivoEntrada(fin, "lista_tokens.txt");

	// Recorre la lista en base a los tokens hasta llegar al final del archivo
	while (!fin.eof()){
		fin >> token;
		setCurrentLine(fin, token, currentLine);

		if (esPrograma(fin, token, currentLine)){
			continue;
		}
		else{
			break;
		}
	}

	return 0;
}

void abrirArchivoEntrada(ifstream &fin, string archivo) {

	fin.open(archivo);
	if (fin.fail()) {
		if (archivo == "lex_output.txt"){
			cout << "File lex_output.txt does not exist.\n";
		}
		else if (archivo == "lista_tokens.txt"){
			cout << "File lista_tokens.txt does not exist.\n";
		}
	}
}

bool esToken(string token){

	string tokenArray[] = {
	"<palabraReservada:inicio>","<palabraReservada:final>","<palabraReservada:Si>","<palabraReservada:finsi>",
	"<palabraReservada:sino>","<palabraReservada:Mientras>","<palabraReservada:finmientras>","<palabraReservada:Escribe>",
	"<identificador>", "<opRelacional>", "<opAritmetico>", "<opAsignacion>",
	"<parentesisIzquierdo>", "<parentesisDerecho>", "<puntoComa>", "<numero>"};

	int size = sizeof(tokenArray)/sizeof(tokenArray[0]);

	for (int i = 0; i < size; i++){
		if (token == tokenArray[i]){
			return true;
		}
	}
	return false;
}

void setCurrentLine(ifstream &fin, string &token, int &currentLine){
	
	// When using "LINE #" in lista_tokens.txt and when curentLine was string
	/*if (token == "LINE"){

		currentLine = token;
		
		fin >> token;
		currentLine += " " + token;

		fin >> token;
	}*/

	if (token == "LINE"){
		currentLine++;
		fin >> token;
	}
}

void getCurrentLine(int &currentLine){

	//cout << currentLine;

	cout << "LINE " << currentLine;
}

bool esPrograma(ifstream &fin, string &token, int &currentLine){

	// De token ser <palabraReservada:inicio> sigue recorriendo la lista de tokens
	if (token == "<palabraReservada:inicio>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		// De encontrar una secuencia de instrucciones sigue recorriendo la lista de tokens
		if (esSecuenciaInst(fin, token, currentLine)){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			// De el ultimo token de la lista ser <palabraReservada:final> se considera el programa compilado
			if (token == "<palabraReservada:final>"){

				cout << "compiled successfully\n";

				return true;
			}
			else{

				cout << "[Error] in ";
				getCurrentLine(currentLine);
				cout << " <programa>: <palabraReservada:final> not found\n";

				return false;
			}
		}
		else{

			cout << "[Error] in ";
			getCurrentLine(currentLine);
			cout << " <programa>: <secuenciaInst> not found\n";

			return false;
		}
	}
	else{

		cout << "[Error] in ";
		getCurrentLine(currentLine);
		cout << " <programa>: <palabraReservada:inicio> not found\n";

		return false;
	}
}

bool esSecuenciaInst(ifstream &fin, string &token, int &currentLine){

	// De encontrar una secuencia de instrucciones entra al if
	if (esInstruccion(fin, token, currentLine)){

		// para evitar pasar por la palabra reservada sin analizarla de ya tenerla en queue
		if (token == "<palabraReservada:finmientras>"){
			return true;
		}

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		// De encontrar una secuencia de instrucciones devuelve cierto
		if (esSecuenciaInst(fin, token, currentLine)){

			return true;
		}
		// De encontrar una instrucción devuelve cierto
		else if (esInstruccion(fin, token, currentLine)){

			return true;
		}
		// De token ser uno relacionado con la finalización de alguna instrucción, devuelve cierto
		else if (token == "<palabraReservada:finmientras>" || token == "<palabraReservada:sino>" || token == "<palabraReservada:finsi>" || token == "<palabraReservada:final>"){ // Aqui poner palabras reservadas de terminacion de inst
			
			return true;
		}
		else{

			cout << "[Error] in ";
			getCurrentLine(currentLine);
			cout << " <secuenciaInst>: <instruccion> or <secuenciaInst> not found\n";

			return false;
		}
	}
	else{

		return false;
	}
}

bool esInstruccion(ifstream &fin, string &token, int &currentLine){

	// De alguna de las siguientes funciones devolver cierto, esInstrucción() devuelve cierto
	if (esMientras(fin, token, currentLine) || esEscribe(fin, token, currentLine) || esAsignacion(fin, token, currentLine) || esSi(fin, token, currentLine)){

		return true;
	}
	else{

		return false;
	}
}

bool esAsignacion(ifstream &fin, string &token, int &currentLine){

	// De token ser <identificador> sigue recorriendo la lista de tokens
	if (token == "<identificador>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		// De token ser <opAsignacion> sigue recorriendo la lista de tokens
		if (token == "<opAsignacion>"){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			// De encontrar una expresión devuelve cierto
			if (esExpresion(fin, token, currentLine)){

				return true;
			}
			else{

				cout << "[Error] in ";
				getCurrentLine(currentLine);
				cout << " <asignacion>: <esExpresion> not found\n";

				return false;
			}
		}
		else{

			return false;
		}
	}
	else{

		return false;
	}
}

bool esSi(ifstream &fin, string &token, int &currentLine){

	// De token ser <palabraReservada:Si> sigue recorriendo la lista de tokens
	if (token == "<palabraReservada:Si>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		// De encontrar una expresión de paréntesis sigue recorriendo la lista de tokens
		if (esExpParentesis(fin, token, currentLine)){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			// De encontrar una secuencia de instrucciones sigue recorriendo la lista de tokens
			if (esSecuenciaInst(fin, token, currentLine)){

				// para evitar pasar por la palabra reservada sin analizarla de ya tenerla en queue
				if (token == "<palabraReservada:finsi>"){

					return true;
				}
				else if (esSino(fin, token, currentLine)){

					return true;
				}

				fin >> token;
				setCurrentLine(fin, token, currentLine);
				
				// De token ser <palabraReservada:finsi> devuelve cierto
				if (token == "<palabraReservada:finsi>"){

					return true;
				}
				// De esSino() devolver cierto, esSi() devuelve cierto
				else if (esSino(fin, token, currentLine)){

					return true;
				}
				else{

					cout << "[Error] in ";
					getCurrentLine(currentLine);
					cout << ": <palabraReservada:finsi> not found\n";

					return false;
				}
			}
			else{

				cout << "[Error] in ";
				getCurrentLine(currentLine);
				cout << ": <secuenciaInst> not found\n";

				return false;
			}
		}
		else{

			cout << "[Error] in ";
			getCurrentLine(currentLine);
			cout << ": <expParentesis> not found\n";

			return false;
		}
	}
	else{

		return false;
	}
}

bool esSino(ifstream &fin, string &token, int &currentLine){

	// De token ser <palabraReservada:sino> sigue recorriendo la lista de tokens
	if (token == "<palabraReservada:sino>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		// De encontrar una secuencia de instrucciones sigue recorriendo la lista de tokens
		if (esSecuenciaInst(fin, token, currentLine)){

			// De token ser <palabraReservada:finsi> devuelve cierto; si no, coge el proximo token
			if (token == "<palabraReservada:finsi>"){

				return true;
			}

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			// De token ser <palabraReservada:finsi> devuelve cierto
			if (token == "<palabraReservada:finsi>"){

				return true;
			}
			else{

				cout << "[Error] in ";
				getCurrentLine(currentLine);
				cout << ": <palabraReservada:finsi> not found\n";

				return false;
			}
		}
		else{

			cout << "[Error] in ";
			getCurrentLine(currentLine);
			cout << ": <secuenciaInst> not found\n";

			return false;
		}
	}
	else{

		return false;
	}
}

bool esEscribe(ifstream &fin, string &token, int &currentLine){

	// De token ser <palabraReservada:Escribe> sigue recorriendo la lista de tokens
	if (token == "<palabraReservada:Escribe>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		// De encontrar una expresión de paréntesis sigue recorriendo la lista de tokens
		if (esExpParentesis(fin, token, currentLine)){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			// De token ser <puntoComa> devuelve cierto
			if (token == "<puntoComa>"){

				return true;
			}
			else{

				cout << "[Error] in ";
				getCurrentLine(currentLine);
				cout << ": <puntoComa> not found\n";

				return false;
			}
		}
		else{

			cout << "[Error] in ";
			getCurrentLine(currentLine);
			cout << ": <expParentesis> not found\n";

			return false;
		}
	}
	else{

		return false;
	}
}

bool esMientras(ifstream &fin, string &token, int &currentLine){

	// De token ser <palabraReservada:Mientras> sigue recorriendo la lista de tokens
	if (token == "<palabraReservada:Mientras>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		// De encontrar una expresión de paréntesis sigue recorriendo la lista de tokens
		if (esExpParentesis(fin, token, currentLine)){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			// De encontrar una secuencia de instrucciones sigue recorriendo la lista de tokens
			if (esSecuenciaInst(fin, token, currentLine)){ // originalmente esSecuenciaInst

				// para evitar pasar por la palabra reservada sin analizarla de ya tenerla en queue
				if (token == "<palabraReservada:finmientras>"){

					return true;
				}

				fin >> token;
				setCurrentLine(fin, token, currentLine);

				// De token ser <palabraReservada:finmientras> devuelve cierto
				if (token == "<palabraReservada:finmientras>"){

					return true;
				}
				else{

					cout << "[Error] in ";
					getCurrentLine(currentLine);
					cout << ": <palabraReservada:finmientras> not found\n";

					return false;
				}
			}
			else{

				cout << "[Error] in ";
				getCurrentLine(currentLine);
				cout << ": <secuenciaInst> not found\n";

				return false;
			}
		}
		else{
			
			cout << "[Error] in ";
			getCurrentLine(currentLine);
			cout << ": <expParentesis> not found\n";

			return false;
		}
	}
	else{

		return false;
	}
}

bool esExpParentesis(ifstream &fin, string &token, int &currentLine){

	// De token ser <parentesisIzquierdo> sigue recorriendo la lista de tokens
	if (token == "<parentesisIzquierdo>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		// De encontrar una expresión sigue recorriendo la lista de tokens
		if (esExpresion(fin, token, currentLine)){

			// aqui ya estoy en parentesisDerecho al coger el prox lo brincamos
			//fin >> token;

			// De token ser <parentesisDerecho> devuelve cierto
			if (token == "<parentesisDerecho>"){

				return true;
			}
			else{

				cout << "[Error] in ";
				getCurrentLine(currentLine);
				cout << ": <parentesisDerecho> not found\n";

				return false;
			}
		}
		else{

			cout << "[Error] in ";
			getCurrentLine(currentLine);
			cout << ": <expresion>\n";

			return false;
		}
	}
	else{

		cout << "[Error] in ";
		getCurrentLine(currentLine);
		cout << ": <parentesisIzquierdo> not found\n";

		return false;
	}
}

bool esExpresion(ifstream &fin, string &token, int &currentLine){

	// De encontrar un factor sigue recorriendo la lista de tokens
	if (esFactor(fin, token, currentLine)){
		
		fin >> token;
		setCurrentLine(fin, token, currentLine);

		// De token ser un operador binario sigue recorriendo la lista de tokens
		if (esOpBinario(fin, token, currentLine)){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			// De encontrar una expresión devuelve cierto
			if (esExpresion(fin, token, currentLine)){
				
				return true;
			}
			else{

				cout << "[Error] in ";
				getCurrentLine(currentLine);
				cout << ": <expresion> not found\n";

				return false;
			}
		}
		else{

			return true;
		}
	}
	else{

		cout << "[Error] in ";
		getCurrentLine(currentLine);
		cout << ": <factor> not found\n";

		return false;
	}
}

bool esFactor(ifstream &fin, string &token, int &currentLine){
	
	// De token ser <numero>, <identificador>, o encuentra una expesión de paréntesis; devuelve cierto
	if (token == "<numero>" || token == "<identificador>" || esExpParentesis(fin, token, currentLine)){

		return true;
	}
	else{

		//cout << "[Error] in \"" << token << "\": " << "is not a <factor>\n";

		return false;
	}
}

bool esOpBinario(ifstream &fin, string &token, int &currentLine){
	
	// De token ser <opAritmetico> o <opRelacional> devuelve cierto
	if (token == "<opAritmetico>" || token == "<opRelacional>"){

		return true;
	}
	else{

		//cout << "[Error] in \"" << token << "\": " << "is not a <opBinario>\n";

		return false;
	}
}