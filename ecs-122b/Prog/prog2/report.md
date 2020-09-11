# ECS 122B - Programming #2 Report

## I. Identification
- Student's name: Han Nguyen
- Student's ID: #917278789

The author has no collaboration for this assignment. Helpful references are mentioned in ***III. References***.

## II. Explanation for Problems and Logic

### 1. Problem 1
The problem is implemented by using $(W+1)-$by$-(n+1)$ dynamic programming table, in which $W+1$ is the number of rows that is enumerated by the constraint of weight bound starting from 0, and $n+1$ is the number of columns enumerated by the number of elements in `weights` starting from 0.

The problem takes $O(nW)$ running time.

### 2. Problem 2
Similar to the problem 1, the problem is implemented by using $(W+1)-$by$-3$ dynamic programming table, according to the constraint requirement of the assignment. The logic is to calculate the second row of the table from the first row, and change the value of first row according with the second row, and restart the process by `n--` to continue the calculation for the second row. After each calculation, the calculated item in `w_x` will be removed to avoid recalculation, until it is empty to break the loop. By doing this, the table will always be updated with the latest two rows. 

After the latest two rows are yielded, they are pushed to the vector `two_rows` to be return. The auxiliary space for this Problem 2 is $\Theta(W)$.

### 3. Problem 3
The problem is implemented by two functions: the required function `findOptSubsetSumBF()` and the helper function `powerSet()`. The use of `powerSet()` is to create all possible subsets of a vector; in this case, it is applied to create all possible subsets of `weights`. This approach is inspired by the `brute_force.pdf` lecture slide.

Once the power set is yielded, the logic loops through each subset of the power set, and calculate its sum. If the sum is 0, the set will be stored in a variable called `valid_set`. After the loop, if `valid_set` is not empty, the boolean `found` will be set to `true`. After that, by using a simple `for` loop, the program matches the items in `valid_set` with the items in `weights` by indices that will be pushed back in a vector `index`, which is returned in a tuple with boolean `found` for `main()` function.

The problem takes $O(2^n)$ time as it loops through every subset of the power set of the weights.

### 4. Problem 4
The problem is implemented by the required `findMaxIndSetBF`. The approach is inspired by brute force algorithm, `brute_force.pdf` in terms of power set and combination. For every subset in the power set of vertices, the subset of length 1 will be removed, and with the other subsets, the combinations with length 2 between the items of each subset will be generated to check if any of them are in `edge_set`. If there is none, it will be regarded as an independent set, and will compare the size of other independent sets to determine the maximal independent set. 

The problem takes $O(n^2 2^n)$, where $n^2$ is to loop through the combination of a subset of power set of vertices and $2^n$ is to loop through the power set of vertices. 

## III. References
- https://www.cs.cmu.edu/~ckingsf/class/02713-s13/lectures/lec15-subsetsum.pdf (for Problem 1; however, the author implements in $(W+1)-by-(n+1)$ table)
- ECS 122B lecture slide `brute_force.pdf` (for Problem 3 & 4)
