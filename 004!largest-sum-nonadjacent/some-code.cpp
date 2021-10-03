#include <stdlib.h>         // EXIT_...
#include <stdio.h>          // printf, fprintf, stderr

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
using   std::accumulate, std::max_element,
        std::size,
        std::exception,
        std::stoi,
        std::string,
        std::string_view,
        std::vector;

template< class C >
auto int_size( const C& c ) -> int { return static_cast<int>( size( c ) ); }

#define ITEMS( c ) std::begin( c ), std::end( c )

struct Result
{
    vector<int>     indices;
    int             sum = 0;
};

auto max_nonadjacents_sum( const vector<int> numbers )
    -> Result
{
    Result result;
    const int n = int_size( numbers );
    if( n == 0 ) {
        // No indices and sum = 0.
    } else if( n == 1 ) {
        result.indicies.push_back( 0 );
    } else if( n == 2 ) {
        const auto it = max_element( ITEMS( numbers ) );
        const int i = static_cast<int>( it - numbers.begin() );
        result.indices.push_back( i );
        result.sum = numbers[i];
    } else {
        vector<int> max_sums( n );
        vector<int> index_of_previous( n );

        for( int i = 0; i < 2; ++i ) {
            
        }
        for( int i = 0; i < n; ++i ) {
        }
    }
    return result;
}
    
void cpp_main( const vector<string_view>& cmd_args )
{
    vector<int> numbers;
    for( const string_view& s: cmd_args ) {
        numbers.push_back( stoi( string( s ) ) );
    }
}

auto main( int n_cmd_parts, char* cmd_parts[] )
    -> int
{
    try {
        cpp_main( vector<string_view>( cmd_parts + 1, cmd_parts + n_cmd_parts ) );
        return EXIT_SUCCESS;
    } catch( const exception& x ) {
        fprintf( stderr, "!%s\n", x.what() );
    }
    return EXIT_FAILURE;
}
