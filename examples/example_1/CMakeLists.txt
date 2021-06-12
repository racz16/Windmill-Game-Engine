set(WM_EXAMPLE_1 "example_1")
add_executable("${WM_EXAMPLE_1}" "main.cpp")

#windmill
target_link_libraries("${WM_EXAMPLE_1}" PRIVATE windmill)
target_include_directories("${WM_EXAMPLE_1}" PRIVATE "../../include")

#install
install(TARGETS "${WM_EXAMPLE_1}")
