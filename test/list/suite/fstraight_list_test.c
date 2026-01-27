#include <list/fstraight_list.h>
#include <suite.h>

#define NIL ((size_t)(-1))

#define FSTRAIGHT_LIST_CHUNK 256

TEST CREATE_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    ASSERT_EQ(NIL, test.head);
    ASSERT_EQ(NIL, test.empty);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(NULL, test.elements);
    ASSERT_NEQ(NULL, test.next);

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);
    destroy_fstraight_list(&test, destroy);

    ASSERT_EQ(0, test.size);

    PASS();
}

TEST CLEAR_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    clear_fstraight_list(&test, destroy);

    ASSERT_EQ(NIL, test.head);
    ASSERT_EQ(NIL, test.empty);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(NULL, test.elements);
    ASSERT_NEQ(NULL, test.next);

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST COPY_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    fstraight_list_s replica = copy_fstraight_list(&test, copy);

    ASSERT_EQ(test.size, replica.size);
    ASSERT_EQ(test.length, replica.length);
    ASSERT_EQ(NIL, replica.empty);

    for (size_t i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        int t = 0, r = 0;
        get_fstraight_list(&test, i, &t);
        get_fstraight_list(&replica, i, &r);
        ASSERT_EQ(t, r);
    }

    destroy_fstraight_list(&test, destroy);
    destroy_fstraight_list(&replica, destroy);

    PASS();
}

TEST COPY_02(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    fstraight_list_s replica = copy_fstraight_list(&test, copy);

    ASSERT_EQ(test.size, replica.size);
    ASSERT_EQ(test.length, replica.length);
    ASSERT_EQ(NIL, replica.empty);

    for (size_t i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        int t = 0, r = 0;
        get_fstraight_list(&test, i, &t);
        get_fstraight_list(&replica, i, &r);
        ASSERT_EQ(t, r);
    }

    destroy_fstraight_list(&test, destroy);
    destroy_fstraight_list(&replica, destroy);

    PASS();
}

