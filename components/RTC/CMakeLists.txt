idf_component_register(SRCS "RtcComponent.cpp" "DS3231.cpp"
                  INCLUDE_DIRS "."
                  REQUIRES esp_driver_gpio I2C common) # Public requirement (i.e. the #include is in the header file, NOT the source file)