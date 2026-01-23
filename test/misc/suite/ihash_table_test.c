#include <misc/ihash_table.h>

#include <suite.h>

TEST CREATE_01(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    ASSERT_EQ(0, table.capacity);
    ASSERT_EQ(0, table.length);
    ASSERT_EQ(sizeof(int), table.key_size);
    ASSERT_EQ(sizeof(int), table.value_size);
    ASSERT_NEQ(NULL, table.hash_key);
    ASSERT_EQ(NULL, table.keys);
    ASSERT_EQ(NULL, table.values);

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST DESTROY_01(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    destroy_ihash_table(&table, destroy, destroy);

    ASSERT_EQ(0, table.capacity);
    ASSERT_EQ(0, table.length);
    ASSERT_EQ(0, table.key_size);
    ASSERT_EQ(0, table.value_size);
    ASSERT_EQ(NULL, table.hash_key);
    ASSERT_EQ(NULL, table.keys);
    ASSERT_EQ(NULL, table.values);

    PASS();
}

TEST CLEAR_01(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    clear_ihash_table(&table, destroy, destroy);

    ASSERT_EQ(0, table.capacity);
    ASSERT_EQ(0, table.length);
    ASSERT_NEQ(0, table.key_size);
    ASSERT_NEQ(0, table.value_size);
    ASSERT_NEQ(NULL, table.hash_key);
    ASSERT_EQ(NULL, table.keys);
    ASSERT_EQ(NULL, table.values);

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST COPY_01(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK - 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    ihash_table_s replica = copy_ihash_table(&table, copy, copy);

    for (int i = 0; i < IHASH_TABLE_CHUNK - 1; ++i) {
        ASSERT(contains_key_ihash_table(&replica, &i));

        int value = -1;
        get_value_ihash_table(&replica, &i, &value);
        ASSERT_EQ(i, value);
    }

    destroy_ihash_table(&replica, destroy, destroy);
    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST COPY_02(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    ihash_table_s replica = copy_ihash_table(&table, copy, copy);

    for (int i = 0; i < IHASH_TABLE_CHUNK; ++i) {
        ASSERT(contains_key_ihash_table(&replica, &i));

        int value = -1;
        get_value_ihash_table(&replica, &i, &value);
        ASSERT_EQ(i, value);
    }

    destroy_ihash_table(&replica, destroy, destroy);
    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST COPY_03(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK + 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    ihash_table_s replica = copy_ihash_table(&table, copy, copy);

    for (int i = 0; i < IHASH_TABLE_CHUNK + 1; ++i) {
        ASSERT(contains_key_ihash_table(&replica, &i));

        int value = -1;
        get_value_ihash_table(&replica, &i, &value);
        ASSERT_EQ(i, value);
    }

    destroy_ihash_table(&replica, destroy, destroy);
    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST INSERT_01(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK - 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST INSERT_02(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST INSERT_03(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK + 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST REMOVE_01(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK - 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    for (int i = 0; i < IHASH_TABLE_CHUNK - 1; ++i) {
        int key = -1, value = -1;
        remove_ihash_table(&table, &i, &key, &value);

        ASSERT_EQ(i, key);
        ASSERT_EQ(i, value);
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST REMOVE_02(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    for (int i = 0; i < IHASH_TABLE_CHUNK; ++i) {
        int key = -1, value = -1;
        remove_ihash_table(&table, &i, &key, &value);

        ASSERT_EQ(i, key);
        ASSERT_EQ(i, value);
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST REMOVE_03(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK + 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    for (int i = 0; i < IHASH_TABLE_CHUNK + 1; ++i) {
        int key = -1, value = -1;
        remove_ihash_table(&table, &i, &key, &value);

        ASSERT_EQ(i, key);
        ASSERT_EQ(i, value);
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST CONTAINS_01(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK - 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    for (int i = 0; i < IHASH_TABLE_CHUNK - 1; ++i) {
        ASSERT(contains_key_ihash_table(&table, &i));
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST CONTAINS_02(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    for (int i = 0; i < IHASH_TABLE_CHUNK; ++i) {
        ASSERT(contains_key_ihash_table(&table, &i));
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST CONTAINS_03(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK + 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    for (int i = 0; i < IHASH_TABLE_CHUNK + 1; ++i) {
        ASSERT(contains_key_ihash_table(&table, &i));
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST GET_VALUE_01(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK - 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    for (int i = 0; i < IHASH_TABLE_CHUNK - 1; ++i) {
        int a = -1;
        get_value_ihash_table(&table, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST GET_VALUE_02(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    for (int i = 0; i < IHASH_TABLE_CHUNK; ++i) {
        int a = -1;
        get_value_ihash_table(&table, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST GET_VALUE_03(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK + 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    for (int i = 0; i < IHASH_TABLE_CHUNK + 1; ++i) {
        int a = -1;
        get_value_ihash_table(&table, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST SET_VALUE_01(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK - 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    int a = -1;
    const int key = (IHASH_TABLE_CHUNK - 1) / 2, value = 42;
    set_value_ihash_table(&table, &key, &value, &a);

    ASSERT_EQ((IHASH_TABLE_CHUNK - 1) / 2, a);
    int b = -1;
    get_value_ihash_table(&table, &key, &b);
    ASSERT_EQ(value, b);

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST SET_VALUE_02(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    int a = -1;
    const int key = (IHASH_TABLE_CHUNK) / 2, value = 42;
    set_value_ihash_table(&table, &key, &value, &a);

    ASSERT_EQ((IHASH_TABLE_CHUNK) / 2, a);
    int b = -1;
    get_value_ihash_table(&table, &key, &b);
    ASSERT_EQ(value, b);

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

TEST SET_VALUE_03(void) {
    ihash_table_s table = create_ihash_table(sizeof(int), sizeof(int), hash, compare);

    for (int i = 0; i < IHASH_TABLE_CHUNK + 1; ++i) {
        insert_ihash_table(&table, &i, &i);
    }

    int a = -1;
    const int key = (IHASH_TABLE_CHUNK + 1) / 2, value = 42;
    set_value_ihash_table(&table, &key, &value, &a);

    ASSERT_EQ((IHASH_TABLE_CHUNK + 1) / 2, a);
    int b = -1;
    get_value_ihash_table(&table, &key, &b);
    ASSERT_EQ(value, b);

    destroy_ihash_table(&table, destroy, destroy);

    PASS();
}

SUITE (ihash_table_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02); RUN_TEST(COPY_03);
    RUN_TEST(INSERT_01); RUN_TEST(INSERT_02); RUN_TEST(INSERT_03);
    RUN_TEST(REMOVE_01); RUN_TEST(REMOVE_02); RUN_TEST(REMOVE_03);
    RUN_TEST(CONTAINS_01); RUN_TEST(CONTAINS_02); RUN_TEST(CONTAINS_03);
    RUN_TEST(GET_VALUE_01); RUN_TEST(GET_VALUE_02); RUN_TEST(GET_VALUE_03);
    RUN_TEST(SET_VALUE_01); RUN_TEST(SET_VALUE_02); RUN_TEST(SET_VALUE_03);
}
