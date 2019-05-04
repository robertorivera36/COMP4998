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
void setCurrentLine(ifstream &fin, string &token, int &currentLine){
	// Using "LINE #" in lista_tokens.txt and curentLine was string
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

bool esPrograma(ifstream &fin, string &token, int &currentLine);

bool esSecuenciaInst(ifstream &fin, string &token, int &currentLine);
bool esInstruccion(ifstream &fin, string &token, int &currentLine);

bool esAsignacion(ifstream &fin, string &token, int &currentLine);
bool esSi(ifstream &fin, string &token, int &currentLine);
bool esSino(ifstream &fin, string &token, int &currentLine);
bool esEscribe(ifstream &fin, string &token, int &currentLine);
bool esMientras(ifstream &fin, string &token, int &currentLine);

bool esExpParentesis(ifstream &fin, string &token, int &currentLine);
bool esExpresion(ifstream &fin, string &token, int &currentLine);
bool esFactor (ifstream &fin, string &token, int &currentLine);

bool esOpBinario(ifstream &fin, string &token, int &currentLine);

int main(){

	ifstream fin;
	ofstream fout;

	string token, next;
	int currentLine;

	abrirArchivoEntrada(fin, "lex_output.txt");
	fout.open("lista_tokens.txt");

	// Extrae lista de tokens de lex_output.txt, luego la inserta a lista_tokens.txt
	while (!fin.eof()){
		fin >> token;

		if (token == "LINE"){
			//fin >> next;

			//fout << token << " " << next << endl;
			fout << token << endl;
		}

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

	// Al finalizar el programa utilizar "lista_tokens.txt"
	abrirArchivoEntrada(fin, "test.txt");

	while (!fin.eof()){
		fin >> token;
		setCurrentLine(fin, token, currentLine);

		if (esPrograma(fin, token, currentLine)){
			continue;
		}
		else{
			break;
		}

		/*if (esSi(fin, token)){
			cout << "ENCONTRE UN SI!!!\n\n";
		}*/

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

bool esPrograma(ifstream &fin, string &token, int &currentLine){

	if (token == "<palabraReservada:inicio>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		if (esSecuenciaInst(fin, token, currentLine)){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			if (token == "<palabraReservada:final>"){

				cout << "compiled successfully\n";

				return true;
			}
			else{

				cout << "[Error] in <programa>: <palabraReservada:final> not found\n";

				return false;
			}
		}
		else{

			cout << "[Error] in <programa>: <secuenciaInst> not found\n";

			return false;
		}
	}
	else{

		cout << "[Error] in <programa>: <palabraReservada:inicio> not found\n";
		getCurrentLine(currentLine);

		return false;
	}
}

bool esSecuenciaInst(ifstream &fin, string &token, int &currentLine){
	if (esInstruccion(fin, token, currentLine)){

		// para evitar pasar por la palabra reservada sin analizarla de ya tenerla en queue
		if (token == "<palabraReservada:finmientras>"){
			return true;
		}

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		if (esSecuenciaInst(fin, token, currentLine)){

			return true;
		}
		else if (esInstruccion(fin, token, currentLine)){

			return true;
		}
		else if (token == "<palabraReservada:finmientras>" || token == "<palabraReservada:sino>" || token == "<palabraReservada:finsi>" || token == "<palabraReservada:final>"){ // Aqui poner palabras reservadas de terminacion de inst
			
			return true;
		}
		else{

			cout << "[Error] in <secuenciaInst>: <instruccion> or <secuenciaInst> not found\n";

			return false;
		}
	}
	else{

		return false;
	}
}

bool esInstruccion(ifstream &fin, string &token, int &currentLine){
	if (esMientras(fin, token, currentLine) || esEscribe(fin, token, currentLine) || esAsignacion(fin, token, currentLine) || esSi(fin, token, currentLine)){

		return true;
	}
	else{

		return false;
	}
}

bool esAsignacion(ifstream &fin, string &token, int &currentLine){
	if (token == "<identificador>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		if (token == "<opAsignacion>"){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			if (esExpresion(fin, token, currentLine)){

				return true;

				/* Commented out because asignacion should not end in ';' */
				/*if (token == "<puntoComa>"){
					cout << "esAsignacion: if 4 --returns true--\n";

					return true;
				}*/
			}
			else{

				cout << "[Error] in <asignacion>: <esExpresion> not found\n";

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

	if (token == "<palabraReservada:Si>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		if (esExpParentesis(fin, token, currentLine)){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

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
				
				if (token == "<palabraReservada:finsi>"){

					return true;
				}
				else if (esSino(fin, token, currentLine)){

					return true;
				}
				else{

					cout << "[Error]: <palabraReservada:finsi> not found\n";

					return false;
				}
			}
			else{

				cout << "[Error]: <secuenciaInst> not found\n";

				return false;
			}
		}
		else{

			cout << "[Error]: <expParentesis> not found\n";

			return false;
		}
	}
	else{

		return false;
	}
}

bool esSino(ifstream &fin, string &token, int &currentLine){

	if (token == "<palabraReservada:sino>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		if (esSecuenciaInst(fin, token, currentLine)){

			if (token == "<palabraReservada:finsi>"){

				return true;
			}

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			if (token == "<palabraReservada:finsi>"){

				return true;
			}
			else{

				cout << "[Error]: <palabraReservada:finsi> not found\n";

				return false;
			}
		}
		else{

			cout << "[Error]: <secuenciaInst> not found\n";

			return false;
		}
	}
	else{

		return false;
	}
}

