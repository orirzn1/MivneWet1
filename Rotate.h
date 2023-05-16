//
//  DataTypes.h
//  MivneWet1edit
//
//  Created by Ori Razon on 16/5/2023.
//

#ifndef Rotate_h
#define Rotate_h

#include "wet1util.h"
#include "DataTypes.h"

template<class nodeType>
node<nodeType>* rightRotate(node<nodeType>* root){
    node<nodeType>* left_son = root->left;
    root->left = left_son->right;
    left_son->right = root;
    return left_son;

}

template<class nodeType>
node<nodeType>* leftRotate(node<nodeType>* root){
    node<nodeType>* right_son = root->right;
    root->right = right_son->left;
    right_son->left = root;
    return right_son;

}

#endif /* Rotate_h */

