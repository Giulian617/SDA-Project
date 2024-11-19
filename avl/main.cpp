#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

constexpr int NEG_INF = -1000000000;
constexpr int POS_INF = 1000000000;

struct AVLNode {
    int value;        
    int height;       
    AVLNode* left;    
    AVLNode* right;   

    AVLNode(int val) : value(val), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    AVLNode* root;

    int height(AVLNode* node) {
        return node ? node->height : 0;
    }

    int balanceFactor(AVLNode* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    AVLNode* rotateRight(AVLNode* y) {
        // New root will be the left child
        AVLNode* x = y->left;

        // Temporarily store x's right subtree
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        // Compute new heights
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        // New root will be the right child
        AVLNode* y = x->right;

        // Temporarily store y's left subtree
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        // Compute new heights
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    AVLNode* balance(AVLNode* node) {
        if (!node) return node;

        // Update the height of the current node
        node->height = max(height(node->left), height(node->right)) + 1;

        // Get the balance factor
        int balance = balanceFactor(node);

        // Left heavy case
        if (balance > 1) {
            // If left-right case, perform a left rotation on the left child first
            if (balanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left);
            // Perform a right rotation
            return rotateRight(node);
        }

        // Right heavy case
        if (balance < -1) {
            // If right-left case, perform a right rotation on the right child first
            if (balanceFactor(node->right) > 0)
                node->right = rotateRight(node->right);
            // Perform a left rotation
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode* insert(AVLNode* node, int value) {
        if (!node) return new AVLNode(value);

        // Recur to the left subtree
        if (value < node->value)
            node->left = insert(node->left, value);  

        // Recur to the right subtree
        else if (value > node->value)
            node->right = insert(node->right, value); 

        return balance(node); 
    }

    // Find the node with the smallest value in a subtree
    AVLNode* findMin(AVLNode* node) {
        while (node->left) node = node->left;
        return node;
    }

    // Delete a value from the AVL Tree rooted at the given node
    AVLNode* erase(AVLNode* node, int value) {
        if (!node) return node;

        if (value < node->value) {
            node->left = erase(node->left, value);
        } else if (value > node->value) {
            node->right = erase(node->right, value);
        } else {
            if (!node->left || !node->right) {
                // Node with only one child or no child
                AVLNode* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }

            // Node with two children: replace with the inorder successor
            AVLNode* temp = findMin(node->right);
            node->value = temp->value;
            node->right = erase(node->right, temp->value);
        }

        return balance(node);
    }

    // Check if a value exists in the tree
    bool contains(AVLNode* node, int value) {
        if (!node) return false;
        if (value == node->value) return true;

        // Search left subtree
        if (value < node->value)
            return contains(node->left, value);

        // Search right subtree
        return contains(node->right, value);
    }

    // Find the largest value less than or equal to X
    int predecesor(AVLNode* node, int value) {
        if (!node) return NEG_INF;

        // Search left subtree
        if (node->value > value)
            return predecesor(node->left, value); 

        // Search right subtree
        int res = predecesor(node->right, value); 

        // Return the best candidate
        return max(node->value, res);            
    }

    // Find the smallest value greater than or equal to X
    int succesor(AVLNode* node, int value) {
        if (!node) return POS_INF; 

        // Search right subtree
        if (node->value < value)
            return succesor(node->right, value); 
        
        // Search left subtree
        int res = succesor(node->left, value);

        // Return the best candidate
        return min(node->value, res);            
    }

    // Query all values in the range [X, Y] in sorted order
    void query(AVLNode* node, int X, int Y, vector<int>& result) {
        if (!node) return;

        // Everything on the left subtree
        if (X <= node->value) query(node->left, X, Y, result); 

        // Perfect match
        if (X <= node->value && node->value <= Y) result.push_back(node->value); 

         // Everything on the right subtree
        if (Y >= node->value) query(node->right, X, Y, result);
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(int value) {
        root = insert(root, value);
    }

    void erase(int value) {
        root = erase(root, value);
    }

    bool contains(int value) {
        return contains(root, value);
    }

    int predecesor(int value) {
        return predecesor(root, value);
    }

    int succesor(int value) {
        return succesor(root, value);
    }

    vector<int> query(int X, int Y) {
        vector<int> result;
        query(root, X, Y, result);
        return result;
    }
};

int main() {
    ifstream f("abce.in");
    ofstream g("abce.out");

    int Q;
    f >> Q;

    AVLTree avl;
    while (Q--) {
        int op, X, Y;
        f >> op >> X;
        if (op == 1) {
            avl.insert(X);
        } else if (op == 2) {
            avl.erase(X);
        } else if (op == 3) {
            g << avl.contains(X) << "\n";
        } else if (op == 4) {
            g << avl.predecesor(X) << "\n";
        } else if (op == 5) {
            g << avl.succesor(X) << "\n";
        } else if (op == 6) {
            f >> Y;
            vector<int> result = avl.query(X, Y);
            for (int num : result) g << num << " ";
            g << "\n";
        }
    }

    return 0;
}
