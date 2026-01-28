#include <tree/ibsearch_tree.h>
#include <suite.h>

TEST CREATE_01(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    ASSERT_EQ(0, test.capacity);
    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ( 0, test.size);
    ASSERT_EQ(NULL, test.elements);

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    destroy_ibsearch_tree(&test, destroy);

    ASSERT_EQ(0, test.capacity);
    ASSERT_EQ(0, test.length);
    ASSERT_EQ(0, test.size);
    ASSERT_EQ(NULL, test.elements);

    PASS();
}

TEST CLEAR_01(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    clear_ibsearch_tree(&test, destroy);

    ASSERT_EQ(0, test.length);
    ASSERT_NEQ(0, test.size);

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST INSERT_01(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST INSERT_02(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST INSERT_03(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_01(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = IBSEARCH_TREE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        remove_ibsearch_tree(&test, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_02(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = IBSEARCH_TREE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        remove_ibsearch_tree(&test, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_03(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = IBSEARCH_TREE_CHUNK; i >= 0; --i) {
        int a = 0;
        remove_ibsearch_tree(&test, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST CONTAINS_01(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        ASSERT(contains_ibsearch_tree(&test, &i));
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST CONTAINS_02(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        ASSERT(contains_ibsearch_tree(&test, &i));
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST CONTAINS_03(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        ASSERT(contains_ibsearch_tree(&test, &i));
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST GET_MAX_01(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = 0, b = 0;

        get_max_ibsearch_tree(&test, &a);
        remove_ibsearch_tree(&test, &a, &b);

        ASSERT_EQ(b, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST GET_MAX_02(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        int a = 0, b = 0;

        get_max_ibsearch_tree(&test, &a);
        remove_ibsearch_tree(&test, &a, &b);

        ASSERT_EQ(b, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST GET_MAX_03(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        int a = 0, b = 0;

        get_max_ibsearch_tree(&test, &a);
        remove_ibsearch_tree(&test, &a, &b);

        ASSERT_EQ(b, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST GET_MIN_01(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = 0, b = 0;

        get_min_ibsearch_tree(&test, &a);
        remove_ibsearch_tree(&test, &a, &b);

        ASSERT_EQ(b, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST GET_MIN_02(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        int a = 0, b = 0;

        get_min_ibsearch_tree(&test, &a);
        remove_ibsearch_tree(&test, &a, &b);

        ASSERT_EQ(b, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST GET_MIN_03(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        int a = 0, b = 0;

        get_min_ibsearch_tree(&test, &a);
        remove_ibsearch_tree(&test, &a, &b);

        ASSERT_EQ(b, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MAX_01(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = IBSEARCH_TREE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        remove_max_ibsearch_tree(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MAX_02(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = IBSEARCH_TREE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        remove_max_ibsearch_tree(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MAX_03(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = IBSEARCH_TREE_CHUNK; i >= 0; --i) {
        int a = 0;
        remove_max_ibsearch_tree(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MIN_01(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = 0;
        remove_min_ibsearch_tree(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MIN_02(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        int a = 0;
        remove_min_ibsearch_tree(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MIN_03(void) {
    ibsearch_tree_s test = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&test, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        int a = 0;
        remove_min_ibsearch_tree(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ibsearch_tree(&test, destroy);

    PASS();
}

TEST GET_FLOOR_01(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    const int a = (IBSEARCH_TREE_CHUNK - 1) / 2;
    int buf = 0;
    remove_ibsearch_tree(&tree, &a, &buf);

    int floor = 0;
    get_floor_ibsearch_tree(&tree, &a, &floor);

    ASSERT_EQ(a - 1, floor);

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_FLOOR_02(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    const int a = (IBSEARCH_TREE_CHUNK) / 2;
    int buf = 0;
    remove_ibsearch_tree(&tree, &a, &buf);

    int floor = 0;
    get_floor_ibsearch_tree(&tree, &a, &floor);

    ASSERT_EQ(a - 1, floor);

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_FLOOR_03(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    const int a = (IBSEARCH_TREE_CHUNK + 1) / 2;
    int buf = 0;
    remove_ibsearch_tree(&tree, &a, &buf);

    int floor = 0;
    get_floor_ibsearch_tree(&tree, &a, &floor);

    ASSERT_EQ(a - 1, floor);

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_CEIL_01(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    const int a = (IBSEARCH_TREE_CHUNK - 1) / 2;
    int buf = 0;
    remove_ibsearch_tree(&tree, &a, &buf);

    int ceil = 0;
    get_ceil_ibsearch_tree(&tree, &a, &ceil);

    ASSERT_EQ(a + 1, ceil);

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_CEIL_02(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    const int a = (IBSEARCH_TREE_CHUNK) / 2;
    int buf = 0;
    remove_ibsearch_tree(&tree, &a, &buf);

    int ceil = 0;
    get_ceil_ibsearch_tree(&tree, &a, &ceil);

    ASSERT_EQ(a + 1, ceil);

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_CEIL_03(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    const int a = (IBSEARCH_TREE_CHUNK + 1) / 2;
    int buf = 0;
    remove_ibsearch_tree(&tree, &a, &buf);

    int ceil = 0;
    get_ceil_ibsearch_tree(&tree, &a, &ceil);

    ASSERT_EQ(a + 1, ceil);

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_FLOOR_01(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    int a = 0;
    remove_max_ibsearch_tree(&tree, &a);

    while (!is_empty_ibsearch_tree(&tree)) {
        int b = 0;
        remove_floor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);

        a = b;
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_FLOOR_02(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    int a = 0;
    remove_max_ibsearch_tree(&tree, &a);

    while (!is_empty_ibsearch_tree(&tree)) {
        int b = 0;
        remove_floor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);

        a = b;
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_FLOOR_03(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    int a = 0;
    remove_max_ibsearch_tree(&tree, &a);

    while (!is_empty_ibsearch_tree(&tree)) {
        int b = 0;
        remove_floor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);

        a = b;
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_CEIL_01(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    int a = 0;
    remove_min_ibsearch_tree(&tree, &a);

    while (!is_empty_ibsearch_tree(&tree)) {
        int b = 0;
        remove_ceil_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a + 1, b);

        a = b;
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_CEIL_02(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    int a = 0;
    remove_min_ibsearch_tree(&tree, &a);

    while (!is_empty_ibsearch_tree(&tree)) {
        int b = 0;
        remove_ceil_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a + 1, b);

        a = b;
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_CEIL_03(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }
    int a = 0;
    remove_min_ibsearch_tree(&tree, &a);

    while (!is_empty_ibsearch_tree(&tree)) {
        int b = 0;
        remove_ceil_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a + 1, b);

        a = b;
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_SUCCESSOR_01(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = i - 1, b = 0;
        get_successor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(i, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_SUCCESSOR_02(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        int a = i - 1, b = 0;
        get_successor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(i, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_SUCCESSOR_03(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        int a = i - 1, b = 0;
        get_successor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(i, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_PREDECESSOR_01(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = IBSEARCH_TREE_CHUNK - 1 - i, b = 0;
        get_predecessor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_PREDECESSOR_02(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        int a = IBSEARCH_TREE_CHUNK - i, b = 0;
        get_predecessor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST GET_PREDECESSOR_03(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        int a = IBSEARCH_TREE_CHUNK + 1 - i, b = 0;
        get_predecessor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_SUCCESSOR_01(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = i - 1, b = 0;
        remove_successor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(i, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_SUCCESSOR_02(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        int a = i - 1, b = 0;
        remove_successor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(i, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_SUCCESSOR_03(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        int a = i - 1, b = 0;
        remove_successor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(i, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_PREDECESSOR_01(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK - 1; ++i) {
        int a = IBSEARCH_TREE_CHUNK - 1 - i, b = 0;
        remove_predecessor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_PREDECESSOR_02(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK; ++i) {
        int a = IBSEARCH_TREE_CHUNK - i, b = 0;
        remove_predecessor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

TEST REMOVE_PREDECESSOR_03(void) {
    ibsearch_tree_s tree = create_ibsearch_tree(sizeof(int), compare);

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        insert_ibsearch_tree(&tree, &i);
    }

    for (int i = 0; i < IBSEARCH_TREE_CHUNK + 1; ++i) {
        int a = IBSEARCH_TREE_CHUNK + 1 - i, b = 0;
        remove_predecessor_ibsearch_tree(&tree, &a, &b);
        ASSERT_EQ(a - 1, b);
    }

    destroy_ibsearch_tree(&tree, destroy);

    PASS();
}

SUITE (ibsearch_tree_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(INSERT_01); RUN_TEST(INSERT_02); RUN_TEST(INSERT_03);
    RUN_TEST(REMOVE_01); RUN_TEST(REMOVE_02); RUN_TEST(REMOVE_03);
    RUN_TEST(CONTAINS_01); RUN_TEST(CONTAINS_02); RUN_TEST(CONTAINS_03);
    RUN_TEST(GET_MAX_01); RUN_TEST(GET_MAX_02); RUN_TEST(GET_MAX_03);
    RUN_TEST(GET_MIN_01); RUN_TEST(GET_MIN_02); RUN_TEST(GET_MIN_03);
    RUN_TEST(REMOVE_MAX_01); RUN_TEST(REMOVE_MAX_02); RUN_TEST(REMOVE_MAX_03);
    RUN_TEST(REMOVE_MIN_01); RUN_TEST(REMOVE_MIN_02); RUN_TEST(REMOVE_MIN_03);
    RUN_TEST(GET_FLOOR_01); RUN_TEST(GET_FLOOR_02); RUN_TEST(GET_FLOOR_03);
    RUN_TEST(GET_CEIL_01); RUN_TEST(GET_CEIL_02); RUN_TEST(GET_CEIL_03);
    RUN_TEST(REMOVE_FLOOR_01); RUN_TEST(REMOVE_FLOOR_02); RUN_TEST(REMOVE_FLOOR_03);
    RUN_TEST(REMOVE_CEIL_01); RUN_TEST(REMOVE_CEIL_02); RUN_TEST(REMOVE_CEIL_03);
    RUN_TEST(GET_SUCCESSOR_01); RUN_TEST(GET_SUCCESSOR_02); RUN_TEST(GET_SUCCESSOR_03);
    RUN_TEST(GET_PREDECESSOR_01); RUN_TEST(GET_PREDECESSOR_02); RUN_TEST(GET_PREDECESSOR_03);
    RUN_TEST(REMOVE_SUCCESSOR_01); RUN_TEST(REMOVE_SUCCESSOR_02); RUN_TEST(REMOVE_SUCCESSOR_03);
    RUN_TEST(REMOVE_PREDECESSOR_01); RUN_TEST(REMOVE_PREDECESSOR_02); RUN_TEST(REMOVE_PREDECESSOR_03);
}
