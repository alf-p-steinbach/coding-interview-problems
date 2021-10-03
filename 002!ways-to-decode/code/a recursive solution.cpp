#include <kickstart+main.hpp>
namespace ks = kickstart::all;
using std::vector, std::string;

#ifdef INTERACTIVE
    constexpr bool interactive = true;
#else
    constexpr bool interactive = false;
#endif

auto is_in_range( const int first, const int last, const int x )
    -> bool
{ return (first <= x and x <= last); }

auto two_digit_value( const ks::Type_<const char*> p_first )
    -> int
{ return 10*(p_first[0] - '0') + (p_first[1] - '0'); }

auto n_ways_to_decode_from( const int start_index, const string& digits, vector<int>& known_ways )
    -> int
{
    const int n = ks::int_size( digits );
    assert( 0 <= start_index and start_index <= n );
    if( start_index == n ) {
        return 1;
    } else if( const int known = known_ways[start_index]; known != -1 ) {
        return known;
    } else {
        int result = 0;
        if( digits[start_index] != '0' ) {
            result += n_ways_to_decode_from( start_index + 1, digits, known_ways );
            if( start_index + 1 < n ) {
                if( is_in_range( 10, 26, two_digit_value( &digits[start_index] ) ) ) {
                    result += n_ways_to_decode_from( start_index + 2, digits, known_ways );
                }
            }
        }
        known_ways[start_index] = result;
        return result;
    }
    for(;;){};  // UB, will never get here.
}

auto n_ways_to_decode( const string& digits )
    -> int
{
    const int n = ks::int_size( digits );
    auto known_ways = vector<int>( n, -1 );
    for( int i = n - 1; i > 0; --i ) {
        n_ways_to_decode_from( 0, digits, known_ways );
    }
    return n_ways_to_decode_from( 0, digits, known_ways );
}

void cpp_main( const string&, const ks::Cmdline_args& )
{
    auto digits = (interactive? string( ks::input( "Code string? " ) ) : "111");
    ks::out << n_ways_to_decode( digits ) << ks::endl;
}
