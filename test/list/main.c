#include <suite.h>

GREATEST_MAIN_DEFS();

int main(const int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(istraight_list_test);
    RUN_SUITE(icircular_list_test);
    RUN_SUITE(idouble_list_test);

    RUN_SUITE(fstraight_list_test);
    RUN_SUITE(fcircular_list_test);
    RUN_SUITE(fdouble_list_test);

    GREATEST_MAIN_END();
}
