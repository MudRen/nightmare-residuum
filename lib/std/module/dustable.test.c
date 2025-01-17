inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/dustable.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}


void test_expire () {
    expect_function("handle_expire", testOb);

    // change testOb to an item which inherits M_CLEAN
    destruct(testOb);
    testOb = new("/std/item/corpse.c");

    expect("expire to remove dustable", (: ({
        assert(undefinedp(testOb), "==", 0),
        testOb->handle_expire(),
        assert(undefinedp(testOb), "==", 1),
    }) :));
}

private string testObFile;
private mixed *calloutInfo;
void test_received () {
    function_exists("handle_received", testOb);

    expect("handle_received sets expire timer", (: ({
        assert(testObFile = file_name(testOb), "regex", "/std/module/dustable#[0-9]+"),

        // start the expire call_out
        testOb->handle_received(this_object()),

        // grab callout tied to the test dustable
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert(sizeof(calloutInfo), "==", 1),
        assert(sizeof(calloutInfo[0]), "==", 3),
        assert(calloutInfo[0][0], "==", testOb),
    }) :));
}