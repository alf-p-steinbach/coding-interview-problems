// Note: due to the sorting this code is O(n log n), whereas the problem requires O(n).
//
#include "testing.hpp"
#include <algorithm>
using std::vector, std::sort;

auto problem::first_missing_positive_in( vector<int> numbers )
    -> int
{
    sort( numbers.begin(), numbers.end() );
    int x = 0;
    for( const int v: numbers ) {
        if( v <= 0 or v == x ) { continue; }
        ++x;
        if( v > x ) { return x; }
    }
    return x + 1;
}

auto main() -> int { return testing::main( problem::tests() ); }
