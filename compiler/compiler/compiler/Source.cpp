//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <sstream>
//#include<map>
//#include <set>
//using namespace std;
//
//vector<string> split(const string& str, char delimiter = ' ') {
//	vector<string> tokens;
//	stringstream ss(str);
//	string token;
//
//	while (getline(ss, token, delimiter))
//		tokens.push_back(token);
//
//	return tokens;
//}
//
//class DFA {
//	private:
//	map<pair<string, char>, string> transition_table;
//	set<string> final_states;
//	set<string> states;
//	public:
//	string start_state;
//	string error_state;
//	string current_state;
//	string previous_state;
//
//	void load_states(string filepath) {
//		fstream file(filepath);
//
//		if (!file) {
//			cerr << "Error opening states file!\n";
//			return;
//		}
//
//		string line;
//		// read all states
//		if (getline(file, line)) {
//			vector<string> file_states = split(line, ',');
//			// loop over each item in the vector
//			for (string s : file_states) {
//				states.insert(s);
//			}
//		}
//
//		// read start state
//		if (getline(file, line)) {
//			start_state = line;
//		}
//
//		// read final states
//		if (getline(file, line)) {
//			vector<string> file_finale_states = split(line, ',');
//			// loop over each item in the vector
//			for (string s : file_finale_states) {
//				final_states.insert(s);
//			}
//		}
//
//		// read error state
//		if (getline(file, line)) {
//			error_state = line;
//		}
//
//		file.close();
//	}
//
//	void load_transition_table(string filepath){
//		fstream file(filepath);
//
//		if (!file) {
//			cerr << "Error opening transtion table file!\n";
//			return;
//		}
//
//		string line, current_s, next_state;
//		char input;
//		vector<string> l;
//
//		while (getline(file, line)) {
//			l = split(line);
//			if (l.size() < 3) {
//				cerr << "Invalid transition line: " << line << "\n";
//				continue;
//			}
//
//			current_s = l[0];
//			input = l[1].empty() ? '$' : l[1][0]; // vector of string but input should be char
//			next_state = l[2];
//
//			transition_table[{current_s, input}] = next_state;
//		}
//
//		file.close();
//	}
//
//	string move_to_next_state(string current_s, char input) {
//		pair<string, char> key = { current_s, input};
//		auto val = transition_table.find(key);
//
//		if (val == transition_table.end()) {
//			previous_state = current_state;
//			current_state = error_state;
//			return error_state;
//		}
//
//		previous_state = current_state;
//		current_state = val->second;
//		return val->second;
//	}
//
//	bool is_final_state(string state) {
//		// if not found, find function return the iterator end of the set
//		return final_states.find(state) != final_states.end();
//	}
//
//	void reset() {
//		current_state = start_state;
//		previous_state = start_state;
//	}
//
//	void print_transitions() {
//		cout << "Current State | Input | Next State\n";
//		cout << "--------------|-------|------------\n";
//
//		for (const auto& entry : transition_table) {
//			const string& current_state = entry.first.first;
//			char input_symbol = entry.first.second;
//			const string& next_state = entry.second;
//
//			cout << "      " << current_state
//				<< "       |   " << input_symbol
//				<< "   |     " << next_state << "\n";
//		}
//	}
//};
//
//class Scanner {
//	DFA dfa;
//	map<string, string> final_state_to_token;
//public:
//	Scanner(string states_file, string transition_table_file, string token_file) {
//		dfa.load_states(states_file);
//		dfa.load_transition_table(transition_table_file);
//		load_final_states_tokens(token_file);
//	}
//
//	void load_final_states_tokens(string filepath) {
//		fstream file(filepath);
//
//		if (!file) {
//			cerr << "Error openning tokens file\n";
//			return;
//		}
//
//		string line, state, token;
//		vector<string> l;
//		while (getline(file, line)) {
//			l = split(line);
//			state = l[0];
//			token = l[1];
//			final_state_to_token[state] = token;
//		}
//
//		file.close();
//	}
//
//	bool scan(string input_filepath, string output_filepath) {
//		ifstream input_file(input_filepath);
//		ofstream output_file(output_filepath);
//
//		if (!input_file) {
//			cerr << "Error openning input file\n";
//			return false;
//		}
//
//		char ch;
//		string current_state = dfa.start_state;
//		string previous_state = dfa.previous_state;
//		string lexeme = "";
//
//		while (input_file.get(ch)) {
//			previous_state = current_state;
//			current_state = dfa.move_to_next_state(current_state, isspace(ch)? '$' : ch);
//
//			if (current_state != dfa.error_state) {
//				lexeme += ch;
//			}
//			else {
//				if (dfa.is_final_state(previous_state)) {
//					output_file << final_state_to_token[previous_state] << " " << lexeme << "\n";
//					dfa.reset();
//					current_state = dfa.start_state;
//					previous_state = dfa.start_state;
//					lexeme = "";
//
//					input_file.unget();
//				}
//				else {
//					cout << current_state << " " << previous_state << lexeme<<"\n";
//					output_file << "Lexical Error :" << " " << ch << " " << "Invalid Character\n";
//					output_file.close();
//					return false;
//				}
//			}
//
//		}
//		if (dfa.is_final_state(current_state)) {
//			output_file << final_state_to_token[current_state] << " " << lexeme << "\n";
//		}
//
//		return true;
//	}
//};
//
//class LL1{
//	map<string, vector<vector<string>>> grammar;
//	set<string> non_terminals;
//	set<string> terminals;
//	map<string, set<string>> first;
//	map<string, set<string>> follow;
//	map<pair<string, string>, vector<string>> parsing_table;
//	string start_symbol;
//
//public:
//	LL1(string filepath) {
//		load_grammer(filepath);
//		compute_first_sets();
//		compute_follow_sets();
//		build_parsing_table();
//
//		print_parse_table(parsing_table);
//	}
//
//	void load_grammer(string filepath){
//		fstream file(filepath);
//
//		if (!file) {
//			cerr << "Can't open grammer file\n";
//			return;
//		}
//
//		string line;
//		vector<string>rule;
//		while (getline(file, line)) {
//			rule = split(line);
//			string lhs = rule[0];
//
//			// each lhs is non-terminal so add all non-terminals to non-terminals set
//			non_terminals.insert(lhs);
//			if (start_symbol.empty()) start_symbol = lhs;
//			vector<string>rhs(rule.begin() + 1, rule.end());
//
//			/*
//			key can't be repeated in map 
//			Expr -> + Term Expr
//			Expr -> - Term Expr
//
//			example of rule in map: 
//			{"Expr", [["+","Term", "Expr"], ["-","Term", "Expr"]]}
//			*/
//			grammar[lhs].push_back(rhs);
//
//			// add terminals from rhs to terminals set
//			// if first character is uppercase then it is non-terminal 
//			// else is terminal
//			// note: may not work with some grammers if there is uppercase in terminals
//			for (string& sym : rhs)
//				if (!isupper(sym[0]) && sym != "e") terminals.insert(sym);
//		}
//
//		file.close();
//	}
//
//	// takes production rule and get first from it
//	set<string> compute_first(const vector<string>& symbols) {
//		set<string> result;
//	
//		// if first element is terminal then add it to result set and return
//		if (terminals.find(symbols[0]) != terminals.end()) {
//			result.insert(symbols[0]);
//			return result;
//		}
//
//		// by reaching this line then the first element is non-terminal
//		// must check if the element has epslon (e) in it
//		// if it has epslon (e) then get the first of the next element also
//		for (auto& sym : symbols) {
//
//			// if element is terminal then inset it and break loop 
//			if (terminals.find(sym) != terminals.end()) {
//				result.insert(sym);
//				break;
//			}
//
//			// check if the first set of the non-terminal is computed
//			// if not then compute it
//			if (first.find(sym) == first.end()) {
//				first[sym] = {}; // initialize first set to prevent recursion
//				for (auto& prod : grammar[sym]) {
//					set<string> temp_first = compute_first(prod);
//					first[sym].insert(temp_first.begin(), temp_first.end());
//				}
//			}
//
//			// get first set of non-terminal
//			set<string> sub_first = first[sym];
//
//			// Add everything from FIRST(sym)
//			result.insert(sub_first.begin(), sub_first.end());
//
//			// If sym doesn't derive epsilon (e), we're done
//			if (sub_first.find("e") == sub_first.end()) break;
//			else result.erase("e");  // We'll continue with next symbol
//		}
//		
//		return result;
//	}
//	
//	void compute_first_sets() {
//		bool changed;
//
//		do {
//			changed = false;
//
//			// loop over each rule in the grammer
//			for (auto& rule : grammar) {
//
//				// get non-terminal (lhs)
//				string A = rule.first;
//
//				// loop over each production rule in rhs
//				for (auto& production : rule.second) {
//					// compute first for each production rule
//					set<string> first_set = compute_first(production);
//
//					// loop over each sympol in the computed first set
//					// check if the sym is already in the first set of the non-terminal
//					// note: set.insert(el).second -> returns true if el is new to the set
//					for (auto& sym : first_set) {
//						if (first[A].insert(sym).second) changed = true;
//					}
//				}
//			}
//
//		} while (changed);
//	}
//
//	/*
//	For each rule: A → α B β
//	
//		If β is not empty:
//			- Add FIRST(β) - {ε} to FOLLOW(B)
//			- If ε ∈ FIRST(β), then also add FOLLOW(A) to FOLLOW(B)
//
//		If β is empty:
//			- Add FOLLOW(A) to FOLLOW(B)
//	*/
//	void compute_follow_sets() {
//		follow[start_symbol].insert("$");
//
//		bool changed;
//		do {
//			changed = false;
//
//			// loop over each rule in the grammer
//			for (auto& rule : grammar) {
//				// non-terminal (lhs)
//				string A = rule.first;
//
//				// loop over each production in the rule
//				for (auto& production : rule.second) {
//					for (int i = 0; i < production.size(); ++i) {
//						string B = production[i];
//
//						if (terminals.count(B)) continue;
//
//						bool epsilon_in_all = true;
//						// Look at β = production[i+1:]
//						for (int j = i + 1; j < production.size(); ++j) {
//							string beta = production[j];
//
//							// Add FIRST(beta) - ε to FOLLOW(B)
//							set<string> first_beta;
//
//							if (terminals.count(beta)) {
//								first_beta.insert(beta);
//							}
//							else {
//								first_beta = first[beta];
//							}
//
//							for (const auto& sym : first_beta) {
//								if (sym != "e" && follow[B].insert(sym).second) {
//									changed = true;
//								}
//							}
//
//							if (!first_beta.count("e")) {
//								epsilon_in_all = false;
//								break;
//							}
//						}
//
//						// If β is empty or all symbols after B can derive ε
//						if (i == production.size() - 1 || epsilon_in_all) {
//							for (const string& sym : follow[A]) {
//								if (follow[B].insert(sym).second) {
//									changed = true;
//								}
//							}
//						}
//					}
//				}
//			}
//		} while (changed);
//	}
//
//	void build_parsing_table() {
//		for (auto& rule : grammar) {
//			string A = rule.first;
//			for (auto& production : rule.second) {
//				set<string> first_set = compute_first(production);
//				for (string a : first_set) {
//					if (a != "e")
//						parsing_table[{A, a}] = production;
//				}
//				if (first_set.count("e")) {
//					for (string b : follow[A]) {
//						parsing_table[{A, b}] = production;
//					}
//				}
//			}
//		}
//	}
//
//	void print_parse_table(const map<pair<string, string>, vector<string>>& table) {
//		cout << "=== Parse Table ===" << endl;
//
//		for (const auto& entry : table) {
//			const string& non_terminal = entry.first.first;
//			const string& terminal = entry.first.second;
//			const vector<string>& production = entry.second;
//
//			cout << "M[" << non_terminal << ", " << terminal << "] = ";
//			for (const string& symbol : production) {
//				cout << symbol << " ";
//			}
//			cout << endl;
//		}
//
//		cout << "===================" << endl;
//	}
//
//};
//
//
//int main() {
//	Scanner scanner("automaton.txt", "transitions.txt", "tokens.txt");
//	scanner.scan("input.txt", "output.txt");
//
//	LL1 parser("grammer.txt");
//
//
//	return 0;
//}