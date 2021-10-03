#include <kickstart+main.hpp>
namespace ks = kickstart::all;
using namespace kickstart::text_conversion;     // `""s`, `<<` on strings.

#include <stack>
using   std::exchange, std::max, std::function, std::move, std::stack,
        std::string_view, std::string, std::vector;

template< class T > using Ptr_ = T*;

template< class Item >
auto int_size( const stack<Item>& st )
    -> int
{
    struct Access: stack<Item>
    {
        static auto container_in( const stack<Item>& st )
            -> const std::deque<Item>&
        { return st.*&Access::c; }
    };
    
    return int( Access::container_in( st ).size() );
}

class Tree
{
public:
    struct Direction{ enum Enum{ left, right, root }; };

    using Inorder_callback = void(
        char value, int level, Direction::Enum dir, ks::Truth has_sibling
        );

    static constexpr Direction::Enum opposite_of[] =
    {
        Direction::right, Direction::left, Direction::root
    };

private:
    struct Node
    {
        char        value;
        Ptr_<Node>  p_child[2];         // Indexed by Direction::Enum
    };
    
    Ptr_<Node>  m_p_root    = nullptr;
    
    static void delete_nodes( const Ptr_<Node> p_root )
    {
        // Can theoretically give stack overflow.
        if( p_root ) {
            delete_nodes( p_root->p_child[Direction::left] );
            delete_nodes( p_root->p_child[Direction::right] );
            delete p_root;
        }
    }

    void apply_inorder(
        const Ptr_<Node>                    p_node,
        const int                           level,
        const Direction::Enum               dir,
        const ks::Truth                     has_sibling,
        const function<Inorder_callback>&   f
        ) const
    {
        const auto apply_f = [&]( const Direction::Enum dir ) -> void
        {
            const ks::Truth child_has_sibling = !!p_node->p_child[opposite_of[dir]];
            apply_inorder( p_node->p_child[dir], level + 1, dir, child_has_sibling, f );
        };

        if( p_node ) {
            apply_f( Direction::left );
            f( p_node->value, level, dir, has_sibling );
            apply_f( Direction::right );
        }
    }

public:
    ~Tree() { delete_nodes( m_p_root ); }

    Tree() {}
    
    Tree( const char root_value ): m_p_root( new Node{ root_value } ) {}
    
    Tree( const char root_value, Tree&& left, Tree&& right ):
        m_p_root( new Node{
            root_value,
            exchange( left.m_p_root, nullptr ),
            exchange( right.m_p_root, nullptr )
            } )
    {}
    
    Tree( Tree&& other ): m_p_root( exchange( other.m_p_root, nullptr ) ) {}
    
    auto is_single_value() const
        -> bool
    {
        return (
            m_p_root
            and not m_p_root->p_child[Direction::left]
            and not m_p_root->p_child[Direction::right]
            );
    }
    
    auto root_value() const
        -> char
    { return (m_p_root? m_p_root->value : ("Empty tree", '\0')); }
    
    void apply_inorder( const function<Inorder_callback>& f ) const
    {
        apply_inorder( m_p_root, 0, Direction::root, false, f );
    }
};

auto parse( const string_view& spec, const ks::Truth auto_complete = true )
    -> Tree
{
    stack<Tree> trees;
    
    auto combine = [&]() -> void
    {
        assert( not trees.empty() );
        Tree parent = move( trees.top() );  trees.pop();
        assert( parent.is_single_value() );
        ks::hopefully( int_size( trees ) >= 2 )
            or KS_FAIL( ""s << "Combining " << int_size( trees ) << " tree(s)." );
        Tree right  = move( trees.top() );  trees.pop();
        Tree left   = move( trees.top() );  trees.pop();
        trees.push( Tree( parent.root_value(), move( left ), move( right ) ) );
    };

    char previous_ch = 0;
    for( const char ch: spec ) {
        switch( ch ) {
            case '0':   [[fallthrough]];
            case '1':   {
                trees.push( ch );
                if( previous_ch == '>' ) {
                    combine();
                }
                break;
            }
            case '>':   {
                ks::hopefully( previous_ch != '>' )
                    or KS_FAIL( "Two successive ‘>’ characters." );
                break;
            }
            case ' ':   {
                break;                  // Ignore spaces.
            }
            default: {
                KS_FAIL( ""s << "Invalid character ‘" << ch << "’ in tree spec." );
            }
        }
        previous_ch = ch;
    }
    ks::hopefully( previous_ch != '>' )
        or KS_FAIL( ""s << "‘>’ without specification of a root node value." );
    if( auto_complete ) { while( int_size( trees ) > 1 ) { combine(); } }
    ks::hopefully( int_size( trees ) == 1 )
        or KS_FAIL( ""s << int_size( trees ) << " trees at end of parsing." );
    return move( trees.top() );
}

void display( const Tree& tree )
{
    vector<int> vline_levels;
    tree.apply_inorder( [&](
        char value, int level, Tree::Direction::Enum dir, const ks::Truth has_sibling
        ) -> void
    {
        constexpr auto& dir_char = R"(/\>)";
        constexpr int indent_size = 4;
        
        if( dir == Tree::Direction::right ) {
            auto& _ = vline_levels;
            _.erase( remove( _.begin(), _.end(), level ), _.end() );
        }

        const int last_vline_level = (vline_levels.empty()? -1 : vline_levels.back());
        const int last_display_level = max( last_vline_level, level );
        const int value_column = indent_size*level + 1;
        const int last_vline_column = indent_size*last_vline_level + 1;
        const int line_length = 1 + max( value_column, last_vline_column );
        auto line = string( line_length, ' ' );
        line[value_column - 1] = dir_char[dir];
        line[value_column] = value;
        for( const int vline_level: vline_levels ) {
            line[indent_size*vline_level] = '|';
        }
        while( line.back() == ' ' ) { line.resize( line.size() - 1 ); }

        string fancy_line;      // Requires working UTF-8 output.
        for( const char& ch: line ) {
            const int col = 1 + int( &ch - &line[0] );
            switch( ch ) {
                case '/':   { fancy_line += "┌"; break; }
                case '\\':  { fancy_line += "└"; break; }
                case '>':   { fancy_line += "─"; break; }
                case '|':   { fancy_line += (col > value_column? "┤" : "│"); break; }
                case ' ':   { fancy_line += (col > value_column? "─" : " "); break; }
                default:    { fancy_line += ch; }
            }
        }
        ks::out << fancy_line << ks::endl;

        if( dir == Tree::Direction::left and has_sibling ) {
            vline_levels.push_back( level );
        }
    } );
}

void cpp_main()
{
    const auto& explanation =
        "In a tree specification you can use spaces and the following characters:\n"
        "  0  → a new node with value 0.\n"
        "  1  → a new node with value 1.\n"
        "  >  → use the following node as root for the preceding two subtrees.";
    
    ks::out << explanation << ks::endl;
    ks::out << ks::endl;
    const string spec = ks::input( "Tree specification, please? " );
    const auto tree = parse( spec );
    display( tree );
}
