#pragma once

#include <string>
#include <vector>

namespace utility {
    using   std::string,        // <string>
            std::vector;        // <vector>

    template< class T >
    auto copy_of( const T& v ) -> T { return v; }
    
    template< class T >
    auto temp_ref_to( T&& v ) -> T& { return v; }

    template< class Value >
    inline auto to_string( const vector<Value>& values )
        -> string
    {
        string result = "{";
        for( const Value& v: values ) {
            if( &v != &values.front() ) { result += ", "; }
            using std::to_string;
            result += to_string( v );
        }
        result += "}";
        return result;
    }

}  // namespace utility
