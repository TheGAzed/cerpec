#include <misc/fbinary_heap.h>
#include <dodac.h>

#include <suite.h>

#define FBINARY_HEAP_CHUNK CERPEC_CHUNK

TEST CREATE_01(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    ASSERT_NEQ(NULL, heap.compare);
    ASSERT_NEQ(NULL, heap.elements);
    ASSERT_EQ(0, heap.length);
    ASSERT_NEQ(0, heap.size);

    destroy_fbinary_heap(&heap, intdst, NULL);

    PASS();
}

TEST DESTROY_01(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    destroy_fbinary_heap(&heap, intdst, NULL);

    ASSERT_EQ(NULL, heap.compare);
    ASSERT_EQ(NULL, heap.elements);
    ASSERT_EQ(0, heap.length);
    ASSERT_EQ(0, heap.size);

    PASS();
}

TEST CLEAR_01(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    clear_fbinary_heap(&heap, intdst, NULL);

    ASSERT_NEQ(NULL, heap.compare);
    ASSERT_NEQ(NULL, heap.elements);
    ASSERT_EQ(0, heap.length);
    ASSERT_NEQ(0, heap.size);

    destroy_fbinary_heap(&heap, intdst, NULL);

    PASS();
}

TEST COPY_01(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK - 1; ++i) {
        push_fbinary_heap(&heap, &i);
    }

    fbinary_heap_s replica = copy_fbinary_heap(&heap, intcpy, NULL);

    ASSERT_EQ(heap.allocator, replica.allocator);
    ASSERT_EQ(heap.compare, replica.compare);
    ASSERT_EQ(heap.ac, replica.ac);
    ASSERT_EQ(heap.length, replica.length);
    ASSERT_EQ(heap.size, replica.size);
    ASSERT_EQ(heap.max, replica.max);

    for (int i = 0; i < FBINARY_HEAP_CHUNK - 1; ++i) {
        int a = 0, b = 0;
        pop_fbinary_heap(&heap, &a);
        pop_fbinary_heap(&replica, &b);

        ASSERT_EQ(a, b);
    }

    destroy_fbinary_heap(&heap, intdst, NULL);
    destroy_fbinary_heap(&replica, intdst, NULL);

    PASS();
}

TEST COPY_02(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK; ++i) {
        push_fbinary_heap(&heap, &i);
    }

    fbinary_heap_s replica = copy_fbinary_heap(&heap, intcpy, NULL);

    ASSERT_EQ(heap.allocator, replica.allocator);
    ASSERT_EQ(heap.compare, replica.compare);
    ASSERT_EQ(heap.ac, replica.ac);
    ASSERT_EQ(heap.length, replica.length);
    ASSERT_EQ(heap.size, replica.size);
    ASSERT_EQ(heap.max, replica.max);

    for (int i = 0; i < FBINARY_HEAP_CHUNK; ++i) {
        int a = 0, b = 0;
        pop_fbinary_heap(&heap, &a);
        pop_fbinary_heap(&replica, &b);

        ASSERT_EQ(a, b);
    }

    destroy_fbinary_heap(&heap, intdst, NULL);
    destroy_fbinary_heap(&replica, intdst, NULL);

    PASS();
}

TEST PUSH_01(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK - 1; ++i) {
        push_fbinary_heap(&heap, &i);
    }

    destroy_fbinary_heap(&heap, intdst, NULL);

    PASS();
}

TEST PUSH_02(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK; ++i) {
        push_fbinary_heap(&heap, &i);
    }

    destroy_fbinary_heap(&heap, intdst, NULL);

    PASS();
}

TEST POP_01(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK - 1; ++i) {
        push_fbinary_heap(&heap, &i);
    }

    for (int i = 0; i < FBINARY_HEAP_CHUNK - 1; ++i) {
        int a = 0;
        pop_fbinary_heap(&heap, &a);

        ASSERT_EQ(a, i);
    }

    destroy_fbinary_heap(&heap, intdst, NULL);

    PASS();
}

TEST POP_02(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK; ++i) {
        push_fbinary_heap(&heap, &i);
    }

    for (int i = 0; i < FBINARY_HEAP_CHUNK; ++i) {
        int a = 0;
        pop_fbinary_heap(&heap, &a);

        ASSERT_EQ(a, i);
    }

    destroy_fbinary_heap(&heap, intdst, NULL);

    PASS();
}

TEST PEEP_01(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK - 1; ++i) {
        push_fbinary_heap(&heap, &i);
    }

    int a = 42;
    peep_fbinary_heap(&heap, &a);
    ASSERT_EQ(0, a);

    destroy_fbinary_heap(&heap, intdst, NULL);

    PASS();
}

