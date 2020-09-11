#ifndef PROG2_HPP
#define PROG2_HPP

#include <tuple>
#include <vector>

unsigned findOptSubsetSum(unsigned W,
    const std::vector<unsigned>& weights);

std::vector<std::vector<unsigned>> findOptSubsetSum2(
        unsigned W,
        const std::vector<unsigned>& weights,
        unsigned x);

std::tuple<bool, std::vector<unsigned>> findOptSubsetSumBF(
    const std::vector<int>& weights);

std::vector<unsigned> findMaxIndSetBF(const std::string& graphData);

#endif  // PROG2_HPP
