add_executable(sine_harness
    main.cpp
)

if(USE_FAST_MATHS)
    target_compile_options(sine_harness PRIVATE -ffast-math)
endif()
