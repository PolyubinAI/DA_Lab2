//
// Created by Ars Polybin on 20.11.2020.
//

#ifndef LAB_2_3_TPATRICIA_HPP
#define LAB_2_3_TPATRICIA_HPP


#include "TPatriciaNode.hpp"

static int GetBit(char *key, int bit)
{
    if (bit < 0) {
        bit = 0;
    }
    return (key[bit / 8] >> (7 - (bit % 8))) & 1U;
}

static int FirstDifferentBit(char *a, char *b)
{
    if (a == 0 || b == 0) {
        return 0;
    }
    int i = 0;

    while (a[i] == b[i]) {
        i++;
    }
    i *= 8;

    while (GetBit(a, i) == GetBit(b, i)) {
        i++;
    }
    return i;
}

static bool SameString(char *a, char *b)
{
    if (a == 0 || b == 0)
        return 0;
    return (strcmp(a, b) == 0);
}

static void Lowercase(char *str) {
        int len = std::strlen(str);
        for (int i = 0; i < len; ++i)
            str[i] = std::tolower(str[i]);
}

struct TPatricia
{
    TPatriciaNode *head;
    int size;

    TPatricia()
    {
        head = new TPatriciaNode();
        head->key = nullptr;
        size = 0;
    }

    void Destruction(TPatriciaNode *node)
    {
        if (node->left->bit > node->bit) {
            Destruction(node->left);
        }
        if (node->right->bit > node->bit) {
            Destruction(node->right);
        }
        delete node;
    }

    ~TPatricia()
    {
        Destruction(head);
    }

    void SetKey(TPatriciaNode *start, TPatriciaNode *finish)
    {
        if (strlen(finish->key) < strlen(start->key))
        {
            delete[] finish->key;
            finish->key = new char[strlen(start->key) + 1];
        }
        strcpy(finish->key, start->key);

        finish->value = start->value;
    }

    TPatriciaNode *Search(TKey *key)
    {
        TPatriciaNode *parent = head;
        TPatriciaNode *lSon = head->left;

        while (parent->bit < lSon->bit)
        {
            parent = lSon;
            if(GetBit(key, lSon->bit))
            {
                lSon = lSon->right;
            }else{
                lSon = lSon->left;
            }
        }

        if (!SameString(key, lSon->key)) {
            return 0;
        }

        return lSon;
    }

    TPatriciaNode *Insert(TKey *key, TValue value)
    {
        TPatriciaNode *parent = head;
        TPatriciaNode *lSon = head->left;
        while (parent->bit < lSon->bit)
        {
            parent = lSon;
            if(GetBit(key, lSon->bit)){
                lSon = lSon->right;
            }else{
                lSon = lSon->left;
            }
        }

        if (SameString(key, lSon->key))
            return 0;

        int lBitPos = FirstDifferentBit(key, lSon->key);

        parent = head;
        TPatriciaNode *l = head->left;

        while (parent->bit < l->bit && l->bit < lBitPos)
        {
            parent = l;
            l = (GetBit(key, l->bit) ? l->right : l->left);
        }

        try
        {
            lSon = new TPatriciaNode();
        }
        catch (const std::bad_alloc &e)
        {
            std::cout << "ERROR: not enough memory\n";
            return 0;
        }

        lSon->Init(lBitPos, key, value,
                      (GetBit(key, lBitPos) ? l : lSon),
                      (GetBit(key, lBitPos) ? lSon : l));

        if (GetBit(key, parent->bit))
            parent->right = lSon;
        else
            parent->left = lSon;

        size++;
        return lSon;
    }

    bool Delete(TKey *k)
    {
        TPatriciaNode *grandPa = nullptr, *son = nullptr, *parent = nullptr;

        parent = head;
        son = (parent->left);
        while (parent->bit < son->bit)
        {
            grandPa = parent;
            parent = son;
            if(GetBit(k, son->bit)){
                son = son->right;
            }else{
                son = son->left;
            }
        }

        if (!SameString(k, son->key))
        {
            return false;
        }
        TPatriciaNode *a, *b;
        char *key;

        if (parent != son)
        {
            SetKey(parent, son);

              key = parent->key;
            b = parent;
            a = (GetBit(key, parent->bit) ? parent->right : parent->left);

            while (b->bit < a->bit)
            {
                b = a;
                a = (GetBit(key, a->bit) ? a->right : a->left);
            }
            if (GetBit(key, b->bit)) {
                b->right = son;
            }else {
                b->left = son;
            }
        }

        TPatriciaNode *ch = (GetBit(k, parent->bit) ? parent->left : parent->right);
        if (GetBit(k, grandPa->bit))
            grandPa->right = ch;
        else
            grandPa->left = ch;

        delete parent;

        --size;

        return true;
    }

    void indexation(TPatriciaNode *node, TPatriciaNode **nodes, int &index)
    {
        node->id = index;
        nodes[index] = node;
        ++index;
        if (node->left->bit > node->bit)
        {
            indexation(node->left, nodes, index);
        }
        if (node->right->bit > node->bit)
        {
            indexation(node->right, nodes, index);
        }
    }

    void Save(std::ofstream &file)
    {
        file.write((const char *)&(size), sizeof(int));

        int index = 0;
        TPatriciaNode **nodes;
        try
        {
            nodes = new TPatriciaNode *[size + 1];
        }
        catch (const std::bad_alloc &e)
        {
            std::cout << "ERROR: fail for allocating memory\n";
            return;
        }
        indexation(head, nodes, index);

        TPatriciaNode *node;
        for (int i = 0; i < (size + 1); ++i)
        {
            node = nodes[i];
            file.write((const char *)&(node->value), sizeof(TValue));
            file.write((const char *)&(node->bit), sizeof(int));
            long long len = node->key ? strlen(node->key) : 0;
            file.write((const char *)&(len), sizeof(int));
            file.write(node->key, sizeof(char) * len);
            file.write((const char *)&(node->left->id), sizeof(int));
            file.write((const char *)&(node->right->id), sizeof(int));
        }
        delete[] nodes;
    }

    void Load(std::ifstream &file)
    {
        int n;
        file.read((char *)&n, sizeof(int));
        size = n;
        if (!size)
            return;

        TPatriciaNode **nodes = new TPatriciaNode *[size + 1];
        nodes[0] = head;
        for (int i = 1; i < (size + 1); ++i) {
            nodes[i] = new TPatriciaNode();
        }
        int bit;
        int len;
        TKey *key = 0;
        TValue value;
        int indLeft, indRight;

        for (int i = 0; i < (size + 1); ++i)
        {
            file.read((char *)&(value), sizeof(TValue));
            file.read((char *)&(bit), sizeof(int));
            file.read((char *)&(len), sizeof(int));
            if (len)
            {
                key = new char[len + 1];
                key[len] = 0;
            }
            file.read(key, len);
            file.read((char *)&(indLeft), sizeof(int));
            file.read((char *)&(indRight), sizeof(int));
            nodes[i]->Init(bit, key, value, nodes[indLeft], nodes[indRight]);
            delete[] key;
        }

        delete[] nodes;

        return;
    }
};


#endif //LAB_2_3_TPATRICIA_HPP
