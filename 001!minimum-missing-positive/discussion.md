# 1. First missing positive.

Based on a [posting by Yogendra Singh](https://www.facebook.com/groups/cppInPracticeQuestions/posts/4576501255704162/) in the [“C++ in-practice questions (most anything!)” Facebook group](https://www.facebook.com/groups/cppInPracticeQuestions), which was a repost of an interview question posting on the [“codeepie” Facebook page](https://www.facebook.com/codeepie), where it was attributed to [the Stripe company](https://stripe.com/en-no).

But apparently this problem was originally [a Leetcode problem](https://leetcode.com/problems/first-missing-positive/), there with slightly different wording.

The wording below is my attempt at avoiding the interpretation problems noted in “C++ in-practice questions (most anything!)”.


## Problem.

***Given an array (e.g. a C++ `std::vector`) of integers, use linear time and constant extra space to find the first positive integer that’s not present. The array can contain duplicates, zeroes and negative numbers, and the numbers can be in any arbitrary order. You can modify the input array in-place.***

## C++ framework for solution and testing.

You can use the following template for testing a C++ solution as a file in the “code” folder:

*“your-solution.cpp”*
~~~cpp
#include "testing.hpp"
using std::vector;      // Maybe more.

auto problem::first_missing_positive_in( vector<int> numbers )
    -> int
{
    // YOUR SOLUTION CODE HERE.
}

auto main() -> int { return testing::main( problem::tests() ); }
~~~

The testing checks and reports the results for the following array contents:

| Test ID: | Numbers in array:|Correct result:|
|---------:|----------------:|--------------:|
|       #1 |3  4  −1  1      |             2 |
|       #2 | 1  2  0        |             3 |
|       #3 | 1  4  5  1  2  |             3 |


## Discussion and a solution.

Except for the linear time requirement a solution would be simple: sort the array, then scan the array and count up for each positive number that’s not a duplicate of the previous one. A duplicate is equal to the current count (before counting up), so those numbers should be ignored. When the count after counting up, isn't the same as the number in the array, that's the first positive number missing.

Expressed in C++:

*“a not-quite solution with n×log(n) time via sorting.cpp“*
~~~cpp
// Note: due to the sorting this code is O(n log n), whereas the problem requires O(n).
//
#include "testing.hpp"
#include <algorithm>
using std::vector, std::sort;

auto problem::first_missing_positive_in( vector<int> numbers )
    -> int
{
    sort( numbers.begin(), numbers.end() );
    int x = 0;
    for( const int v: numbers ) {
        if( v <= 0 or v == x ) { continue; }
        ++x;
        if( v > x ) { return x; }
    }
    return x + 1;
}

auto main() -> int { return testing::main( problem::tests() ); }
~~~

It’s difficult and time-consuming to test the big-O behavior so the simple test framework provided here, reports success:

>~~~txt
>     Id     E     A      (where E is Expected and A is Actual)
>     #1     2     2    ok {3, 4, -1, 1}
>     #2     3     3    ok {1, 2, 0}
>     #3     3     3    ok {1, 4, 5, 1, 2}
> 
> All tests completed successfully.
>~~~

O(*n*×log *n*) time won’t do for a solution. However, the above is sufficiently simple to believe that it produces the correct answer for any arbitrary array contents, whatever you throw at it. And so this code can be used as *a reference* for testing other solution attempts.

Also, the problem can now be reframed in terms of this code, namely to provide the exact same effect as this code in all cases, but in O(*n*) time with only O(1) extra storage. Which clarifies what the real problem is. O(*n*) time.

---

asd
