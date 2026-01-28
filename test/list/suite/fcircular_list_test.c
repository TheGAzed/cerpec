#include <list/fcircular_list.h>
#include <suite.h>

#define FCIRCULAR_LIST_CHUNK CERPEC_CHUNK

#define NIL ((size_t)(-1))

TEST CREATE_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    ASSERT_EQ(NIL, test.empty);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(NULL, test.elements);
    ASSERT_NEQ(NULL, test.next);

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);
    destroy_fcircular_list(&test, destroy);

    ASSERT_EQ(0, test.size);

    PASS();
}

TEST CLEAR_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    clear_fcircular_list(&test, destroy);

    ASSERT_EQ(NIL, test.empty);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(NULL, test.elements);
    ASSERT_NEQ(NULL, test.next);

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST COPY_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    fcircular_list_s replica = copy_fcircular_list(&test, copy);

    ASSERT_EQ(test.size, replica.size);
    ASSERT_EQ(test.length, replica.length);
    ASSERT_EQ(NIL, replica.empty);

    for (size_t i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int t = 0, r = 0;
        get_fcircular_list(&test, i, &t);
        get_fcircular_list(&replica, i, &r);
        ASSERT_EQ(t, r);
    }

    destroy_fcircular_list(&test, destroy);
    destroy_fcircular_list(&replica, destroy);

    PASS();
}

TEST COPY_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    fcircular_list_s replica = copy_fcircular_list(&test, copy);

    ASSERT_EQ(test.size, replica.size);
    ASSERT_EQ(test.length, replica.length);
    ASSERT_EQ(NIL, replica.empty);

    for (size_t i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int t = 0, r = 0;
        get_fcircular_list(&test, i, &t);
        get_fcircular_list(&replica, i, &r);
        ASSERT_EQ(t, r);
    }

    destroy_fcircular_list(&test, destroy);
    destroy_fcircular_list(&replica, destroy);

    PASS();
}

TEST IS_EMPTY_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    ASSERT(is_empty_fcircular_list(&test));

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST IS_EMPTY_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    const int a = 0;
    insert_at_fcircular_list(&test, &a, test.length);

    ASSERT_FALSE(is_empty_fcircular_list(&test));

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST INSERT_AT_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST INSERT_AT_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST GET_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_fcircular_list(&test, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST GET_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int t = 0;
        get_fcircular_list(&test, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST REMOVE_FIRST_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_first_fcircular_list(&test, &i, &t, compare);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST REMOVE_FIRST_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int t = 0;
        remove_first_fcircular_list(&test, &i, &t, compare);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST REMOVE_AT_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_at_fcircular_list(&test, 0, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST REMOVE_AT_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int t = 0;
        remove_at_fcircular_list(&test, 0, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST REVERSE_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    reverse_fcircular_list(&test);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int t = 0;
        remove_at_fcircular_list(&test, test.length - 1, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST REVERSE_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    reverse_fcircular_list(&test);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int t = 0;
        remove_at_fcircular_list(&test, test.length - 1, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST SHIFT_NEXT_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }


    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_fcircular_list(&test, 0, &t);
        ASSERT_EQ(i, t);

        shift_next_fcircular_list(&test, 1);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST SHIFT_NEXT_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }


    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int t = 0;
        get_fcircular_list(&test, 0, &t);
        ASSERT_EQ(i, t);

        shift_next_fcircular_list(&test, 1);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST SPLICE_01(void) {
    fcircular_list_s one = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);
    fcircular_list_s two = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < (FCIRCULAR_LIST_CHUNK - 1) / 2; ++i) {
        insert_at_fcircular_list(&one, &i, one.length);
    }

    for (int i = (FCIRCULAR_LIST_CHUNK - 1) / 2; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&two, &i, two.length);
    }

    splice_fcircular_list(&one, &two, one.length); // concatenate

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_fcircular_list(&one, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&one, destroy);
    destroy_fcircular_list(&two, destroy);

    PASS();
}

TEST SPLICE_02(void) {
    fcircular_list_s one = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);
    fcircular_list_s two = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK / 2; ++i) {
        insert_at_fcircular_list(&one, &i, one.length);
    }

    for (int i = FCIRCULAR_LIST_CHUNK / 2; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&two, &i, two.length);
    }

    splice_fcircular_list(&one, &two, one.length); // concatenate

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int t = 0;
        get_fcircular_list(&one, (size_t)i, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&one, destroy);
    destroy_fcircular_list(&two, destroy);

    PASS();
}

