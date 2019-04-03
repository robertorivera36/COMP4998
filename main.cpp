#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

void menu();

// devuelve cierto si el lexema es una palabra reservada
bool esReservada(char lexema[]);

void abrirArchivoEntrada(ifstream &fin);

int main(){

    ifstream fin;
    ofstream fout;

    int test_case;

    /*
    // añnadir cuando terminemos con el menu de test case
    abrirArchivoEntrada(fin);
    */
	fout.open("output.txt");

    char ch;
    char lexema[20];

    int j;
    int lineCount = 1;

    menu();
        cin >> test_case;

        switch(test_case){
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
            case 0: {
                cout << "Quitting LEX\n";
                break;
            }
            default: {
                cout << "Out of bounds\n";
                break;
            }
        }

    // line counter for output
    fout << "Line " << lineCount++ << ":\n\n";
    
    while(!fin.eof()){
    	bool error = false;
        ch = fin.get();

        if (isalpha(ch) && isalnum(fin.peek())){
            do{
                lexema[j++] = ch;
                ch = fin.get();
            }while(isalnum(ch) && (ch != ' ' || ch != '\n'));

            lexema[j] = '\0';

            char c;
            int i = 0;
            while (lexema[i]){
				c = lexema[i];
				if (isupper(c)){
					error = true;
				}
				if (error){
					break;
				}
				i++;
			}

            j = 0;

            if (esReservada(lexema)){
                fout << lexema << " : <palabraReservada>\n";
            }
            else if (error){
	            fout << "[Error] " << "Linea " << lineCount-1 << ": " << "letras mayusculas no se permiten " << '\"' << lexema << "\"\n";
	        }
            else{
            	fout << lexema << " : <identificador>\n";
            }
        }

        if (isdigit(ch) && isdigit(fin.peek())){
            do{
                lexema[j++] = ch;
                ch = fin.get();
            }while(isdigit(ch) && (ch != ' ' || ch != '\n'));

            lexema[j] = '\0';
            j = 0;

            fout << lexema << " : <entero>\n";
        }

        else if (ch == '='){
            if (fin.peek() == '='){
                char a = fin.peek();
                fout << ch << a << " : <opRelacional>, IGU\n";
                ch = fin.get();
            }
            else{
                fout << ch << " : <opAsignacion>\n";
            }
        }

        else if (ch == '<'){
            if (fin.peek() == '='){
                char a = fin.peek();
                fout << ch << a << " : <opRelacional>, MEI\n";
                ch = fin.get();
            }
            else if (fin.peek() == '>'){
                char a = fin.peek();
                fout << ch << a << " : <opRelacional>, DIF\n";
                ch = fin.get();
            }
            else{
                fout << ch << " : <opRelacional>, MEN\n";
            }
        }

        else if (ch == '>'){
            if (fin.peek() == '='){
                char a = fin.peek();
                fout << ch << a << " : <opRelacional>, MAI\n";
                ch = fin.get();
            }
            else{
                fout << ch << " : <opRelacional>, MAY\n";
            }
        }

        else if (ch == '+'){

            fout << ch << " : <opAritmetico>, SUM\n";
        }

        else if (ch == '-'){

            fout << ch << " : <opAritmetico>, RES\n";
        }

        else if (ch == '*'){

            fout << ch << " : <opAritmetico>, MUL\n";
        }

        else if (ch == '/'){
            if (fin.peek() == '*'){
                do{
                    ch = fin.get();
                    ch = fin.get(); // añadido para evitar que lea el primer '*'como operador y el ultimo '/' como operador
                }while(ch != '*' && fin.peek() != '/');
                fin.get();
            }

            else{
                fout << ch << " : <opAritmetico>, DIF\n";
            }
        }

        else if (ch == '('){

            fout << ch << " : <parentesisIzquierdo>\n";
        }

        else if (ch == ')'){

            fout << ch << " : <parentesisDerecho>\n";
        }

        else if (ch == ';'){

            fout << ch << " : <puntoComa>\n";
        }

        // line counter for output
        else if (ch == '\n'){
            fout << "\nLine " << lineCount++ << ":\n\n";
        }

        else if (ch != '+' && ch != '-' && ch != '*' && ch != '/' && ch != '(' && ch != ')' && ch != '=' && ch != ';' && ch != ' ' && ch != '\n'){
        	fout << "Error en linea " << lineCount-1 << ' ' << ch << " is not defined\n\n";
        }
    }

    //fin.close("sourceCode.txt");
    fout.close();

    return 0;
}

void abrirArchivoEntrada(ifstream &fin){
    fin.open("sourceCode.txt");
    if (fin.fail()) {
        cout << "File sourceCode.txt does not exist.\n";
    }
}

void menu() {
    cout << "1. Test case 1\n";
    cout << "2. Test case 2\n";
    cout << "3. Test case 3\n";
    cout << "4. Test case 4\n";
    cout << "5. Test case 5\n";
    cout << "6. Test case 6\n";
    cout << "0. Exit LEX\n";
}

bool esReservada(char lexema[]) {

    char reservada[32][20] = {"inicio","final","Si","finsi","sino","Mientras","finmientras","Escribe"};
    
    for (int i = 0; i < 32; ++i) {
        if (strcmp(reservada[i], lexema) == 0) {
            return true;
        }
    }
    return false;
}