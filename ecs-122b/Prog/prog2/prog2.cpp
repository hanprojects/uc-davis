#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <vector>

// ******************* //
//      PROBLEM 1      //
// ******************* //

unsigned findOptSubsetSum(unsigned W, const std::vector<unsigned>& weights)
{
    // Get weight size & table
    int N = weights.size();
    unsigned table[W+1][N+1];
    
    // Initialize first row & column to 0
    for (int w = 0; w <= W; w++)
        table[w][0] = 0;
    for (int n = 0; n <= N; n++)
        table[0][n] = 0;
    
    // For every other item, if w[i] <= w, then OPT(i,w) = max(w[i] + OPT(i-1, w-w[i]), OPT(i-1, w)) - Reference mentioned in report.md
    for (int w = 1; w <= W; w++)
    {
        for (int n = 1; n <= N; n++)
        {
            if (weights[n-1] <= w)
                table[w][n] = std::max(weights[n-1] + table[w-weights[n-1]][n-1], table[w][n-1]);
            else
                table[w][n] = table[w][n-1];
        }
    }
    
    // Return the value of the last item in the table
    return table[W][N];
}

/*
int main ()
{
    std::vector <unsigned> weights{5, 9, 1, 3, 6};
    unsigned weight = findOptSubsetSum(8, weights);
    std::cout << weight;
}
*/


// ******************* //
//      PROBLEM 2      //
// ******************* //

std::vector<std::vector<unsigned>> findOptSubsetSum2(unsigned W, const std::vector<unsigned>& weights, unsigned x)
{
    // Get new vector weights with constraint x
    std::vector<unsigned> w_x;
    for (int i = 0; i < x; i++)
        w_x.push_back(weights[i]);
    
    // Create table with constraint number of row <= 3 and initialize first row & column to 0
    unsigned table[W+1][3];
    
    for (int w = 0; w <= W; w++)
        table[w][0] = 0;
    for (int n = 0; n <= 2; n++)
        table[0][n] = 0;
    
    // For every other item, do the same thing to Problem 1, but execute according to the constraint
    for (int n = 1; n < 3; n++)
    {
        for (int w = 1; w <= W; w++)
        {
            if (w_x[n-1] <= w)
                table[w][n] = std::max(w_x[n-1] + table[w-w_x[n-1]][n-1], table[w][n-1]);
            else
                table[w][n] = table[w][n-1];
        }
        
        // Erase the first item of w_x after calculating.
        w_x.erase(w_x.begin());
        
        // If w_x is empty, break the loop cuz it is finished
        if (w_x.empty())
            break;
        
        // Otherwise, set first row to the second row and restart the process
        else
        {
            for (int k = 0; k <= W; k++)
                table[k][0] = table[k][1];
            n--;
        }
    }
    
    // Push the two rows to two_rows and return
    std::vector<std::vector<unsigned>> two_rows;
    std::vector<unsigned> row1;
    std::vector<unsigned> row2;
    
    for (int i = 0; i <= W; i++)
        row1.push_back(table[i][0]);
    for (int j = 0; j <= W; j++)
        row2.push_back(table[j][1]);
    
    two_rows.push_back(row1);
    two_rows.push_back(row2);
    
    return two_rows;
}

/*
int main () {
    std::vector <unsigned > weights{5, 9, 1, 3, 6};
    printf("=== Example 1 ===\n");
    auto lastTwoRows = findOptSubsetSum2(8, weights, 3);
    for (const auto& row : lastTwoRows)
    {
        for (unsigned val : row)
            printf("%u ", val);
        std::cout << std::endl;
    }
}
*/


// ******************* //
//      PROBLEM 3      //
// ******************* //

// Declare helper functions
std::vector<std::vector<int>> powerSet(const std::vector<int>& weights);

// powerSet()
std::vector<std::vector<int>> powerSet(const std::vector<int>& weights)
{
    // If weight vector is empty, return empty
    if (weights.empty())
        return {{}};
    
    else
    {
        // Get subset of each recursion of power set by increasing the starting point
        std::vector<std::vector<int>> subset = powerSet(std::vector<int>(weights.begin() + 1, weights.end()));

        int weight = weights[0];
        const int n = subset.size();

        // For each item in current subset, push_back the item to the total power set, and push_back the weight to the last item of the power set
        for (int i = 0; i < n; i++)
        {
            const std::vector<int>& s = subset[i];
            subset.push_back(s);
            subset.back().push_back(weight);
        }

        return subset;
    }
}

// tuple findOptSubsetSumBF()
std::tuple<bool, std::vector<unsigned>> findOptSubsetSumBF(const std::vector<int>& weights)
{
    // Declare necessary variables
    const int target_sum = 0;
    int sum = 0;
    const int size = weights.size();
    bool found = false;
    std::vector<unsigned> index;
    std::vector<int> valid_set;
    std::vector<std::vector<int>> power_set;

    if (weights.empty())
        return std::tuple<bool, std::vector<unsigned>>{false, {}};

    // Get the total power set of weights from the helper function
    power_set = powerSet(weights);
    
    // Loop through each subitem of the power set, add up the sum. If sum == 0, set found = true and set valid set to the current item of power set. Otherwise, continue
    int n = power_set.size();
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < power_set[i].size(); j++)
        {
            sum += power_set[i][j];
        }

        if (sum == target_sum)
        {
            valid_set = power_set[i];
            sum = 0;
        }
        else
        {
            sum = 0;
            continue;
        }
        
    }

    if (!valid_set.empty())
        found = true;
        
    // Sort the valid set by ascending order
    sort(valid_set.begin(), valid_set.end());

    // Retrieve the matching index of item in valid set and weight
    for (unsigned x = 0; x < valid_set.size(); x++)
    {
        for (unsigned y = 0; y < size; y++)
        {
            if (valid_set[x] == weights[y])
                index.push_back(y);
        }
    }

    // Sort index by ascending order
    sort(index.begin(), index.end());

    // Return based on the boolean value
    if (found == true)
        return std::tuple<bool, std::vector<unsigned>>{found, index};
    else
        return std::tuple<bool, std::vector<unsigned>>{found, {}};
}

