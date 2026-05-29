#include <suite.h>

GREATEST_MAIN_DEFS();

int main(const int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(ibinary_heap_test);
    RUN_SUITE(isc_hash_map_test);

    RUN_SUITE(fbinary_heap_test);
    RUN_SUITE(fsc_hash_map_test);

    GREATEST_MAIN_END();
}
