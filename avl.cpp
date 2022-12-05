#include <iostream>
#include <cstdlib>
#include <stack>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// for outputting results
ofstream outputFile;

// individual node structure: element, left child ptr, right child ptr, isRoot
struct Node
{
    int data;
    Node *left;
    Node *right;
    bool isRoot;
};

// global variable for referencing AVL tree
Node *AvlTree;

void Initialize()
{
    // Initializing AVL Tree to NULL
    AvlTree = NULL;
}

// Helper function to make sure is proper BST
void ListItemsInOrder(Node *t)
{
    if (t == NULL)
        return;
    ListItemsInOrder(t->left);
    cout << t->data << " ";
    ListItemsInOrder(t->right);
}

// Helper function for range search
// Is basically a recursive inorder traversal
void ListItemsInRange(Node *t, int a, int b)
{
    if (t == NULL)
        return;

    // Recursively start in the left subtree
    ListItemsInRange(t->left, a, b);

    // if node is within range, print it
    if (t->data >= a && t->data <= b)
    {
        cout << t->data << ", ";
        outputFile << t->data << ", ";
    }

    // Recursively go into the right subtree
    ListItemsInRange(t->right, a, b);
}

// Search for a specific key.
void Search(int key)
{
    // start searching at root
    Node *current = AvlTree;
    bool foundKey = false; // indicator if key was found

    if (current == NULL)
    {
        // Nothing in AVL Tree; is empty
        cout << "NULL" << endl;
        outputFile << "NULL\n";
    }
    else
    {
        // AVL Tree is not empty; search
        while (current != NULL)
        {
            // key is greater than current node
            if (current->data > key)
            {
                current = current->left;
            }
            // key is less than current node
            else if (current->data < key)
            {
                current = current->right;
            }
            // key found! print it.
            else
            {
                cout << current->data << endl;
                outputFile << current->data << endl;
                current = NULL; // to exit while loop
                foundKey = true;
            }
        }

        // search was unsuccesful
        if (!foundKey)
        {
            cout << "NULL" << endl;
            outputFile << "NULL\n";
        }
    }
}

// Range search (values between a and b, inclusive)
void Search(int a, int b)
{

    // start at root of AVL tree
    Node *current = AvlTree;

    if (current == NULL)
    {
        // Nothing in AVL Tree; is empty
        cout << "NULL" << endl;
        outputFile << "NULL\n";
    }
    else
    {
        // AVL Tree is not empty; search for items in range using helper function
        ListItemsInRange(AvlTree, a, b);
        cout << endl;
        outputFile << "\n";
    }
}

