#pragma once
#include <vector>

namespace problem {
    using std::vector;
    extern auto first_missing_positive_in( vector<int> numbers ) -> int;

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
        static const vector<Test_case> the_tests =
        {
            {Test_case::Id(1), 2, {3, 4, -1, 1}},
            {Test_case::Id(2), 3, {1, 2, 0}},
            {Test_case::Id(3), 3, {1, 4, 5, 1, 2}},
            // ADD MORE TEST CASES IF DESIRED
        };
        return the_tests;
    }
}  // namespace problem
