//#define CATCH_CONFIG_MAIN
//#include <catch2/catch.hpp>
//#include <modern/lib.hpp>

// TEST_CASE( "Quick check", "[main]" ) {
    
//     REQUIRE( 2+2 == 5 );
    
// }

#include <iostream>

using namespace std;

#include <ut/StringView.hpp>
using namespace ut;

struct Amount
{
    int a, b;
    string unit;
};

bool parseAmount(char const* s, Amount& out)
{
    ut::svstream svs{s};

    Amount amt;

    svs >> amt.a;

    if (amt.a < 1 || svs.fail())
        return false;

    auto t = svs.tellg();

    char slash;
    svs >> slash;

    if (svs.fail())
        return false;

    if (slash == '/')
    {
        svs >> amt.b;
        if (amt.b < 1 || svs.fail())
            return false;
    }
    else
    {
        amt.b = 1;
        svs.seekg(t, svs.beg);
    }

    svs >> amt.unit;
    if (svs.fail())
        return false;

    svs >> std::ws; // consume remaining whitespace
    if (!svs.eof()) // strict parsing, unused, non-ws chars means failure
        return false;

    out = amt;
    return true;
}

void printAmount(char const* s)
{
    Amount amt;
    if (parseAmount(s, amt))
    {
        cout << amt.a << ' ' << amt.b << ' ' << amt.unit << endl;
    }
    else
    {
        cout << "can't parse" << endl;
    }
}

int main() 
{



    string line;
    while (getline(cin, line))
    {
        printAmount(line.c_str());
    }


    return EXIT_SUCCESS;
}
