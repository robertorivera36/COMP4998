#include <iostream>
#include <string>
#include <fstream>

void lex(std::ifstream& input_source, std::ofstream& output_lex) {

    enum Token { inicio, Final, Si, sino, finsi, Mientras, finmientras, Escribe };

    char c;

    Token token;

    switch(token) {
        case 1: {
            std::cout << "Test\n";
            break;
        }
        default: {
            break;
        }
    }
}

int main() {

    std::ifstream input_source;
    std::ofstream output_lex;

    input_source.open("source.txt");
    output_lex.open("output.txt");

    lex(input_source, output_lex);

    input_source.close();
    output_lex.close();
}