#include <sequence/fqueue.h>
#include <dodac.h>

#include <suite.h>

#define FQUEUE_CHUNK CERPEC_CHUNK

TEST CREATE_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(0, test.size);

    destroy_fqueue(&test, intdst, NULL);

    PASS();
}

TEST DESTROY_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    destroy_fqueue(&test, intdst, NULL);

    ASSERT_EQ(0, test.length);
    ASSERT_NEQ(sizeof(int), test.size);
    ASSERT_EQ(0, test.size);

    PASS();
}

TEST CLEAR_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    clear_fqueue(&test, intdst, NULL);

    ASSERT_EQ(0, test.length);
    ASSERT_EQ(sizeof(int), test.size);
    ASSERT_NEQ(0, test.size);

    destroy_fqueue(&test, intdst, NULL);

    PASS();
}

TEST COPY_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        enqueue_fqueue(&test, &i);
    }

    fqueue_s replica = copy_fqueue(&test, intcpy, NULL);

    ASSERT_EQ(test.allocator, replica.allocator);
    ASSERT_EQ(test.length, replica.length);
    ASSERT_EQ(test.max, replica.max);
    ASSERT_EQ(test.size, replica.size);

    destroy_fqueue(&test, intdst, NULL);
    destroy_fqueue(&replica, intdst, NULL);

    PASS();
}

TEST COPY_02(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        enqueue_fqueue(&test, &i);
    }

    fqueue_s replica = copy_fqueue(&test, intcpy, NULL);

    ASSERT_EQ(test.allocator, replica.allocator);
    ASSERT_EQ(test.length, replica.length);
    ASSERT_EQ(test.max, replica.max);
    ASSERT_EQ(test.size, replica.size);

    destroy_fqueue(&test, intdst, NULL);
    destroy_fqueue(&replica, intdst, NULL);

    PASS();
}

TEST ENQUEUE_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        enqueue_fqueue(&test, &i);
    }

    destroy_fqueue(&test, intdst, NULL);

    PASS();
}

TEST ENQUEUE_02(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        enqueue_fqueue(&test, &i);
    }

    destroy_fqueue(&test, intdst, NULL);

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

    destroy_fqueue(&test, intdst, NULL);

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

    destroy_fqueue(&test, intdst, NULL);

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

    destroy_fqueue(&test, intdst, NULL);

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

    destroy_fqueue(&test, intdst, NULL);

    PASS();
}

TEST MAP_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        enqueue_fqueue(&test, &i);
    }

    int value = 1;
    each_fqueue(&test, intincrement, &value);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fqueue(&test, intdst, NULL);

    PASS();
}

TEST MAP_02(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        enqueue_fqueue(&test, &i);
    }

    int value = 1;
    each_fqueue(&test, intincrement, &value);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fqueue(&test, intdst, NULL);

    PASS();
}

TEST APPLY_01(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        enqueue_fqueue(&test, &i);
    }

    apply_fqueue(&test, intqsort, NULL);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fqueue(&test, intdst, NULL);

    PASS();
}

TEST APPLY_02(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        enqueue_fqueue(&test, &i);
    }

    apply_fqueue(&test, intqsort, NULL);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fqueue(&test, intdst, NULL);

    PASS();
}

TEST APPLY_03(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK - 1; ++i) {
        enqueue_fqueue(&test, &i);
    }

    apply_fqueue(&test, intrqsort, NULL);

    for (int i = FQUEUE_CHUNK - 2; i >= 0; --i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fqueue(&test, intdst, NULL);

    PASS();
}

TEST APPLY_04(void) {
    fqueue_s test = create_fqueue(sizeof(int), FQUEUE_CHUNK);

    for (int i = 0; i < FQUEUE_CHUNK; ++i) {
        enqueue_fqueue(&test, &i);
    }

    apply_fqueue(&test, intrqsort, NULL);

    for (int i = FQUEUE_CHUNK - 1; i >= 0; --i) {
        int a = 0;
        dequeue_fqueue(&test, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fqueue(&test, intdst, NULL);

    PASS();
}

SUITE (fqueue_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02);
    RUN_TEST(ENQUEUE_01); RUN_TEST(ENQUEUE_02);
    RUN_TEST(PEEK_01); RUN_TEST(PEEK_02);
    RUN_TEST(DEQUEUE_01); RUN_TEST(DEQUEUE_02);
    RUN_TEST(MAP_01); RUN_TEST(MAP_02);
    RUN_TEST(APPLY_01); RUN_TEST(APPLY_02); RUN_TEST(APPLY_03); RUN_TEST(APPLY_04);
}
