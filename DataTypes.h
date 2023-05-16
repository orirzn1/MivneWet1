//
//  DataTypes.h
//  MivneWet1edit
//
//  Created by Ori Razon on 16/5/2023.
//

#ifndef DataTypes_h
#define DataTypes_h

#include "wet1util.h"

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
    void insert(node<nodeType>* Node, nodeType data)
    {
        
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

