#include "origin.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    // set_short("");
    // set_long("");
    set_exits(([
        "west": ORIGIN_ROOM + "north.c",
        "south": ORIGIN_ROOM + "east.c",
        "southwest": ORIGIN_ROOM + "center.c",
    ]));
}