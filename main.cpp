#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

// devuelve cierto si el lexema es una palabra reservada
bool esReservada(char lexema[]);

int main(){

	ifstream fin;
    ofstream fout;

    fin.open("test2.txt");
    fout.open("output.txt");

    char ch;
    char lexema[20];

    int j;

    while(!fin.eof()){
    	ch = fin.get();

    	if (isalpha(ch) && isalnum(fin.peek())){
    		do{
    			lexema[j++] = ch;
    			ch = fin.get();
    		}while(isalnum(ch) && (ch != ' ' || ch != '\n'));

    		lexema[j] = '\0';
    		j = 0;

    		if (esReservada(lexema)){
    			cout << lexema << " : <palabraReservada>\n";
    		}
    		else{
    			cout << lexema << " : <identificador>\n";
    		}
    	}

    	if (isdigit(ch) && isdigit(fin.peek())){
    		do{
    			lexema[j++] = ch;
    			ch = fin.get();
    		}while(isdigit(ch) && (ch != ' ' || ch != '\n'));

    		lexema[j] = '\0';
    		j = 0;

    		cout << lexema << " : <numero>\n";
    	}

    	else if (ch == '='){
    		if (fin.peek() == '='){
    			char a = fin.peek();
    			cout << ch << a << " : <opRelacional>, IGU\n";
    			ch = fin.get();
    		}
    		else{
    			cout << ch << " : <opAsignacion>\n";
    		}
    	}

    	else if (ch == '<'){
    		if (fin.peek() == '='){
    			char a = fin.peek();
    			cout << ch << a << " : <opRelacional>, MEI\n";
    			ch = fin.get();
    		}
    		else if (fin.peek() == '>'){
    			char a = fin.peek();
    			cout << ch << a << " : <opRelacional>, DIF\n";
    			ch = fin.get();
    		}
    		else{
    			cout << ch << " : <opRelacional>, MEN\n";
    		}
    	}

    	else if (ch == '>'){
    		if (fin.peek() == '='){
    			char a = fin.peek();
    			cout << ch << a << " : <opRelacional>, MAI\n";
    			ch = fin.get();
    		}
    		else{
    			cout << ch << " : <opAsignacion>, MAY\n";
    		}
    	}

    	else if (ch == '+'){

    		cout << ch << " : <opAritmetico>, SUM\n";
    	}

    	else if (ch == '-'){

    		cout << ch << " : <opAritmetico>, RES\n";
    	}

    	else if (ch == '*'){

    		cout << ch << " : <opAritmetico>, MUL\n";
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
    			cout << ch << " : <opAritmetico>, DIF\n";
    		}
    	}

    	else if (ch == '('){

    		cout << ch << " : <parentesisIzquierdo>\n";
    	}

    	else if (ch == ')'){

    		cout << ch << " : <parentesisDerecho>\n";
    	}

    	else if (ch == ';'){

    		cout << ch << " : <puntoComa>\n";
    	}
    }

    return 0;
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