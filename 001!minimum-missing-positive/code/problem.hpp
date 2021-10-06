#pragma once
// Given an array (e.g. a C++ `std::vector`) of integers, use linear time and constant
// space to find the first positive integer that’s not present. The array can contain
// duplicates and negative numbers, and the numbers can be in any arbitrary order. You
// can modify the input array in-place.

#include "../../common-code/utility.hpp"    // temp_ref, copy_of
#include <vector>

namespace problem {
    using std::vector;
    using utility::copy_of, utility::temp_ref_to;

    using Result    = int;
    using Data      = vector<int>;

    extern auto first_missing_positive_in( Data& numbers ) -> Result;   // YOUR CODE.

    struct Test_case
    {
        enum Id: int {};

        const Id        id;
        const Result    expected_result;
        const Data      data;
        
        auto actual_result() const
            -> Result
        { return first_missing_positive_in( temp_ref_to( copy_of( data ) ) ); } 
    };

    inline auto tests() -> const vector<Test_case>&
    {
        static const vector<Test_case> the_tests =
        {
            { Test_case::Id( 1 ), Result( 1 ), Data{} },
            { Test_case::Id( 2 ), Result( 1 ), Data{ 0, -1, 0, -2, -3 } },
            { Test_case::Id( 3 ), Result( 2 ), Data{ 1 } },
            { Test_case::Id( 4 ), Result( 6 ), Data{ 1, 2, 3, 4, 5 } },
            { Test_case::Id( 5 ), Result( 2 ), Data{ 3, 4, -1, 1 } },
            { Test_case::Id( 6 ), Result( 3 ), Data{ 1, 2, 0 } },
            { Test_case::Id( 7 ), Result( 3 ), Data{ 1, 4, 5, 1, 2 } },
            // ADD MORE TEST CASES IF DESIRED
        };
        return the_tests;
    }
}  // namespace problem
