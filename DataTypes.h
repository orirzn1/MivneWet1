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
private:
    node<nodeType>* root;
    
public:
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



    node<nodeType>* rightRotate(node<nodeType>* node_a){
        node<nodeType>* node_b = node_a->left;
        node_a->left = node_b->right;
        node_b->right = node_a;
        return node_b;

    }


    node<nodeType>* leftRotate(node<nodeType>* node_a){
        node<nodeType>* node_b = node_a->right;
        node_a->right = node_b->left;
        node_b->left = node_a;
        return node_b;

    }


    void insert(node<nodeType>* Node)
    {
        nodeType data = Node->data;
        root = insert_recursion(Node, data);
    }
    
    node<nodeType>* insert_recursion(node<nodeType>* Node, nodeType data)
    {
        if(Node == nullptr)
        {
            return new node<nodeType>(data);
        }
        if (data.ID < Node->data.ID)
            Node->left = insert_recursion(Node->left, data);
        else if (data.ID > Node->data.ID)
            Node->right = insert_recursion(Node->right, data);
        else
        {
            throw Failure();
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
    
    void remove(node<nodeType>* Node)
    {
        nodeType data = Node->data;
        root = remove_recursion(Node, data);
    }
    
    
    node<nodeType>* remove_recursion(node<nodeType>* Node, nodeType data)
    {
        if (Node == nullptr)
            return Node;
        if (data.ID < root->data.ID)
            Node->left = remove_recursion(Node->left, data.ID);
        else if(data.ID > Node->data.ID)
            Node->right = remove_recursion(Node->right, data.ID);
        else
        {
            
            if((Node->left == NULL) || (Node->right == NULL))
            {
                node<nodeType>* temp;
                if (Node->left)
                    temp = Node->left;
                else
                    temp = Node->right;
                // No child case
                if (temp == NULL)
                {
                    temp = root;
                    Node = NULL;
                }
                else // One child case
                *Node = *temp;
                free(temp);
            }
            else
            {
                // node with two children: Get smallest in the right subtree
                node<nodeType>* temp = leftLeaf(Node->right);
     
                // Copy data
                Node->data.ID = temp->data.Id;
     
                // Delete smallest in right subtree
                Node->right = remove_recursion(Node->right,temp->data.ID);
            }
        }
            if (Node == NULL)
            return Node;
        
            Node->height = 1 + max(height(root->left), height(root->right));

            int balance = getBalance(Node);
         
            //Check balance
         
            // LL
            if (balance > 1 &&
                getBalance(Node->left) >= 0)
                return rightRotate(Node);
         
            // LR
            if (balance > 1 &&
                getBalance(Node->left) < 0)
            {
                root->left = leftRotate(Node->left);
                return rightRotate(Node);
            }
         
            // RR
            if (balance < -1 &&
                getBalance(Node->right) <= 0)
                return leftRotate(Node);
         
            // RL
            if (balance < -1 &&
                getBalance(Node->right) > 0)
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

