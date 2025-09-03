#include <tree/iavl_tree.h>
#include <suite.h>

TEST CREATE_01(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, test.capacity);
    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected size to be sizeof.", sizeof(int), test.size);
    ASSERT_NEQm("[ERROR] Expected capacity to not be zero.", 0, test.size);
    ASSERT_EQm("[ERROR] Expected capacity to be NULL.", NULL, test.elements);

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    destroy_iavl_tree(&test, destroy);

    ASSERT_EQm("[ERROR] Expected capacity to be zero.", 0, test.capacity);
    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected size to be zero.", 0, test.size);
    ASSERT_EQm("[ERROR] Expected elements to be NULL.", NULL, test.elements);

    PASS();
}

TEST CLEAR_01(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    clear_iavl_tree(&test, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_NEQm("[ERROR] Expected size to be zero.", 0, test.size);

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST INSERT_01(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK - 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST INSERT_02(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK; ++i) {
        insert_iavl_tree(&test, &i);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST INSERT_03(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK + 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST REMOVE_01(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK - 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = IAVL_TREE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        remove_iavl_tree(&test, &i, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST REMOVE_02(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = IAVL_TREE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        remove_iavl_tree(&test, &i, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST REMOVE_03(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK + 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = IAVL_TREE_CHUNK; i >= 0; --i) {
        int a = 0;
        remove_iavl_tree(&test, &i, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST CONTAINS_01(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK - 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK - 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_iavl_tree(test, &i));
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST CONTAINS_02(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_iavl_tree(test, &i));
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST CONTAINS_03(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK + 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK + 1; ++i) {
        ASSERTm("[ERROR] Expected element to be contained.", contains_iavl_tree(test, &i));
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST GET_MAX_01(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK - 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK - 1; ++i) {
        int a = 0, b = 0;

        get_max_iavl_tree(test, &a);
        remove_iavl_tree(&test, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST GET_MAX_02(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK; ++i) {
        int a = 0, b = 0;

        get_max_iavl_tree(test, &a);
        remove_iavl_tree(&test, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST GET_MAX_03(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK + 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK + 1; ++i) {
        int a = 0, b = 0;

        get_max_iavl_tree(test, &a);
        remove_iavl_tree(&test, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST GET_MIN_01(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK - 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK - 1; ++i) {
        int a = 0, b = 0;

        get_min_iavl_tree(test, &a);
        remove_iavl_tree(&test, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST GET_MIN_02(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK; ++i) {
        int a = 0, b = 0;

        get_min_iavl_tree(test, &a);
        remove_iavl_tree(&test, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST GET_MIN_03(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK + 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK + 1; ++i) {
        int a = 0, b = 0;

        get_min_iavl_tree(test, &a);
        remove_iavl_tree(&test, &a, &b);

        ASSERT_EQm("[ERROR] Expected elements to be equal.", b, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MAX_01(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK - 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = IAVL_TREE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        remove_max_iavl_tree(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MAX_02(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = IAVL_TREE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        remove_max_iavl_tree(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MAX_03(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK + 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = IAVL_TREE_CHUNK; i >= 0; --i) {
        int a = 0;
        remove_max_iavl_tree(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MIN_01(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK - 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK - 1; ++i) {
        int a = 0;
        remove_min_iavl_tree(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MIN_02(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK; ++i) {
        int a = 0;
        remove_min_iavl_tree(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

TEST REMOVE_MIN_03(void) {
    iavl_tree_s test = create_iavl_tree(sizeof(int), compare);

    for (int i = 0; i < IAVL_TREE_CHUNK + 1; ++i) {
        insert_iavl_tree(&test, &i);
    }

    for (int i = 0; i < IAVL_TREE_CHUNK + 1; ++i) {
        int a = 0;
        remove_min_iavl_tree(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_iavl_tree(&test, destroy);

    PASS();
}

SUITE (iavl_tree_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(INSERT_01); RUN_TEST(INSERT_02); RUN_TEST(INSERT_03);
    RUN_TEST(REMOVE_01); RUN_TEST(REMOVE_02); RUN_TEST(REMOVE_03);
    RUN_TEST(CONTAINS_01); RUN_TEST(CONTAINS_02); RUN_TEST(CONTAINS_03);
    RUN_TEST(GET_MAX_01); RUN_TEST(GET_MAX_02); RUN_TEST(GET_MAX_03);
    RUN_TEST(GET_MIN_01); RUN_TEST(GET_MIN_02); RUN_TEST(GET_MIN_03);
    RUN_TEST(REMOVE_MAX_01); RUN_TEST(REMOVE_MAX_02); RUN_TEST(REMOVE_MAX_03);
    RUN_TEST(REMOVE_MIN_01); RUN_TEST(REMOVE_MIN_02); RUN_TEST(REMOVE_MIN_03);
}
