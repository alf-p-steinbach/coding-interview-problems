// Note: this attempted solution fails e.g. for input (1 4 5 1 2).
//
#include "testing.hpp"
#include <kickstart.hpp>        // https://github.com/progrock-libraries/kickstart
namespace ks = kickstart::all;
using std::vector, std::string, std::swap, std::partition;

auto is_positive( const int x ) -> bool { return (x > 0); };

auto problem::first_missing_positive_in( vector<int> numbers )
    -> int
{
    const auto it_beyond_positives = partition( numbers.begin(), numbers.end(), is_positive );
    const auto positive_numbers = ks::all_of( numbers.begin(), it_beyond_positives );
    const int n =  ks::int_size( positive_numbers );
    
    // Place each value v at index v - 1.
    for( int& v: positive_numbers ) {
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

auto main() -> int { return testing::main( problem::tests() ); }
