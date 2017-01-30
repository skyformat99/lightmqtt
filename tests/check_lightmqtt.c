#include <stdlib.h>
#include <check.h>

#include "check_lightmqtt.h"

Suite* lightmqtt_suite(void)
{
    Suite *result = suite_create("Light MQTT");

    suite_add_tcase(result, tcase_validate_connect());
    suite_add_tcase(result, tcase_encode_connect());
    suite_add_tcase(result, tcase_encode_remaining_length());

    return result;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = lightmqtt_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
