#include <iostream>
#include <string>
#include <vector>

// Google Unit Testing Framework
#include <gtest/gtest.h>

// Declare functions to test
std::vector<unsigned> findMatches(const std::string& s, const std::string& target);
int solveWeightedIntervalScheduling(const std::string& filename);




// PROBLEM 1 TEST CASES //

TEST(Prob1Test, case0)
{
    const std::string s = "aabbcdeebbcbcdbcde";
    const std::string target = "bcd";
    
    EXPECT_EQ(findMatches(s, target), {3,11,14}) << "Test case failed.";
}

TEST(Prob1Test, case1)
{
    const std::string s = "aaaaaaa";
    const std::string target = "a";

    EXPECT_EQ(findMatches(s, target), {0,1,2,3,4,5,6}) << "Test case failed.";
}

TEST(Prob1Test, case2)
{
    const std::string s = "abcdefg";
    const std::string target = "f";
    
    EXPECT_EQ(findMatches(s, target), {5}) << "Test case failed.";
}

TEST(Prob1Test, case3)
{
    const std::string s = "hellohowyoudoing";
    const std::string target = "a";
    
    EXPECT_EQ(findMatches(s, target), {}) << "Test case failed.";
}

TEST(Prob1Test, case4)
{
    const std::string s = "iloveyouminshan";
    const std::string target = "minshan";
    
    EXPECT_EQ(findMatches(s, target), {8}) << "Test case failed.";
}




// PROBLEM 2 TEST CASES //

TEST(Prob2Test, test0)
{
    const std::string filename = "test.txt";
    EXPECT_EQ(solveWeightedIntervalScheduling(filename), 8) << "Test case failed.";
}

TEST(Prob2Test, test1)
{
    const std::string filename = "test1.txt";
    EXPECT_EQ(solveWeightedIntervalScheduling(filename), 28) << "Test case failed.";
}

TEST(Prob2Test, test2)
{
    const std::string filename = "test2.txt";
    EXPECT_EQ(solveWeightedIntervalScheduling(filename), 43) << "Test case failed.";
}

TEST(Prob2Test, test3)
{
    const string filename = "test3.txt";
    EXPECT_EQ(solveWeightedIntervalScheduling(filename), 59) << "Test case failed.";
}

TEST(Prob2Test, test4)
{
    const std::string filename = "test4.txt";
    EXPECT_EQ(solveWeightedIntervalScheduling(filename), 71) << "Test case failed.";
}





int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
