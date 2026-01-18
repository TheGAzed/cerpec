#include <sequence/fdeque.h>
#include <suite.h>

#define FDEQUE_CHUNK 256

TEST CREATE_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected size to not be zero.", sizeof(int), test.size);
    ASSERT_NEQm("[ERROR] Expected size to not be zero.", 0, test.size);

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    destroy_fdeque(&test, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_NEQm("[ERROR] Expected size to be zero.", sizeof(int), test.size);
    ASSERT_EQm("[ERROR] Expected size to be zero.", 0, test.size);

    PASS();
}

TEST CLEAR_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    clear_fdeque(&test, destroy);

    ASSERT_EQm("[ERROR] Expected length to be zero.", 0, test.length);
    ASSERT_EQm("[ERROR] Expected size to not be zero.", sizeof(int), test.size);
    ASSERT_NEQm("[ERROR] Expected size to not be zero.", 0, test.size);

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST ENQUEUE_FRONT_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_front_fdeque(&test, &i);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST ENQUEUE_FRONT_02(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_front_fdeque(&test, &i);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST ENQUEUE_BACK_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_back_fdeque(&test, &i);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST ENQUEUE_BACK_02(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_back_fdeque(&test, &i);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_front_fdeque(&test, &i);

        int a = 0;
        peek_front_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_02(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_front_fdeque(&test, &i);

        int a = 0;
        peek_front_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_04(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_back_fdeque(&test, &i);

        int a = 0;
        peek_front_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST PEEK_FRONT_05(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_back_fdeque(&test, &i);

        int a = 0;
        peek_front_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_front_fdeque(&test, &i);

        int a = 0;
        peek_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_02(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_front_fdeque(&test, &i);

        int a = 0;
        peek_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be 0.", 0, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_04(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_back_fdeque(&test, &i);

        int a = 0;
        peek_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST PEEK_BACK_05(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_back_fdeque(&test, &i);

        int a = 0;
        peek_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected peeked element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_front_fdeque(&test, &i);
    }

    for (int i = FDEQUE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        dequeue_front_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_02(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_front_fdeque(&test, &i);
    }

    for (int i = FDEQUE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        dequeue_front_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_04(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_back_fdeque(&test, &i);
    }

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_front_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST DEQUEUE_FRONT_05(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_back_fdeque(&test, &i);
    }

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_front_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_front_fdeque(&test, &i);
    }

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_02(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_front_fdeque(&test, &i);
    }

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_04(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_back_fdeque(&test, &i);
    }

    for (int i = FDEQUE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST DEQUEUE_BACK_05(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_back_fdeque(&test, &i);
    }

    for (int i = FDEQUE_CHUNK - 1; i >= 0; --i)  {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected dequeued element to be i.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST MAP_FRONT_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_front_fdeque(&test, &i);
    }

    int value = 1;
    map_front_fdeque(&test, increment, &value);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST MAP_FRONT_02(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_front_fdeque(&test, &i);
    }

    int value = 1;
    map_front_fdeque(&test, increment, &value);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST MAP_BACK_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_front_fdeque(&test, &i);
    }

    int value = 1;
    map_back_fdeque(&test, increment, &value);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST MAP_BACK_02(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_front_fdeque(&test, &i);
    }

    int value = 1;
    map_back_fdeque(&test, increment, &value);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i + value, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST APPLY_01(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_back_fdeque(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fdeque(&test, sort, &cmp);

    for (int i = FDEQUE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST APPLY_02(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_back_fdeque(&test, &i);
    }

    struct compare cmp = { .compare_element = compare, };
    apply_fdeque(&test, sort, &cmp);

    for (int i = FDEQUE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST APPLY_03(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        enqueue_back_fdeque(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_fdeque(&test, sort, &cmp);

    for (int i = 0; i < FDEQUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

TEST APPLY_04(void) {
    fdeque_s test = create_fdeque(sizeof(int), FDEQUE_CHUNK);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        enqueue_back_fdeque(&test, &i);
    }

    struct compare cmp = { .compare_element = compare_reverse, };
    apply_fdeque(&test, sort, &cmp);

    for (int i = 0; i < FDEQUE_CHUNK; ++i) {
        int a = 0;
        dequeue_back_fdeque(&test, &a);
        ASSERT_EQm("[ERROR] Expected elements to be equal.", i, a);
    }

    destroy_fdeque(&test, destroy);

    PASS();
}

SUITE (fdeque_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(ENQUEUE_FRONT_01); RUN_TEST(ENQUEUE_FRONT_02);
    RUN_TEST(ENQUEUE_BACK_01); RUN_TEST(ENQUEUE_BACK_02);
    RUN_TEST(PEEK_FRONT_01); RUN_TEST(PEEK_FRONT_02);
    RUN_TEST(PEEK_FRONT_04); RUN_TEST(PEEK_FRONT_05);
    RUN_TEST(PEEK_BACK_01); RUN_TEST(PEEK_BACK_02);
    RUN_TEST(PEEK_BACK_04); RUN_TEST(PEEK_BACK_05);
    RUN_TEST(DEQUEUE_FRONT_01); RUN_TEST(DEQUEUE_FRONT_02);
    RUN_TEST(DEQUEUE_FRONT_04); RUN_TEST(DEQUEUE_FRONT_05);
    RUN_TEST(DEQUEUE_BACK_01); RUN_TEST(DEQUEUE_BACK_02);
    RUN_TEST(DEQUEUE_BACK_04); RUN_TEST(DEQUEUE_BACK_05);
    RUN_TEST(MAP_FRONT_01); RUN_TEST(MAP_FRONT_02);
    RUN_TEST(MAP_BACK_01); RUN_TEST(MAP_BACK_02);
    RUN_TEST(APPLY_01); RUN_TEST(APPLY_02); RUN_TEST(APPLY_03); RUN_TEST(APPLY_04);
}
