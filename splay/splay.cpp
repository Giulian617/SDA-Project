#include <bits/stdc++.h>
using namespace std;

const int NEG_INF = -1e9 - 1;
const int INF = 1e9 + 1;

struct SplayNode
{
    int value;
    SplayNode *parent, *left, *right;

    SplayNode(int _value) : value(_value), parent(nullptr), left(nullptr), right(nullptr) {}
};

class Splay
{
private:
    SplayNode *root;

    SplayNode *getMinimum(SplayNode *node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    SplayNode *rotate_left(SplayNode *node)
    {
        SplayNode *p = node->parent;
        SplayNode *B = node->left;

        if (B != nullptr)
            B->parent = p;
        p->right = B;

        node->parent = p->parent;
        node->left = p;
        p->parent = node;

        if (node->parent != nullptr)
        {
            if (node->parent->left == p)
                node->parent->left = node;
            else
                node->parent->right = node;
        }

        return node;
    }

    SplayNode *rotate_right(SplayNode *node)
    {
        SplayNode *p = node->parent;
        SplayNode *B = node->right;

        if (B != nullptr)
            B->parent = p;
        p->left = B;

        node->parent = p->parent;
        node->right = p;
        p->parent = node;

        if (node->parent != nullptr)
        {
            if (node->parent->left == p)
                node->parent->left = node;
            else
                node->parent->right = node;
        }

        return node;
    }

    void splay(SplayNode *node)
    {
        while (node->parent != nullptr)
        {
            SplayNode *p = node->parent;
            SplayNode *g = p->parent;

            if (g == nullptr) // zig/zag cases
            {
                if (p->left == node) // zig
                    node = rotate_right(node);
                else // zag
                    node = rotate_left(node);
            }
            else if (p->left == node && g->left == p) // zig-zig
                node = rotate_right(rotate_right(node));
            else if (p->right == node && g->right == p) // zag-zag
                node = rotate_left(rotate_left(node));
            else if (p->left == node && g->right == p) // zig-zag
                node = rotate_left(rotate_right(node));
            else if (p->right == node && g->left == p) // zag-zig
                node = rotate_right(rotate_left(node));
        }

        this->root = node;
    }

    SplayNode *insert(SplayNode *node, int x, SplayNode *&to_splay)
    {
        if (node == nullptr)
        {
            to_splay = new SplayNode(x);
            return to_splay;
        }

        if (x == node->value)
            to_splay = node;
        else if (x < node->value)
        {
            node->left = insert(node->left, x, to_splay);
            node->left->parent = node;
        }
        else
        {
            node->right = insert(node->right, x, to_splay);
            node->right->parent = node;
        }

        return node;
    }

    SplayNode *erase(SplayNode *node, int x, SplayNode *&to_splay, bool check)
    {
        if (node == nullptr)
            return nullptr;

        if (x < node->value)
        {
            if (check)
                to_splay = node;
            node->left = erase(node->left, x, to_splay, true);
            if (node->left != nullptr)
                node->left->parent = node;

            return node;
        }

        if (x > node->value)
        {
            if (check)
                to_splay = node;
            node->right = erase(node->right, x, to_splay, true);
            if (node->right != nullptr)
                node->right->parent = node;

            return node;
        }

        // node has at most one child
        if (node->left == nullptr || node->right == nullptr)
        {
            SplayNode *aux = nullptr;
            if (node->left == nullptr)
                aux = node->right;
            else
                aux = node->left;

            if (aux != nullptr)
                aux->parent = node->parent;

            delete node;
            return aux;
        }

        // node has two children
        SplayNode *exchange = getMinimum(node->right); // this node has no left child

        node->value = exchange->value;
        node->right = erase(node->right, exchange->value, to_splay, false);
        if (node->right != nullptr)
            node->right->parent = node;

        return node;
    }

    bool search(SplayNode *node, int x, SplayNode *&to_splay)
    {
        if (node == nullptr)
            return false;

        if (x == node->value)
        {
            to_splay = node;
            return true;
        }

        to_splay = node;
        if (x < node->value)
            return search(node->left, x, to_splay);

        return search(node->right, x, to_splay);
    }

    int predecessor(SplayNode *node, int x, SplayNode *&to_splay)
    {
        if (node == nullptr)
            return NEG_INF;

        to_splay = node;

        if (x < node->value)
            return predecessor(node->left, x, to_splay);

        return max(node->value, predecessor(node->right, x, to_splay));
    }

    int successor(SplayNode *node, int x, SplayNode *&to_splay)
    {
        if (node == nullptr)
            return INF;

        to_splay = node;

        if (x > node->value)
            return successor(node->right, x, to_splay);

        return min(node->value, successor(node->left, x, to_splay));
    }

    void inorder_walk(SplayNode *node, int x, int y, long long &sum)
    {
        if (node == nullptr)
            return;

        if (x < node->value)
            inorder_walk(node->left, x, y, sum);

        if (x <= node->value && node->value <= y)
            sum += node->value;

        if (y > node->value)
            inorder_walk(node->right, x, y, sum);
    }

    void deleteTree(SplayNode *node)
    {
        if (node == nullptr)
            return;

        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    Splay() : root(nullptr) {}

    ~Splay()
    {
        deleteTree(this->root);
    }

    void insert(int x)
    {
        SplayNode *to_splay = nullptr;
        this->root = insert(this->root, x, to_splay);
        splay(to_splay);
    }

    void erase(int x)
    {
        SplayNode *to_splay = nullptr;
        this->root = erase(this->root, x, to_splay, true);
        if (to_splay != nullptr)
            splay(to_splay);
    }
    bool search(int x)
    {
        SplayNode *to_splay = nullptr;
        bool ans = search(this->root, x, to_splay);
        if (to_splay != nullptr)
            splay(to_splay);
        return ans;
    }
    int predecessor(int x)
    {
        SplayNode *to_splay = nullptr;
        int ans = predecessor(this->root, x, to_splay);
        if (to_splay != nullptr)
            splay(to_splay);
        return ans;
    }
    int successor(int x)
    {
        SplayNode *to_splay = nullptr;
        int ans = successor(this->root, x, to_splay);
        if (to_splay != nullptr)
            splay(to_splay);
        return ans;
    }
    long long inorder_walk(int x, int y)
    {
        long long sum = 0;
        inorder_walk(this->root, x, y, sum);
        return sum;
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    Splay splay;
    int Q;
    cin >> Q;

    for (int q = 0; q < Q; q++)
    {
        int op, x, y;
        cin >> op;

        if (op == 1)
        {
            cin >> x;
            splay.insert(x);
        }
        else if (op == 2)
        {
            cin >> x;
            splay.erase(x);
        }
        else if (op == 3)
        {
            cin >> x;
            cout << splay.search(x) << '\n';
        }
        else if (op == 4)
        {
            cin >> x;
            cout << splay.predecessor(x) << '\n';
        }
        else if (op == 5)
        {
            cin >> x;
            cout << splay.successor(x) << '\n';
        }
        else
        {
            cin >> x >> y;
            cout << splay.inorder_walk(x, y) << '\n';
        }
    }
    return 0;
}