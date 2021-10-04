#pragma once
// Given an array (e.g. a C++ `std::vector`) of integers, use linear time and constant
// space to find the first positive integer that’s not present. The array can contain
// duplicates and negative numbers, and the numbers can be in any arbitrary order. You
// can modify the input array in-place.

#include <vector>

namespace problem {
    using std::vector;
    extern auto first_missing_positive_in( vector<int> numbers ) -> int;    // YOUR CODE.

    struct Test_case
    {
        enum Id: int {};
        using Result    = int;
        using Data      = vector<int>;

        Id          id;
        Result      expected_result;
        Data        data;
        
        auto actual_result() const -> Result { return first_missing_positive_in( data ); } 
    };
    
    inline auto tests() -> const vector<Test_case>&
    {
        using Tc = Test_case;
        static const vector<Test_case> the_tests =
        {
            {Tc::Id( 1 ), Tc::Result( 2 ), { 3, 4, -1, 1 }},
            {Tc::Id( 2 ), Tc::Result( 3 ), { 1, 2, 0 }},
            {Tc::Id( 3 ), Tc::Result( 3 ), { 1, 4, 5, 1, 2 }},
            // ADD MORE TEST CASES IF DESIRED
        };
        return the_tests;
    }
}  // namespace problem
