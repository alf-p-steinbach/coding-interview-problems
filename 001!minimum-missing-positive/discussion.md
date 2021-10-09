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

A first step towards O(*n*) is to remove the noise, namely the possible zeroes and negative numbers:

~~~cpp
const auto it_end = remove_if( numbers.begin(), numbers.end(), is_not_positive );
numbers.erase( it_end, numbers.end() );
~~~

It would be nice if also the noise of possible duplicates could be removed, but for the general case with numbers that can be larger than the array size, when one’s limited to O(1) extra storage eliminating duplicates requires sorting (e.g., `std::unique` operates on a sorted sequence), which is generally O(*n*×log *n*).

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
using   std::remove_if, std::swap,      // <algorithm>
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

The last of the tests here is included because it was the first I found where the above code fails:

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

Test case #7 fails because when the `swap` loop encounters `5` it swaps it with the `2`, thereby placing the `2` one position too far in the array, and proceeds further up the array without getting the chance to place the `2` correctly. This results in an array starting with `1 1`. And hence the erroneous conclusion that the first missing positive number is `2`.

---

One way to avoid that is to only swap numbers that should go earlier in the array. By this rule the `5` will not be swapped with the `2`, so the `2` will not be brought to a place where it's not subsequently considered. However, this means that for an arbitrary permutation of the numbers 1 through *n*, about *n*/2 of the array positions will not yield any swapping, and one may need more than *n*/2 swaps to get all the numbers home (so to speak)…

One way to get more swappings is, for any given array position keep on swapping until either the new number there belongs further on in the array, or is a duplicate of the one swapped out.

These ideas yield code that correctly sorts any permutation of 1 through *n*, in place, in O(*n*) time:

*“solution-1.via-placement.cpp”*
~~~cpp
#include "testing.hpp"
#include <algorithm>

using   std::remove_if, std::swap,      // <algorithm>
        std::string,                    // <string>
        std::vector;                    // <vector>

auto is_not_positive( const int x ) -> bool { return (x <= 0); };

auto problem::first_missing_positive_in( vector<int>& numbers )
    -> int
{
    const auto it_end = remove_if( numbers.begin(), numbers.end(), is_not_positive );
    numbers.erase( it_end, numbers.end() );
    
    // Place each value v at index v - 1, if that index is earlier in the array.
    for( int& v: numbers ) {
        const int i = static_cast<int>( &v - &numbers[0] );
        while( v - 1 < i ) {
            const int original_v = v;
            swap( v, numbers[v - 1] );
            if( v == original_v ) { break; }
        }
    }
    
    // If there is now an index i without value i + 1, well that's it.
    for( const int& v: numbers ) {
        const int i = int( &v - &numbers[0] );
        if( v != i + 1 ) {
            return i + 1;
        }
    }
    
    // Otherwise first missing is the number beyond the given positive numbers range.
    const int n =  static_cast<int>( numbers.size() );
    return n + 1;
}
~~~

***It’s far from obvious that this code is correct.***

But first, it succeeds for all the seven specific tests defined here:

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

Secondly, it succeeds in sorting all permutations of all sequences 1 through *n* for *n* in the range 0 through 10 (the last one has 10! = 3 628 800 permutations):

*“solution-1.via-placement.main-with-sorting-testing.cpp”*
~~~cpp
// A test of whether “solution-1.via-placement.cpp” sorts correctly.
//
#include "testing.hpp"
#include <algorithm>
#include <iostream>

using   std::is_sorted, std::next_permutation,  // <algorithm>
        std::cout, std::cerr, std::endl,        // <iostream>
        std::string,                            // <string>
        std::vector;                            // <vector>

#define IT_RANGE_OF( c )    c.begin(), c.end()

auto main() -> int
{
    using utility::to_string;

    for( int n = 0; n <= 10; ++n ) {
        vector<int> permutation;
        for( int i = 1; i <= n; ++i ) {
            permutation.push_back( i );
        }
        do {
            vector<int> data = permutation;
            const int missing = problem::first_missing_positive_in( data );
            if( not is_sorted( IT_RANGE_OF( data ) ) ) {
                cerr    << "!Failed to sort " << to_string( permutation )
                        << " of length " << n << ","
                        << " producing " << to_string( data ) << "." << endl;
                return EXIT_FAILURE;
            }
            if( missing != n + 1 ) {
                cerr    << "!Expected " << n + 1 << " but got " << missing
                        << " sequence " << to_string( permutation ) << "." << endl;
                return EXIT_FAILURE;
            }
        } while( next_permutation( IT_RANGE_OF( permutation ) ) );
    }
    cout << "Sorted all permutations of natural number sequences length 10 and lower." << endl;
    return EXIT_SUCCESS;
}
~~~

>~~~txt
>Sorted all permutations of natural number sequences length 10 and lower.
>~~~

This testing strongly *indicates* that the code is correct as a sorting algorithm for permutations of a sequence of 1 through *n*, and it indicates to some degree that it’s a correct solution to the problem of finding the first missing positive number.

asd
