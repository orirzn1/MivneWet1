//
//  DataTypes.h
//  MivneWet1edit
//
//  Created by Ori Razon on 16/5/2023.
//

#ifndef DataTypes_h
#define DataTypes_h

#include "wet1util.h"

struct movieData
{
    int ID;
    Genre genre;
    int views;
    bool vipOnly;
    movieData(int ID, Genre genre, int views, bool VIP) : ID(ID), genre(genre), views(views), vipOnly(VIP) {}
    ~movie() = default;
}

struct userData
{
    int ID;
    bool vipStatus;
    userData(int ID, bool status) : ID(ID), vipStatus(status) {}
    ~userData() = default;
}

#endif /* DataTypes_h */
