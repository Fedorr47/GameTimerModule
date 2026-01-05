add_executable(GameTimerTests
  test_timer_basic.cpp
)

target_link_libraries(GameTimerTests
  PRIVATE
    GameTimerLib
    GTest::gtest_main
)

target_compile_features(GameTimerTests PRIVATE cxx_std_23)
set_property(TARGET GameTimerTests PROPERTY CXX_SCAN_FOR_MODULES ON)

include(GoogleTest)
gtest_discover_tests(GameTimerTests)