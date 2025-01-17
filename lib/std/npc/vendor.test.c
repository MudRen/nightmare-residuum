inherit M_TEST;
inherit M_MOVE;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/npc/vendor.c");
}
void after_each_test () {
    // vendor handle_remove to destruct vendor inventory
    if (objectp(testOb)) testOb->handle_remove();
    if (objectp(testOb)) destruct(testOb);
}

void test_vendor () {
    expect_function("is_vendor", testOb);

    expect("is_vendor behaves", (: ({
        assert(testOb->is_living(), "==", 1),
        assert(testOb->is_vendor(), "==", 1),
        assert(testOb->is_npc(), "==", 1),
    }) :));
}

void test_vendor_inventory () {
    object vi, ob;

    expect_function("handle_remove", testOb);
    expect_function("query_vendor_inventory", testOb);

    // grab reference to vendor inventory
    vi = testOb->query_vendor_inventory();
    // setup a test item
    ob = new(STD_ITEM);

    expect("vendor inventory is created and cleaned", (: ({
        // vendor inventory exists
        assert(objectp($(vi)), "==", 1),
        // move test item to vendor inventory
        assert($(ob)->handle_move($(vi)), "==", 1),
        // remove vendor
        assert(testOb->handle_remove(), "==", 1),
        // verify vendor inventory is destructed
        assert(objectp($(vi)), "==", 0),
        // verify item is destructed
        assert(objectp($(ob)), "==", 0),
    }) :));

    if (ob) destruct(ob);
}

void test_max_items () {
    object vi;

    expect_function("set_max_items", testOb);

    // grab reference to vendor inventory
    vi = testOb->query_vendor_inventory();

    expect("vendor inventory max items behaves", (: ({
        // verify default max items
        assert($(vi)->query_max_items(), "==", 0),
        // verify changing max items
        testOb->set_max_items(10),
        assert($(vi)->query_max_items(), "==", 10),
        // verify changing max items
        testOb->set_max_items(123),
        assert($(vi)->query_max_items(), "==", 123),
    }) :));
}

void test_vendor_currency () {
    expect_function("query_vendor_currency", testOb);
    expect_function("set_vendor_currency", testOb);

    expect("vendor currency is settable and queryable", (: ({
        assert(testOb->query_vendor_currency(), "==", 0),

        testOb->set_vendor_currency("copper"),
        assert(testOb->query_vendor_currency(), "==", "copper"),

        testOb->set_vendor_currency("silver"),
        assert(testOb->query_vendor_currency(), "==", "silver"),
    }) :));
}

void test_vendor_type () {
    expect_function("query_vendor_types", testOb);
    expect_function("set_vendor_types", testOb);

    expect("vendor type is settable and queryable", (: ({
        assert(testOb->query_vendor_types(), "==", 0),

        testOb->set_vendor_types(({ STD_WEAPON })),
        assert(testOb->query_vendor_types(), "==", ({ STD_WEAPON })),

        testOb->set_vendor_types(({ STD_ARMOR })),
        assert(testOb->query_vendor_types(), "==", ({ STD_ARMOR })),

        testOb->set_vendor_types(({ STD_ITEM })),
        assert(testOb->query_vendor_types(), "==", ({ STD_ITEM })),

        testOb->set_vendor_types(({ STD_ARMOR, STD_WEAPON })),
        assert(testOb->query_vendor_types(), "==", ({ STD_ARMOR, STD_WEAPON })),
    }) :));
}

void test_apply_list_verb () {
    object room;

    expect_function("direct_list_from_obj", testOb);
    expect_function("direct_list_str_from_obj", testOb);

    room = new(STD_ROOM);

    this_object()->handle_move("/domain/Nowhere/room/void.c");

    expect("direct_list_from_obj returns true when same environment", (: ({
        // true with same environment
        assert(testOb->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        assert(this_object()->query_environment_path(), "==", "/domain/Nowhere/room/void.c"),
        assert(testOb->direct_list_from_obj(testOb), "==", 1),
        // false with no object sent
        assert(testOb->direct_list_from_obj(), "==", 0),
        // false with different environments
        assert(testOb->handle_move($(room)), "==", 1),
        assert(testOb->direct_list_from_obj(testOb), "==", 0),
    }) :));

    expect("direct_list_str_from_obj returns true when same environment", (: ({
        // true with same environment
        assert(testOb->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        assert(this_object()->query_environment_path(), "==", "/domain/Nowhere/room/void.c"),
        assert(testOb->direct_list_from_obj(testOb), "==", 1),
        // false with no object sent
        assert(testOb->direct_list_from_obj(), "==", 0),
        // false with different environments
        assert(testOb->handle_move($(room)), "==", 1),
        assert(testOb->direct_list_from_obj(testOb), "==", 0),
    }) :));

    testOb->handle_remove();
    room->handle_remove();
}

void test_apply_buy_verb () {
    object room;

    expect_function("direct_buy_str_from_obj", testOb);

    room = new(STD_ROOM);

    this_object()->handle_move("/domain/Nowhere/room/void.c");
    expect("direct_buy_str_from_obj returns true when same environment", (: ({
        // true with same environment
        assert(testOb->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        assert(this_object()->query_environment_path(), "==", "/domain/Nowhere/room/void.c"),
        assert(testOb->direct_buy_str_from_obj("", testOb), "==", 1),
        // false with no object sent
        assert(testOb->direct_buy_str_from_obj(""), "==", 0),
        assert(testOb->direct_buy_str_from_obj(), "==", 0),
        // false with different environments
        assert(testOb->handle_move($(room)), "==", 1),
        assert(testOb->direct_buy_str_from_obj("", testOb), "==", 0),
    }) :));

    testOb->handle_remove();
    room->handle_remove();
}