TEST IS_EMPTY_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    ASSERT(is_empty_fstraight_list(&test));

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST IS_EMPTY_02(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    const int a = 0;
    insert_at_fstraight_list(&test, &a, test.length);

    ASSERT_FALSE(is_empty_fstraight_list(&test));

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST INSERT_AT_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST INSERT_AT_02(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST GET_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_fstraight_list(&test, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST GET_02(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        int t = 0;
        get_fstraight_list(&test, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST REMOVE_FIRST_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_first_fstraight_list(&test, &i, &t, compare);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST REMOVE_FIRST_02(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        int t = 0;
        remove_first_fstraight_list(&test, &i, &t, compare);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST REMOVE_AT_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_at_fstraight_list(&test, 0, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST REMOVE_AT_02(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        int t = 0;
        remove_at_fstraight_list(&test, 0, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST REVERSE_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    reverse_fstraight_list(&test);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_at_fstraight_list(&test, test.length - 1, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST REVERSE_02(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    reverse_fstraight_list(&test);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        int t = 0;
        remove_at_fstraight_list(&test, test.length - 1, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST SPLICE_01(void) {
    fstraight_list_s one = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);
    fstraight_list_s two = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < (FSTRAIGHT_LIST_CHUNK - 1) / 2; ++i) {
        insert_at_fstraight_list(&one, &i, one.length);
    }

    for (int i = (FSTRAIGHT_LIST_CHUNK - 1) / 2; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        insert_at_fstraight_list(&two, &i, two.length);
    }

    splice_fstraight_list(&one, &two, one.length); // concatenate

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_fstraight_list(&one, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&one, destroy);
    destroy_fstraight_list(&two, destroy);

    PASS();
}

TEST SPLICE_02(void) {
    fstraight_list_s one = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);
    fstraight_list_s two = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK / 2; ++i) {
        insert_at_fstraight_list(&one, &i, one.length);
    }

    for (int i = FSTRAIGHT_LIST_CHUNK / 2; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        insert_at_fstraight_list(&two, &i, two.length);
    }

    splice_fstraight_list(&one, &two, one.length); // concatenate

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        int t = 0;
        get_fstraight_list(&one, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&one, destroy);
    destroy_fstraight_list(&two, destroy);

    PASS();
}

TEST SPLIT_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    fstraight_list_s split = split_fstraight_list(&test, 0, test.length / 2);

    for (int i = 0; i < (FSTRAIGHT_LIST_CHUNK - 1) / 2; ++i) {
        int s = 0;
        get_fstraight_list(&split, (size_t)i, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = (FSTRAIGHT_LIST_CHUNK - 1) / 2; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_fstraight_list(&test, (size_t)(i) - (FSTRAIGHT_LIST_CHUNK - 1) / 2, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&test, destroy);
    destroy_fstraight_list(&split, destroy);

    PASS();
}

TEST SPLIT_02(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    fstraight_list_s split = split_fstraight_list(&test, 0, test.length / 2);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK / 2; ++i) {
        int s = 0;
        get_fstraight_list(&split, (size_t)i, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = FSTRAIGHT_LIST_CHUNK / 2; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        int t = 0;
        get_fstraight_list(&test, (size_t)(i) - FSTRAIGHT_LIST_CHUNK / 2, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fstraight_list(&test, destroy);
    destroy_fstraight_list(&split, destroy);

    PASS();
}

TEST EXTRACT_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    fstraight_list_s extract = extract_fstraight_list(&test, odd, NULL);
    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; i += 2) {
        int s = 0;
        get_fstraight_list(&test, (size_t)(i) / 2, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = 1; i < FSTRAIGHT_LIST_CHUNK - 1; i += 2) {
        int e = 0;
        get_fstraight_list(&extract, (size_t)(i) / 2, &e);
        ASSERT_EQ(i, e);
    }

    destroy_fstraight_list(&test, destroy);
    destroy_fstraight_list(&extract, destroy);

    PASS();
}

TEST EXTRACT_02(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    fstraight_list_s extract = extract_fstraight_list(&test, odd, NULL);
    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; i += 2) {
        int s = 0;
        get_fstraight_list(&test, (size_t)(i) / 2, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = 1; i < FSTRAIGHT_LIST_CHUNK; i += 2) {
        int e = 0;
        get_fstraight_list(&extract, (size_t)(i) / 2, &e);
        ASSERT_EQ(i, e);
    }

    destroy_fstraight_list(&test, destroy);
    destroy_fstraight_list(&extract, destroy);

    PASS();
}

TEST MAP_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    int value = 1;
    each_fstraight_list(&test, increment, &value);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_fstraight_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST MAP_02(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    int value = 1;
    each_fstraight_list(&test, increment, &value);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        int a = 0;
        get_fstraight_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST APPLY_01(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fstraight_list(&test, sort, &cmp);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_fstraight_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST APPLY_02(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        insert_at_fstraight_list(&test, &i, test.length);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fstraight_list(&test, sort, &cmp);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        int a = 0;
        get_fstraight_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST APPLY_03(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        int reverse = (FSTRAIGHT_LIST_CHUNK - 1) - i - 1;
        insert_at_fstraight_list(&test, &reverse, test.length);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fstraight_list(&test, sort, &cmp);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_fstraight_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

TEST APPLY_04(void) {
    fstraight_list_s test = create_fstraight_list(sizeof(int), FSTRAIGHT_LIST_CHUNK);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        int reverse = (FSTRAIGHT_LIST_CHUNK) - i - 1;
        insert_at_fstraight_list(&test, &reverse, test.length);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fstraight_list(&test, sort, &cmp);

    for (int i = 0; i < FSTRAIGHT_LIST_CHUNK; ++i) {
        int a = 0;
        get_fstraight_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstraight_list(&test, destroy);

    PASS();
}

SUITE (fstraight_list_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02);
    RUN_TEST(IS_EMPTY_01); RUN_TEST(IS_EMPTY_02);
    RUN_TEST(INSERT_AT_01); RUN_TEST(INSERT_AT_02);
    RUN_TEST(GET_01); RUN_TEST(GET_02);
    RUN_TEST(REMOVE_FIRST_01); RUN_TEST(REMOVE_FIRST_02);
    RUN_TEST(REMOVE_AT_01); RUN_TEST(REMOVE_AT_02);
    RUN_TEST(REVERSE_01); RUN_TEST(REVERSE_02);
    RUN_TEST(SPLICE_01); RUN_TEST(SPLICE_02);
    RUN_TEST(SPLIT_01); RUN_TEST(SPLIT_02);
    RUN_TEST(EXTRACT_01); RUN_TEST(EXTRACT_02);
    RUN_TEST(MAP_01); RUN_TEST(MAP_02);
    RUN_TEST(APPLY_01); RUN_TEST(APPLY_02); RUN_TEST(APPLY_03); RUN_TEST(APPLY_04);
}
