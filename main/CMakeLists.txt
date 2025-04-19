idf_component_register(SRCS "ble_module.c" "count_rad.c" "main.c" "ble_module.c"
                    INCLUDE_DIRS ".")

add_compile_definitions(ESP_TESTING_BUILD)