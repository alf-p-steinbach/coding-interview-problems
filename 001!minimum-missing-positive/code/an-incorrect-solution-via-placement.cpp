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
