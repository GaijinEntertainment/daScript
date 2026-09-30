#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/ast/ast_serializer.h"

using namespace das;

TEST_CASE("strings a host streams outside a program record do not alias the host's storage") {
    const vector<string> paths = { "scripts/game/es/first_game_script.das", "scripts/game/es/second_game_script.das" };
    SerializationStorageVector written;
    {
        AstSerializer writer(&written, true);
        vector<string> queue = paths;
        writer << queue;
        for ( auto & name : queue ) writer << name;
    }
    written.flush();

    SerializationStorageVector stream;
    stream.buffer = written.buffer;
    AstSerializer reader(&stream, false);
    {
        vector<string> savedQueue;
        reader << savedQueue;
        REQUIRE(savedQueue == paths);
        for ( auto & name : savedQueue ) {
            for ( auto & ch : name ) ch = 'x';
        }
    }
    for ( auto & expected : paths ) {
        string name;
        reader << name;
        CHECK(name == expected);
    }
}
