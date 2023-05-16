//
//  DataTypes.h
//  MivneWet1edit
//
//  Created by Ori Razon on 16/5/2023.
//

#ifndef DataTypes_h
#define DataTypes_h

#include "wet1util.h"
#include <exception>
#include <new>

class Failure : public std::exception
{
public:
    const char * what() const throw ()
    {
        return "ID already exists";
    }
};
template<class nodeType>
struct node
{
    nodeType data;
    node* left;
    node* right;
    int height;
    node(const nodeType& data) : data(data), left(nullptr), right(nullptr), height(1){}
};

template<class nodeType>
class tree
{
public:
    node<nodeType>* root;
    tree() : root(nullptr) {}
    ~tree()
    {
        //TO DO
    }
    int height(node<nodeType>* N)
    {
        if (N == NULL)
            return 0;
        return N->height;
    }
    int getBalance(node<nodeType>* N)
    {
        return height(N->left) - height(N->right);
    }
    int max(int a, int b)
    {
        if(a>b)
            return a;
        return b;
    }
    node<nodeType>* insert(node<nodeType>* Node, nodeType data)
    {
        if(Node == nullptr)
        {
            return new node(data);
        }
        if (data.ID < Node->data.ID)
            Node->left = insert(Node->left, data);
        else if (data.ID > Node->data.ID)
            Node->right = insert(Node->right, data);
        else
        {
            throw Failure(); 
            return Node;
        }
        // Update parent node's height
        Node->height = 1 + max(height(Node->left), height(Node->right));
        // Get balance of parent
        int balance = getBalance(Node);
        // Check if parent is unbalanced and split into LL, RR, LR, RL rotation cases
        // LL
        if (balance > 1 && data.ID < Node->left->data.ID)
            return rightRotate(Node);
        // RR
        if (balance < -1 && data.ID > Node->right->data.ID)
            return leftRotate(Node);
     
        // LR
        if (balance > 1 && data.ID > Node->left->data.ID)
        {
            Node->left = leftRotate(Node->left);
            return rightRotate(Node);
        }
     
        // RL
        if (balance < -1 && data.ID < Node->right->data.ID)
        {
            Node->right = rightRotate(Node->right);
            return leftRotate(Node);
        }
        return Node;
    }
    node<nodeType>* leftLeaf(node<nodeType>* Node)
    {
        node<nodeType>* current = Node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }
    node<nodeType>* remove(node<nodeType>* Node, nodeType data)
    {
        if (root == NULL)
            return root;
        if (data.ID < root->data.ID)
            root->left = deleteNode(root->left, data.ID);
        else if(data.ID > root->data.ID)
            root->right = deleteNode(root->right, data.ID);
        else
        {
            
            if((root->left == NULL) || (root->right == NULL))
            {
                node<nodeType>* temp;
                if (root->left)
                    temp = root->left;
                else
                    temp = root->right;
                // No child case
                if (temp == NULL)
                {
                    temp = root;
                    root = NULL;
                }
                else // One child case
                *root = *temp;
                free(temp);
            }
            else
            {
                // node with two children: Get the inorder
                // successor (smallest in the right subtree)
                node<nodeType>* temp = leftLeaf(root->right);
     
                // Copy the inorder successor's
                // data to this node
                root->data.ID = temp->data.Id;
     
                // Delete the inorder successor
                root->right = deleteNode(root->right,temp->data.ID);
            }
        }
         
            // If the tree had only one node
            // then return
            if (root == NULL)
            return root;
         
            // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
            root->height = 1 + max(height(root->left),
                                   height(root->right));
         
            // STEP 3: GET THE BALANCE FACTOR OF
            // THIS NODE (to check whether this
            // node became unbalanced)
            int balance = getBalance(root);
         
            // If this node becomes unbalanced,
            // then there are 4 cases
         
            // Left Left Case
            if (balance > 1 &&
                getBalance(root->left) >= 0)
                return rightRotate(root);
         
            // Left Right Case
            if (balance > 1 &&
                getBalance(root->left) < 0)
            {
                root->left = leftRotate(root->left);
                return rightRotate(root);
            }
         
            // Right Right Case
            if (balance < -1 &&
                getBalance(root->right) <= 0)
                return leftRotate(root);
         
            // Right Left Case
            if (balance < -1 &&
                getBalance(root->right) > 0)
            {
                root->right = rightRotate(root->right);
                return leftRotate(root);
            }
         
            return root;
    }
};

struct movieData
{
    int ID;
    Genre genre;
    int views;
    bool vipOnly;
    movieData(int ID, Genre genre, int views, bool VIP) : ID(ID), genre(genre), views(views), vipOnly(VIP) {}
    ~movieData() = default;
};

struct userData
{
    int ID;
    bool vipStatus;
    userData(int ID, bool status) : ID(ID), vipStatus(status) {}
    ~userData() = default;
};

#endif /* DataTypes_h */

