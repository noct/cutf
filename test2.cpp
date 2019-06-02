#include "cutf.h"

#define ok(statement)                                       \
    if( !(statement) )                                      \
    {                                                       \
        printf("Failed statement: %s\n", #statement);       \
        r = 1;                                              \
    }

int simpleStringTest()
{
    const wchar_t* chineseText = L"主体";
    auto s = widetoutf8(chineseText);
    size_t r = 0;

    printf("simple string test:  ");

    ok( s.length() == 6 );
    uint8_t utf8_array[] = { 0xE4, 0xB8, 0xBB, 0xE4, 0xBD, 0x93 };

    for(int i = 0; i < 6; i++)
        ok(((uint8_t)s[i]) == utf8_array[i]);

    auto ws = utf8towide(s);
    ok(ws.length() == 2);
    ok(ws == chineseText);

    if( r == 0 )
        printf("ok.\n");

    return (int)r;
}

