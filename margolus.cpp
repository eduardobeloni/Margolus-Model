
#include "margolus.h"

#include <cstdlib>
#include <string>


Margolus::Margolus(RuleType init_rule)
{
    this->set_curr_rule(init_rule);
}


int Margolus::block_cmp(short *neigh[2][2]) const
{
    int block_index = 0;

    if (*neigh[0][0])
        block_index |= 1;
    if (*neigh[0][1])
        block_index |= 2;
    if (*neigh[1][0])
        block_index |= 4;
    if (*neigh[1][1])
        block_index |= 8;

// the index (decimal configuration) of new block which neigh must became
    return block_index;
}


void Margolus::update_block(short *neigh[2][2]) const
{
    int new_block = curr_rule[block_cmp(neigh)];

    for (int w = 0; w < 2; w++)
        for (int i = 0; i < 2; i++)
        {
            if (new_block > 0)
            {
                *neigh[w][i] = new_block % 2;
                new_block /= 2;
            }
            else
                *neigh[w][i] = 0;
        }
}


void Margolus::set_curr_rule(RuleType new_rule)
{
    std::string str_rule;

    switch (new_rule)
    {
    case BBM:
        str_rule = "0;8;4;3;2;5;9;7;1;6;10;11;12;13;14;15;";
        break;
    case BOUNCE_GAS:
        str_rule = "0;8;4;3;2;5;9;14;1;6;10;13;12;11;7;15;";
        break;
    case CRITTERS:
        str_rule = "15;14;13;3;11;5;6;1;7;9;10;2;12;4;8;0;";
        break;
    case HPP_GAS:
        str_rule = "0;8;4;12;2;10;9;14;1;6;5;13;3;11;7;15;";
        break;
    case ROTATIONS:
        str_rule = "0;2;8;12;1;10;9;11;4;6;5;14;3;7;13;15;";
        break;
    case SAND:
        str_rule = "0;4;8;12;4;12;12;13;8;12;12;14;12;13;14;15;";
        break;
    case STRING_THING:
        str_rule = "0;1;2;12;4;10;9;7;8;6;5;11;3;13;14;15;";
        break;
    case SWAP_ON_DIAG:
        str_rule = "0;8;4;12;2;10;6;14;1;9;5;13;3;11;7;15;";
        break;
    case TRON:
        str_rule = "15;1;2;3;4;5;6;7;8;9;10;11;12;13;14;0;";
        break;
    }

    std::string tmp = "";
    int i = 0;

    for (unsigned int w = 0; w < str_rule.length(); w++)
    {
        tmp += str_rule[w];

        if (str_rule[w] == ';')
        {
            curr_rule[i] = atoi(tmp.c_str());
            i++;
            tmp = "";
        }
    }
}
