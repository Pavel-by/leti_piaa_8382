set test=%1

IF NOT EXIST build md build
g++ src\main_second.cpp -o build\main_second.exe
build\main_second.exe < tests_second\test%test%.txt
