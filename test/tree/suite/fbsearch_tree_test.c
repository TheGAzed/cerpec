#include <tree/fbsearch_tree.h>
#include <suite.h>

#define FBSEARCH_TREE_CHUNK CERPEC_CHUNK

TEST CREATE_01(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ( 0, test.size);
    ASSERT_NEQ(NULL, test.elements);

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    destroy_fbsearch_tree(&test, destroy);

    ASSERT_EQ(0, test.length);
    ASSERT_EQ(0, test.size);
    ASSERT_EQ(NULL, test.elements);

    PASS();
}

TEST CLEAR_01(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    clear_fbsearch_tree(&test, destroy);

    ASSERT_EQ(0, test.length);
    ASSERT_NEQ(0, test.size);

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST INSERT_01(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST INSERT_02(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_01(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = FBSEARCH_TREE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        remove_fbsearch_tree(&test, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_02(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = FBSEARCH_TREE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        remove_fbsearch_tree(&test, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST CONTAINS_01(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        ASSERT(contains_fbsearch_tree(&test, &i));
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST CONTAINS_02(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        ASSERT(contains_fbsearch_tree(&test, &i));
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST GET_MAX_01(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = 0, b = 0;

        get_max_fbsearch_tree(&test, &a);
        remove_fbsearch_tree(&test, &a, &b);

        ASSERT_EQ(b, a);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST GET_MAX_02(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        int a = 0, b = 0;

        get_max_fbsearch_tree(&test, &a);
        remove_fbsearch_tree(&test, &a, &b);

        ASSERT_EQ(b, a);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST GET_MIN_01(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = 0, b = 0;

        get_min_fbsearch_tree(&test, &a);
        remove_fbsearch_tree(&test, &a, &b);

        ASSERT_EQ(b, a);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST GET_MIN_02(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        int a = 0, b = 0;

        get_min_fbsearch_tree(&test, &a);
        remove_fbsearch_tree(&test, &a, &b);

        ASSERT_EQ(b, a);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MAX_01(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = FBSEARCH_TREE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        remove_max_fbsearch_tree(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MAX_02(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = FBSEARCH_TREE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        remove_max_fbsearch_tree(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MIN_01(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = 0;
        remove_min_fbsearch_tree(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MIN_02(void) {
    fbsearch_tree_s test = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&test, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        int a = 0;
        remove_min_fbsearch_tree(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fbsearch_tree(&test, destroy);

    PASS();
}

TEST GET_FLOOR_01(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }
    const int a = (FBSEARCH_TREE_CHUNK - 1) / 2;
    int buf = 0;
    remove_fbsearch_tree(&tree, &a, &buf);

    int floor = 0;
    get_floor_fbsearch_tree(&tree, &a, &floor);

    ASSERT_EQ(a - 1, floor);

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_FLOOR_02(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }
    const int a = (FBSEARCH_TREE_CHUNK) / 2;
    int buf = 0;
    remove_fbsearch_tree(&tree, &a, &buf);

    int floor = 0;
    get_floor_fbsearch_tree(&tree, &a, &floor);

    ASSERT_EQ(a - 1, floor);

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_CEIL_01(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }
    const int a = (FBSEARCH_TREE_CHUNK - 1) / 2;
    int buf = 0;
    remove_fbsearch_tree(&tree, &a, &buf);

    int ceil = 0;
    get_ceil_fbsearch_tree(&tree, &a, &ceil);

    ASSERT_EQ(a + 1, ceil);

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_CEIL_02(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }
    const int a = (FBSEARCH_TREE_CHUNK) / 2;
    int buf = 0;
    remove_fbsearch_tree(&tree, &a, &buf);

    int ceil = 0;
    get_ceil_fbsearch_tree(&tree, &a, &ceil);

    ASSERT_EQ(a + 1, ceil);

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_FLOOR_01(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }
    int a = 0;
    remove_max_fbsearch_tree(&tree, &a);

    while (!is_empty_fbsearch_tree(&tree)) {
        int b = 0;
        remove_floor_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);

        a = b;
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_FLOOR_02(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }
    int a = 0;
    remove_max_fbsearch_tree(&tree, &a);

    while (!is_empty_fbsearch_tree(&tree)) {
        int b = 0;
        remove_floor_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);

        a = b;
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_CEIL_01(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }
    int a = 0;
    remove_min_fbsearch_tree(&tree, &a);

    while (!is_empty_fbsearch_tree(&tree)) {
        int b = 0;
        remove_ceil_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a + 1, b);

        a = b;
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_CEIL_02(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }
    int a = 0;
    remove_min_fbsearch_tree(&tree, &a);

    while (!is_empty_fbsearch_tree(&tree)) {
        int b = 0;
        remove_ceil_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a + 1, b);

        a = b;
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_SUCCESSOR_01(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = i - 1, b = 0;
        get_successor_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(i, b);
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_SUCCESSOR_02(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        int a = i - 1, b = 0;
        get_successor_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(i, b);
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_PREDECESSOR_01(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = FBSEARCH_TREE_CHUNK - 1 - i, b = 0;
        get_predecessor_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_PREDECESSOR_02(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        int a = FBSEARCH_TREE_CHUNK - i, b = 0;
        get_predecessor_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_SUCCESSOR_01(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = i - 1, b = 0;
        remove_successor_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(i, b);
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_SUCCESSOR_02(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        int a = i - 1, b = 0;
        remove_successor_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(i, b);
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_PREDECESSOR_01(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = FBSEARCH_TREE_CHUNK - 1 - i, b = 0;
        remove_predecessor_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_PREDECESSOR_02(void) {
    fbsearch_tree_s tree = create_fbsearch_tree(sizeof(int), CERPEC_CHUNK, compare);

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        insert_fbsearch_tree(&tree, &i);
    }

    for (int i = 0; i < FBSEARCH_TREE_CHUNK; ++i) {
        int a = FBSEARCH_TREE_CHUNK - i, b = 0;
        remove_predecessor_fbsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);
    }

    destroy_fbsearch_tree(&tree, destroy);

    PASS();
}

SUITE (fbsearch_tree_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(INSERT_01); RUN_TEST(INSERT_02);
    RUN_TEST(REMOVE_01); RUN_TEST(REMOVE_02);
    RUN_TEST(CONTAINS_01); RUN_TEST(CONTAINS_02);
    RUN_TEST(GET_MAX_01); RUN_TEST(GET_MAX_02);
    RUN_TEST(GET_MIN_01); RUN_TEST(GET_MIN_02);
    RUN_TEST(REMOVE_MAX_01); RUN_TEST(REMOVE_MAX_02);
    RUN_TEST(REMOVE_MIN_01); RUN_TEST(REMOVE_MIN_02);
    RUN_TEST(GET_FLOOR_01); RUN_TEST(GET_FLOOR_02);
    RUN_TEST(GET_CEIL_01); RUN_TEST(GET_CEIL_02);
    RUN_TEST(REMOVE_FLOOR_01); RUN_TEST(REMOVE_FLOOR_02);
    RUN_TEST(REMOVE_CEIL_01); RUN_TEST(REMOVE_CEIL_02);
    RUN_TEST(GET_SUCCESSOR_01); RUN_TEST(GET_SUCCESSOR_02);
    RUN_TEST(GET_PREDECESSOR_01); RUN_TEST(GET_PREDECESSOR_02);
    RUN_TEST(REMOVE_SUCCESSOR_01); RUN_TEST(REMOVE_SUCCESSOR_02);
    RUN_TEST(REMOVE_PREDECESSOR_01); RUN_TEST(REMOVE_PREDECESSOR_02);
}
