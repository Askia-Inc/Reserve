//
// Created by James on 10/31/2021.
//

#ifndef RESERVE_VALIDATOR_H
#define RESERVE_VALIDATOR_H

#include <vector>

using namespace std;

class Validator
{
public:
    void addStake(CAmount add);
    void calculateProbability(CAmount totalStake);
    void adjustStake(int nHeight, ValidatorParams* validatorParams);
    Validator();

public:
    std::vector<unsigned char> valtype pubkey;
    CAmount originalStake;
    CAmount adjustedStake;
    CService address;
    int lastBlockHeight;
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
        return (pubkey==val.pubkey);
    }

};
#endif //RESERVE_VALIDATOR_H
