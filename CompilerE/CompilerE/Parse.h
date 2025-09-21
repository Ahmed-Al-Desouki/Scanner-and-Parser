#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include "Node.h"

using namespace std;

class Parse {
private:
    Node root;
    vector<string> tokens;
    int indx = 0;
public:
    Parse(string pathToken) {
        GetTokens(pathToken);
        root = Program();
        root.print(); // طباعة الشجرة
    }

    void GetTokens(string pathToken) {
        ifstream file(pathToken);
        if (!file.is_open()) {
            cerr << "error in open file token" << endl;
            exit(1);
        }

        string line;
        while (getline(file, line)) {
            string str = "";
            for (int i = 0; i < line.length(); i++) {
                char c = line[i];
                if (c == '<') continue;
                if (c == '>') {
                    if (!str.empty()) {
                        tokens.push_back(str);
                    }
                    str = "";
                } else if (c == ',') {
                    tokens.push_back(str);
                    str = "";
                } else {
                    str += c;
                }
            }
        }

        tokens.push_back("$");
    }

    string CurrentToken() {
        if (indx < tokens.size()) {
            return tokens[indx];
        }
        return "";
    }

    Node Exptail(Node left) {
        if (CurrentToken() == "plus" || CurrentToken() == "minus") {
            string op = CurrentToken();
            indx++;
            Node val = Val();
            Node newNode(op, "", {left, val});
            return Exptail(newNode);
        }
        return left;
    }

    Node Num() {
        if (CurrentToken() == "num") {
            string value = tokens[indx + 1];
            indx += 2;
            return Node("num", value);
        } else {
            cout << "error: expected 'num'\n";
            return Node();
        }
    }

    Node Val() {
        if (CurrentToken() == "id") {
            return ID();
        } else if (CurrentToken() == "num") {
            return Num();
        } else {
            cout << "error input\n";
            return Node();
        }
    }

    Node Stmt() {
        if (CurrentToken() == "print") {
            indx++;
            Node id = ID();
            return Node("print", "", {id});
        } else {
            Node id = ID();
            if (CurrentToken() == "assign") {
                indx++;
                Node val = Val();
                Node expr = Exptail(val);
                return Node("assign", "", {id, expr});
            } else {
                cout << "error: expected 'assign' after identifier\n";
                return Node();
            }
        }
    }

    Node Stmts1() {
        vector<Node> statements;
        while (CurrentToken() == "print" || CurrentToken() == "id") {
            Node stmt = Stmt();
            statements.push_back(stmt);
        }
        return Node("Stmts", "", statements);
    }

    Node Stmts() {
        return Stmts1();
    }

    Node ID() {
        if (CurrentToken() == "id") {
            string value = tokens[indx + 1];
            indx += 2;
            return Node("id", value);
        } else {
            cout << "error: expected 'id'\n";
            return Node();
        }
    }

    Node Dcl() {
        if (CurrentToken() == "floattkn" || CurrentToken() == "inttkn") {
            string type = CurrentToken();
            indx++;
            Node id = ID();
            return Node(type, "", {id});
        }
        return Node();
    }

    Node Dcls1() {
        vector<Node> declarations;
        while (CurrentToken() == "floattkn" || CurrentToken() == "inttkn") {
            Node dcl = Dcl();
            declarations.push_back(dcl);
        }
        return Node("Dcls", "", declarations);
    }

    Node Dcls() {
        return Dcls1();
    }

    Node Program() {
        Node dcls = Dcls();
        Node stmts = Stmts();
        if (CurrentToken() == "$") {
            cout << "Success\n";
            return Node("Program", "", {dcls, stmts});
        } else {
            cout << "Failed at token: " << CurrentToken() << " at index " << indx << "\n";
            return Node();
        }
    }
};