bool esEscribe(ifstream &fin, string &token, int &currentLine){

	if (token == "<palabraReservada:Escribe>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		if (esExpParentesis(fin, token, currentLine)){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			if (token == "<puntoComa>"){

				return true;
			}
			else{

				cout << "[Error]: <puntoComa> not found\n";

				return false;
			}
		}
		else{

			cout << "[Error]: <expParentesis> not found\n";
			getCurrentLine(currentLine);

			return false;
		}
	}
	else{

		return false;
	}
}

bool esMientras(ifstream &fin, string &token, int &currentLine){

	if (token == "<palabraReservada:Mientras>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		if (esExpParentesis(fin, token, currentLine)){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			if (esSecuenciaInst(fin, token, currentLine)){ // originalmente esSecuenciaInst

				// para evitar pasar por la palabra reservada sin analizarla de ya tenerla en queue
				if (token == "<palabraReservada:finmientras>"){

					return true;
				}

				fin >> token;
				setCurrentLine(fin, token, currentLine);

				if (token == "<palabraReservada:finmientras>"){

					return true;
				}
				else{

					cout << "[Error]: <palabraReservada:finmientras> not found\n";

					return false;
				}
			}
			else{

				cout << "[Error]: <secuenciaInst> not found\n";

				return false;
			}
		}
		else{
			
			cout << "[Error]: <expParentesis> not found\n";

			return false;
		}
	}
	else{

		return false;
	}
}

bool esExpParentesis(ifstream &fin, string &token, int &currentLine){

	if (token == "<parentesisIzquierdo>"){

		fin >> token;
		setCurrentLine(fin, token, currentLine);

		if (esExpresion(fin, token, currentLine)){

			// aqui ya estoy en parentesisDerecho al coger el prox lo brincamos
			//fin >> token;

			if (token == "<parentesisDerecho>"){

				return true;
			}
			else{

				cout << "[Error]: <parentesisDerecho> not found\n";

				return false;
			}
		}
		else{

			cout << "[Error] in: <expresion>\n";

			return false;
		}
	}
	else{

		cout << "[Error]: <parentesisIzquierdo> not found\n";

		return false;
	}
}

bool esExpresion(ifstream &fin, string &token, int &currentLine){

	if (esFactor(fin, token, currentLine)){
		
		fin >> token;
		setCurrentLine(fin, token, currentLine);

		if (esOpBinario(fin, token, currentLine)){

			fin >> token;
			setCurrentLine(fin, token, currentLine);

			if (esExpresion(fin, token, currentLine)){
				
				return true;
			}
			else{

				cout << "[Error]: <expresion> not found\n";

				return false;
			}
		}
		else{

			return true;
		}
	}
	else{

		cout << "[Error]: <factor> not found\n";

		return false;
	}
}

bool esFactor(ifstream &fin, string &token, int &currentLine){
	
	if (token == "<numero>" || token == "<identificador>" || esExpParentesis(fin, token, currentLine)){

		return true;
	}
	else{

		//cout << "[Error] in \"" << token << "\": " << "is not a <factor>\n";

		return false;
	}
}

bool esOpBinario(ifstream &fin, string &token, int &currentLine){
	
	if (token == "<opAritmetico>" || token == "<opRelacional>"){

		return true;
	}
	else{

		//cout << "[Error] in \"" << token << "\": " << "is not a <opBinario>\n";

		return false;
	}
}