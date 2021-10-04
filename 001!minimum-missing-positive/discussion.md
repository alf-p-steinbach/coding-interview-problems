# 1. First missing positive.

Based on a [posting by Yogendra Singh](https://www.facebook.com/groups/cppInPracticeQuestions/posts/4576501255704162/) in the [“C++ in-practice questions (most anything!)” Facebook group](https://www.facebook.com/groups/cppInPracticeQuestions), which was a repost of an interview question posting on the [“codeepie” Facebook page](https://www.facebook.com/codeepie), where it was attributed to [the Stripe company](https://stripe.com/en-no).

But apparently this problem was originally [a Leetcode problem](https://leetcode.com/problems/first-missing-positive/), there with slightly different wording.

The wording below is my attempt at avoiding the interpretation problems noted in “C++ in-practice questions (most anything!)”.


## Problem.

***Given an array (e.g. a C++ `std::vector`) of integers, use linear time and constant space to find the first positive integer that’s not present. The array can contain duplicates and negative numbers, and the numbers can be in any arbitrary order. You can modify the input array in-place.***

## C++ framework for solution and testing.

You can use the following template for testing a C++ solution as a file in the “code” folder:

*your-solution.cpp*
~~~cpp
#include "testing.hpp"
#include <vector>
using std::vector;      // Maybe more.

auto problem::first_missing_positive_in( vector<int> numbers )
    -> int
{
    // YOUR SOLUTION CODE HERE.
}

auto main() -> int { return testing::main( problem::tests() ); }
~~~


## Discussion.


