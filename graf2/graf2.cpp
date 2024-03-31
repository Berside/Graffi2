#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <random>
#include <list>
#include <cstdlib> 
#include <ctime>
#include <memory>
#include <chrono>
#include <sstream> // For stringstream
using namespace std;

static int letterChangeCount = 0;
struct tnode {
    int n;
    list<tnode*> root;
    vector<char> letters;
    tnode(int data) : n(data) {}
    ~tnode() {
        for (auto child : root) {
            delete child;
        }
    }
    void addLetter(char letter) {
        letters.push_back(letter);
    }
    string getLetters() {
        stringstream ss;
        for (char letter : letters) {
            ss << letter;
        }
        return ss.str();
    }
};

tnode* readNTreeFromFile(ifstream& file) {
    stack<tnode*> nodeStack;
    stack<char> letterStack;
    tnode* root = nullptr;
    tnode* currentNode = nullptr;
    int value;
    char c;
    bool isEmptyNode = false;
    while (file.get(c)) {
        if (isdigit(c)) {
            file.putback(c);
            file >> value;
            tnode* newNode = new tnode(value);
            if (root == nullptr) {
                root = newNode;
            }
            else if (currentNode != nullptr) {
                currentNode->root.push_back(newNode);
            }
            nodeStack.push(newNode);
            currentNode = newNode;
            isEmptyNode = false;
            while (!letterStack.empty()) {
                char letter = letterStack.top();
                letterStack.pop();
                currentNode->addLetter(letter);
            }
        }
        else if (isalpha(c)) {
            char nextChar = file.peek();
            if (isdigit(nextChar)) {
                letterStack.push(c);
            }
        }
        else if (c == '(') {
            isEmptyNode = true;
        }
        else if (c == ')') {
            if (!isEmptyNode) {
                if (!nodeStack.empty()) {
                    nodeStack.pop();
                    if (!nodeStack.empty()) {
                        currentNode = nodeStack.top();
                    }
                    else {
                        currentNode = nullptr;
                    }
                }
            }
            isEmptyNode = false;
        }
    }
    return root;
}

void printNTree(tnode* x, vector<bool>& flag, int depth = 0, bool isLast = false) {
    if (x == NULL) return;
    for (int i = 1; i < depth; ++i) {
        cout << "|   ";
    }
    if (depth == 0) {
        cout << x->n;
    }
    else if (isLast) {
        cout << "-- " << x->n;
        flag[depth] = false;
    }
    else {
        cout << "|- " << x->n;
    }
    string letters = x->getLetters();
    if (!letters.empty()) {
        cout << " (" << letters << ")";
    }
    cout << '\n';
    int it = 0;
    for (auto i = x->root.begin(); i != x->root.end(); ++i, ++it) {
        printNTree(*i, flag, depth + 1, it == (x->root.size()) - 1);
    }
    flag[depth] = true;
}

void modifyOddNodes(tnode* node) {
    if (node == nullptr) return;
    if (node->n % 2 != 0) {
        for (char& letter : node->letters) {
            if (letter == 'Y') {
                letter = 'B';
                letterChangeCount++;
            }
            else if (letter == 'W') {
                letter = 'Y';
                letterChangeCount++;
            }
        }
    }
    for (auto child : node->root) {
        modifyOddNodes(child);
    }
}

char randomLetter() {
    static mt19937 gen(time(0)); // Use Mersenne Twister for better randomness
    uniform_int_distribution<> dis(0, 2);
    char letters[] = { 'W', 'B', 'Y' };
    return letters[dis(gen)];
}

void generateRandomNaryTree(tnode*& node, int& n, int maxDepth = 10) {
    if (n == 0 || maxDepth == 0) return;
    node = new tnode(n); 
    node->addLetter(randomLetter()); 
    n--;
    int childCount = rand() % (n + 1);
    for (int i = 0; i < childCount; ++i) {
        tnode* child = nullptr;
        generateRandomNaryTree(child, n, maxDepth - 1);
        if (child != nullptr) {
            node->root.push_back(child); 
        }
    }
}


int main() {
    srand(time(0));
    ifstream file("Module.txt");
    tnode* root = readNTreeFromFile(file);
    setlocale(LC_ALL, "Russian");
    cout << "Выберите способ запуск программы:" << endl;
    cout << "Если вы хотите построить дерево используя входные данные. Напишите: 1" << endl;
    cout << "Если вы хотите построить дерево используя случайную генерацию. Напишите: 2" << endl;
    cout << "Если вы не хотите строить бинарное дерево. Напишите любое число" << endl;
    cout << endl;
    int k;
    cin >> k;
    cout << endl;
    if (k == 1) {
        vector<bool> flag(100, false);
        printNTree(root, flag);
        cout << endl;
        auto start1 = chrono::high_resolution_clock::now();
        modifyOddNodes(root);
        auto end1 = chrono::high_resolution_clock::now();
        auto duration1 = chrono::duration_cast<chrono::microseconds>(end1 - start1).count();
        cout << "Number of letters changed: " << letterChangeCount << endl;
        cout << "Время выполения алгоритма: " << duration1 << " microseconds" << endl << endl;
        printNTree(root, flag);
    }
    if (k == 2) {
        tnode* root1 = nullptr;
        int n;
        cout << "Напиши количество узлов для генерации бинарного дерева:" << endl;
        cin >> n;
        int b = n;
        vector<bool> flag(100, false);
        cout << endl;
        generateRandomNaryTree(root1, n);
        if (b < 10) {
            printNTree(root1, flag);
        }
        auto start1 = chrono::high_resolution_clock::now();
        modifyOddNodes(root1);
        auto end1 = chrono::high_resolution_clock::now();
        auto duration1 = chrono::duration_cast<chrono::microseconds>(end1 - start1).count();
        cout << endl;
        cout << "Number of letters changed: " << letterChangeCount << endl;
        cout << "Время выполения алгоритма: " << duration1 << " microseconds" << endl << endl;
    }
    delete root;
    root = nullptr;
    return 0;
}
