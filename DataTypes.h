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
        return "FAILURE";
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
    node(nodeType* data, keyType& key) : key(key), data(data), left(nullptr), right(nullptr), height(1){}
    /*node& operator=(const node& other){
        key = other.key;
        delete data;
        data = other.data;
        left = other.left;
        right = other.right;
        height = other.height;

        return *this;
    }*/
};

template<class nodeType, class keyType>
class tree
{
protected:
    int largest_node_ID;
    int node_count;
    node<nodeType, keyType>* root;
    
public:
    tree() : root(nullptr), largest_node_ID(0), node_count(0) {}
    ~tree()
    {
        destroyTree(root);
    }
    
    /*tree& operator = (const tree& other)
    {
        largest_node_ID = other.largest_node_ID;
        node_count = other.node_count;
        root = other.root;
        
        return *this; 
    }*/
    
    void destroyTree(node<nodeType, keyType>* N)
    {
        if(N)
        {
            destroyTree(N->right);
            destroyTree(N->left);
            //if(N->data)
                //delete N->data;
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

    void insert(nodeType* data, keyType& key)
    {
        if(root == nullptr || key > rightLeaf(root)->key)
            largest_node_ID = data->ID;
        root = insert_recursion(root, *data, key);
        node_count++;
    }
    
    node<nodeType, keyType>* insert_recursion(node<nodeType, keyType>* Node, nodeType& data, keyType& key)
    {
        if(Node == nullptr)
        {
            node<nodeType, keyType>* new_node =  new node<nodeType, keyType>(&data, key);
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
    
    void rearrange(keyType& key)
    {
        root = rearrange_recursion(root, key);
        if(rightLeaf(root))
        {
            largest_node_ID = rightLeaf(root)->data->ID;
        }
        node_count--;
    }
    
    node<nodeType, keyType>* rearrange_recursion(node<nodeType, keyType>* Node, keyType& key)
    {
        if (Node == nullptr)
            throw Failure();
        if (key < Node->key)
            Node->left = rearrange_recursion(Node->left, key);
        else if(key > Node->key)
            Node->right = rearrange_recursion(Node->right, key);
        else
        {
            if((Node->left == nullptr) || (Node->right == nullptr))
            {
                node<nodeType, keyType> *temp;
                if (Node->left)
                    temp = Node->left;
                else
                    temp = Node->right;
                // No child case
                if (temp == nullptr) {
                    Node = nullptr;
                    std::cout << "case 1"<< std::endl;
                } else {// One child case
                    std::cout << "case 2"<< std::endl;
                    Node = temp;
                }
            }
            else
            {
                // node with two children: Get smallest in the right subtree
                node<nodeType, keyType>* temp = leftLeaf(Node->right);

                // Copy data
                Node->data = new nodeType(*(temp->data));
                Node->key = temp->key;

                // Delete smallest in right subtree
                if(Node->right != nullptr)
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
    
    
    
    void remove(keyType& key)
    {
        root = remove_recursion(root, key);
        if(rightLeaf(root))
        {
            largest_node_ID = rightLeaf(root)->data->ID;
        }
        node_count--;
    }
    
    
    node<nodeType, keyType>* remove_recursion(node<nodeType, keyType>* Node, keyType& key)
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
                node<nodeType, keyType> *temp;
                if (Node->left)
                    temp = Node->left;
                else
                    temp = Node->right;
                // No child case
                if (temp == nullptr) {
                    std::cout << "case 1"<< std::endl;
                    temp = Node;
                    Node = nullptr;
                    delete temp->data;
                    delete temp;
                } else {// One child case
                    std::cout << "case 2"<< std::endl;
                    node<nodeType, keyType> *old_ptr = Node;
                    Node = temp;
                    delete old_ptr->data;
                    delete old_ptr;

                }
            }
            else
            {
                // node with two children: Get smallest in the right subtree
                node<nodeType, keyType>* temp = leftLeaf(Node->right);

                // Copy data
                delete Node->data;  // Delete the old node's data
                Node->data = new nodeType(*(temp->data));
                Node->key = temp->key;

                // Delete smallest in right subtree
                if(Node->right != nullptr)
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
        std::cout << Node->data->ID << std::endl;

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
        if(rating > other.rating)
            return true;
        if(rating == other.rating && views > other.views)
            return true;
        if(rating == other.rating && views == other.views && ID < other.ID)
            return true;
        
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
    tree<userData, int> users;
    int group_watches[5]; //this will hold the number of watches as a collective
    int total_views[5]; //this will hold the number of views taking the size of the group into account
    
    groupData(int ID) : ID(ID), user_count(0), VIP_count(0), users(), group_watches(), total_views(){}
    
   /* groupData& operator=(const groupData& other){
        ID = other.ID;
        user_count = other.user_count;
        VIP_count = other.VIP_count;
        for (int i = 0; i < 5; i++)
            group_watches[i] = other.group_watches[i];
        for (int i = 0; i < 5; i++)
            total_views[i] = other.total_views[i];
        users = other.users;
        return *this;

        
    }*/
    
    void add_user(userData* data)
    {
        users.insert(data, data->ID);
        user_count++;
        if(data->vipStatus == true)
            VIP_count++; 
    }
    Genre findFavoriteGenre()
    {
        int max = 0;
        for(int i = 1; i < 5; i++)
        {
           if(total_views[i] > total_views[i-1])
               max = i;
        }
        if(total_views[max] == 0)
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

class groupTreeClass : public tree<groupData, int>
{
    public:
    void removeAllUsers(node<userData, int>* userTreeRoot)
    {
        
        if(userTreeRoot)
        {
            removeAllUsers(userTreeRoot->right);
            removeAllUsers(userTreeRoot->left);
            userTreeRoot->data->group = nullptr;
        }
    }
};

#endif /* DataTypes_h */

