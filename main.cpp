#include <iostream>
#include <fstream>
#include <cstring>

#include "TPatricia.hpp"
#include "TPatriciaNode.hpp"

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    
    std::ofstream fout;
    std::ifstream fin;

    char input[MAX_SIZE];
    TValue value;

    TPatricia *patricia;
    try
    {
        patricia = new TPatricia();
    }
    catch (const std::bad_alloc &e)
    {
        std::cout << "ERROR: fail for allocating\n";
        exit(0);
    }

    TPatriciaNode *node;

    while ((std::cin >> input))
    {
        if (!std::strcmp(input, "+"))
        {
            std::cin >> input;
            Lowercase(input);
            std::cin >> value;

            std::cout << (patricia->Insert(input, value) ? "OK" : "Exist");
            std::cout << '\n';
        }
        else if (!std::strcmp(input, "-"))
        {
            std::cin >> input;
            Lowercase(input);

            std::cout << (patricia->Delete(input) ? "OK" : "NoSuchWord");
            std::cout << '\n';
        }
        else if (!std::strcmp(input, "!"))
        {
            std::cin >> input;
            if (!std::strcmp(input, "Save"))
            {
                std::cin >> input;
                fout.open(input, std::ios::out | std::ios::binary | std::ios::trunc);
                if (!fout.is_open()) {
                    std::cout << "ERROR: can't create file\n";
                    continue;
                }

                patricia->Save(fout);
                std::cout << "OK\n";

                fout.close();
            }
            else if (!std::strcmp(input, "Load"))
            {
                std::cin >> input;
                fin.open(input, std::ios::in | std::ios::binary);
                if (!fin.is_open()) {
                    std::cout << "ERROR: can't open file\n";
                    continue;
                }

                delete patricia;
                patricia = new TPatricia();
                patricia->Load(fin);

                std::cout << "OK\n";

                fin.close();
            }
        }
        else
        {
            Lowercase(input);
            node = patricia->Search(input);
            if (!node)
                std::cout << "NoSuchWord";
            else
                std::cout << "OK: " << node->value;
            std::cout << '\n';
        }
    }

    delete patricia;

    return 0;
}
