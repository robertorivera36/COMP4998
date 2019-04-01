#include <iostream>
#include <string>
#include <fstream>

void menu() {
    std::cout << "1. Test case 1\n";
    std::cout << "2. Test case 2\n";
    std::cout << "3. Test case 3\n";
    std::cout << "4. Test case 4\n";
    std::cout << "5. Test case 5\n";
    std::cout << "6. Test case 6\n";
    std::cout << "0. Exit LEX\n";
}

void lex(std::ifstream& input_source, std::ofstream& output_lex) {}

int main() {

    int text_case;

    std::ifstream input_source;
    std::ofstream output_lex;

    do {
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
    } while (text_case != 0);

    output_lex.open("output.txt");

    lex(input_source, output_lex);

    input_source.close();
    output_lex.close();
}
