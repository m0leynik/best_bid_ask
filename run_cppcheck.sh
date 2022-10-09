cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
cppcheck --enable=all --project=compile_commands.json -i _deps -i build
