//
// Created by James on 10/31/2021.
//
#include <validator.h>
#include <stakeparams.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <consensus/amount.h>
#include <string>
#include <script/script.h>

#ifndef RESERVE_POOL_H
#define RESERVE_POOL_H

class StakePool {
private:
    CAmount totalStake;
    std::vector<Validator> validatorPool;
    std::unordered_map<std::string , Validator> validatorPoolScript;
    std::vector<uint8_t> vData;
    uint32_t lastValidationTime;

    void sort(int nHeight);

public:
    StakePool() delete;
    StakePool();
    bool addValidator(Validator* v, int nHeight, std::vector<std::string>* rterror);
    bool removeValidator(Validator* v, int nHeight, std::vector<std::string>* rterror);
    bool recalculateProbabilities(int nHeight, std::vector<std::string>* rterror);
    Validator* retrieveNextValidator(int nHeight, std::vector<std::string>* rterror);
    void serialize();
    bool validatorExists(Validator* v);
    bool suspendValidator(Validator* v, int nHeight, std::vector<std::string>* rterror);
    bool unsuspendValidator(Validator* v, int nHeight, std::vector<std::string>* rterror);
    bool viable();
    uint32_t GetLastValidationTime() {return lastValidationTime;}
    void SetLastValidationTime(uint32_t t) {lastValidationTime = t;}
};
#endif //RESERVE_POOL_H
