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
#include <iostream>
#include <memory>

class Failure : public std::exception
{
public:
    const char * what() const throw ()
    {
        return "ID already exists";
    }
};
template<class nodeType, class keyType>
struct node
{
    keyType key;
    nodeType* data;
    node* left;
    node* right;
    int height;
    node(nodeType* data, keyType key) : key(key), data(data), left(nullptr), right(nullptr), height(1){}
};

template<class nodeType, class keyType>
class tree
{
private:
    node<nodeType, keyType>* root;
    int largest_node_ID;
    int node_count;
    
public:
    tree() : root(nullptr), largest_node_ID(0), node_count(0) {}
    ~tree()
    {
        destroyTree(root);
    }
    
    void destroyTree(node<nodeType, keyType>* N)
    {
        if(N)
        {
            destroyTree(N->right);
            destroyTree(N->left);
            delete N; 
        }
    }
    
    int getLargestNodeID()
    {
        return largest_node_ID;
    }
    int height(node<nodeType, keyType>* N)
    {
        if (N == nullptr)
            return 0;
        return N->height;
    }
    int getBalance(node<nodeType, keyType>* N)
    {
        return height(N->left) - height(N->right);
    }
    int max(int a, int b)
    {
        if(a>b)
            return a;
        return b;
    }
    node<nodeType, keyType>* getRoot()
    {
        return root;
    }
    int getCount()
    {
        return node_count; 
    }

    node<nodeType, keyType>* rightRotate(node<nodeType, keyType>* node_a){
        node<nodeType, keyType>* node_b = node_a->left;
        node_a->left = node_b->right;
        node_b->right = node_a;


        node_a->height = max(height(node_a->left), height(node_a->right))+1;
        node_b->height = max(height(node_b->left), height(node_b->right))+1;
        return node_b;
    }


    node<nodeType, keyType>* leftRotate(node<nodeType, keyType>* node_a){
        node<nodeType, keyType>* node_b = node_a->right;
        node_a->right = node_b->left;
        node_b->left = node_a;


        node_a->height = max(height(node_a->left), height(node_a->right))+1;
        node_b->height = max(height(node_b->left), height(node_b->right))+1;
        return node_b;

    }

    void insert(nodeType& data, keyType key)
    {
        if(key > rightLeaf(root)->key || root == nullptr)
            largest_node_ID = data.ID;
        root = insert_recursion(root, data, key);
        node_count++;
    }
    
    node<nodeType, keyType>* insert_recursion(node<nodeType, keyType>* Node, nodeType& data, keyType key)
    {
        if(Node == nullptr)
        {
            return new node<nodeType, keyType>(&data, key);
        }
        if (data.ID < Node->data->ID)
            Node->left = insert_recursion(Node->left, data, key);
        else if (data.ID > Node->data->ID)
            Node->right = insert_recursion(Node->right, data, key);
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
        if (balance > 1 && data.ID < Node->left->data->ID)
            return rightRotate(Node);
        // RR
        if (balance < -1 && data.ID > Node->right->data->ID)
            return leftRotate(Node);
     
        // LR
        if (balance > 1 && data.ID > Node->left->data->ID)
        {
            Node->left = leftRotate(Node->left);
            return rightRotate(Node);
        }
     
        // RL
        if (balance < -1 && data.ID < Node->right->data->ID)
        {
            Node->right = rightRotate(Node->right);
            return leftRotate(Node);
        }
        return Node;
    }
    
    
    node<nodeType, keyType>* leftLeaf(node<nodeType, keyType>* Node)
    {
        node<nodeType, keyType>* current = Node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }
    
    node<nodeType, keyType>* rightLeaf(node<nodeType, keyType>* Node)
    {
        node<nodeType, keyType>* current = Node;
        while (current->right != nullptr)
            current = current->right;
        return current;
    }
    
