#include <stdlib.h>     // EXIT_...

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using   std::setw,                              // <iomanip>
        std::cout, std::cerr, std::endl,        // <iostream>
        std::exception, std::runtime_error,     // <stdexcept>
        std::string, std::to_string,            // <string>
        std::vector;                            // <vector>

extern int first_missing_positive_in( vector<int>& numbers );   // YOUR CODE.

struct Test_case
{
    enum Id: int {};

    Id          id;
    int         expected_result;
    vector<int> data;
    
    auto data_as_string() const
        -> string
    {
        string result = "{";
        for( const int& v: data ) {
            if( &v != &data.front() ) { result += ", "; }
            result += to_string( v );
        }
        result += "}";
        return result;
    }
};

void cpp_main()
{
    auto& out = cout;
    const auto w4 = setw( 4 );  const auto w6 = setw( 6 );
    using X = runtime_error;

    static const vector<Test_case> tests =
    {
        {Test_case::Id( 1 ), 2, {3, 4, -1, 1}},
        {Test_case::Id( 2 ), 3, {1, 2, 0}},
        {Test_case::Id( 3 ), 3, {1, 4, 5, 1, 2}},
        // ADD MORE TEST CASES IF DESIRED
    };

    int first_fail = 0;
    for( const Test_case& test: tests ) {
        vector<int> mutable_data = test.data;
        const int result = first_missing_positive_in( mutable_data );
        out << w4 << "#" << test.id
            << w4 << test.expected_result
            << w4 << result
            << w6 << (result == test.expected_result? "ok" : "FAIL")
            << "  " << test.data_as_string()
            << endl;
        if( first_fail == 0 and result != test.expected_result ) {
            first_fail = test.id;
        }
    }

    if( first_fail != 0 ) {
        throw X( string() + "Test case #" + to_string( first_fail ) + " failed." );
    }
}

auto main() -> int
{
    try {
        cpp_main();
        return EXIT_SUCCESS;
    } catch( const exception& x ) {
        cerr << "!" << x.what() << endl;
    }
    return EXIT_FAILURE;
}
