#include <misc/ibinary_heap.h>

#include <suite.h>

TEST CREATE_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    ASSERT_EQ(0, heap.capacity);
    ASSERT_NEQ(NULL, heap.compare);
    ASSERT_EQ(NULL, heap.elements);
    ASSERT_EQ(0, heap.length);
    ASSERT_NEQ(0, heap.size);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    destroy_ibinary_heap(&heap, destroy);

    ASSERT_EQ(0, heap.capacity);
    ASSERT_EQ(NULL, heap.compare);
    ASSERT_EQ(NULL, heap.elements);
    ASSERT_EQ(0, heap.length);
    ASSERT_EQ(0, heap.size);

    PASS();
}

TEST CLEAR_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    clear_ibinary_heap(&heap, destroy);

    ASSERT_EQ(0, heap.capacity);
    ASSERT_NEQ(NULL, heap.compare);
    ASSERT_EQ(NULL, heap.elements);
    ASSERT_EQ(0, heap.length);
    ASSERT_NEQ(0, heap.size);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST COPY_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    ibinary_heap_s replica = copy_ibinary_heap(&heap, copy);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        int a = 0, b = 0;
        pop_ibinary_heap(&heap, &a);
        pop_ibinary_heap(&replica, &b);

        ASSERT_EQ(a, b);
    }

    destroy_ibinary_heap(&heap, destroy);
    destroy_ibinary_heap(&replica, destroy);

    PASS();
}

TEST COPY_02(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    ibinary_heap_s replica = copy_ibinary_heap(&heap, copy);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        int a = 0, b = 0;
        pop_ibinary_heap(&heap, &a);
        pop_ibinary_heap(&replica, &b);

        ASSERT_EQ(a, b);
    }

    destroy_ibinary_heap(&heap, destroy);
    destroy_ibinary_heap(&replica, destroy);

    PASS();
}

TEST COPY_03(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    ibinary_heap_s replica = copy_ibinary_heap(&heap, copy);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        int a = 0, b = 0;
        pop_ibinary_heap(&heap, &a);
        pop_ibinary_heap(&replica, &b);

        ASSERT_EQ(a, b);
    }

    destroy_ibinary_heap(&heap, destroy);
    destroy_ibinary_heap(&replica, destroy);

    PASS();
}

