inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/account.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_account () {
    expect_function("is_account", testOb);

    expect_true(testOb->is_account(), "is_account handled");
}