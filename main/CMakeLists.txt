idf_component_register(SRCS "startup.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES RTC I2C Common Display Touch EXIO Singletons)