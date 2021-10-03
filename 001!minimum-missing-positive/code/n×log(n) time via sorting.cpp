// Note: due to the sorting this code is O(n log n), whereas the problem requires O(n).
//
#include <kickstart+main.hpp>
using std::vector, std::sort;

auto smallest_positive_not_in( vector<int>& numbers )
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

void cpp_main()
{
    using namespace kickstart::all;
    auto numbers = parts_to_vector_<int>( string( input( "Numbers? " ) ) );
    out << smallest_positive_not_in( numbers ) << endl;
}
