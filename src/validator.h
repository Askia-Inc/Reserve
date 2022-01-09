//
// Created by James on 10/31/2021.
//

#ifndef RESERVE_VALIDATOR_H
#define RESERVE_VALIDATOR_H

#include <vector>
#include <consensus/amount.h>
#include <script/script.h>

class Validator
{
public:
    void addStake(CAmount add);
    void calculateProbability(CAmount totalStake);
    void adjustStake(int nHeight);
    Validator();

public:
    CScript scriptPubKey;
    CAmount originalStake;
    CAmount adjustedStake;
    int lastBlockHeight;
    uint32_t lastBlockTime;
    double probability;
    bool suspended;
    int suspendedBlock;

    bool operator < (const Validator& val) const
    {
        return (probability < val.probability);
    }

    bool operator > (const Validator& val) const
    {
        return (probability > val.probability);
    }

    bool operator == (const Validator& val) const
    {
        return (scriptPubKey==val.scriptPubKey);
    }
    
    bool IsNew () { return lastBlockHeight < 0; }

};
#endif //RESERVE_VALIDATOR_H