TEST SPLIT_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    fcircular_list_s split = split_fcircular_list(&test, 0, test.length / 2);

    for (int i = 0; i < (FCIRCULAR_LIST_CHUNK - 1) / 2; ++i) {
        int s = 0;
        get_fcircular_list(&split, (size_t)i, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = (FCIRCULAR_LIST_CHUNK - 1) / 2; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int t = 0;
        get_fcircular_list(&test, (size_t)(i) - (FCIRCULAR_LIST_CHUNK - 1) / 2, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&test, destroy);
    destroy_fcircular_list(&split, destroy);

    PASS();
}

TEST SPLIT_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    fcircular_list_s split = split_fcircular_list(&test, 0, test.length / 2);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK / 2; ++i) {
        int s = 0;
        get_fcircular_list(&split, (size_t)i, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = FCIRCULAR_LIST_CHUNK / 2; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int t = 0;
        get_fcircular_list(&test, (size_t)(i) - FCIRCULAR_LIST_CHUNK / 2, &t);
        ASSERT_EQ(i, t);
    }

    destroy_fcircular_list(&test, destroy);
    destroy_fcircular_list(&split, destroy);

    PASS();
}

TEST EXTRACT_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    fcircular_list_s extract = extract_fcircular_list(&test, odd, NULL);
    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; i += 2) {
        int s = 0;
        get_fcircular_list(&test, (size_t)(i) / 2, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = 1; i < FCIRCULAR_LIST_CHUNK - 1; i += 2) {
        int e = 0;
        get_fcircular_list(&extract, (size_t)(i) / 2, &e);
        ASSERT_EQ(i, e);
    }

    destroy_fcircular_list(&test, destroy);
    destroy_fcircular_list(&extract, destroy);

    PASS();
}

TEST EXTRACT_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    fcircular_list_s extract = extract_fcircular_list(&test, odd, NULL);
    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; i += 2) {
        int s = 0;
        get_fcircular_list(&test, (size_t)(i) / 2, &s);
        ASSERT_EQ(i, s);
    }

    for (int i = 1; i < FCIRCULAR_LIST_CHUNK; i += 2) {
        int e = 0;
        get_fcircular_list(&extract, (size_t)(i) / 2, &e);
        ASSERT_EQ(i, e);
    }

    destroy_fcircular_list(&test, destroy);
    destroy_fcircular_list(&extract, destroy);

    PASS();
}

TEST MAP_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    int value = 1;
    each_fcircular_list(&test, increment, &value);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_fcircular_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST MAP_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    int value = 1;
    each_fcircular_list(&test, increment, &value);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int a = 0;
        get_fcircular_list(&test, (size_t)i, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST APPLY_01(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fcircular_list(&test, sort, &cmp);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_fcircular_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST APPLY_02(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        insert_at_fcircular_list(&test, &i, test.length);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fcircular_list(&test, sort, &cmp);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int a = 0;
        get_fcircular_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST APPLY_03(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int reverse = (FCIRCULAR_LIST_CHUNK - 1) - i - 1;
        insert_at_fcircular_list(&test, &reverse, test.length);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fcircular_list(&test, sort, &cmp);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK - 1; ++i) {
        int a = 0;
        get_fcircular_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

TEST APPLY_04(void) {
    fcircular_list_s test = create_fcircular_list(sizeof(int), FCIRCULAR_LIST_CHUNK);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int reverse = (FCIRCULAR_LIST_CHUNK) - i - 1;
        insert_at_fcircular_list(&test, &reverse, test.length);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fcircular_list(&test, sort, &cmp);

    for (int i = 0; i < FCIRCULAR_LIST_CHUNK; ++i) {
        int a = 0;
        get_fcircular_list(&test, (size_t)i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fcircular_list(&test, destroy);

    PASS();
}

SUITE (fcircular_list_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02);
    RUN_TEST(IS_EMPTY_01); RUN_TEST(IS_EMPTY_02);
    RUN_TEST(INSERT_AT_01); RUN_TEST(INSERT_AT_02);
    RUN_TEST(GET_01); RUN_TEST(GET_02);
    RUN_TEST(REMOVE_FIRST_01); RUN_TEST(REMOVE_FIRST_02);
    RUN_TEST(REMOVE_AT_01); RUN_TEST(REMOVE_AT_02);
    RUN_TEST(REVERSE_01); RUN_TEST(REVERSE_02);
    RUN_TEST(SHIFT_NEXT_01); RUN_TEST(SHIFT_NEXT_02);
    RUN_TEST(SPLICE_01); RUN_TEST(SPLICE_02);
    RUN_TEST(SPLIT_01); RUN_TEST(SPLIT_02);
    RUN_TEST(EXTRACT_01); RUN_TEST(EXTRACT_02);
    RUN_TEST(MAP_01); RUN_TEST(MAP_02);
    RUN_TEST(APPLY_01); RUN_TEST(APPLY_02); RUN_TEST(APPLY_03); RUN_TEST(APPLY_04);
}
