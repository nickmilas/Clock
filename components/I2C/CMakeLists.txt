idf_component_register(SRCS "I2CBusMaster.cpp"
                  INCLUDE_DIRS "."
                  REQUIRES esp_driver_i2c esp_driver_gpio Common) # Public requirement (i.e. the #include is in the header file, NOT the source file)