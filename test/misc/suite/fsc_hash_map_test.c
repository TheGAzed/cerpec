#include <misc/fsc_hash_map.h>
#include <dodac.h>

#include <suite.h>

#define FHASH_TABLE_CHUNK CERPEC_CHUNK

TEST CREATE_01(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    ASSERT_EQ(0, table.length);
    ASSERT_EQ(sizeof(int), table.key_size);
    ASSERT_EQ(sizeof(int), table.value_size);
    ASSERT_NEQ(NULL, table.hash_key);
    ASSERT_NEQ(NULL, table.keys);
    ASSERT_NEQ(NULL, table.values);

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST DESTROY_01(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    ASSERT_EQ(0, table.length);
    ASSERT_EQ(0, table.key_size);
    ASSERT_EQ(0, table.value_size);
    ASSERT_EQ(NULL, table.hash_key);
    ASSERT_EQ(NULL, table.keys);
    ASSERT_EQ(NULL, table.values);

    PASS();
}

TEST CLEAR_01(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    clear_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    ASSERT_EQ(0, table.length);
    ASSERT_NEQ(0, table.key_size);
    ASSERT_NEQ(0, table.value_size);
    ASSERT_NEQ(NULL, table.hash_key);
    ASSERT_NEQ(NULL, table.keys);
    ASSERT_NEQ(NULL, table.values);

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST COPY_01(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    fsc_hash_map_s replica = copy_fsc_hash_map(&table, intcpy, NULL, intcpy, NULL);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        ASSERT(contains_key_fsc_hash_map(&replica, &i));

        int value = -1;
        get_value_fsc_hash_map(&replica, &i, &value);
        ASSERT_EQ(i, value);
    }

    destroy_fsc_hash_map(&replica, intdst, NULL, intdst, NULL);
    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST COPY_02(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    fsc_hash_map_s replica = copy_fsc_hash_map(&table, intcpy, NULL, intcpy, NULL);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        ASSERT(contains_key_fsc_hash_map(&replica, &i));

        int value = -1;
        get_value_fsc_hash_map(&replica, &i, &value);
        ASSERT_EQ(i, value);
    }

    destroy_fsc_hash_map(&replica, intdst, NULL, intdst, NULL);
    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST INSERT_01(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST INSERT_02(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST REMOVE_01(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        int key = -1, value = -1;
        remove_fsc_hash_map(&table, &i, &key, &value);

        ASSERT_EQ(i, key);
        ASSERT_EQ(i, value);
    }

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST REMOVE_02(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        int key = -1, value = -1;
        remove_fsc_hash_map(&table, &i, &key, &value);

        ASSERT_EQ(i, key);
        ASSERT_EQ(i, value);
    }

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST CONTAINS_01(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        ASSERT(contains_key_fsc_hash_map(&table, &i));
    }

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST CONTAINS_02(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        ASSERT(contains_key_fsc_hash_map(&table, &i));
    }

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST GET_VALUE_01(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        int a = -1;
        get_value_fsc_hash_map(&table, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST GET_VALUE_02(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        int a = -1;
        get_value_fsc_hash_map(&table, &i, &a);
        ASSERT_EQ(i, a);
    }

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST SET_VALUE_01(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK - 1; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    int a = -1;
    const int key = (FHASH_TABLE_CHUNK - 1) / 2, value = 42;
    set_fsc_hash_map(&table, &key, &value, &a);

    ASSERT_EQ((FHASH_TABLE_CHUNK - 1) / 2, a);
    int b = -1;
    get_value_fsc_hash_map(&table, &key, &b);
    ASSERT_EQ(value, b);

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

TEST SET_VALUE_02(void) {
    fsc_hash_map_s table = create_fsc_hash_map(sizeof(int), sizeof(int), FHASH_TABLE_CHUNK, inthshmurmur, NULL, intcmp);

    for (int i = 0; i < FHASH_TABLE_CHUNK; ++i) {
        int j = i;
        insert_fsc_hash_map(&table, &i, &j);
    }

    int a = -1;
    const int key = (FHASH_TABLE_CHUNK) / 2, value = 42;
    set_fsc_hash_map(&table, &key, &value, &a);

    ASSERT_EQ((FHASH_TABLE_CHUNK) / 2, a);
    int b = -1;
    get_value_fsc_hash_map(&table, &key, &b);
    ASSERT_EQ(value, b);

    destroy_fsc_hash_map(&table, intdst, NULL, intdst, NULL);

    PASS();
}

SUITE (fsc_hash_map_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02);
    RUN_TEST(INSERT_01); RUN_TEST(INSERT_02);
    RUN_TEST(REMOVE_01); RUN_TEST(REMOVE_02);
    RUN_TEST(CONTAINS_01); RUN_TEST(CONTAINS_02);
    RUN_TEST(GET_VALUE_01); RUN_TEST(GET_VALUE_02);
    RUN_TEST(SET_VALUE_01); RUN_TEST(SET_VALUE_02);
}
