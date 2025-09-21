#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <set>


using namespace std;


using ParseTable = map<pair<string, string>, vector<string>>;


ParseTable read_parse_table(const string& filename) {
    ParseTable table;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open parsing table file '" << filename << "'" << endl;
        exit(1);
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string non_term, term;
        iss >> non_term >> term;
        vector<string> prod;
        string symbol;
        if (iss >> symbol) {
            if (symbol != "epsilon") {
                prod.push_back(symbol);
                while (iss >> symbol) {
                    prod.push_back(symbol);
                }
            }
        }
        table[{non_term, term}] = prod;
    }
    file.close();
    return table;
}

map<string, set<string>> read_sets(const string& filename) {
    map<string, set<string>> sets;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file '" << filename << "'" << endl;
        exit(1);
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string non_term;
        if (!(iss >> non_term)) {
            cerr << "Warning: Skipping empty line in " << filename << endl;
            continue;
        }
        set<string> terminals;
        string term;
        while (iss >> term) {
            terminals.insert(term);
        }
        if (!terminals.empty()) {
            sets[non_term] = terminals;
        }
    }
    file.close();
    return sets;
}
struct Production {
    string non_terminal;
    vector<string> symbols;

    bool operator<(const Production& other) const {
        if (non_terminal != other.non_terminal) {
            return non_terminal < other.non_terminal;
        }
        return symbols < other.symbols;
    }
};

ParseTable generate_parse_table() {
    ParseTable table;

    vector<Production> productions = {
        {"Exp", {"Term", "Expr"}},           // Exp → Term Expr
        {"Expr", {"+", "Term", "Expr"}},     // Expr → + Term Expr
        {"Expr", {"-", "Term", "Expr"}},     // Expr → - Term Expr
        {"Expr", {}},                        // Expr → ε
        {"Term", {"Factor", "Termp"}},       // Term → Factor Termp
        {"Termp", {"*", "Factor", "Termp"}}, // Termp → * Factor Termp
        {"Termp", {"/", "Factor", "Termp"}}, // Termp → / Factor Termp
        {"Termp", {}},                       // Termp → ε
        {"Factor", {"(", "Exp", ")"}},       // Factor → ( Exp )
        {"Factor", {"id"}}                   // Factor → id
    };

    auto first = read_sets("first.txt");
    auto follow = read_sets("follow.txt");

    map<Production, set<string>> first_of_production;
    for (const auto& prod : productions) {
        set<string> first_set;
        if (prod.symbols.empty()) {
            first_set.insert("epsilon");
        }
        else {
            string first_sym = prod.symbols[0];
            if (first.count(first_sym)) {
                first_set = first[first_sym];
            }
            else {

                first_set.insert(first_sym);
            }
            if (first_set.count("epsilon") && prod.symbols.size() > 1) {//A -> B C , C -> e First A = First C
                first_set.erase("epsilon");
                string second_sym = prod.symbols[1];
                if (first.count(second_sym)) {
                    for (const auto& t : first[second_sym]) {
                        first_set.insert(t);
                    }
                }
                else {
                    first_set.insert(second_sym);
                }
            }
        }
        first_of_production[prod] = first_set;
    }

    for (const auto& prod : productions) {
        string A = prod.non_terminal;
        auto first_set = first_of_production[prod];

        for (const auto& a : first_set) {
            if (a != "epsilon") {
                pair<string, string> key = { A, a };
                table[key] = prod.symbols;
            }
        }

        if (first_set.count("epsilon")) {
            for (const auto& b : follow[A]) {
                pair<string, string> key = { A, b };
                table[key] = prod.symbols;
            }
        }
    }

    return table;
}

vector<string> read_scanner_output(const string& filename) {
    vector<string> tokens;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open scanner output file '" << filename << "'" << endl;
        exit(1);
    }
    string token;
    while (file >> token) {
        tokens.push_back(token);
    }
    if (tokens.empty()) {
        cerr << "Error: Scanner output file '" << filename << "' is empty" << endl;
        exit(1);
    }
    file.close();
    return tokens;
}


bool parse(const ParseTable& table, const vector<string>& tokens) {
    stack<string> s;
    s.push("$");
    s.push("Exp");
    vector<string> input = tokens;
    if (input.empty() || input.back() != "$") {
        input.push_back("$");
    }
    size_t i = 0;

    while (!s.empty()) {
        string top = s.top();
        s.pop();
        string current = (i < input.size()) ? input[i] : "$";

        if (top == current) {
            if (top == "$") {
                return true;
            }
            i++;
        }
        else {
            auto key = make_pair(top, current);
            if (table.find(key) != table.end()) {
                const auto& prod = table.at(key);
                for (auto it = prod.rbegin(); it != prod.rend(); ++it) {
                    s.push(*it);
                }
            }
            else {
                return false;
            }
        }
    }
    return false;
}

void print_set_map(const map<string, set<string>>& set_map, const string& name) {
    cout << name << " Sets:\n";
    for (const auto& entry : set_map) {
        cout << entry.first << ": { ";
        bool first_terminal = true;
        for (const auto& terminal : entry.second) {
            if (!first_terminal) {
                cout << ", ";
            }
            cout << terminal;
            first_terminal = false;
        }
        cout << " }\n";
    }
    cout << endl;
}

int main() {

    //auto first = read_sets("first.txt");
    //print_set_map(first, "First");
    //auto follow = read_sets("follow.txt");

    auto table = generate_parse_table();

    //auto table = read_parse_table("parse_table.txt");

    auto tokens = read_scanner_output("scanner_output.txt");

    bool valid = parse(table, tokens);

    cout << "Valid: " << (valid ? "true" : "false") << endl;

    return 0;
}