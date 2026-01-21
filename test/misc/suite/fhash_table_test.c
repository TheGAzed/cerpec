#include <misc/fhash_table.h>

#include <suite.h>

#define FHASH_TABLE_CHUNK 256

TEST CREATE_01() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    ASSERT_EQ(0, table.length);
    ASSERT_EQ(sizeof(int), table.key_size);
    ASSERT_EQ(sizeof(int), table.value_size);
    ASSERT_NEQ(NULL, table.hash);
    ASSERT_NEQ(NULL, table.keys);
    ASSERT_NEQ(NULL, table.values);

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST DESTROY_01() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    destroy_fhash_table(&table, destroy, destroy);

    ASSERT_EQ(0, table.length);
    ASSERT_EQ(0, table.key_size);
    ASSERT_EQ(0, table.value_size);
    ASSERT_EQ(NULL, table.hash);
    ASSERT_EQ(NULL, table.keys);
    ASSERT_EQ(NULL, table.values);

    PASS();
}

TEST CLEAR_01() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    clear_fhash_table(&table, destroy, destroy);

    ASSERT_EQ(0, table.length);
    ASSERT_NEQ(0, table.key_size);
    ASSERT_NEQ(0, table.value_size);
    ASSERT_NEQ(NULL, table.hash);
    ASSERT_NEQ(NULL, table.keys);
    ASSERT_NEQ(NULL, table.values);

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST COPY_01() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    fhash_table_s replica = copy_fhash_table(&table, copy, copy);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        ASSERT(contains_key_fhash_table(&replica, &i));

        int value = -1;
        get_value_fhash_table(&replica, &i, &value);
        ASSERT_EQ(i, value);
    }

    destroy_fhash_table(&replica, destroy, destroy);
    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST COPY_02() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    fhash_table_s replica = copy_fhash_table(&table, copy, copy);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        ASSERT(contains_key_fhash_table(&replica, &i));

        int value = -1;
        get_value_fhash_table(&replica, &i, &value);
        ASSERT_EQ(i, value);
    }

    destroy_fhash_table(&replica, destroy, destroy);
    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST INSERT_01() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST INSERT_02() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST REMOVE_01() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        int key = -1, value = -1;
        remove_fhash_table(&table, &i, &key, &value);

        ASSERT_EQ(i, key);
        ASSERT_EQ(i, value);
    }

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST REMOVE_02() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        int key = -1, value = -1;
        remove_fhash_table(&table, &i, &key, &value);

        ASSERT_EQ(i, key);
        ASSERT_EQ(i, value);
    }

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST CONTAINS_01() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        ASSERT(contains_key_fhash_table(&table, &i));
    }

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST CONTAINS_02() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        ASSERT(contains_key_fhash_table(&table, &i));
    }

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST GET_VALUE_01() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        int a = -1;
        get_value_fhash_table(&table, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST GET_VALUE_02() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        int a = -1;
        get_value_fhash_table(&table, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST SET_VALUE_01() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    int a = -1;
    const int key = (FHASH_TABLE_CHUNK - 1) / 2, value = 42;
    set_value_fhash_table(&table, &key, &value, &a);

    ASSERT_EQ((FHASH_TABLE_CHUNK - 1) / 2, a);
    int b = -1;
    get_value_fhash_table(&table, &key, &b);
    ASSERT_EQ(value, b);

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

TEST SET_VALUE_02() {
    fhash_table_s table = create_fhash_table(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, hash);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        insert_fhash_table(&table, &i, &i);
    }

    int a = -1;
    const int key = (FHASH_TABLE_CHUNK) / 2, value = 42;
    set_value_fhash_table(&table, &key, &value, &a);

    ASSERT_EQ((FHASH_TABLE_CHUNK) / 2, a);
    int b = -1;
    get_value_fhash_table(&table, &key, &b);
    ASSERT_EQ(value, b);

    destroy_fhash_table(&table, destroy, destroy);

    PASS();
}

SUITE (fhash_table_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02);
    RUN_TEST(INSERT_01); RUN_TEST(INSERT_02);
    RUN_TEST(REMOVE_01); RUN_TEST(REMOVE_02);
    RUN_TEST(CONTAINS_01); RUN_TEST(CONTAINS_02);
    RUN_TEST(GET_VALUE_01); RUN_TEST(GET_VALUE_02);
    RUN_TEST(SET_VALUE_01); RUN_TEST(SET_VALUE_02);
}
