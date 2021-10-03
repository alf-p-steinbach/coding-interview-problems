#include <kickstart+main.hpp>
namespace ks = kickstart::all;
using std::vector, std::string, std::swap, std::partition;

auto is_positive( const int x ) -> bool { return (x > 0); };

// 1 4 5 1 2

int first_missing_positive_in( vector<int>& numbers )
{
    const auto it_beyond_positives = partition( numbers.begin(), numbers.end(), is_positive );
    ks::out << it_beyond_positives - numbers.begin() << " positives." << ks::endl;
    const auto positive_numbers = ks::all_of( numbers.begin(), it_beyond_positives );
    const int n =  ks::int_size( positive_numbers );
    
    // Place each value v at index v - 1.
    for( int& v: positive_numbers ) {
        ks::out << "Checking " << v << "..." << ks::endl;
        if( v <= n ) {
            swap( v, numbers[v - 1] );
        }
    }
    
    // If there is now an index i without value i + 1, well that's it.
    for( const int& v: positive_numbers ) {
        const int i = int( &v - &numbers[0] );
        if( v != i + 1 ) {
            return i + 1;
        }
    }
    
    // Otherwise first missing is the number beyond the given positive numbers range.
    return n + 1;
}

struct Test_case
{
    int         id;
    int         expected_result;
    vector<int> data;
    
    auto data_as_string() const
        -> string
    {
        using ks::operator<<;

        string result = "{ ";
        for( const int& v: data ) {
            if( &v != &data.front() ) { result << ", "; }
            result << v;
        }
        result << " }";
        return result;
    }
};

auto test( const vector<Test_case>& test_cases )
    -> int
{
    using namespace ks::string_output_operator;
    using ks::out, ks::endl, ks::ascii::at_right_in, ks::str;

    int first_fail = 0;
    for( const Test_case& tc: test_cases ) {
        auto mutable_data = tc.data;
        const int result = first_missing_positive_in( mutable_data );
        out << at_right_in( 4, "#"s << tc.id )
            << at_right_in( 4, str( tc.expected_result ) )
            << at_right_in( 4, str( result ) )
            << at_right_in( 6, (result == tc.expected_result? "ok" : "FAIL") )
            << "  " << tc.data_as_string()
            << endl;
        if( result != tc.expected_result and first_fail == 0 ) {
            first_fail = tc.id;
        }
    }
    return first_fail;
}

void cpp_main()
{
    static const vector<Test_case> test_cases =
    {
        {1, 2, {3, 4, -1, 1}},
        {2, 3. {1, 2, 0}},
        {3, 3, {1, 4, 5, 1, 2}},
    };
    if( const int first_fail = test( test_cases ) ) {
        ks::fail_app( ""s << "Test case #" << first_fail << " failed." );
    }
}
