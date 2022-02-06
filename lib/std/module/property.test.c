inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/property.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({
        "test_property_bad_arguments",
        "test_property_single",
        "test_properties_multiple",
     });
}

void test_property_bad_arguments () {
    expect_function("query_property", testOb);
    expect_function("query_properties", testOb);
    expect_function("set_property", testOb);
    expect_function("set_property", testOb);
    expect_function("set_properties", testOb);
    expect_function("add_property", testOb);
    expect_function("remove_property", testOb);

    expect_catches (({
        (: testOb->query_property() :),
        (: testOb->query_property(0) :),
        (: testOb->query_property(0.0) :),
        (: testOb->query_property("") :),
        (: testOb->query_property(({})) :),
        (: testOb->query_property(([])) :),
    }), "*Bad argument 1 to property->query_property\n", "query_property handled invalid argument 1");

    expect_catches (({
        (: testOb->set_properties() :),
        (: testOb->set_properties(0) :),
        (: testOb->set_properties(1.0) :),
        (: testOb->set_properties("") :),
        (: testOb->set_properties(({})) :),
        (: testOb->set_properties(([])) :),
    }), "*Bad argument 1 to property->set_properties\n", "set_properties handled invalid argument 1");

    expect_catches (({
        (: testOb->set_property() :),
        (: testOb->set_property(0, 0) :),
        (: testOb->set_property(0.0, 0.0) :),
        (: testOb->set_property("", "") :),
        (: testOb->set_property(({}), ({})) :),
        (: testOb->set_property(([]), ([])) :),
        (: testOb->set_property() :),
    }), "*Bad argument 1 to property->set_property\n", "set_property handled invalid argument 1");
    expect_catches (({
        (: testOb->set_property("test_key") :),
        (: testOb->set_property("test_key", "") :),
        (: testOb->set_property("test_key", ({})) :),
        (: testOb->set_property("test_key", ([])) :),
    }), "*Bad argument 2 to property->set_property\n", "set_property handled invalid argument 2");

    expect_catches (({
        (: testOb->add_property() :),
        (: testOb->add_property(0, 0) :),
        (: testOb->add_property(0.0, 0.0) :),
        (: testOb->add_property("", "") :),
        (: testOb->add_property(({}), ({})) :),
        (: testOb->add_property(([]), ([])) :),
    }), "*Bad argument 1 to property->add_property\n", "add_property handled invalid argument 1");
    expect_catches (({
        (: testOb->add_property("test_key") :),
        (: testOb->add_property("test_key", "") :),
        (: testOb->add_property("test_key", ({})) :),
        (: testOb->add_property("test_key", ([])) :),
    }), "*Bad argument 2 to property->add_property\n", "add_property handled invalid argument 2");

    expect_catches (({
        (: testOb->remove_property() :),
        (: testOb->remove_property(0) :),
        (: testOb->remove_property(0.0) :),
        (: testOb->remove_property("") :),
        (: testOb->remove_property(({})) :),
        (: testOb->remove_property(([])) :),
    }), "*Bad argument 1 to property->remove_property\n", "remove_property handled invalid argument 1");
}

void test_property_single () {
    string *values = ({}), *results = ({});

    values += ({ testOb->query_property("test_key") });
    results += ({ UNDEFINED });

    testOb->set_property("test_key1", "test_value");
    values += ({ testOb->query_property("test_key1") });
    results += ({ "test_value" });

    testOb->set_property("test_key2", to_float(123));
    values += ({ testOb->query_property("test_key2") });
    results += ({ to_float(123) });

    testOb->add_property("test_key3", 10);
    values += ({ testOb->query_property("test_key3") });
    results += ({ 10 });
    testOb->add_property("test_key3", 1);
    values += ({ testOb->query_property("test_key3") });
    results += ({ 11 });

    testOb->remove_property("test_key1");
    values += ({ testOb->query_property("test_key1") });
    results += ({ UNDEFINED });
    testOb->remove_property("test_key2");
    values += ({ testOb->query_property("test_key2") });
    results += ({ UNDEFINED });

    expect_arrays_equal(values, results, "add, set, remove, and query property are handled");
}

void test_properties_multiple () {
    string *values = ({}), *results = ({});
    mapping tmp;
    mapping m1 = ([ "test_key1": "test_value" ]);
    mapping m2 = ([ "test_key2": to_float(123) ]);

    values += ({ identify(testOb->query_properties()) });
    results += ({ identify(([])) });

    tmp = m1;
    testOb->set_properties(m1);
    values += ({ identify(testOb->query_properties()) });
    results += ({ identify(tmp) });

    tmp += m2;
    testOb->set_properties(m2);
    values += ({ identify(testOb->query_properties()) });
    results += ({ identify(tmp) });

    testOb->remove_property("test_key2");
    values += ({ identify(testOb->query_properties()) });
    results += ({ identify(tmp) });

    expect_arrays_equal(values, results, "set and query properties are handled");
}