TEST PUSH_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST PUSH_02(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST PUSH_03(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST POP_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        int a = 0;
        pop_ibinary_heap(&heap, &a);

        ASSERT_EQ(a, i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST POP_02(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        int a = 0;
        pop_ibinary_heap(&heap, &a);

        ASSERT_EQ(a, i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST POP_03(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        int a = 0;
        pop_ibinary_heap(&heap, &a);

        ASSERT_EQ(a, i);
    }

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST PEEP_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    int a = 42;
    peep_ibinary_heap(&heap, &a);
    ASSERT_EQ(0, a);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST PEEP_02(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    int a = 42;
    peep_ibinary_heap(&heap, &a);
    ASSERT_EQ(0, a);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST PEEP_03(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    int a = 42;
    peep_ibinary_heap(&heap, &a);
    ASSERT_EQ(0, a);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST REPLACE_01(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    int const a = 42;
    int b = -1;
    replace_ibinary_heap(&heap, 0, &a, &b);
    ASSERT_EQ(0, b);

    int c = 0;
    peep_ibinary_heap(&heap, &c);
    ASSERT_EQ(1, c);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST REPLACE_02(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    int const a = 42;
    int b = -1;
    replace_ibinary_heap(&heap, 0, &a, &b);
    ASSERT_EQ(0, b);

    int c = 0;
    peep_ibinary_heap(&heap, &c);
    ASSERT_EQ(1, c);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST REPLACE_03(void) {
    ibinary_heap_s heap = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        push_ibinary_heap(&heap, &i);
    }

    int const a = 42;
    int b = -1;
    replace_ibinary_heap(&heap, 0, &a, &b);
    ASSERT_EQ(0, b);

    int c = 0;
    peep_ibinary_heap(&heap, &c);
    ASSERT_EQ(1, c);

    destroy_ibinary_heap(&heap, destroy);

    PASS();
}

TEST MELD_01(void) {
    ibinary_heap_s one = create_ibinary_heap(sizeof(int), compare);
    ibinary_heap_s two = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < (IBINARY_HEAP_CHUNK - 1) / 2; ++i) {
        push_ibinary_heap(&one, &i);
    }

    for (int i = (IBINARY_HEAP_CHUNK - 1) / 2; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        push_ibinary_heap(&two, &i);
    }

    meld_ibinary_heap(&one, &two);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        int a = -1;
        pop_ibinary_heap(&one, &a);

        ASSERT_EQ(a, i);
    }

    destroy_ibinary_heap(&one, destroy);
    destroy_ibinary_heap(&two, destroy);

    PASS();
}

TEST MELD_02(void) {
    ibinary_heap_s one = create_ibinary_heap(sizeof(int), compare);
    ibinary_heap_s two = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < (IBINARY_HEAP_CHUNK) / 2; ++i) {
        push_ibinary_heap(&one, &i);
    }

    for (int i = (IBINARY_HEAP_CHUNK) / 2; i < IBINARY_HEAP_CHUNK; ++i) {
        push_ibinary_heap(&two, &i);
    }

    meld_ibinary_heap(&one, &two);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        int a = -1;
        pop_ibinary_heap(&one, &a);

        ASSERT_EQ(a, i);
    }

    destroy_ibinary_heap(&one, destroy);
    destroy_ibinary_heap(&two, destroy);

    PASS();
}

TEST MELD_03(void) {
    ibinary_heap_s one = create_ibinary_heap(sizeof(int), compare);
    ibinary_heap_s two = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < (IBINARY_HEAP_CHUNK + 1) / 2; ++i) {
        push_ibinary_heap(&one, &i);
    }

    for (int i = (IBINARY_HEAP_CHUNK + 1) / 2; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        push_ibinary_heap(&two, &i);
    }

    meld_ibinary_heap(&one, &two);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        int a = -1;
        pop_ibinary_heap(&one, &a);

        ASSERT_EQ(a, i);
    }

    destroy_ibinary_heap(&one, destroy);
    destroy_ibinary_heap(&two, destroy);

    PASS();
}

TEST MAP_01(void) {
    ibinary_heap_s test = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        push_ibinary_heap(&test, &i);
    }

    int value = 1;
    each_ibinary_heap(&test, increment, &value);

    for (int i = 0; i < IBINARY_HEAP_CHUNK - 1; ++i) {
        int a = -1;
        pop_ibinary_heap(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_ibinary_heap(&test, destroy);

    PASS();
}

TEST MAP_02(void) {
    ibinary_heap_s test = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        push_ibinary_heap(&test, &i);
    }

    int value = 1;
    each_ibinary_heap(&test, increment, &value);

    for (int i = 0; i < IBINARY_HEAP_CHUNK; ++i) {
        int a = -1;
        pop_ibinary_heap(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_ibinary_heap(&test, destroy);

    PASS();
}

TEST MAP_03(void) {
    ibinary_heap_s test = create_ibinary_heap(sizeof(int), compare);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        push_ibinary_heap(&test, &i);
    }

    int value = 1;
    each_ibinary_heap(&test, increment, &value);

    for (int i = 0; i < IBINARY_HEAP_CHUNK + 1; ++i) {
        int a = -1;
        pop_ibinary_heap(&test, &a);
        ASSERT_EQ(i + value, a);
    }

    destroy_ibinary_heap(&test, destroy);

    PASS();
}

SUITE (ibinary_heap_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02); RUN_TEST(COPY_03);
    RUN_TEST(PUSH_01); RUN_TEST(PUSH_02); RUN_TEST(PUSH_03);
    RUN_TEST(POP_01); RUN_TEST(POP_02); RUN_TEST(POP_03);
    RUN_TEST(PEEP_01); RUN_TEST(PEEP_02); RUN_TEST(PEEP_03);
    RUN_TEST(REPLACE_01); RUN_TEST(REPLACE_02); RUN_TEST(REPLACE_03);
    RUN_TEST(MELD_01); RUN_TEST(MELD_02); RUN_TEST(MELD_03);
    RUN_TEST(MAP_01); RUN_TEST(MAP_02); RUN_TEST(MAP_03);
}
