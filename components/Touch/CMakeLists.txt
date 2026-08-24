idf_component_register(SRCS "TouchComponent.cpp" "GT911.cpp"
                  INCLUDE_DIRS "."
                  REQUIRES esp_driver_gpio I2C Common)