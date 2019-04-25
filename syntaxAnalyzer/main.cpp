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
void abrirArchivoEntrada(ifstream &fin);

// Devuelve cierto si el string es un token
bool esToken(string token);

int main(){

	ifstream fin;
	ofstream fout;

	abrirArchivoEntrada(fin);

	string token;

	while (!fin.eof()){
		fin >> token;
		
		if (esToken(token)){
			cout << token << endl;
		}
	}

	return 0;
}

void abrirArchivoEntrada(ifstream &fin) {
	fin.open("lex_output.txt");
	if (fin.fail()) {
		cout << "File lex_output.txt does not exist.\n";
	}
}

bool esToken(string token){

	string tokenArray[] = {"<palabraReservada>", "<identificador>", "<opRelacional>", "<opAritmetico", "<asignacion>",
	"<parentesisIzquierdo>", "<parentesisDerecho>", "<puntoComa>", "<numero>"};

	int size = sizeof(tokenArray)/sizeof(tokenArray[0]);

	for (int i = 0; i < size; i++){
		if (token == tokenArray[i]){
			return true;
		}
	}
	return false;
}