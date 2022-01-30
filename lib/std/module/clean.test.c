inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/clean.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_handle_remove", "test_internal_remove", "test_clean_up" });
}

void test_handle_remove () {
    int *values = ({});

    expect_function("handle_remove", testOb);

    values += ({ objectp(testOb) });
    values += ({ testOb->handle_remove() });
    values += ({ objectp(testOb) });
    expect_arrays_equal(values, ({
        1,  // objectp
        1,  // handle_remove
        0,  // objectp
    }), "handle_remove behaves");
}

void test_internal_remove () {
    int *values = ({}), *results = ({});
    object container, ob;

    expect_true(member_array("internal_remove", functions(testOb, 0)) > -1 && !function_exists("internal_remove", testOb), "internal_remove is protected");

    container = new(STD_CONTAINER);
    ob = new(STD_OBJECT);
    ob->handle_move(container);
    values += ({ environment(ob) == container });
    results += ({ 1 });

    values += ({ container->handle_remove() });
    results += ({ 1 });

    values += ({ objectp(ob) });
    results += ({ 0 });

    expect_arrays_equal(values, results, "internal_remove behaves");

    if (objectp(container)) destruct(container);
    if (objectp(ob)) destruct(ob);
}

void test_clean_up () {
    int *values = ({}), *results = ({});

    expect_function("query_no_clean", testOb);
    expect_function("set_no_clean", testOb);
    expect_function("clean_up", testOb);
    expect_function("clean_never", testOb);
    expect_function("clean_later", testOb);

    values += ({ testOb->clean_never() });
    results += ({ 0 });
    values += ({ testOb->clean_later() });
    results += ({ 1 });

    values += ({ testOb->query_no_clean() });
    results += ({ 0 });
    testOb->set_no_clean(1);
    values += ({ testOb->query_no_clean() });
    results += ({ 1 });
    values += ({ testOb->clean_up() });
    results += ({ 0 });
    testOb->set_no_clean(0);
    values += ({ testOb->query_no_clean() });
    results += ({ 0 });
    values += ({ objectp(testOb) });
    results += ({ 1 });
    values += ({ testOb->clean_up() });
    results += ({ 1 });
    values += ({ objectp(testOb) });
    results += ({ 0 });

    expect_arrays_equal(values, results, "clean_up behaves");
}