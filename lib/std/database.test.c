inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/database.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_ignore () { return ({ "handle_remove" }); }

nosave private object db;
void test_connect () {
    expect_function("connect", testOb);
    expect_function("close", testOb);
    expect_function("query_handle", testOb);
    expect_function("query", testOb);

    expect("connect returns a handle", (: ({
        // connect
        db = testOb->connect(([ "db": "/save/test/database.db", ])),
        assert(objectp(db), "==", 1),
        assert(testOb->query_handle(), ">", 0),
    }) :));
    expect("handle is queryable", (: ({
        // query non-existant
        assert(db->query("SELECT * FROM `Test`"), "==", UNDEFINED),

        // create
        db->query("CREATE TABLE `Test` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, `thing` TEXT NOT NULL )"),

        // table is empty
        assert(db->query("SELECT * FROM `Test`"), "==", UNDEFINED),

        // insert into table
        db->query("INSERT INTO `Test` (thing) VALUES ('some thing')"),
        db->query("INSERT INTO `Test` (thing) VALUES ('another thing')"),

        // table has records
        assert(db->query("SELECT * FROM `Test`"), "==", ({ ({ 1, "some thing" }), ({ 2, "another thing" }) })),

        // delete table records
        assert(db->query("DELETE FROM `Test`"), "==", UNDEFINED),

        // table is empty
        assert(db->query("SELECT * FROM `Test`"), "==", UNDEFINED),

        // drop table
        assert(db->query("DROP TABLE `Test`"), "==", UNDEFINED),
    }) :));
    expect("close stops a handle", (: ({
        // close
        assert(testOb->close(), "==", 1),
        assert(testOb->query_handle(), "==", 0),
    }) :));
}