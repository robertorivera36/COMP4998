//
//  main.cpp
//  COMP4998
//
//  Created by George Perez, Roberto Rivera, Andres Garcia on 3/29/19.
//  Copyright © 2019 George Perez, Roberto Rivera, Andres Garcia. All rights reserved.
//

#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <stdio.h>
#include <string.h>
using namespace std;

// Dado un nombre de archivo, abre ese archivo
void abrirArchivoEntrada(ifstream &fin) {

    string filename;
    cout << "Enter the filename (sourcecode.txt) and then press enter: ";
    cin >> filename;
    fin.open(filename);
    if (fin.fail()) {
        cout << "File " << filename << " does not exist.\n";
    }
}

// Imprime opciones de menu
void menu() {
    cout << "0. Custom File\n";
    cout << "1. Test case 1\n";
    cout << "2. Test case 2\n";
    cout << "3. Test case 3\n";
    cout << "4. Test case 4\n";
    cout << "5. Test case 5\n";
    cout << "6. Test case 6\n";
    cout << "10. Exit LEX\n";
}

/*

Titulo: Tabla de Simbolos
Descripción: Devuelve cierto si el lexema es una palabra reservada, sino es una
             palabra reservada se añade a la tabla de simbolos
Entrada: lexema, indice del lexema
Salida: Valor booleano referente al lexema analizado

*/
bool tablaDeSimbolos(char lexema[], int index) {

    char simbolo[32][100] = {"inicio","final","Si","finsi","sino","Mientras","finmientras","Escribe"};

    int i = 0;
    for (; i < 8; ++i) {
        if (strcmp(simbolo[i], lexema) == 0) {
          return true;
        }
    }
    simbolo[i][100] = lexema[index];
    return false;
}

