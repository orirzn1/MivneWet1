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

