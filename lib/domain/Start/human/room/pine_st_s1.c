#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west and east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "market_st_w3.c",
        "south": HUMAN_ROOM + "pine_st_s2.c",
    ]));
    set_looks(([
        "street": "A village street labeled S Pine St.",
    ]));
}