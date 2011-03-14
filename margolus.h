
#ifndef MARGOLUS_H
#define MARGOLUS_H


class Margolus
{
private:
    short curr_rule[16];

    int block_cmp(short *neigh[2][2]) const;

public:
    enum RuleType
    {
        BBM, BOUNCE_GAS, CRITTERS,
        HPP_GAS, ROTATIONS, SAND,
        STRING_THING, SWAP_ON_DIAG, TRON
    };

    Margolus(RuleType init_rule);

    void update_block(short *neigh[2][2]) const;
    void set_curr_rule(RuleType new_rule);
};

#endif