int main() {

    int max = 100, index = 0, test_case, j = 0, lineCount = 1;

    ifstream fin;
    ofstream fout;

    char ch, lexema[max];

    menu();
    cin >> test_case;
    switch(test_case) {
        case 0: {
            abrirArchivoEntrada(fin);
            break;
        }
        case 1: {
            fin.open("test1.txt");
            break;
        }
        case 2: {
            fin.open("test2.txt");
            break;
        }
        case 3: {
            fin.open("test3.txt");
            break;
        }
        case 4: {
            fin.open("test4.txt");
            break;
        }
        case 5: {
            fin.open("test5.txt");
            break;
        }
        case 6: {
            fin.open("test6.txt");
            break;
        }
        case 10: {
            cout << "Quitting LEX...\n";
            break;
        }
        default: {
            cout << "Out of bounds\n";
            break;
        }
    }

    fout.open("output.txt", ios::out);

    fout << "LINE " << lineCount++ << "\n";

    /*
    Descripción: Si el archivo de entrada no está vacío, recorrerá caracter por caracter indetificando lexemas y
    asignando los respectivos tokens tan y cuando no haya llegado al final del archivo.
    */

    while (!fin.eof() && !(fin.peek() == '\xFF')) {
        bool error = false;
        ch = fin.get();


        // Descripción: Verifica si el caracter extraido es alfabetico y si el caracter de le prosigue es alfanumerico.
        if (isalpha(ch) && isalnum(fin.peek())) {
            // Descripción: Mientras el caracter sea alfanumerico y no haya espacios ni newlines, se concatena al lexema actual.
            do {
                lexema[j++] = ch;
                ch = fin.get();
            } while (isalnum(ch) && (ch != ' ' || ch != '\n'));

            lexema[j] = '\0';

            int i = 0;

            /*
            Descripción: Verifica si el lexema actual contiene letras mayúsculas, de conseguir una letra mayúscula la
                         variable error se le asigna true.
            */
            while (lexema[i]) {
              if (isupper(lexema[i])) {
                error = true;
              }
              if (error) {
                break;
              }
              i++;
            }

            j = 0;

            /*
            Descripción: Verifica si el lexema actual es una palabra reservada, si error es true, o si ninguna de las condiciones anteriores
                         resultan ser ciertas por defecto el lexema se define como un identificador.
            */
            if (tablaDeSimbolos(lexema, index)) {
                fout << '\"' << lexema << '\"' << " : <palabraReservada:" << lexema << ">\n";
                index++;
            }
            else if (error) {
                fout << " [Error] Line " << lineCount - 1 << ": " << '\"' << lexema << '\"' << " [words cannot have capitalized letters in them]\n";
            }
            else{
                fout << '\"' << lexema << '\"' << " : <identificador>\n";
            }
        }

        // Descripción: Verifica si el caracter extraido es un digito y si el próximo caracter es un digito también.
        else if (isdigit(ch) && isdigit(fin.peek())) {
          // Descripción: Mientras el caracter sea alfanumerico y no haya espacios ni newlines, se concatena al lexema actual.
            do {
                lexema[j++] = ch;
                ch = fin.get();
            } while (isalnum(ch) && (ch != ' ' || ch != '\n'));

            lexema[j] = '\0';

            int i = 0;

            /*
            Descripción: Verifica si el lexema actual contiene caracteres alfabeticos, de conseguir un alguno la
                         variable error se le asigna true.
            */
            while (lexema[i]) {
              if (!isdigit(lexema[i])) {
                error = true;
              }
              if (error) {
                break;
              }
              i++;
            }

            j = 0;

            /*
            Descripción: Verifica si error es true, de no cumplirse la condición por defecto el lexema se define como un numero.
            */
            if (error) {
                fout << " [Error] Line " << lineCount - 1 << ": " << '\"' << lexema << '\"' << " [verifity integer]\n";
            }
            else{
                fout << '\"' << lexema << '\"' << " : <numero>\n";
            }
        }
        /*
        Descripción: Verifica si el caracter extraido es un digito con un digito adicional. Si NO hay un digito adicional
                     se muestra un error ya que numeros tienen que tener ceros a la izquierda si son de un digito
        */
        if (isdigit(ch) && !isdigit(fin.peek())) {

          fout << " [Error] Line " << lineCount - 1 << ": " << '\"' << ch << '\"' << " [single-digit numbers must have leading zero]\n";
        }


        /*
        Nombre: Automata Relacional
        Descripción:

        1. Se verifica que el caracter extraido sea un "=", si el próximo caracter es un "=" nuevamente se devuelve como
        una igualdad. Si el próximo caracter no es un "=" se deuelve una asignación.

        2. Se verifica que el caracter extraido sea un "<", si el próximo caracter es un "=" se devuelve como un menor igual.
        Si el próximo caracter es un ">", se devuelve una diferencia.
        Si el próximo caracter no es ninguno de los anteriores, se devuelve un menor.

        3. Se verifica que el caracter extraido sea un ">", si el próximo caracter es un "=" se devuelve como un mayor igual.
        Si el próximo caracter no es ninguno de los anteriores, se devuelve un mayor.
        */
        else if (ch == '='){
            if (fin.peek() == '=') {
                char a = fin.peek();
                fout << '\"' << ch << a << '\"' << " : <opRelacional> , IGU\n";
                ch = fin.get();
            } else {
                fout << '\"' << ch << '\"' << " : <opAsignacion>\n";
            }
        }

        else if (ch == '<') {
            if (fin.peek() == '=') {
                char a = fin.peek();
                fout << '\"' << ch << a << '\"' << " : <opRelacional> , MEI\n";
                ch = fin.get();
            } else if (fin.peek() == '>') {
                char a = fin.peek();
                fout << '\"' << ch << a << '\"' << " : <opRelacional> , DIF\n";
                ch = fin.get();
            } else{
                fout << '\"' << ch << '\"' << " : <opRelacional> , MEN\n";
            }
        }

        else if (ch == '>') {
            if (fin.peek() == '=') {
                char a = fin.peek();
                fout << '\"' << ch << a << '\"' << " : <opRelacional> , MAI\n";
                ch = fin.get();
            } else {
                fout << '\"' << ch << '\"' << " : <opRelacional> , MAY\n";
            }
        }


        /*
        Nombre: Automata Aritmetico
        Descripción:

        1. Se verifica si el caracter extraido es un "+". De serlo, se devuelve una suma.

        2. Se verifica si el caracter extraido es un "-". De serlo, se devuelve una resta.

        3. Se verifica si el caracter extraido es un "*". De serlo, se devuelve una multiplicación.

        4. Se verifica si el caracter extraido es un "/". De serlo, se devuelve una suma. De no serlo, se verifica si el próximo caracter
        es un "*". Esto corre en un loop hasta que consiga un "*" inmediatamente seguido por un "/". En este caso, se identifca un bloque
        de comentario y se ignora por el analizador lexico.
        */
        else if (ch == '+') {

            fout << '\"' << ch << '\"' << " : <opAritmetico> , SUM\n";
        }

        else if (ch == '-') {

            fout << '\"' << ch << '\"' << " : <opAritmetico> , RES\n";
        }

        else if (ch == '*') {

            fout << '\"' << ch << '\"' << " : <opAritmetico> , MUL\n";
        }

        else if (ch == '/') {
            if (fin.peek() == '*') {
                do {
                    ch = fin.get();
                    ch = fin.get();
                } while (ch != '*' && fin.peek() != '/');
                fin.get();
            } else {
                fout << '\"' << ch << '\"' << " : <opAritmetico> , DIV\n";
            }
        }

        /*
        Nombre: Resto del Automata Completo
        Descripción:

        1. Se verifica si el caracter extraido es un "(". De serlo, se devuelve un parentesis izquierdo.

        2. Se verifica si el caracter extraido es un ")". De serlo, se devuelve un parentesis derecho.

        3. Se verifica si el caracter extraido es un ";". De serlo, se devuelve un punto y coma.

        4. Se verifica si el caracter extraido es un "\n". De serlo, se incrementa el numero de linea.
        */

        else if (ch == '(') {

            fout << '\"' << ch << '\"' << " : <parentesisIzquierdo>\n";
        }

        else if (ch == ')') {

            fout << '\"' << ch << '\"' << " : <parentesisDerecho>\n";
        }

        else if (ch == ';') {

          fout << '\"' << ch << '\"' << " : <puntoComa>\n";
        }

        else if (ch == '\n' && !(fin.peek() == '\xFF')) {

          fout << "LINE " << lineCount++ << "\n";
        }

        else if (ch == '\n' && fin.peek() == '\xFF') {
          fout << "ENDOFFILE";
        }

        else if (ch != '+' && ch != '-' && ch != '*' && ch != '/' && ch != '(' && ch != ')' && ch != '=' && ch != ';' && ch != '\t' && ch != ' ' && ch != '\n') {

          fout << " [Error]  Line " << lineCount - 1 << ": " << ch << " [not defined]\n";
        }
    }
    fin.close();
    fout.close();

    /*
    Descripción: Prepara el file para el analizador sintactico eliminando newlines dejados por el lexico al final del archivo.
    */
    ifstream format_output;
    ofstream lex_output;

    format_output.open("output.txt");
    lex_output.open("../syntaxAnalyzer/lex_output.txt");

    while (!format_output.eof()) {
      string line;
      std::getline(format_output, line, '\n');

      if (line == "ENDOFFILE") {
        continue;
      } else if (line == "\"final\" : <palabraReservada:final>") {
        lex_output << line;
      } else {
        lex_output << line << '\n';
      }
    }

    format_output.close();
    lex_output.close();

    return 0;
}
