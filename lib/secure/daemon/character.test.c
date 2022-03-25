inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/daemon/character.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_valid_name () {
    expect_function("query_valid_name", testOb);

    expect("query_valid_name handles names", (: ({
        assert(testOb->query_valid_name("valid"), "==", 1),
        assert(testOb->query_valid_name("abcdefghijklm"), "==", 1),
        assert(testOb->query_valid_name("nopqrstuvwxyz"), "==", 1),
        assert(testOb->query_valid_name("a'b c-d"), "==", 1), // valid decorators

        assert(testOb->query_valid_name(""), "==", 0),
        assert(testOb->query_valid_name("abc"), "==", 0), // too short
        assert(testOb->query_valid_name("abcdefghijklmnopqrstuvwxyz"), "==", 0), // too long
        assert(testOb->query_valid_name("!@#$%^&*()"), "==", 0), // invalid chars
    }) :));
}

void test_save_path () {
    expect_function("query_save_path", testOb);

    expect("query_save_path handles valid names", (: ({
        assert(testOb->query_save_path("name"), "regex", "^/save/character/n/name"),
    }) :));
    expect("query_save_path handles bad arguments", (: ({
        assert((: testOb->query_save_path("") :), "catch", "*Bad argument 1 to character->query_save_path\n"),
        assert((: testOb->query_save_path("abc") :), "catch", "*Bad argument 1 to character->query_save_path\n"),
    }) :));
}