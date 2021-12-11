//
// Created by James on 10/31/2021.
//

#include <stakepool.h>
#include <validator.h>
#include <stakeparams.h>
#include <consensus/amount.h>
#include <chain.h>
#include <cmath>
#include <crypto/sha256.h>
#include <script/script.h>
#include <boost/crc.hpp>
#include <float.h>
#include <vector>
#include <unordered_map>
#include <script/script.h>

StakePool::StakePool() {
    totalStake = 0;
    lastValidationTime = 0;
}

bool StakePool::addValidator(Validator* validator, int nHeight, std::vector<std::string>* rterror) {
    if (validatorExists(validator)) {
        rterror.push_back("Validator exists");
        return false;
    }

    validatorPool.push_back(validator);
    validatorPoolScript[validator->scriptPubKey.ToString()] = validator;
    return recalculateProbabilities(nHeight, rterror);
}

bool StakePool::removeValidator(Validator* validator, int nHeight, std::vector<std::string>* rterror) {
    if (validator->suspended) {
        rterror.push_back("Validator suspended");
        return false;
    }

    if (!validatorExists(validator)) {
        rterror.push_back("Validator does not exist");
        return false;
    }

    auto it = validatorPoolScript.find(validator->scriptPubKey.ToString());
    for (size_t i = 0; i < validatorPool.size(); i++) {
        if (it.second == validatorPool.at(i)) {
            validatorPool.erase(validatorPool.begin()+i);
            break;
        }
    }
    validatorPoolScript.erase(it);
    return recalculateProbabilities(nHeight, rterror);
}

bool StakePool::recalculateProbabilities(int nHeight, std::vector<std::string>* rterror) {
    totalStake = 0;
    for (Validator& v : validatorPool) {
        if (v.suspended) {
            int elapsedTime = nHeight - v.suspendedBlock;
            if (elapsedTime >= VALIDATOR_SUSPENSION_DURATION) {
                unsuspendValidator(v, nHeight, rterror);

            }
        }

        if (!v.suspended){
            v.adjustedStake(nHeight);
            totalStake += v.adjustedStake;
        }
    }

    for (Validator& v : validatorPool) {
        v.calculateProbability(totalStake);
    }
    sort(nHeight);
    return true;
}

Validator* StakePool::retrieveNextValidator(int nHeight, std::vector<std::string>* rterror) {
    recalculateProbabilities(nHeight, rterror);
    
    vData.empty();
    serialize();

    boost::crc_32_type hasher;
    hasher.process_bytes(vData.begin(), vData.begin() + vData.size());
    double result = abs((double) hasher.checksum());

    double selection = result / DBL_MAX;
    Validator* selectedValidator = nullptr;

    double probabilitiesSummed = 0.0;
    for (Validator& v : ValidatorPool) {
        probabilitiesSummed += v.probability
        if (selection <= probabilitiesSummed) {
            selectedValidator = &v;
            break;
        }
    }

    if (!v) {
        rterror.push_back("No validator found");
        return nullptr;
    }

    return v;
}

void StakePool::serialize() {
    for (Validator& v : ValidatorPool) {
        std::vector<unsigned char> s = ToByteVector(v->scriptPubKey);
        vData.insert(vData.back(), s.front(), s.back());
    }
}

void StakePool::sort(int nHeight) {
    std::sort(validatorPool.begin(), validatorPool.end(), std::reverse());
}

bool StakePool::validatorExists(Validator* v) {
    return validatorPoolScript.count(v->scriptPubKey.ToString()) == 1;
}

bool StakePool::suspendValidator(Validator *v, int nHeight, std::vector <std::string>* rterror) {
    if(!validatorExists(v)) {
        rterror.push_back("Validator does not exist");
        return false;
    }

    v->suspended = true;
    v->suspendedBlock = nHeight;
    v->adjustedStake = 0;
    recalculateProbabilities(nHeight, rterror);
}

bool StakePool::unsuspendValidator(Validator *v, int nHeight, std::vector <std::string>* rterror) {
    v->suspended = false;
    v->suspendedBlock = 0;
    v->lastBlockHeight = nHeight;
    return true;
}

bool StakePool::viable() {
    bool viable = false;
    
    for (Validator& v : ValidatorPool) {
        if (!v.suspended) {
            viable = true;
        }
    }
    
    return viable;
}