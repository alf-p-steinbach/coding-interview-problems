#include "testing.hpp"
#include <algorithm>

using   std::remove_if, std::swap,      // <algorithm>
        std::string,                    // <string>
        std::swap,                      // <utility>
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
