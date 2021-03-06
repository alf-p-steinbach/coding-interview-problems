// A test of whether “solution-1.via-placement.cpp” sorts correctly.
//
#include "testing.hpp"
#include <algorithm>
#include <iostream>

using   std::is_sorted, std::next_permutation,  // <algorithm>
        std::cout, std::cerr, std::endl,        // <iostream>
        std::string,                            // <string>
        std::vector;                            // <vector>

#define IT_RANGE_OF( c )    c.begin(), c.end()

auto main() -> int
{
    using utility::to_string;

    for( int n = 0; n <= 10; ++n ) {
        vector<int> permutation;
        for( int i = 1; i <= n; ++i ) {
            permutation.push_back( i );
        }
        do {
            vector<int> data = permutation;
            const int missing = problem::first_missing_positive_in( data );
            if( not is_sorted( IT_RANGE_OF( data ) ) ) {
                cerr    << "!Failed to sort " << to_string( permutation )
                        << " of length " << n << ","
                        << " producing " << to_string( data ) << "." << endl;
                return EXIT_FAILURE;
            }
            if( missing != n + 1 ) {
                cerr    << "!Expected " << n + 1 << " but got " << missing
                        << " sequence " << to_string( permutation ) << "." << endl;
                return EXIT_FAILURE;
            }
        } while( next_permutation( IT_RANGE_OF( permutation ) ) );
    }
    cout << "Sorted all permutations of natural number sequences length 10 and lower." << endl;
    return EXIT_SUCCESS;
}
