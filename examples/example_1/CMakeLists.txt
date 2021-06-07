set(EXAMPLE_1 "example_1")
add_executable("${EXAMPLE_1}" "main.cpp")

#windmill
target_link_libraries("${EXAMPLE_1}" PRIVATE windmill)
target_include_directories("${EXAMPLE_1}" PRIVATE "../../include")

#install
install(TARGETS "${EXAMPLE_1}")
