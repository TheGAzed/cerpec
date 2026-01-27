#include <sequence/fqueue.h>
#include <suite.h>

#define FQUEUE_CHUNK 256

TEST CREATE_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(0, test.size);

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    destroy_fqueue(&test, destroy);

    ASSERT_EQ(0, test.length);
    ASSERT_NEQ(sizeof(int), test.size);
    ASSERT_EQ(0, test.size);

    PASS();
}

TEST CLEAR_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    clear_fqueue(&test, destroy);

    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(0, test.size);

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST ENQUEUE_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        enqueue_fqueue(&test, &i);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST ENQUEUE_02(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        enqueue_fqueue(&test, &i);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST PEEK_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        enqueue_fqueue(&test, &i);

        int a = 0;
        peek_fqueue(&test, &a);
        ASSERT_EQ(0, a);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST PEEK_02(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        enqueue_fqueue(&test, &i);

        int a = 0;
        peek_fqueue(&test, &a);
        ASSERT_EQ(0, a);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST DEQUEUE_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        enqueue_fqueue(&test, &i);
    }

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST DEQUEUE_02(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        enqueue_fqueue(&test, &i);
    }

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST MAP_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        enqueue_fqueue(&test, &i);
    }

    int value = 1;
    each_fqueue(&test, increment, &value);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST MAP_02(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        enqueue_fqueue(&test, &i);
    }

    int value = 1;
    each_fqueue(&test, increment, &value);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST APPLY_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        enqueue_fqueue(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fqueue(&test, sort, &cmp);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST APPLY_02(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        enqueue_fqueue(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fqueue(&test, sort, &cmp);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST APPLY_03(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        enqueue_fqueue(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_fqueue(&test, sort, &cmp);

    for (int i = FQUEUE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

TEST APPLY_04(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        enqueue_fqueue(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_fqueue(&test, sort, &cmp);

    for (int i = FQUEUE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fqueue(&test, destroy);

    PASS();
}

SUITE (fqueue_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(ENQUEUE_01); RUN_TEST(ENQUEUE_02);
    RUN_TEST(PEEK_01); RUN_TEST(PEEK_02);
    RUN_TEST(DEQUEUE_01); RUN_TEST(DEQUEUE_02);
    RUN_TEST(MAP_01); RUN_TEST(MAP_02);
    RUN_TEST(APPLY_01); RUN_TEST(APPLY_02); RUN_TEST(APPLY_03); RUN_TEST(APPLY_04);
}
