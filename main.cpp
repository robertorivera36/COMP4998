#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <stdio.h>
#include <string.h>

void menu() {
    std::cout << "1. Test case 1\n";
    std::cout << "2. Test case 2\n";
    std::cout << "3. Test case 3\n";
    std::cout << "4. Test case 4\n";
    std::cout << "5. Test case 5\n";
    std::cout << "6. Test case 6\n";
    std::cout << "0. Exit LEX\n";
}

bool lex(char buffer[]) {

	char keywords[32][20] = {"inicio","final","Si","finsi","sino","Mientras","finmientras",
							"Escribe"};
	
	for (int i = 0; i < 32; ++i) {
		if (strcmp(keywords[i], buffer) == 0) {
			return true;
		}
	}
	return false;
}

int main() {

    int text_case;

    char prev_ch, ch, buffer[20];
    int j = 0;

    std::ifstream input_source;
    std::ofstream output_lex;

        menu();
        std::cin >> text_case;
        switch(text_case) {
            case 1: {
                input_source.open("test1.txt");
                break;
            }
            case 2: {
                input_source.open("test2.txt");
                break;
            }
            case 3: {
                input_source.open("test3.txt");
                break;
            }
            case 4: {
                input_source.open("test4.txt");
                break;
            }
            case 5: {
                input_source.open("test5.txt");
                break;
            }
            case 6: {
                input_source.open("test6.txt");
                break;
            }
            case 0: {
                std::cout << "Quitting LEX\n";
                break;
            }
            default: {
                std::cout << "Out of bounds\n";
                break;
            }
        }

    output_lex.open("output.txt");

    while(!input_source.eof()) {
        ch = input_source.get();
        prev_ch = ch;

        if (isalnum(ch)) {
            buffer[j++] = ch;  
        } else if (isdigit(ch) && isdigit(input_source.peek())) {
            char a = input_source.peek();
            output_lex << ch << a << " es <numero>\n";
        } else if ((ch == '\n') || (j != 0)) {
            buffer[j] = '\0';
            j = 0;

            if (lex(buffer)) {
                output_lex << buffer << " es <palabraReservada>\n";
            } else {
                output_lex << buffer << " es <identificador>\n";
            }
        }

        if (ch == '<') {
            if (input_source.peek() == '=') {
                output_lex << ch << "es <opRelacional>\n";
            } else if (input_source.peek() == '>') {
                output_lex << ch << " es <opRelacional>\n";
            } else {
                output_lex << ch << " es <opRelacional>\n";
            }
        }

        if (ch == '=') {
            if (input_source.peek() == '=') {
                char a = input_source.peek();
                output_lex << ch << a << " es <opRelacional>\n";
            }
        }

        if (ch == '>') {
            if (input_source.peek() == '=') {
                char a = input_source.peek();
                output_lex << ch << a << " es <opRelacional>\n";
            } else if (input_source.peek() == ' '){
                output_lex << ch << " es <opRelacional>\n";
            } else {
                output_lex << "ERROR\n";
            }
        }

        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            output_lex << ch << " es <opAritmetico>\n";
        } else if (ch == '(') {
            output_lex << ch << " es <parentesisIzquierdo>\n";
        } else if (ch == ')') {
            output_lex << ch << " es <parentesisDerecho>\n";
        } else if (ch == '=') {
            output_lex << ch << " es <asignador>\n";
        } else if (ch == ';') {
            output_lex << ch << " es <puntoComa>\n";
        }
    }

    input_source.close();
    output_lex.close();
}
