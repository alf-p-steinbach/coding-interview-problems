#pragma once
#include "utility.hpp"

#include <stdlib.h>     // EXIT_...

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace testing {
    using utility::to_string;

    using   std::setw,                              // <iomanip>
            std::cout, std::cerr, std::endl,        // <iostream>
            std::exception, std::runtime_error,     // <stdexcept>
            std::string, std::to_string,            // <string>
            std::remove_const_t,                    // type_traits
            std::vector;                            // <vector>

    template< class Test_case >
    inline auto run( const vector<Test_case>& tests )
        -> typename Test_case::Id
    {
        auto& out = cout;
        const auto w6 = setw( 6 );
        using X = runtime_error;

        using Id        = typename Test_case::Id;
        using Result    = remove_const_t<decltype( Test_case::expected_result )>;
        using Data      = remove_const_t<decltype( Test_case::data )>;
        using utility::to_string, std::to_string;

        bool    failure                     = false;
        Id      first_fail                  = {};
        Result  first_fail_expected_result  = {};
        Result  first_fail_actual_result    = {};
        out << w6 << "Id" << w6 << "E" << w6 << "A" << w6 << ""
            << "(where E is Expected and A is Actual)"
            << endl;
        for( const Test_case& test: tests ) {
            const Result result = test.actual_result();
            out << w6 << ("#" + to_string( test.id ))
                << w6 << test.expected_result
                << w6 << result
                << w6 << (result == test.expected_result? "ok" : "FAIL")
                << " " << to_string( test.data )
                << endl;
            if( first_fail == 0 and result != test.expected_result ) {
                failure = true;
                first_fail = test.id;
                first_fail_expected_result = test.expected_result;
                first_fail_actual_result = result;
            }
        }

        out << endl;
        if( !!first_fail ) {
            out << "!Test case #" << to_string( first_fail ) << " failed:"
                << " expected " << to_string( first_fail_expected_result )
                << " but got actual result " << to_string( first_fail_actual_result )
                << "." << endl;
        } else {
            out << "All tests completed successfully." << endl;
        }
        return first_fail;
    }

    template< class Test_case >
    inline auto main( const vector<Test_case>& tests )
        -> int
    {
        try {
            const typename Test_case::Id id_of_first_fail = run( tests );
            return -id_of_first_fail;
        } catch( const exception& x ) {
            cerr << "!" << x.what() << endl;
        }
        return EXIT_FAILURE;
    }
}  // namespace testing
