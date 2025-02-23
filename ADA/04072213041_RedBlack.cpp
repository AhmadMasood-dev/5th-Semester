#include <iostream>
using namespace std;

struct Node
{
    int data;
    string color;
    Node *left, *right, *parent;

    Node(int data)
    {
        this->data = data;
        this->color = "RED";
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }
};

class RedBlackTree
{
private:
    Node *root;
    Node *NIL;

    void leftRotate(Node *x)
    {
        Node *y = x->right;
        x->right = y->left;
        if (y->left != NIL)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node *x)
    {
        Node *y = x->left;
        x->left = y->right;
        if (y->right != NIL)
        {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            root = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void fixInsert(Node *k)
    {
        while (k != root && k->parent->color == "RED")
        {
            if (k->parent == k->parent->parent->left)
            {
                Node *u = k->parent->parent->right;
                if (u->color == "RED")
                {
                    k->parent->color = "BLACK";
                    u->color = "BLACK";
                    k->parent->parent->color = "RED";
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->right)
                    {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = "BLACK";
                    k->parent->parent->color = "RED";
                    rightRotate(k->parent->parent);
                }
            }
            else
            {
                Node *u = k->parent->parent->left;
                if (u->color == "RED")
                {
                    k->parent->color = "BLACK";
                    u->color = "BLACK";
                    k->parent->parent->color = "RED";
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->left)
                    {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = "BLACK";
                    k->parent->parent->color = "RED";
                    leftRotate(k->parent->parent);
                }
            }
        }
        root->color = "BLACK";
    }

    Node *searchHelper(Node *node, int data)
    {
        if (node == NIL || data == node->data)
        {
            return node;
        }
        if (data < node->data)
        {
            return searchHelper(node->left, data);
        }
        return searchHelper(node->right, data);
    }

    void transplant(Node *u, Node *v)
    {
        if (u->parent == nullptr)
        {
            root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    Node *minimum(Node *node)
    {
        while (node->left != NIL)
        {
            node = node->left;
        }
        return node;
    }

    void fixDelete(Node *x)
    {
        while (x != root && x->color == "BLACK")
        {
            if (x == x->parent->left)
            {
                Node *w = x->parent->right;
                if (w->color == "RED")
                {
                    w->color = "BLACK";
                    x->parent->color = "RED";
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == "BLACK" && w->right->color == "BLACK")
                {
                    w->color = "RED";
                    x = x->parent;
                }
                else
                {
                    if (w->right->color == "BLACK")
                    {
                        w->left->color = "BLACK";
                        w->color = "RED";
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = "BLACK";
                    w->right->color = "BLACK";
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else
            {
                Node *w = x->parent->left;
                if (w->color == "RED")
                {
                    w->color = "BLACK";
                    x->parent->color = "RED";
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == "BLACK" && w->left->color == "BLACK")
                {
                    w->color = "RED";
                    x = x->parent;
                }
                else
                {
                    if (w->left->color == "BLACK")
                    {
                        w->right->color = "BLACK";
                        w->color = "RED";
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = "BLACK";
                    w->left->color = "BLACK";
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = "BLACK";
    }

public:
    RedBlackTree()
    {
        NIL = new Node(0);
        NIL->color = "BLACK";
        NIL->left = NIL->right = NIL;
        root = NIL;
    }

    void insert(int data)
    {
        Node *new_node = new Node(data);
        new_node->left = NIL;
        new_node->right = NIL;

        Node *parent = nullptr;
        Node *current = root;

        while (current != NIL)
        {
            parent = current;
            if (new_node->data < current->data)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }

        new_node->parent = parent;

        if (parent == nullptr)
        {
            root = new_node;
        }
        else if (new_node->data < parent->data)
        {
            parent->left = new_node;
        }
        else
        {
            parent->right = new_node;
        }

        if (new_node->parent == nullptr)
        {
            new_node->color = "BLACK";
            return;
        }

        if (new_node->parent->parent == nullptr)
        {
            return;
        }

        fixInsert(new_node);
    }

    void deleteNode(int data)
    {
        Node *z = searchHelper(root, data);
        if (z == NIL)
        {
            cout << "Value " << data << " not found in the tree.\n";
            return;
        }
        Node *y = z;
        Node *x;
        string originalColor = y->color;

        if (z->left == NIL)
        {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == NIL)
        {
            x = z->left;
            transplant(z, z->left);
        }
        else
        {
            y = minimum(z->right);
            originalColor = y->color;
            x = y->right;
            if (y->parent == z)
            {
                x->parent = y;
            }
            else
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (originalColor == "BLACK")
        {
            fixDelete(x);
        }
    }

    Node *search(int data)
    {
        return searchHelper(root, data);
    }
};

int main()
{
    RedBlackTree rbt;

    rbt.insert(50);
    rbt.insert(10);
    rbt.insert(25);
    rbt.insert(75);
    rbt.insert(90);
    rbt.insert(80);
    rbt.insert(60);
    rbt.insert(65);
    cout << "\n--------------------------\n";

    cout << "Search for 59: ";
    bool found = rbt.search(59) != rbt.search(0);
    if (found)
        cout << "Found" << endl;
    else
        cout << "Not Found" << endl;
    cout << "\n--------------------------\n";
    cout << "Search for 25: ";
    found = (rbt.search(25) != rbt.search(0));

    if (found)
        cout << "Found" << endl;
    else
        cout << "Not Found" << endl;
    cout << "\n--------------------------\n";

    cout << "Deleting 25...\n";
    rbt.deleteNode(25);
    cout << "Search for 25: ";
    found = (rbt.search(25) != rbt.search(0));
    if (found)
        cout << "Found\n";
    else
        cout << "Not Found\n";
    return 0;
}