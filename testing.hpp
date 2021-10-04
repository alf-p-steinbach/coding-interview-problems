#include <stdlib.h>     // EXIT_...

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
namespace testing {
    using   std::setw,                              // <iomanip>
            std::cout, std::cerr, std::endl,        // <iostream>
            std::exception, std::runtime_error,     // <stdexcept>
            std::string, std::to_string,            // <string>
            std::vector;                            // <vector>

    template< class Value >
    inline auto to_string( const vector<Value>& values )
        -> string
    {
        string result = "{";
        for( const Value& v: values ) {
            if( &v != &values.front() ) { result += ", "; }
            using std::to_string;
            result += to_string( v );
        }
        result += "}";
        return result;
    }

    template< class Test_case >
    inline auto run( const vector<Test_case>& tests )
        -> bool
    {
        auto& out = cout;
        const auto w6 = setw( 6 );
        using X = runtime_error;

        using Id        = typename Test_case::Id;
        using Result    = typename Test_case::Result;
        using Data      = typename Test_case::Data;
        using testing::to_string, std::to_string;

        bool            failure                     = false;
        Id              first_fail                  = {};
        Result          first_fail_expected_result  = {};
        Result          first_fail_actual_result    = {};
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
        if( failure ) {
            out << "!Test case #" << to_string( first_fail ) << " failed:"
                << " expected " << to_string( first_fail_expected_result )
                << " but got actual result " << to_string( first_fail_actual_result )
                << "." << endl;
        } else {
            out << "All tests completed successfully." << endl;
        }
        return not failure;
    }

    template< class Test_case >
    inline auto main( const vector<Test_case>& tests )
        -> int
    {
        try {
            const bool ok = run( tests );
            return (ok? EXIT_SUCCESS : EXIT_FAILURE);
        } catch( const exception& x ) {
            cerr << "!" << x.what() << endl;
        }
        return EXIT_FAILURE;
    }
}  // namespace testing
