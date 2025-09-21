#include <iostream>
#include <fstream>
#include <cctype>
#include <string>



using namespace std;

int ptr = 0;
string str;

void Exp();
void Term();
void Expr();
void Factor();
void Termp();

void skipSpaces() {
    while (ptr < str.size() && isspace(str[ptr])) {
        ptr++;
    }
}

void match(char c) {
    skipSpaces();
    if (ptr < str.size() && str[ptr] == c) {
        ptr++;
    }
    else {
        cout << "Error: Expected '" << c << "' but found '"
            << (ptr < str.size() ? str[ptr] : ' ') << "'" << endl;
        exit(1);
    }
}

void Factor() {
    skipSpaces();
    if (str[ptr] == '(') {
        match('(');
        Exp();
        match(')');
    }
    else if (isalpha(str[ptr])) {  
        while (ptr < str.size() && isalnum(str[ptr])) {
            ptr++; 
        }
    }
    else {
        cout << "Error: Invalid factor at position " << ptr << endl;
        exit(1);
    }
}

void Termp() {
    skipSpaces();
    if (str[ptr] == '*') {
        match('*');
        Factor();
        Termp();
    }
    else if (str[ptr] == '/') {
        match('/');
        Factor();
        Termp();
    }
}

void Term() {
    Factor();
    Termp();
}

void Expr() {
    skipSpaces();
    if (str[ptr] == '+') {
        match('+');
        Term();
        Expr();
    }
    else if (str[ptr] == '-') {
        match('-');
        Term();
        Expr();
    }
}

void Exp() {
    Term();
    Expr();
}

int main() {
    ifstream file("string.txt");
    if (!file) {
        cout << "Error: Could not open file.\n";
        return 1;
    }

    getline(file, str);
    file.close();

    ptr = 0;
    Exp();

	cout << "Pareed Successfully" << endl;

    //skipSpaces();
    if (ptr == str.size()) {
        cout << "Valid Expression" << endl;
    }
    else {
        cout << "Invalid Expression" << endl;
    }

    return 0;
}