TEST PEEP_02(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK; ++i) {
        push_fbinary_heap(&heap, &i);
    }

    int a = 42;
    peep_fbinary_heap(&heap, &a);
    ASSERT_EQ(0, a);

    destroy_fbinary_heap(&heap, intdst, NULL);

    PASS();
}

TEST REPLACE_01(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK - 1; ++i) {
        push_fbinary_heap(&heap, &i);
    }

    int const a = 42;
    int b = -1;
    replace_fbinary_heap(&heap, 0, &a, &b);
    ASSERT_EQ(0, b);

    int c = 0;
    peep_fbinary_heap(&heap, &c);
    ASSERT_EQ(1, c);

    destroy_fbinary_heap(&heap, intdst, NULL);

    PASS();
}

TEST REPLACE_02(void) {
    fbinary_heap_s heap = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK; ++i) {
        push_fbinary_heap(&heap, &i);
    }

    int const a = 42;
    int b = -1;
    replace_fbinary_heap(&heap, 0, &a, &b);
    ASSERT_EQ(0, b);

    int c = 0;
    peep_fbinary_heap(&heap, &c);
    ASSERT_EQ(1, c);

    destroy_fbinary_heap(&heap, intdst, NULL);

    PASS();
}

TEST MELD_01(void) {
    fbinary_heap_s one = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);
    fbinary_heap_s two = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < (FBINARY_HEAP_CHUNK - 1) / 2; ++i) {
        push_fbinary_heap(&one, &i);
    }

    for (int i = (FBINARY_HEAP_CHUNK - 1) / 2; i < FBINARY_HEAP_CHUNK - 1; ++i) {
        push_fbinary_heap(&two, &i);
    }

    meld_fbinary_heap(&one, &two);

    for (int i = 0; i < FBINARY_HEAP_CHUNK - 1; ++i) {
        int a = -1;
        pop_fbinary_heap(&one, &a);

        ASSERT_EQ(a, i);
    }

    destroy_fbinary_heap(&one, intdst, NULL);
    destroy_fbinary_heap(&two, intdst, NULL);

    PASS();
}

TEST MELD_02(void) {
    fbinary_heap_s one = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);
    fbinary_heap_s two = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < (FBINARY_HEAP_CHUNK) / 2; ++i) {
        push_fbinary_heap(&one, &i);
    }

    for (int i = (FBINARY_HEAP_CHUNK) / 2; i < FBINARY_HEAP_CHUNK; ++i) {
        push_fbinary_heap(&two, &i);
    }

    meld_fbinary_heap(&one, &two);

    for (int i = 0; i < FBINARY_HEAP_CHUNK; ++i) {
        int a = -1;
        pop_fbinary_heap(&one, &a);

        ASSERT_EQ(a, i);
    }

    destroy_fbinary_heap(&one, intdst, NULL);
    destroy_fbinary_heap(&two, intdst, NULL);

    PASS();
}

TEST MAP_01(void) {
    fbinary_heap_s test = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK - 1; ++i) {
        push_fbinary_heap(&test, &i);
    }

    int value = 1;
    each_fbinary_heap(&test, intincrement, &value);

    for (int i = 0; i < FBINARY_HEAP_CHUNK - 1; ++i) {
        int a = -1;
        pop_fbinary_heap(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fbinary_heap(&test, intdst, NULL);

    PASS();
}

TEST MAP_02(void) {
    fbinary_heap_s test = create_fbinary_heap(sizeof(int), FBINARY_HEAP_CHUNK, intcmp, NULL);

    for (int i = 0; i < FBINARY_HEAP_CHUNK; ++i) {
        push_fbinary_heap(&test, &i);
    }

    int value = 1;
    each_fbinary_heap(&test, intincrement, &value);

    for (int i = 0; i < FBINARY_HEAP_CHUNK; ++i) {
        int a = -1;
        pop_fbinary_heap(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_fbinary_heap(&test, intdst, NULL);

    PASS();
}

SUITE (fbinary_heap_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02);
    RUN_TEST(PUSH_01); RUN_TEST(PUSH_02);
    RUN_TEST(POP_01); RUN_TEST(POP_02);
    RUN_TEST(PEEP_01); RUN_TEST(PEEP_02);
    RUN_TEST(REPLACE_01); RUN_TEST(REPLACE_02);
    RUN_TEST(MELD_01); RUN_TEST(MELD_02);
    RUN_TEST(MAP_01); RUN_TEST(MAP_02);
}
