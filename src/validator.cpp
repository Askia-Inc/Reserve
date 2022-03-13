//
// Created by James on 10/31/2021.
//

#include <validator.h>

#include <consensus/amount.h>
#include <uint256.h>
#include <script/script.h>
#include <stakeparams.h>

#include <math.h>

Validator::Validator() {

    originalStake = 0;
    adjustedStake = 0;
    lastBlockHeight = -1;
    lastBlockTime = -1;
    probability = 0.0;
    suspended = false;
    suspendedBlock = 0;
}

void Validator::addStake(CAmount add) {
    this->originalStake += add;
}

void Validator::calculateProbability(CAmount totalStake) {
    probability = (adjustedStake * 1.0) / totalStake;
}

void Validator::adjustStake(int nHeight) {
    int elapsedBlocks = nHeight - lastBlockHeight;
    adjustedStake = originalStake + originalStake * pow((1 + STAKE_INTEREST_RATE), elapsedBlocks);
}


