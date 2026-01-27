#include <sequence/fstack.h>
#include <suite.h>

#define FSTACK_CHUNK 256

TEST CREATE_01(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(0, test.size);

    destroy_fstack(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    destroy_fstack(&test, destroy);

    ASSERT_EQ(0, test.length);
    ASSERT_NEQ(sizeof(int), test.size);
    ASSERT_EQ(0, test.size);

    PASS();
}

TEST CLEAR_01(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    clear_fstack(&test, destroy);

    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(0, test.size);

    destroy_fstack(&test, destroy);

    PASS();
}

TEST PUSH_01(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK - 1; ++i) {
        push_fstack(&test, &i);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

TEST PUSH_02(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK; ++i) {
        push_fstack(&test, &i);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

TEST PEEP_01(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK - 1; ++i) {
        push_fstack(&test, &i);

        int a = 0;
        peep_fstack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

TEST PEEP_02(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK; ++i) {
        push_fstack(&test, &i);

        int a = 0;
        peep_fstack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

TEST POP_01(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK - 1; ++i) {
        push_fstack(&test, &i);
    }

    for (int i = FSTACK_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        pop_fstack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

TEST POP_02(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK; ++i) {
        push_fstack(&test, &i);
    }

    for (int i = FSTACK_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        pop_fstack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

TEST MAP_01(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK - 1; ++i) {
        push_fstack(&test, &i);
    }

    int value = 1;
    each_fstack(&test, increment, &value);

    for (int i = FSTACK_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        pop_fstack(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

TEST MAP_02(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK; ++i) {
        push_fstack(&test, &i);
    }

    int value = 1;
    each_fstack(&test, increment, &value);

    for (int i = FSTACK_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        pop_fstack(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

TEST APPLY_01(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK - 1; ++i) {
        push_fstack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fstack(&test, sort, &cmp);

    for (int i = FSTACK_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        pop_fstack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

TEST APPLY_02(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK; ++i) {
        push_fstack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fstack(&test, sort, &cmp);

    for (int i = FSTACK_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        pop_fstack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

TEST APPLY_03(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK - 1; ++i) {
        push_fstack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_fstack(&test, sort, &cmp);

    for (int i = 0; i < FSTACK_CHUNK - 1; ++i) {
        int a = 0;
        pop_fstack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

TEST APPLY_04(void) {
    fstack_s test = create_fstack(sizeof(int), FSTACK_CHUNK);

    for (int i = 0; i < FSTACK_CHUNK; ++i) {
        push_fstack(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_fstack(&test, sort, &cmp);

    for (int i = 0; i < FSTACK_CHUNK; ++i) {
        int a = 0;
        pop_fstack(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fstack(&test, destroy);

    PASS();
}

SUITE (fstack_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(PUSH_01); RUN_TEST(PUSH_02);
    RUN_TEST(PEEP_01); RUN_TEST(PEEP_02);
    RUN_TEST(POP_01); RUN_TEST(POP_02);
    RUN_TEST(MAP_01); RUN_TEST(MAP_02);
    RUN_TEST(APPLY_01); RUN_TEST(APPLY_02); RUN_TEST(APPLY_03); RUN_TEST(APPLY_04);
}
