#ifndef PROG3_HPP
#define PROG3_HPP

#include <tuple>
#include <vector>

bool nextPermutation(std::vector<unsigned>& vals);

std::tuple<std::vector<std::array<unsigned, 2>>, std::vector<unsigned>>
    findZBoxesAndMatches(const std::string& T, const std::string& P);

// You can add methods to this class if you want. The only thing you cannot
// change is the order and number of the member variables. (You can see why if
// you look at how instances of HMM are initialized in the example code.)
struct HMM
{
    std::vector<std::string> states;
    std::vector<float> initDist;
    std::vector<std::vector<float>> transProbs;  // N-by-N transition probabilities (A)
    std::vector<std::string> possibleObs;  // W possible observations
    std::vector<std::vector<float>> emisProbs;  // N-by-W emission probabilities (B)
};

std::tuple<std::vector<std::string>,float> findMostProbablePath(
    const HMM& hmm,
    const std::vector<std::string>& obs);
 
#endif // PROG3_HPP
