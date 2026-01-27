#include <sequence/istack.h>
#include <suite.h>


TEST CREATE_01(void) {
    istack_s test = create_istack(sizeof(int));

    ASSERT_EQ(0, test.capacity);
    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(0, test.size);
    ASSERT_EQ(NULL, test.elements);

    destroy_istack(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    istack_s test = create_istack(sizeof(int));

    destroy_istack(&test, destroy);

    ASSERT_EQ(0, test.capacity);
    ASSERT_EQ(0, test.length);
    ASSERT_NEQ(sizeof(int), test.size);
    ASSERT_EQ(0, test.size);
    ASSERT_EQ(NULL, test.elements);

    PASS();
}

TEST CLEAR_01(void) {
    istack_s test = create_istack(sizeof(int));

    clear_istack(&test, destroy);

    ASSERT_EQ(0, test.capacity);
    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(0, test.size);

    destroy_istack(&test, destroy);

    PASS();
}

TEST PUSH_01(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK - 1; ++i) {
        push_istack(&test, &i);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST PUSH_02(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK; ++i) {
        push_istack(&test, &i);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST PUSH_03(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK + 1; ++i) {
        push_istack(&test, &i);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST PEEP_01(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK - 1; ++i) {
        push_istack(&test, &i);

        int a = 0;
        peep_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST PEEP_02(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK; ++i) {
        push_istack(&test, &i);

        int a = 0;
        peep_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST PEEP_03(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK + 1; ++i) {
        push_istack(&test, &i);

        int a = 0;
        peep_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST POP_01(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK - 1; ++i) {
        push_istack(&test, &i);
    }

    for (int i = ISTACK_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST POP_02(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK; ++i) {
        push_istack(&test, &i);
    }

    for (int i = ISTACK_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST POP_03(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK + 1; ++i) {
        push_istack(&test, &i);
    }

    for (int i = ISTACK_CHUNK; i >= 0; --i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST MAP_01(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK - 1; ++i) {
        push_istack(&test, &i);
    }

    int value = 1;
    each_istack(&test, increment, &value);

    for (int i = ISTACK_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST MAP_02(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK; ++i) {
        push_istack(&test, &i);
    }

    int value = 1;
    each_istack(&test, increment, &value);

    for (int i = ISTACK_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST MAP_03(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK + 1; ++i) {
        push_istack(&test, &i);
    }

    int value = 1;
    each_istack(&test, increment, &value);

    for (int i = ISTACK_CHUNK; i >= 0; --i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST APPLY_01(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK - 1; ++i) {
        push_istack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_istack(&test, sort, &cmp);

    for (int i = ISTACK_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST APPLY_02(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK; ++i) {
        push_istack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_istack(&test, sort, &cmp);

    for (int i = ISTACK_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST APPLY_03(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK + 1; ++i) {
        push_istack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_istack(&test, sort, &cmp);

    for (int i = ISTACK_CHUNK; i >= 0; --i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST APPLY_04(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK - 1; ++i) {
        push_istack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_istack(&test, sort, &cmp);

    for (int i = 0; i < ISTACK_CHUNK - 1; ++i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST APPLY_05(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK; ++i) {
        push_istack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_istack(&test, sort, &cmp);

    for (int i = 0; i < ISTACK_CHUNK; ++i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

TEST APPLY_06(void) {
    istack_s test = create_istack(sizeof(int));

    for (int i = 0; i < ISTACK_CHUNK + 1; ++i) {
        push_istack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_istack(&test, sort, &cmp);

    for (int i = 0; i < ISTACK_CHUNK + 1; ++i) {
        int a = 0;
        pop_istack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_istack(&test, destroy);

    PASS();
}

SUITE (istack_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(PUSH_01); RUN_TEST(PUSH_02); RUN_TEST(PUSH_03);
    RUN_TEST(PEEP_01); RUN_TEST(PEEP_02); RUN_TEST(PEEP_03);
    RUN_TEST(POP_01); RUN_TEST(POP_02); RUN_TEST(POP_03);
    RUN_TEST(MAP_01); RUN_TEST(MAP_02); RUN_TEST(MAP_03);
    RUN_TEST(APPLY_01); RUN_TEST(APPLY_02); RUN_TEST(APPLY_03); RUN_TEST(APPLY_04); RUN_TEST(APPLY_05); RUN_TEST(APPLY_06);
}
