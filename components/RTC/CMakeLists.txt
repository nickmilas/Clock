idf_component_register(SRCS "DS3231.cpp"
                  INCLUDE_DIRS "."
                  REQUIRES I2C common) # Public requirement (i.e. the #include is in the header file, NOT the source file)