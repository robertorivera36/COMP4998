//
//  main.cpp
//  COMP4998
//
//  Created by George Perez, Roberto Rivera, Andres Garcia on 4/25/19.
//  Copyright © 2019 George Perez, Roberto Rivera, Andres Garcia. All rights reserved.
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

bool esAsignacion(ifstream &fin, string &token);
bool esSi(ifstream &fin, string &token);
bool esEscribe(ifstream &fin, string &token);
bool esMientras(ifstream &fin, string &token);

bool esExpresion(ifstream &fin, string &token);
bool esFactor (ifstream &fin, string &token);

int main(){

	ifstream fin;
	ofstream fout;

	int lineCount;

	string token;

	bool secuenciaInst;
	bool instruccion;
	bool asignacion, instSi, instEscribe, instMientras;
	bool expresion;

	abrirArchivoEntrada(fin, "lex_output.txt");
	fout.open("lista_tokens.txt");

	// Extrae lista de tokens de lex_output.txt, luego la inserta a lista_tokens.txt
	while (!fin.eof()){
		fin >> token;

		if (token == "LINE"){
			lineCount++;
			cout << "LINE " << lineCount << "\n";
			//fout << "LINE " << lineCount << "\n";
		}

		// Si utilizar lista_tokens.txt no funciona, hacer todo dentro de este else if
		else if (esToken(token)){
			// Para evitar que al final haya una linea vacia que causa que el ultimo token este duplicado
			if (token == "<palabraReservada:final>"){
				cout << token;
				fout << token;
			}
			else{
				cout << token << endl;
				fout << token << endl;
			}
		}
	}

	fin.close();
	fout.close();

	abrirArchivoEntrada(fin, "lista_tokens.txt");

	while (!fin.eof()){
		fin >> token;

		if (token == "<palabraReservada:inicio>"){
			fin >> token;

			if (token == "<identificador>" || token == "<palabraReservada:Si>" || token == "<palabraReservada:Escribe>" || "<palabraReservada:Mientras>"){

				// <asignacion>
				if (esAsignacion(fin, token)){
					// code
				}
				else if (esSi(fin, token)){
					// code
				}
				else if (esEscribe(fin, token)){
					// code
				}
				else if (esMientras(fin, token)){
					// code
				}
			}
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
	"<identificador>", "<opRelacional>", "<opAritmetico", "<opAsignacion>",
	"<parentesisIzquierdo>", "<parentesisDerecho>", "<puntoComa>", "<numero>"};

	int size = sizeof(tokenArray)/sizeof(tokenArray[0]);

	for (int i = 0; i < size; i++){
		if (token == tokenArray[i]){
			return true;
		}
	}
	return false;
}

bool esAsignacion(ifstream &fin, string &token){
	if (token == "<identificador>"){
		fin >> token;

		if (token == "<opAsignacion>"){
			fin >> token;

			//<factor>
			if (token == "<identificador>" || token == "<numero>" || token == "<parentesisIzquierdo>"){

				//<expParentesis>
				if (token == "<parentesisIzquierdo>"){
					fin >> token;
				}
				else{
					fin >> token;
				}
			}
		}
	}
	else{
		return false;
	}
}

bool esSi(ifstream &fin, string &token){
	// code
}
bool esEscribe(ifstream &fin, string &token){
	// code
}
bool esMientras(ifstream &fin, string &token){
	// code
}