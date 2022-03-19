inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/array.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_distinct_array () {
    expect_function("distinct_array", testOb);

    expect("distinct_array handles inputs", (: ({
        assert((: testOb->distinct_array(({ 1, 2, 2, 3, 2, 1, 3, 2, 1 })) :), "==", ({ 3, 2, 1 })),
        assert((: testOb->distinct_array(({ 1, 4, 4, 3, 4, 1, 3, 2, 1 })) :), "==", ({ 4, 3, 2, 1 })),
        assert((: testOb->distinct_array(({})) :), "==", ({})),
        assert((: testOb->distinct_array(0) :), "==", ({})),
        assert((: testOb->distinct_array() :), "==", ({})),
    }) :));
}