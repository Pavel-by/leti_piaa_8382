set test=%1

IF NOT EXIST build md build
g++ src\main.cpp -o build\main.exe
build\main.exe < tests\test%test%.txt
