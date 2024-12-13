#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
struct Node {
    int value = NULL;
    Node* left = nullptr;
    Node* right = nullptr;
    Node(const int& newValue) {
        value = newValue;
    }
};

class BST {
public:
    Node* root;
    BST() {
        root = nullptr;
    }
    void insertNode(const int& value) {
        Node* newNode = new Node(value);
        if (root == nullptr) {
            root = newNode;
            return;
        }
        Node* current = root;
        while (current != nullptr) {
            if (value < current->value) {
                if (current->left == nullptr) {
                    current->left = newNode;
                    return;
                }
                else {
                    current = current->left;
                }
            }
            else if (value > current->value) {
                if (current->right == nullptr) {
                    current->right = newNode;
                    return;
                }
                else {
                    current = current->right;
                }
            }
        }
    }
    void printPreOrder(Node* current, vector<int>& temp) {
        if (current == nullptr) {
            return;
        }
        temp.push_back(current->value);
        printPreOrder(current->left, temp);
        printPreOrder(current->right, temp);
    }
    void printInOrder(Node* current, vector<int>& temp) {
        if (current == nullptr) {
            return;
        }
        printInOrder(current->left, temp);
        temp.push_back(current->value);
        printInOrder(current->right, temp);
    }
    void printPostOrder(Node* current, vector<int>& temp) {
        if (current == nullptr) {
            return;
        }
        printPostOrder(current->left, temp);
        printPostOrder(current->right, temp);
        temp.push_back(current->value);
    }
    void printLevelOrder2(const int& n, vector<int>& temp) {
        temp.push_back(root->value);
        for (int i = 0; i < n; i++) {
            Node* current = root;
            while (current != nullptr) {
                if (current->value == temp.at(i)) {
                    if (current->left != nullptr) {
                        temp.push_back(current->left->value);
                    }
                    if (current->right != nullptr) {
                        temp.push_back(current->right->value);
                    }
                    current = nullptr;
                }
                else {

                    if (current->value < temp.at(i)) {
                        current = current->right;
                    }
                    else {
                        current = current->left;
                    }
                }
            }
        }
    }
};
void printVector(const int& n, const vector<int>& temp) {
    for (int i = 0; i < n; i++) {
        if (i < n - 1) {
            cout << temp.at(i) << " ";
        }
        else {
            cout << temp.at(i) << endl;
        }
    }
}
int main()
{
    BST test;
    int n;
    cin >> n; cin.ignore();
    vector<int> temp(n);
    for (int i = 0; i < n; i++) {
        int vi;
        cin >> vi; cin.ignore();
        temp.at(i) = vi;
    }
    for (int i = 0; i < n; i++) {
        test.insertNode(temp.at(i));
    }
    temp.clear();
    test.printPreOrder(test.root, temp);
    printVector(n, temp);
    temp.clear();
    test.printInOrder(test.root, temp);
    printVector(n, temp);
    temp.clear();
    test.printPostOrder(test.root, temp);
    printVector(n, temp);
    temp.clear();
    test.printLevelOrder2(n, temp);
    printVector(n, temp);
    temp.clear();
}