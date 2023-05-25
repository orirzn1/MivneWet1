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

class movieData;

class Failure : public std::exception
{
public:
    const char * what() const throw ()
    {
        return "FAILURE";
    }
};

template<class nodeType, class keyType>
struct node
{
    keyType key;
    nodeType data;
    node* left;
    node* right;
    int height;
    node(nodeType data, keyType& key) : key(key), data(data), left(nullptr), right(nullptr), height(1){}

};

template<class nodeType, class keyType>
class tree
{
protected:
    int largest_node_ID;
    int node_count;
    node<nodeType, keyType>* root;
    
public:

    tree() :  largest_node_ID(0), node_count(0), root(nullptr) {}
    ~tree()
    {
        destroyTree(root);
    }
    
    int getCount()
    {
        return node_count; 
    }
    

    
    void destroyTree(node<nodeType, keyType>* N)
    {
        if(N)
        {
            destroyTree(N->right);
            destroyTree(N->left);

            delete N; 
        }
        N = nullptr;
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

    void insert(nodeType& data, keyType& key)
    {
        if(root == nullptr || key > rightLeaf(root)->key)
            largest_node_ID = data.get()->ID;
        root = insert_recursion(root, data, key);
        node_count++;
    }
    
    node<nodeType, keyType>* insert_recursion(node<nodeType, keyType>* Node, nodeType& data, keyType& key)
    {
        if(Node == nullptr)
        {
            node<nodeType, keyType>* new_node =  new node<nodeType, keyType>(data, key);
            return new_node;
        }
        if (key < Node->key)
            Node->left = insert_recursion(Node->left, data, key);
        else if (key > Node->key)
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
        if (balance > 1 && key < Node->left->key)
            return rightRotate(Node);
        // RR
        if (balance < -1 && key > Node->right->key)
            return leftRotate(Node);
     
        // LR
        if (balance > 1 && key > Node->left->key)
        {
            Node->left = leftRotate(Node->left);
            return rightRotate(Node);
        }
     
        // RL
        if (balance < -1 && key < Node->right->key)
        {
            Node->right = rightRotate(Node->right);
            return leftRotate(Node);
        }
        return Node;
    }
    
    
    node<nodeType, keyType>* leftLeaf(node<nodeType, keyType>* Node)
    {
        if (Node == nullptr)
            return nullptr;
        node<nodeType, keyType>* current = Node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }
    
    node<nodeType, keyType>* rightLeaf(node<nodeType, keyType>* Node)
    {
        if (Node == nullptr)
            return nullptr;
        node<nodeType, keyType>* current = Node;
        while (current->right != nullptr)
            current = current->right;
        return current;
    }

    
    bool contains(keyType& key)
    {
        node<nodeType, keyType>* current = root;
        while(current) {
            if (!(key>current->key) && !(key<current->key)) {
                return true;
            }
            else if (key < current->key) {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        return false;
    }
    
    void remove(keyType& key)
    {

        if(!contains(key))
            throw Failure();
        root = remove_recursion(root, key);
        if(rightLeaf(root))
        {
            largest_node_ID = rightLeaf(root)->data.get()->ID;
        }
        node_count--;
    }
    
    node<nodeType,keyType>* balance(node<nodeType,keyType>* Node)
    {
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
    
    node<nodeType,keyType>* findMin(node<nodeType,keyType>* Node)
    {
        return Node->left ? findMin(Node->left) : Node;
    }
    
    node<nodeType,keyType>* getMinNodeRightChild(node<nodeType,keyType>* Node) {
      if (!Node->left) {
          return Node->right;
      }
      Node->left = getMinNodeRightChild(Node->left);
      return balance(Node);
    }
    
    node<nodeType, keyType>* remove_recursion(node<nodeType, keyType>* Node, keyType& key)
    {
        try {
            if (Node == nullptr)
            {
                return nullptr;
            }
            if (key < Node->key)
            {
                Node->left = remove_recursion(Node->left, key);
            }
            else if (key > Node->key)
            {
                Node->right = remove_recursion(Node->right, key);
            }
            else
            {
                //currentNode is the node with the given key that we want to delete
                node<nodeType,keyType>* leftChild = Node->left;
                node<nodeType,keyType>* rightChild = Node->right;
                delete Node;
                if (!rightChild) {
                    return leftChild;
                }
                node<nodeType,keyType>* minNode = findMin(rightChild);
                minNode->right = getMinNodeRightChild(rightChild);
                minNode->left = leftChild;
                
                return balance(minNode);
            }
            return balance(Node);
        }
        catch (...) {
            throw;
        }
    }
    
    node<nodeType, keyType>* findNode(const keyType& key)
    {
        return findNodeRecursion(root, key);
    }
    
    node<nodeType, keyType>* findNodeRecursion(node<nodeType, keyType>* Node, const keyType& key)
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

    StatusType insertDescendingOrder(int* const output)
    {
        int initial_index = 0;
        if(this->node_count == 0)
            return StatusType::FAILURE;
        insertDescendingOrderRecursion(output, root, initial_index);
        return StatusType::SUCCESS;
    }
    
    void insertDescendingOrderRecursion(int* const output, node<nodeType, keyType>* node, int& index)
    {

        if(node == nullptr)
            return;
        if(node->right)
            insertDescendingOrderRecursion(output, node->right, index);
        output[index] = node->data.get()->ID;
        ++index;
        if(node->left)
            insertDescendingOrderRecursion(output, node->left, index);
    }
};

struct movieData
{
    int ID;
    Genre genre;
    int views;
    bool vipOnly;
    double rating;
    int num_of_ratings;
    movieData(int ID, Genre genre, int views, bool VIP) : ID(ID), genre(genre), views(views), vipOnly(VIP), rating(0), num_of_ratings(0) {}
    movieData() = default; 
    ~movieData() = default;
    
    bool operator < (const movieData& other)
    {
        if(rating < other.rating)
        {
            return true;
        }
        if(rating == other.rating && views < other.views)
        {
            return true;
        }
        if(rating == other.rating && views == other.views && ID > other.ID)
        {
            return true;
        }

        return false;
    }
    bool operator > (const movieData& other)
    {
        if(rating > other.rating)
        {
            return true;
        }
        if(rating == other.rating && views > other.views)
        {
            return true;
        }
        if(rating == other.rating && views == other.views && ID < other.ID)
        {
            return true;
        }

        return false;
    }
};

struct groupData;

struct userData
{
    int ID;
    bool vipStatus;
    int views[5];
    groupData* group;
    int group_watches_before_joining[5];
    userData(int ID, bool status) : ID(ID), vipStatus(status), views(), group(nullptr), group_watches_before_joining() {}
    ~userData() = default;
};


struct groupData
{
    int ID;
    int user_count;
    int VIP_count;
    tree<std::shared_ptr<userData>, int> users;
    int group_watches[5]; //this will hold the number of watches as a collective
    int total_views[5]; //this will hold the number of views taking the size of the group into account
    
    groupData(int ID) : ID(ID), user_count(0), VIP_count(0), users(), group_watches(), total_views(){}
    
    void add_user(std::shared_ptr<userData> data)
    {
        users.insert(data, data.get()->ID);
        user_count++;
        if(data->vipStatus == true)
            VIP_count++; 
    }
    Genre findFavoriteGenre()
    {
        int max = 0;
        for(int i = 0; i < 4; i++)
        {

           if(total_views[i] > total_views[max] || (total_views[i]==total_views[max] && i < max)) {
               max = i;

           }
        }

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

class groupTreeClass : public tree<std::shared_ptr<groupData>, int>
{
    public:
    void removeAllUsers(node<std::shared_ptr<userData>, int>* userTreeRoot)
    {
        
        if(userTreeRoot)
        {
            removeAllUsers(userTreeRoot->right);
            removeAllUsers(userTreeRoot->left);
            for(int i = 0; i < 5; i++)
                userTreeRoot->data.get()->views[i] += (userTreeRoot->data.get()->group->group_watches[i] - userTreeRoot->data.get()->group_watches_before_joining[i]);
            userTreeRoot->data.get()->group = nullptr;
        }
    }
};

#endif /* DataTypes_h */

