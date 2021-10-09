# IQ 1: First missing positive.

Based on a [posting by Yogendra Singh](https://www.facebook.com/groups/cppInPracticeQuestions/posts/4576501255704162/) in the [“C++ in-practice questions (most anything!)” Facebook group](https://www.facebook.com/groups/cppInPracticeQuestions), which was a repost of an interview question posting on the [“codeepie” Facebook page](https://www.facebook.com/codeepie), where it was attributed to [the Stripe company](https://stripe.com/en-no).

But apparently this problem was originally [a Leetcode problem](https://leetcode.com/problems/first-missing-positive/), there with slightly different wording.

The wording below is my attempt at avoiding the interpretation problems noted in the “C++ in-practice questions (most anything!)” group.


## The problem.

***Given an array (e.g. a C++ `std::vector`) of integers, use linear time and constant extra space to find the first positive integer that’s not present. The array can contain duplicates, zeroes and negative numbers, and the numbers can be in any arbitrary order. You can modify the input array in-place.***

## A C++ framework for solution and testing.

You can use the following template for testing a C++ solution as a file in the “code” folder:

*“your-solution.cpp”*
~~~cpp
#include "testing.hpp"
using std::vector;

auto problem::first_missing_positive_in( vector<int>& numbers )
    -> int
{ /* YOUR SOLUTION CODE HERE. */ }

auto main() -> int { return testing::main( problem::tests() ); }
~~~

Or you can omit the definition of `main` and just compile and link in “main-for-testing.cpp”, which provides exactly the above `main` function.

The testing checks and reports the results for the following array contents:

| Test ID: | Correct result:| Numbers in array:|
|---------:|---------------:|:-----------------|
|       #1 |              1 | *none*           |
|       #2 |              1 | 0  −1  0  −2  −3 |
|       #3 |              2 | 1                |
|       #4 |              6 | 1  2  3  4  5    |
|       #5 |              2 | 3  4  −1  1      |
|       #6 |              3 | 1  2  0          |
|       #7 |              3 | 1  4  5  1  2    |


## Discussion with a “not the intended” solution.

Except for the linear time requirement a solution would be simple: sort the array, then scan the array and count up for each positive number that’s not a duplicate of the previous one. A duplicate is equal to the current count (before counting up), so those numbers should be ignored. When the count after counting up, isn't the same as the number in the array, that's the first positive number missing.

Expressed in C++:

*“a-not-quite-solution-with-n×log(n)-time-via-sorting.cpp“*
~~~cpp
// Due to the sorting this code is O(n log n), whereas the problem requires O(n).
//
#include "testing.hpp"
#include <algorithm>
using std::vector, std::sort;

auto problem::first_missing_positive_in( vector<int>& numbers )
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
~~~

The simple test framework provided here doesn’t test the big-O behavior, so it reports success:

>~~~txt
>    Id     E     A      (where E is Expected and A is Actual)
>    #1     1     1    ok {}
>    #2     1     1    ok {0, -1, 0, -2, -3}
>    #3     2     2    ok {1}
>    #4     6     6    ok {1, 2, 3, 4, 5}
>    #5     2     2    ok {3, 4, -1, 1}
>    #6     3     3    ok {1, 2, 0}
>    #7     3     3    ok {1, 4, 5, 1, 2}
>
>All tests completed successfully.
>~~~

O(*n*×log *n*) time won’t do for a solution. But the above code is sufficiently simple to believe that it produces the correct answer for any arbitrary array contents, whatever data you throw at it. And so this code ***can be used as a reference*** for testing and gaining confidence in other solution attempts, for example for testing with longer sequences of dynamically generated random numbers.

This is a general technique for big O challenges.

---

A first step towards O(*n*) is to remove the noise, namely the possible negative numbers:

~~~cpp
const auto it_end = remove_if( numbers.begin(), numbers.end(), is_not_positive );
numbers.erase( it_end, numbers.end() );
~~~

It would be nice if also the noise of possible duplicates could be removed, but when one’s limited to O(1) extra storage that requires sorting (e.g., `std::unique` operates on a sorted sequence), which is generally O(*n*×log *n*).

But wait! Sorting can be faster the more one knows about the data. For example, if one knows that the data is already sorted, then sorting is an O(1) operation. And except for the O(1) requirement on extra storage, the integers or more generally records with keys 1 through *n*, in some arbitrary order, can be sorted in O(*n*) time by simply placing each value *v* at index *v* in a 1 based array. Or in C++, at index *v*−1 in a 0 based array.

Can that also be done in place in the original data array?

One idea for that is to scan through the array and just `swap` each encountered number into its proper place in the array; using `swap` instead of plain assignment to avoid erasing any number. And then, if that results in the numbers 1 through *n* in sequence at the start of the array, then the first missing positive number is *n*+1. And otherwise the first positive missing is the first that’s missing within or before the sequence.

***This idea is subtly incorrect***, but it’s simple and it “works” for the examples in the original question (test cases #5 and #6 here), so I posted code equivalent to the following in the belief that it had to be correct:

*“an-incorrect-solution-via-placement.cpp”*
~~~cpp
// Note: this attempted solution fails e.g. for input (1 4 5 1 2).
//
#include "testing.hpp"
#include <algorithm>
using   std::swap, std::remove_if,      // <algorithm>
        std::vector,                    // <vector>
        std::string;                    // <string>

auto is_not_positive( const int x ) -> bool { return (x <= 0); };

auto problem::first_missing_positive_in( vector<int>& numbers )
    -> int
{
    const auto it_end = remove_if( numbers.begin(), numbers.end(), is_not_positive );
    numbers.erase( it_end, numbers.end() );

    // Place each value v at index v - 1.
    const int n = static_cast<int>( numbers.size() );
    for( int& v: numbers ) {
        if( v <= n ) {
            swap( v, numbers[v - 1] );
        }
    }
    
    // If there is now an index i without value i + 1, well (one believes that) that's it.
    for( const int& v: numbers ) {
        const int i = static_cast<int>( &v - &numbers[0] );
        if( v != i + 1 ) {
            return i + 1;
        }
    }
    
    // Otherwise first missing is the number beyond the given positive numbers range.
    return n + 1;
}
~~~

The last of the tests here is was the first I found where the above code fails:

>~~~txt
>    Id     E     A      (where E is Expected and A is Actual)
>    #1     1     1    ok {}
>    #2     1     1    ok {0, -1, 0, -2, -3}
>    #3     2     2    ok {1}
>    #4     6     6    ok {1, 2, 3, 4, 5}
>    #5     2     2    ok {3, 4, -1, 1}
>    #6     3     3    ok {1, 2, 0}
>    #7     3     2  FAIL {1, 4, 5, 1, 2}
>
>!Test case #7 failed: expected 3 but got actual result 2.
>~~~

Test case #7 fails because when the `swap` loop encounters `5` it swaps it with the `2`, thereby placing the `2` one position too far in the array, and proceeds further up the array without getting the chance to place the `2` correctly. This results in an array starting with `1 1`. And erroneous conclusion that the first missing is `2`.

---