/*
int main ()
{
    std::vector<int> weights{-3, 8, -2, 5};
    auto output = findOptSubsetSumBF(weights);
    std::cout << "First: " << std::boolalpha << std::get<0>(output) << "\n";
    for (unsigned index : std::get<1>(output)) std :: cout << index << " ";
    std::cout << std::endl;
}
*/


// ******************* //
//      PROBLEM 4      //
// ******************* //

// Declare helper functions
std::vector<std::vector<unsigned>> powerSet2(const std::vector<unsigned>& weights);
std::vector<std::vector<unsigned>> combSet(const std::vector<unsigned>& weights);

// powerSet()
std::vector<std::vector<unsigned>> powerSet2(const std::vector<unsigned>& weights)
{
    // If weight vector is empty, return empty
    if (weights.empty())
        return std::vector<std::vector<unsigned>> (1, std::vector<unsigned>());
    
    else
    {
        // Get subset of each recursion of power set by increasing the starting point
        std::vector<std::vector<unsigned>> subset = powerSet2(std::vector<unsigned>(weights.begin() + 1, weights.end()));

        int weight = weights[0];
        const int n = subset.size();

        // For each item in current subset, push_back the item to the total power set, and push_back the weight to the last item of the power set
        for (int i = 0; i < n; i++)
        {
            const std::vector<unsigned>& s = subset[i];
            subset.push_back(s);
            subset.back().push_back(weight);
        }

        return subset;
    }
}

// combSet()
std::vector<std::vector<unsigned>> combSet(const std::vector<unsigned>& weights)
{
    std::vector<std::vector<unsigned>> ps = powerSet2(weights);
    std::vector<std::vector<unsigned>> comb;

    for (int i = 0; i < ps.size(); i++)
    {
        if (ps[i].size() == 2)
            comb.push_back(ps[i]);
    }

    return comb;
}

// findMaxIndSetBF()
std::vector<unsigned> findMaxIndSetBF(const std::string& graphData)
{
    std::ifstream file(graphData);
    
    // Read first line to get number of vertices
    std::string string_n;
    getline(file, string_n);
    int n = stoi(string_n); // convert to integer

    // Read the rest lines to get unique vertices & edges
    unsigned a, b;
    std::vector<unsigned> vertex_set;
    std::vector<std::vector<unsigned>> edge_set;
    
    while (file >> a >> b)
    {
        std::vector<unsigned> ab = {a,b};
        sort(ab.begin(), ab.end());
        edge_set.push_back(ab);
    }

    for (int i = 0; i < n; i++)
        vertex_set.push_back(i);
    
    // If there is no edge, return {}
    if (edge_set.empty())
        return {};

    // If there is one edge, return 1 vertex
    else if (edge_set.size() == 1)
        return {vertex_set[0]};
    
    // Main Implementation
    std::vector<std::vector<unsigned>> power_set = powerSet2(vertex_set);
    std::vector<std::vector<unsigned>> ps;

    // Remove subset of power set with size 1
    for (int i = 0; i < power_set.size(); i++)
    {
        if (power_set[i].size() > 1)
            ps.push_back(power_set[i]);
    }

    // Initialize vector to store independent sets
    std::vector<std::vector<unsigned>> ind;

    // Loop through the new power set and its combination of 2 for each subset. If the combination does not match any in the edge set, push the subset to the independent sets
    for (int i = 0; i < ps.size(); i++)
    {
        std::vector<std::vector<unsigned>> comb;
        
        if (ps[i].size() == 2)
        {
            if (std::find(edge_set.begin(), edge_set.end(), ps[i]) == edge_set.end())
                ind.push_back(ps[i]);
        }
        else if (ps[i].size() > 2)
        {
            comb = combSet(ps[i]);
            for (int j = 0; j < comb.size(); j++)
            {
                if (std::find(edge_set.begin(), edge_set.end(), comb[j]) == edge_set.end())
                {
                    if (j == comb.size() - 1)
                        ind.push_back(ps[i]);
                    else
                        continue;
                }
                else
                    break;
            }
        }
    }

    // Sort the indepedent set
    sort(ind.begin(), ind.end());
    std::vector<unsigned> curr_max;
    
    // Find the independent set with max size
    for (int i = 0; i < ind.size()-1; i++)
    {
        if (ind[i].size() < ind[i+1].size())
            curr_max = ind[i+1];
        else
            curr_max = ind[i];
    }

    // Sort the max independent set by ascending order
    sort(curr_max.begin(), curr_max.end());
    return curr_max;
}

/*
int main ()
{
    auto indices = findMaxIndSetBF("graph.txt");
    
    for (unsigned index : indices)
        std::cout << index << " ";
    
    std::cout << std::endl;
}
*/
