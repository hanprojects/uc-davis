# ECS 122B - Programming #3 Report

## I. Identification
- Student's name: Han Nguyen
- Student's ID: #917278789

The author has no collaboration for this assignment. The turned-in programming assignment #3 is truly her honest work and efforts with references mentioned in ***III. References***.

## II. Explanation for Problems and Logic

### 1. Problem 1
The logic for solving Problem #1 is inspired by the lecture slide `brute_force.pdf`. First, we get `n` as the size of vector `vals`, and find `k` as the rightmost index where `vals[k] < vals[k+1]` within a `while` loop decrementing `k` where the opposite occurs. Then, we find the rightmost `r` where `r > k` such that `vals[r] > vals[k]` within a `while` loop decrementing `r` until such `r` is found. 

Once found, we swap `vals[r]` and `vals[k]` by using `std::swap`, and reverse the vector by using `std::reverse`, and finally return `true`.

### 2. Problem 2
Based on the test case, the author observes that to compute Z-boxes, `P$T` is required, and to compute matching indices, only `PT` is required. Hence, she creates two similar functions `std::std::vector<std::array<unsigned, 2>> z_boxes` and `std::vector<unsigned> index` with concatenation of `P$T` and `PT`, respectively. The logic for each function is inspired by the lecture slide `string_matching.pdf` to compute the array of `Z` and explore Z-boxes or matching indices accordingly. 

The runtime is $O(|P| + |T|)$. The author also refers to outside academic sources that have been mentioned below in ***III. References***.

### 3. Problem 3
The author has run out of time for this problem and truly understands that it might give her a zero to this problem.

## III. References
- `brute_force.pdf` (for Problem 1)
- `string_matching.pdf` (for Problem 2, for Z-algorithm)
- https://contest.cs.cmu.edu/295/tutorials/z-string-matching.pdf (for Problem 2, to compute Z-boxes)