    void remove(keyType key)
    {
        root = remove_recursion(root, key);
        largest_node_ID = rightLeaf(root)->data->ID;
        node_count--;
    }
    
    
    node<nodeType, keyType>* remove_recursion(node<nodeType, keyType>* Node, keyType key)
    {
        if (Node == nullptr)
            throw Failure(); 
        if (key < Node->key)
            Node->left = remove_recursion(Node->left, key);
        else if(key > Node->key)
            Node->right = remove_recursion(Node->right, key);
        else
        {
            if((Node->left == nullptr) || (Node->right == nullptr))
            {
                node<nodeType, keyType>* temp;
                if (Node->left)
                    temp = Node->left;
                else
                    temp = Node->right;
                // No child case
                if (temp == nullptr)
                {
                    temp = Node;
                    Node = nullptr;
                }
                else // One child case
                    *Node = *temp;
                free(temp);
            }
            else
            {
                // node with two children: Get smallest in the right subtree
                node<nodeType, keyType>* temp = leftLeaf(Node->right);
     
                // Copy data
                Node->key = temp->key;
     
                // Delete smallest in right subtree
                Node->right = remove_recursion(Node->right,temp->key);
            }
        }
            if (Node == nullptr)
            return Node;
        
            Node->height = 1 + max(height(Node->left), height(Node->right));

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
                Node->left = leftRotate(Node->left);
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
    
    node<nodeType, keyType>* findNode(keyType key)
    {
        return findNodeRecursion(root, key);
    }
    
    node<nodeType, keyType>* findNodeRecursion(node<nodeType, keyType>* Node, keyType key)
    {
        if (Node == nullptr)
            throw Failure();
        if (key < Node->key)
            return findNodeRecursion(Node->left, key);
        else if(key > Node->key)
            return findNodeRecursion(Node->right, key); 
        else
        {
            return Node;
        }
    }

    //remember to delete
    void printTree() const
    {
        printTree(root, 0);
    }
    
    StatusType insertDescendingOrder(int *const output)
    {
        if(this->node_count == 0)
            return StatusType::FAILURE;
        insertDescendingOrderRecursion(output, root, 0);
        return StatusType::SUCCESS;
    }
    
    void insertDescendingOrderRecursion(int *const output, node<nodeType, keyType>* node, int index)
    {
        if(node == nullptr)
            return;
        insertDescendingOrderRecursion(output, node->right, index);
        output[index] = node->data->ID;
        index++;
        insertDescendingOrderRecursion(output, node->left, index);
    }

private:
    void printTree(node<nodeType, keyType>* Node, int level) const {
        if (Node == nullptr)
            return;

        // Print right subtree
        printTree(Node->right, level + 1);

        // Indentation based on the level
        for (int i = 0; i < level; ++i)
            std::cout << "    ";

        // Print current node's ID
        std::cout << Node->data.ID << std::endl;

        // Print left subtree
        printTree(Node->left, level + 1);
    }
};

struct movieData
{
    int ID;
    Genre genre;
    int views;
    bool vipOnly;
    int rating;
    int num_of_ratings;
    movieData(int ID, Genre genre, int views, bool VIP) : ID(ID), genre(genre), views(views), vipOnly(VIP), rating(0), num_of_ratings(0) {}
    ~movieData() = default;
    
    bool operator < (const movieData& other)
    {
        if(rating < other.rating)
            return true;
        if(rating == other.rating && views < other.views)
            return true;
        if(rating == other.rating && views == other.views && ID > other.ID)
            return true;
        
        return false;
    }
    bool operator > (const movieData& other)
    {
        return !(*this < other);
    }
};

struct groupData;

struct userData
{
    int ID;
    bool vipStatus;
    int views[5];
    groupData* group;
    userData(int ID, bool status) : ID(ID), vipStatus(status), views(), group(nullptr) {}
    ~userData() = default;
};


struct groupData
{
    int ID;
    int user_count;
    bool VIP;
    tree<userData, int> users;
    int views[5];
    
    groupData(int ID) : ID(ID), user_count(0), VIP(false), users(), views(){}
    void add_user(userData data)
    {
        users.insert(data, data.ID);
        user_count++;
        if(data.vipStatus == true)
            VIP = true; 
    }
    Genre findFavoriteGenre()
    {
        int max = 0;
        for(int i = 1; i < 5; i++)
        {
           if(views[i] > views[i-1])
               max = i;
        }
        if(views[max] == 0)
            throw Failure();
        switch(max)
        {
            case 0:
                return Genre::COMEDY;
                break;
            case 1:
                return Genre::DRAMA;
                break;
            case 2:
                return Genre::ACTION;
                break;
            case 3:
                return Genre::FANTASY;
                break;
            default:
                return Genre::NONE;
                break;
        }
    }
};

#endif /* DataTypes_h */

