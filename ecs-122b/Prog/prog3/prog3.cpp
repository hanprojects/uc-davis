#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "prog3.hpp"


// **************** //
//    PROBLEM 1     //
// **************** //

bool nextPermutation(std::vector<unsigned>& vals)
{
    // Get size of vals and get k at the rightmost index of vals
    unsigned n = vals.size();
    unsigned k = n - 1;

    // In the case that all vals[k-1] >= vals[k], we are at last of the permutation and hence return false
    while (vals[k-1] >= vals[k])
    {
        k -= 1;
        if (k == 0)
            return false;
        else
            continue;
    }

    // Set r at the rightmost index of vals
    unsigned r = n - 1;

    // Find r such that r > k and vals[r] > vals[k]. If not found, decrease r and continue
    while ((r > k) && (vals[r] <= vals[k-1]))
    {
        r -= 1;
        continue;
    }

    // If found, swap vals[k] and vals[r] and reverse the set. Then return true
    std::swap(vals[k-1], vals[r]);
    std::reverse(vals.begin() + k, vals.end());

    return true;
}


// **************** //
//    PROBLEM 2     //
// **************** //

// Declare helper functions
std::vector<unsigned> index(const std::string& T, const std::string& P);
std::vector<std::array<unsigned, 2>> z_boxes(const std::string& T, const std::string& P);

// index()
std::vector<unsigned> index(const std::string& T, const std::string& P)
{
    // Declare necessary variables
    std::string concat = P + T;
    unsigned concat_length = concat.length();
    unsigned p_length = P.length();

    unsigned Z[concat_length];
    Z[0] = 0;

    unsigned r = 0;
    unsigned l = 0;

    // Compute Z[k] for 1 <= k < concat_length and update r and l (see slide #11)
    for (unsigned k = 1; k < concat_length; k++)
    {
        // Case k > r:
        if (k > r)
        {
            unsigned i = 0;

            // Comparing character of concat at 1 and k onwards until mismatch
            while (k + i < concat_length && concat[i] == concat[k+i])
                i++;
            
            // If Z[k] > 0, then r = k + Z[k] - 1, l = k
            Z[k] = i;
            
            if (Z[k] > 0)
            {
                r = k + Z[k] - 1;
                l = k;
            }
        }

        // Case k <= r: k is in a Z-box, so concat[k] is in alpha = concat[l..r]
        else if (k <= r)
        {
            // Beta = concat[k..r] = concat[k'..Z[l]]
            unsigned k_prime = k - l;
            unsigned beta_length = r - k + 1;

            // Case 1: Z[k'] < |Beta|
            if (Z[k_prime] < beta_length)
                Z[k] = Z[k_prime];
            
            // Case 2: Z[k'] >= |Beta|
            else if (Z[k_prime] >= beta_length)
            {
                unsigned q = r + 1;
                
                while (q < concat_length && concat[q] == concat[q-k])
                    q++;
                
                Z[k] = q - k;
                r = q -1;
                l = k;
            }
        }

        Z[k] = std::min(Z[k], p_length);
    }

    std::vector<unsigned> indices;
    for (unsigned n = 1; n < sizeof(Z); n++)
    {
        if (Z[n] == p_length)
            indices.push_back(n - p_length);
    }

    return indices;
}

// z_boxes()
std::vector<std::array<unsigned, 2>> z_boxes(const std::string& T, const std::string& P)
{
    // Declare necessary variables
    std::string concat = P + "$" + T;
    const unsigned concat_length = concat.length();
    unsigned p_length;

    unsigned Z[concat_length];
    Z[0] = 0;

    unsigned r = 0;
    unsigned l = 0;

    // Compute Z[k] for 1 <= k < concat_length and update r and l (see slide #11)
    for (unsigned k = 1; k < concat_length; k++)
    {
        // Case k > r:
        if (k > r)
        {
            unsigned i = 0;

            // Comparing character of concat at 1 and k onwards until mismatch
            while (k + i < concat_length && concat[i] == concat[k+i])
                i++;
            
            // If Z[k] > 0, then r = k + Z[k] - 1, l = k
            Z[k] = i;
            
            if (Z[k] > 0)
            {
                r = k + Z[k] - 1;
                l = k;
            }
        }

        // Case k <= r: k is in a Z-box, so concat[k] is in alpha = concat[l..r]
        else if (k <= r)
        {
            // Beta = concat[k..r] = concat[k'..Z[l]]
            unsigned k_prime = k - l;
            unsigned beta_length = r - k + 1;

            // Case 1: Z[k'] < |Beta|
            if (Z[k_prime] < beta_length)
                Z[k] = Z[k_prime];
            
            // Case 2: Z[k'] >= |Beta|
            else if (Z[k_prime] >= beta_length)
            {
                unsigned q = r + 1;
                
                while (q < concat_length && concat[q] == concat[q-k])
                    q++;
                
                Z[k] = q - k;
                r = q -1;
                l = k;
            }
        }
        
        Z[k] = std::min(Z[k], p_length);
    }

    std::vector<std::array<unsigned, 2>> zboxes;

    // Add Z-boxes to `zboxes`
    for (unsigned n = 1; n < sizeof(Z); n++)
    {
        if (Z[n] != 0)
            zboxes.push_back({n, n + Z[n] - 1});
    }

    return zboxes;
}

// Main Return Function
std::tuple<std::vector<std::array<unsigned, 2>>, std::vector<unsigned>> findZBoxesAndMatches(const std::string& T, const std::string& P)
{
    // Declare tuple variables
    std::vector<std::array<unsigned, 2>> zboxes;
    std::vector<unsigned> indices;

    indices = index(T, P);
    zboxes = z_boxes(T, P);

    return std::tuple<std::vector<std::array<unsigned, 2>>, std::vector<unsigned>>{zboxes, indices};
}


// **************** //
//    PROBLEM 3     //
// **************** //

std::tuple<std::vector<std::string>,float> findMostProbablePath(const HMM& hmm, const std::vector<std::string>& obs)
{
    return std::tuple<std::vector<std::string>,float> {{{}}, 0.0};
}
