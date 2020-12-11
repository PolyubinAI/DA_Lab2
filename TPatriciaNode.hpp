//
// Created by Ars Polybin on 20.11.2020.
//

#ifndef LAB_2_3_TPATRICIANODE_HPP
#define LAB_2_3_TPATRICIANODE_HPP

#include <cstring>

const int MAX_SIZE = 256;
typedef unsigned long long TValue;
typedef char TKey;


struct TPatriciaNode
{
    int id = -1;
    int bit;

    TKey *key;
    TValue value;

    TPatriciaNode *left;
    TPatriciaNode *right;

    void Init(int b, TKey *k, TValue v, TPatriciaNode *l, TPatriciaNode *r)
    {
        bit = b;
        if (k)
        {
            key = new char[strlen(k) + 1];
            strcpy(key, k);
        }
        else
            key = k;
        value = v;
        left = l;
        right = r;
    }

    TPatriciaNode()
    {
        Init(-1, 0, 0, this, this);
    }

    TPatriciaNode(int b, TKey *k, TValue v)
    {
        Init(b, k, v, this, this);
    }

    TPatriciaNode(int b, TKey *k, TValue v, TPatriciaNode *l, TPatriciaNode *r)
    {
        Init(b, k, v, l, r);
    }

    ~TPatriciaNode()
    {
        delete [] key;
    }
};

#endif //LAB_2_3_TPATRICIANODE_HPP
