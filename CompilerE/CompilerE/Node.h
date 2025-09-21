#include<iostream>
#include<string>
#include<vector>

using namespace std;

class Node{
public:
    int noChilds;
    vector<Node> childs;
    string name;
    string value;

    Node(string n, string v = "", vector<Node> c = {}):name(n), value(v), childs(c), noChilds(c.size()){}
    Node(){
         this->noChilds = 0;
         this->childs = {};
         this->name = "";
         this->value = "";
    }

    void print(){
        cout << name << ", " << value << "\n \t";
        for(int i=0; i<noChilds; i++){
            cout << childs[i].name << ", ";
        }
        cout << "\n";
        for(int i=0; i<noChilds; i++){
            childs[i].print();
        }
    }
};
