#include <verb.h>

inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/vendor_inventory.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_ignore () { return ({ "clean_up" });
}

void test_receive () {
    object living, item;

    expect_function("handle_receive", testOb);

    // create test items
    living = new(STD_LIVING);
    item = new(STD_ITEM);

    expect("inventory receives only items", (: ({
        assert(testOb->handle_receive($(living)), "==", 0),
        assert(testOb->handle_receive($(item)), "==", 1),
    }) :));

    if (living) destruct(living);
    if (item) destruct(item);
}

void test_max_items () {
    object item1, item2;

    expect_function("query_max_items", testOb);
    expect_function("set_max_items", testOb);

    // create test items
    item1 = new(STD_ITEM);
    item2 = new(STD_ITEM);

    expect("max items prevents receiving items", (: ({
        // set max items to 1
        testOb->set_max_items(1),
        assert(testOb->query_max_items(), "==", 1),
        // 0 of 1 items, receive passes
        assert(testOb->handle_receive($(item1)), "==", 1),
        // move item to inventory to count
        assert($(item1)->handle_move(testOb), "==", 1),
        // 1 of 1 items, receive fails
        assert(testOb->handle_receive($(item2)), "==", 0),
        // set max items to 2
        testOb->set_max_items(2),
        assert(testOb->query_max_items(), "==", 2),
        // 1 of 2 items, receive passes
        assert(testOb->handle_receive($(item2)), "==", 1),
    }) :));

    if (item1) destruct(item1);
    if (item2) destruct(item2);
}
