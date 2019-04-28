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

bool esPrograma(ifstream &fin, string &token);

bool esSecuenciaInst(ifstream &fin, string &token);
bool esInstruccion(ifstream &fin, string &token);

bool esAsignacion(ifstream &fin, string &token);
bool esSi(ifstream &fin, string &token);
bool esEscribe(ifstream &fin, string &token);
bool esMientras(ifstream &fin, string &token);

bool esExpParentesis(ifstream &fin, string &token);
bool esExpresion(ifstream &fin, string &token);
bool esFactor (ifstream &fin, string &token);

bool esOpBinario(ifstream &fin, string &token);

int main(){

	ifstream fin;
	ofstream fout;

	int lineCount;

	string token;

	abrirArchivoEntrada(fin, "lex_output.txt");
	fout.open("lista_tokens.txt");

	// Extrae lista de tokens de lex_output.txt, luego la inserta a lista_tokens.txt
	while (!fin.eof()){
		fin >> token;

		if (token == "LINE"){
			lineCount++;
			//cout << "LINE " << lineCount << "\n";
			//fout << "LINE " << lineCount << "\n";
		}

		// Si utilizar lista_tokens.txt no funciona, hacer todo dentro de este else if
		else if (esToken(token)){
			// Para evitar que al final haya una linea vacia que causa que el ultimo token este duplicado
			if (token == "<palabraReservada:final>"){
				//cout << token;
				fout << token;
			}
			else{
				//cout << token << endl;
				fout << token << endl;
			}
		}
	}

	fin.close();
	fout.close();

	abrirArchivoEntrada(fin, "test.txt");

	while (!fin.eof()){
		fin >> token;

		if (esPrograma(fin, token)){
			cout << "ENCONTRE UN PROGRAMA!!!\n\n";
		}

		/*if (esMientras(fin, token)){
			cout << "ENCONTRE UN MIENTRAS!!!\n\n";
		}*/

		/*if (esEscribe(fin, token)){
			cout << "Encontre un escribe!!!\n\n";
		}*/

		/*if (esAsignacion(fin, token)){
			cout << "Encontre una asignacion!!!\n";
		}*/

		/*if (esInstruccion(fin, token)){
			cout << "Encontre una instruccion!!!\n";
		}*/

		/*if (esExpresion(fin, token)){
			cout << "Encontre una expresion!!!\n";
		}

		if (esOpBinario(fin, token)){
			cout << "Encontre un OpBinario!!!\n";
		}

		if (esExpParentesis(fin, token)){
			cout << "Encontre una expParentesis!!!\n";
		}*/
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

bool esPrograma(ifstream &fin, string &token){
	cout << token << endl;

	if (token == "<palabraReservada:inicio>"){
		cout << "esPrograma: if 1\n";

		fin >> token;
		cout << token << endl;

		if (esSecuenciaInst(fin, token)){
			cout << "esPrograma: if 2\n";

			fin >> token;
			cout << token << endl;

			if (token == "<palabraReservada:final>"){
				cout << "esPrograma: if 3 --returns true--\n";

				return true;
			}
			else{
				cout << "esPrograma: else 3 --returns false--\n";

				return false;
			}
		}
		else{
			cout << "esPrograma: else 2 --returns false--\n";

			return false;
		}
	}
	else{
		cout << "esPrograma: else 1 --returns false--\n";

		return false;
	}
}

bool esSecuenciaInst(ifstream &fin, string &token){
	if (esInstruccion(fin, token)){
		cout << "esSecuenciaInst: if 1\n";
		cout << token << endl;

		if (token == "<palabraReservada:finmientras>"){
			cout << "esSecuenciaInst: if NUEVO --returns true--\n";
			return true;
		}

		fin >> token;

		if (esSecuenciaInst(fin, token)){
			cout << "esSecuenciaInst: if 2 --returns true--\n";

			return true;
		}
		else if (esInstruccion(fin, token)){
			cout << "esSecuenciaInst: else if 2 --returns true--\n";

			return true;
		}
		else if (token == "<palabraReservada:finmientras>" || token == "<palabraReservada:finsi>" || token == "<palabraReservada:final>"){ // Aqui poner palabras reservadas de terminacion de inst
			return true;
		}
		else{
			cout << "esSecuenciaInst: else 2 --returns false--\n";

			return false;
		}
	}
	else{
		cout << "esSecuenciaInst: else 1 --returns false--\n";

		return false;
	}
}

bool esInstruccion(ifstream &fin, string &token){ // (Must test potential flaw) si el primero es falso el token ya se movio
	if (esMientras(fin, token) || esEscribe(fin, token) || esAsignacion(fin, token)){

		return true;
	}
	else{

		return false;
	}
}

bool esAsignacion(ifstream &fin, string &token){
	if (token == "<identificador>"){
		cout << "esAsignacion: if 1\n";

		fin >> token;

		if (token == "<opAsignacion>"){
			cout << "esAsignacion: if 2\n";

			fin >> token;

			if (esExpresion(fin, token)){
				cout << "esAsignacion: if 3 --returns true--\n";
				cout << token << endl << endl;

				return true;

				/* Commented out because asignacion should not end in ';' */
				/*if (token == "<puntoComa>"){
					cout << "esAsignacion: if 4 --returns true--\n";

					return true;
				}*/
			}
			else{
				cout << "esAsignacion: else 3 --returns false--\n";

				return false;
			}
		}
		else{
			cout << "esAsignacion: else 2 --returns false--\n";

			return false;
		}
	}
	else{
		cout << "esAsignacion: else 1 --returns false--\n";

		return false;
	}
}

bool esSi(ifstream &fin, string &token){
	if (token == "<palabraReservada:Si>"){

		fin >> token;

		if (esExpParentesis(fin, token)){
			
			fin >> token;

			if (esSecuenciaInst(fin, token)){

				fin >> token;

				if (token == "<palabraReservada:sino>"){

					fin >> token;

					if (esSecuenciaInst(fin, token)){
						
						return true;
					}
				}
				else if (token == "<palabraReservada:finsi>"){

					return true;
				}
				else{

					return false;
				}
			}
		}
	}
	else{

		return false;
	}
}

bool esEscribe(ifstream &fin, string &token){
	cout << token << endl;

	if (token == "<palabraReservada:Escribe>"){
		cout << "esEscribe: if 1\n";

		fin >> token;
		cout << token << endl;

		if (esExpParentesis(fin, token)){
			cout << "esEscribe: if 2\n";

			fin >> token;
			cout << token << endl;

			if (token == "<puntoComa>"){
				cout << "esEscribe: if 3 --returns true--\n";

				return true;
			}
			else{
				cout << "esEscribe: else 3 --returns false--\n";

				return false;
			}
		}
		else{
			cout << "esEscribe: else 2 --returns false--\n";

			return false;
		}
	}
	else{
		cout << "esEscribe: else 1 --returns false--\n";

		return false;
	}
}

bool esMientras(ifstream &fin, string &token){
	cout << token << endl;

	if (token == "<palabraReservada:Mientras>"){
		cout << "esMientras: if 1\n";

		fin >> token;
		cout << token << endl;

		if (esExpParentesis(fin, token)){
			cout << "esMientras: if 2\n";

			fin >> token;
			cout << token << endl;

			if (esSecuenciaInst(fin, token)){ // originalmente esSecuenciaInst
				cout << "esMientras: if 3\n";

				if (token == "<palabraReservada:finmientras>"){
					cout << "esMientras: if 4 NUEVO --returns true--\n";

					return true;
				}

				fin >> token;
				cout << token << endl;

				if (token == "<palabraReservada:finmientras>"){
					cout << "esMientras: if 4 --returns true--\n";

					return true;
				}
				else{
					cout << "esMientras: else 4 --returns false--\n";

					return false;
				}
			}
			else{
				cout << "esMientras: else 3 --returns false--\n";

				return false;
			}
		}
		else{
			cout << "esMientras: else 2 --returns false--\n";

			return false;
		}
	}
	else{
		cout << "esMientras: else 1 --returns false--\n";

		return false;
	}
}

bool esExpParentesis(ifstream &fin, string &token){
	cout << token << endl;

	if (token == "<parentesisIzquierdo>"){
		cout << "esExpParentesis: if 1\n";

		fin >> token;
		cout << token << endl;

		if (esExpresion(fin, token)){
			cout << "esExpParentesis: if 2\n";

			// aqui ya estoy en parentesisDerecho al coger el prox lo brincamos
			//fin >> token;
			cout << token << endl;

			if (token == "<parentesisDerecho>"){
				cout << "esExpParentesis: if 3 --returns true--\n";

				return true;
			}
			else{
				cout << "esExpParentesis: else 3 --returns false--\n";

				return false;
			}
		}
		else{
			cout << "esExpParentesis: else 2 --returns false--\n";

			return false;
		}
	}
	else{
		cout << "esExpParentesis: else 1 --returns false--\n";

		return false;
	}
}

bool esExpresion(ifstream &fin, string &token){
	cout << token << endl;

	if (esFactor(fin, token)){
		cout << "esExpresion: if 1\n";
		
		fin >> token;
		cout << token << endl;

		if (esOpBinario(fin, token)){
			cout << "esExpresion: if 2\n";

			fin >> token;
			cout << token << endl;

			if (esExpresion(fin, token)){
				cout << "esExpresion: if 3 --returns true--\n";
				
				return true;
			}
			else{
				cout << "esExpresion: else 3 --returns false--\n";

				return false;
			}
		}
		else{
			cout << "esExpresion: else 2 --returns true--\n";

			return true;
		}
	}
	else{
		cout << "esExpresion: else 1 --returns false--\n";

		return false;
	}
}

bool esFactor(ifstream &fin, string &token){
	if (token == "<numero>" || token == "<identificador>" || esExpParentesis(fin, token)){

		return true;
	}
	else{

		return false;
	}
}

bool esOpBinario(ifstream &fin, string &token){
	if (token == "<opAritmetico>" || token == "<opRelacional>"){

		return true;
	}
	else{

		return false;
	}
}