// helper function for creating new node during Insert
Node *createNewNode(int key)
{
    // allocate memory for new node
    Node *newNode = new Node;

    // set node properties; left & right children will be empty (null)
    newNode->data = key;
    // helps to re-identify root if necessary during rebalancing
    newNode->isRoot = false;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// ROTATIONS

// for RR imbalance; moves right child up (left rotation)
void RR_imbalance(Node *n, Node *parent)
{

    cout << "RR IMBALANCE ON " << n->data << endl;
    Node *oldParent = n;
    Node *rightChild = n->right;

    // make unbalanced node's right child its parent, rearrange pointers
    oldParent->right = rightChild->left;
    rightChild->left = oldParent;

    // if unbalanced node has a parent, make parent point to
    // unbalanced node's right child
    // (which child depends on if unbalanced node was left or right child)
    if (parent != NULL && parent->right == oldParent)
    {
        parent->right = rightChild;
    }
    else if (parent != NULL && parent->left == oldParent)
    {
        parent->left = rightChild;
    }

    // if unbalanced node was originally the root,
    // make right child the new root (since is now parent)
    if (n->isRoot)
    {
        oldParent->isRoot = false;
        rightChild->isRoot = true;
        AvlTree = rightChild;
    }
}

// for LL imbalance; move left child up; right rotation
void LL_imbalance(Node *n, Node *parent)
{
    cout << "LL IMBALANCE ON " << n->data << endl;
    Node *oldParent = n;
    Node *leftChild = n->left;

    // make unbalanced node's left child its parent, rearrange pointers
    oldParent->left = leftChild->right;
    leftChild->right = oldParent;

    // if unbalanced node has a parent, make parent point to
    // unbalanced node's left child
    // (which child depends on if unbalanced node was left or right child)
    if (parent != NULL && parent->left == oldParent)
    {
        parent->left = leftChild;
    }
    else if (parent != NULL && parent->right == oldParent)
    {
        parent->right = leftChild;
    }

    // if unbalanced node was originally the root,
    // make left child the new root (since is now parent)
    if (n->isRoot)
    {
        oldParent->isRoot = false;
        leftChild->isRoot = true;
        AvlTree = leftChild;
    }
}

void LR_imbalance(Node *n, Node *parent)
{
    cout << "LR IMBALANCE ON " << n->data << endl;
    Node *child = n->left;
    Node *grandchild = n->left->right;

    // if unbalanced node has a parent, make parent point to
    // unbalanced node's grandchild
    // (which child depends on if unbalanced node was left or right child)
    if (parent != NULL && parent->left == n)
    {
        parent->left = grandchild;
    }
    else if (parent != NULL && parent->right == n)
    {
        parent->right = grandchild;
    }

    // rearrange pointers, bringing grandchild up
    // and making original unbalanced node & its child
    // children of grandchild
    child->right = grandchild->left;
    grandchild->left = child;
    n->left = grandchild->right;
    grandchild->right = n;

    // if unbalanced node was originally the root,
    // make grandchild the new root (since is now parent)
    if (n->isRoot)
    {
        n->isRoot = false;
        grandchild->isRoot = true;
        AvlTree = grandchild;
    }
}

void RL_imbalance(Node *n, Node *parent)
{
    cout << "RL IMBALANCE ON " << n->data << endl;
    Node *child = n->right;
    Node *grandchild = n->right->left;

    // if unbalanced node has a parent, make parent point to
    // unbalanced node's grandchild
    // (which child depends on if unbalanced node was left or right child)
    if (parent != NULL && parent->right == n)
    {
        parent->right = grandchild;
    }
    else if (parent != NULL && parent->left == n)
    {
        parent->left = grandchild;
    }

    // rearrange pointers, bringing grandchild up
    // and making original unbalanced node & its child
    // children of grandchild
    child->left = grandchild->right;
    grandchild->right = child;
    n->right = grandchild->left;
    grandchild->left = n;

    // if unbalanced node was originally the root,
    // make grandchild the new root (since is now parent)
    if (n->isRoot)
    {
        n->isRoot = false;
        grandchild->isRoot = true;
        AvlTree = grandchild;
    }
}

// Helper function for checking imbalances
// Returns height of a specific node
int getHeight(Node *n)
{
    if (n == NULL)
    {
        // if empty tree, height is 0
        return 0;
    }
    if (n->left == NULL && n->right == NULL)
    {
        // if node with no children, height is 1
        return 1;
    }
    else
    {
        // recursively find left & right subtrees' heights
        int left_height = getHeight(n->left);
        int right_height = getHeight(n->right);

        // get the max between the two
        int max_height = max(left_height, right_height);
        return max_height + 1;
    }

    return 0;
}

// Rebalancing function.
// Checks for imbalances, & directs rotations as necessary
void checkImbalance(stack<Node *> trackStack)
{
    Node *current = NULL;
    Node *parent = NULL;
    int current_balance_factor;
    int left_balance_factor;
    int right_balance_factor;

    // go through entire track stack (backtrace)
    while (!trackStack.empty())
    {
        // get first item in track stack
        current = trackStack.top();
        // pop this item off stack for next iteration of while loop
        trackStack.pop();

        // calculate this node's balance factor using
        // getHeight helper function
        current_balance_factor = getHeight(current->left) - getHeight(current->right);

        // if node has left/right children, get those subtrees' balance factors
        if (current->left)
        {
            left_balance_factor = getHeight(current->left->left) - getHeight(current->left->right);
        }
        if (current->right)
        {
            right_balance_factor = getHeight(current->right->left) - getHeight(current->right->right);
        }

        // the following comparisons look at current node's balance factor
        // and its children's to classify imbalances

        // LL imbalance
        if (current_balance_factor == 2 && left_balance_factor == 1)
        {
            // if there is another item on the
            // track stack, current node has a parent
            if (!trackStack.empty())
            {
                parent = trackStack.top();
            }
            LL_imbalance(current, parent);
        }
        // RR imbalance
        else if (current_balance_factor == -2 && right_balance_factor == -1)
        {
            // if there is another item on the
            // track stack, current node has a parent
            if (!trackStack.empty())
            {
                parent = trackStack.top();
            }
            RR_imbalance(current, parent);
        }
        // LR imbalance
        else if (current_balance_factor == 2 && left_balance_factor == -1)
        {
            // if there is another item on the
            // track stack, current node has a parent
            if (!trackStack.empty())
            {
                parent = trackStack.top();
            }
            LR_imbalance(current, parent);
        }
        // RL imbalance
        else if (current_balance_factor == -2 && right_balance_factor == 1)
        {
            // if there is another item on the
            // track stack, current node has a parent
            if (!trackStack.empty())
            {
                parent = trackStack.top();
            }
            RL_imbalance(current, parent);
        }
    }
}

// Insert a new node
void Insert(int key)
{
    Node *current = AvlTree;
    Node *parent = NULL;
    bool isLeftChild;
    bool isDuplicate = false;

    // use to keep track of path taken to Insert new node
    // Will use track stack to back trace nodes and
    // check for any imbalances introduced
    stack<Node *> trackStack;

    if (current == NULL)
    {
        // Empty AVL tree, insert at root
        AvlTree = createNewNode(key);
        AvlTree->isRoot = true;
    }
    else
    {
        // Non-empty AVL tree, find where to insert
        // as traverse through tree, push keys visited
        // onto the track stack
        // keep track of which child to properly
        // connect parent ptr to newly inserted node later
        while (current != NULL)
        {
            if (current->data > key)
            {
                trackStack.push(current);
                parent = current;
                current = current->left;
                isLeftChild = true;
            }
            else if (current->data < key)
            {
                trackStack.push(current);
                parent = current;
                current = current->right;
                isLeftChild = false;
            }
            else
            {
                // no duplicates allowed.
                isDuplicate = true;
                current = NULL;
            }
        }

        // only insert if not duplicate
        if (!isDuplicate)
        {
            current = createNewNode(key);
            if (isLeftChild)
            {
                parent->left = current;
                trackStack.push(current);
            }
            else
            {
                parent->right = current;
                trackStack.push(current);
            }
        }
    }

    // backtrace thru track stack and
    // check/resolve any imbalances
    checkImbalance(trackStack);
}

void Delete(int key)
{
    Node *current = AvlTree;
    Node *parent = NULL;
    bool foundNode = false;
    bool isLeftChild;

    // use to keep track of path taken to Insert new node
    // Will use track stack to back trace nodes and
    // check for any imbalances introduced
    stack<Node *> trackStack;

    // you know the drill -- find the node to be deleted
    // (as long as tree is not empty)
    // and push visited nodes to the track stack
    // and keep track of if it's a left or right child
    if (current != NULL)
    {
        while (foundNode == false && current != NULL)
        {
            if (current->data > key)
            {
                trackStack.push(current);
                parent = current;
                current = current->left;
                isLeftChild = true;
            }
            else if (current->data < key)
            {
                trackStack.push(current);
                parent = current;
                current = current->right;
                isLeftChild = false;
            }
            else
            {
                foundNode = true;
            }
        }
    }

    // once found node to be deleted ...
    if (current != NULL && current->data == key)
    {
        cout << current->data << endl;

        // CASE 1: TRIVIAL DELETE - element is a leaf (0 children)
        if (current->left == NULL && current->right == NULL)
        {
            cout << "TRIVIAL DELETE" << endl;
            // de-allocate memory
            delete current;
            // change ptr of node to null (removes from tree)
            current = NULL;
            if (isLeftChild)
            {
                parent->left = NULL;
            }
            else
            {
                parent->right = NULL;
            }
        }
        // CASE 3: element has 2 children
        else if (current->left != NULL && current->right != NULL)
        {
            cout << "HAS 2 CHILDREN" << endl;
            Node *minInRight = NULL;

            // find the min node in right tree
            // start in the right subtree, and keep goign left
            minInRight = current->right;
            while (minInRight->left != NULL)
            {
                minInRight = minInRight->left;
            }

            cout << "MIN IN RIGHT IS " << minInRight->data << endl;

            // copy minInRight's data into node to be deleted
            // and delete original minInRight node
            current->data = minInRight->data;
            if (current->right->data != minInRight->data)
            {
                Node *tmp = current->right;
                while (tmp->left->left != NULL)
                {
                    tmp = tmp->left;
                }
                cout << "NOT EQUAL" << endl;
                tmp->left = minInRight->right;
            }
            else
            {
                cout << "EQUAL" << endl;
                current->right = minInRight->right;
            }
            delete minInRight;
        }
        // CASE 2: element has 1 child only
        else
        {
            cout << "HAS 1 CHILD" << endl;
            // has left child; rearrange ptrs to "skip" over itself
            if (current->left != NULL)
            {
                // tmp ptr to current node so can delete later
                Node *tmp = current;
                // move current ptr to its left child
                current = current->left;

                if (isLeftChild)
                {
                    parent->left = current;
                }
                else
                {
                    parent->right = current;
                }

                // delete the tmp ptr (deletes target node)
                delete tmp;
            }
            // has right child; same as previous
            else
            {
                Node *tmp = current;
                current = current->right;
                if (isLeftChild)
                {
                    parent->left = current;
                }
                else
                {
                    parent->right = current;
                }
                delete tmp;
            }
        }
    }

    // backtrace thru track stack and
    // check/resolve any imbalances
    checkImbalance(trackStack);
}

int main(int argc, char **argv)
{
    outputFile.open("output.txt");

    ifstream inputFile;
    string fileName;

    // get input file name from command line
    for (int i = 0; i < argc; ++i)
    {
        fileName = argv[i];
    }
    inputFile.open(fileName);

    string userInput;
    string command;
    string argument;
    int argumentValue;

    // parse thru input line by line
    while (inputFile.good())
    {
        getline(inputFile, userInput);

        // search for functions

        if (userInput.find("Initialize") != string::npos)
        {
            cout << "Initializing AVL Tree" << endl;
            Initialize();
        }

        else if (userInput.find("Insert") != string::npos)
        {
            // extract argument for insert
            argument = userInput.substr(7, userInput.length() - 8);

            // convert to integer using stringstream
            stringstream ss;
            ss << argument;
            ss >> argumentValue;
            cout << "Inserting " << argumentValue << endl;

            // call function
            Insert(argumentValue);
        }

        else if (userInput.find("Delete") != string::npos)
        {
            // extract argument for delete
            argument = userInput.substr(7, userInput.length() - 8);

            // convert to integer using stringstream
            stringstream ss;
            ss << argument;
            ss >> argumentValue;
            cout << "Deleting " << argumentValue << endl;

            // call function
            Delete(argumentValue);
        }

        else if (userInput.find("Search") != string::npos)
        {
            // extract argument for search
            argument = userInput.substr(7, userInput.length() - 8);

            // see if a comma exists in argument; distinguishes if
            // specific search vs range search
            size_t comma = argument.find(",");

            // if comma exists, is range search
            if (comma != string::npos)
            {
                // extract both arguments for search
                string firstArg = argument.substr(0, comma);
                string secondArg = argument.substr(comma + 1, argument.length() - comma);

                // convert to integers using stringstream
                stringstream ss1, ss2;
                int firstArgVal, secondArgVal = 0;
                ss1 << firstArg;
                ss1 >> firstArgVal;
                ss2 << secondArg;
                ss2 >> secondArgVal;

                cout << "Searching within range " << firstArgVal << " and " << secondArgVal << endl;
                // call function
                Search(firstArgVal, secondArgVal);
            }
            // if no comma, is specific search
            else
            {
                // convert argument to integer using stringstream
                stringstream ss;
                ss << argument;
                ss >> argumentValue;
                cout << "Searching " << argumentValue << endl;

                // call function
                Search(argumentValue);
            }
        }

        else
        {
            cout << "Invalid command. Moving on to next command." << endl;
        }
    }

    // close files
    outputFile.close();
    inputFile.close();

    cout << "Done! Please check output.txt for results." << endl;

    return 0;
}