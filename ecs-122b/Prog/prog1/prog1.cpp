#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "prog1.hpp"

// ******************* //
//      PROBLEM 1      //
// ******************* //

std::vector<unsigned> findMatches(const std::string& s, const std::string& target)
{
    // get length of s and target
    unsigned target_length = target.length();
    unsigned s_length = s.length();
    
    // use vector to apply push_back()
    std::vector<unsigned> index_arr;
    
    int i, j;

    // loop (avoid overload) through s and target
    for (i = 0; i < s_length - target_length + 1; i++)
    {
        for (j = 0; j < target_length; j++)
        {
            // if any char of s does not match with corresponding char of target, break
            if (s[i + j] != target[j])
                break;
            else
                continue;
        }
        
        // when matches to the end, push_back the index
        if (j == target_length)
        {
            index_arr.push_back(i);
        }
    }

    return index_arr;
}

// ******************* //
//      PROBLEM 2      //
// ******************* //

struct Interval
{
    int start, end, opt;
};

bool endTimeCompare(const Interval& e1, const Interval& e2);
int noOverlapping(std::vector<Interval>& arr, int size);
int computeOpt(std::vector<Interval> arr, int size);
// int solveWeightedIntervalScheduling(const std::string& filename);

// bool endTimeCompare()
bool endTimeCompare(const Interval& e1, const Interval& e2)
{
    if (e1.end < e2.end)
        return true;
    else
        return false;
}

// int noOverlapping()
// to find the closest interval with no overlapping by binary search
int noOverlapping(std::vector<Interval>& arr, int size)
{
    int low = 0;
    int high = size;
    
    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (arr[mid].end <= arr[size].start)
        {
            if (arr[mid + 1].end <= arr[size].start)
                low = mid + 1;
            else
                return mid;
        }

        else
            high = mid - 1;
    }

    return -1;
}

// int computeOpt()
int computeOpt(std::vector<Interval> arr, int size)
{
    // invalid size, return -1
    if (size < 0)
        return 0; // fixed to 0 for gradescope

    // if arr has 1 interval, return it
    if (size == 0)
        return arr[size].opt;

    // get the non-overlapping closest interval to the current interval
    int closest_interval = noOverlapping(arr, size);

    // compute opt1 & opt2 with or without current opt
    int opt1 = arr[size].opt + computeOpt(arr, closest_interval);
    int opt2 = computeOpt(arr, size - 1);

    return std::max(opt1, opt2);
}

// int solveWeightedIntervalScheduling()
int solveWeightedIntervalScheduling(const std::string& filename)
{
    // use vector to apply push_back()
    std::vector<Interval> arr;
    int a, b, c; // hold values for start, end, opt
    
    std::ifstream file(filename);

    // read file and push_back
    while (file >> a >> b >> c)
    {
        arr.push_back({a, b, c});
    }
    
    // sort by finish time
    std::sort(arr.begin(), arr.end(), endTimeCompare);
    
    int size = sizeof(arr);

    return computeOpt(arr, size-1); 
}
