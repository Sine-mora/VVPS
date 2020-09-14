Текущият проект по ВВПС е тестван под Linux OS и Windows 10.
Документацията към програмата е в папка docs.
Prerequisites - Linux based OS, cmake (sudo apt-get install cmake)
Организация на папките:
FrequentPatternVVPS/
|-- build/
|-- CMakeLists.txt
|-- lib/
|   `-- googletest 
|-- src/
|   |-- CMakeLists.txt
|   |-- DataComponent/
|   |   |--DataComponent.h
|   |   |--DataComponent.cpp
|   |   |--CSV.h
|   |-- FPGrowth/
|   |   |--FPGrowth.hpp
|   |   |--FPGrowth.cpp
|   `-- main.cpp
`-- tеst/
    |-- CMakeLists.txt
    |-- FPGrowthTest.cpp
    `-- main.cpp

Компилация:
CMakeLists.txt съдържа скрипт за генериране на Makefile, използван за билдване на програмата.
За компилиране на програмата, отворете конзола, отидете в директорията на този проект и изпълнете следните команди:
1) mkdir build, ако няма build директория
2) cd build
3) cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ..
    Примерен изход от командата в конзолата:
-- The C compiler identification is GNU 6.3.0
-- The CXX compiler identification is GNU 6.3.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- FrequentPatternGrowth is located in /home/default/workspace/RandomProjects/vvps/FrequentPatternVVPS
-- Found PythonInterp: /usr/bin/python (found version "2.7.13") 
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Looking for pthread_create
-- Looking for pthread_create - not found
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE  
-- Configuring done
-- Generating done
-- Build files have been written to: /home/default/workspace/RandomProjects/vvps/FrequentPatternVVPS/build

Сега има няколко файла и папки в build директорията. За компилация, напишете:
4) make -j4
Примерен изход:
Scanning dependencies of target FrequentPatternGrowth_run
Scanning dependencies of target gtest
Scanning dependencies of target FrequentPatternGrowth_lib
[  5%] Building CXX object src/CMakeFiles/FrequentPatternGrowth_run.dir/main.cpp.o
[ 10%] Building CXX object src/CMakeFiles/FrequentPatternGrowth_lib.dir/main.cpp.o
[ 15%] Building CXX object src/CMakeFiles/FrequentPatternGrowth_run.dir/FPGrowth/FPGrowth.cpp.o
[ 21%] Building CXX object lib/googletest/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o
[ 26%] Building CXX object src/CMakeFiles/FrequentPatternGrowth_lib.dir/FPGrowth/FPGrowth.cpp.o
[ 31%] Building CXX object src/CMakeFiles/FrequentPatternGrowth_run.dir/DataComponent/DataComponent.cpp.o
[ 36%] Building CXX object src/CMakeFiles/FrequentPatternGrowth_lib.dir/DataComponent/DataComponent.cpp.o
[ 42%] Linking CXX executable FrequentPatternGrowth_run
[ 47%] Linking CXX static library ../../libgtest.a
[ 47%] Built target gtest
Scanning dependencies of target gtest_main
Scanning dependencies of target gmock
[ 52%] Building CXX object lib/googletest/googletest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o
[ 57%] Building CXX object lib/googletest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o
[ 57%] Built target FrequentPatternGrowth_run
[ 63%] Linking CXX static library libFrequentPatternGrowth_lib.a
[ 63%] Built target FrequentPatternGrowth_lib
Scanning dependencies of target FrequentPatternGrowth_test
[ 68%] Building CXX object test/CMakeFiles/FrequentPatternGrowth_test.dir/FPTest.cpp.o
[ 73%] Building CXX object test/CMakeFiles/FrequentPatternGrowth_test.dir/main.cpp.o
[ 78%] Linking CXX static library ../../libgtest_main.a
[ 78%] Built target gtest_main
[ 84%] Linking CXX static library ../../libgmock.a
[ 84%] Built target gmock
Scanning dependencies of target gmock_main
[ 89%] Building CXX object lib/googletest/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o
[ 94%] Linking CXX executable FrequentPatternGrowth_test
[ 94%] Built target FrequentPatternGrowth_test
[100%] Linking CXX static library ../../libgmock_main.a
[100%] Built target gmock_main


След тази команда има няколко изпълними файла.
$ find . -executable -type f
./test/FrequentPatternGrowth_test
./src/FrequentPatternGrowth_run

Важно: make -j4 трябва да се изпълни винаги, когато се променя кода на програмата.

За пускане на основната програма в build директорията напишете: ./src/FrequentPatternGrowth_run

За пускане на тестове: ./test/FrequentPatternGrowth_test
    Изход:
    [==========] Running 60 tests from 4 test suites.
[----------] Global test environment set-up.
[----------] 1 test from MyTest
[ RUN      ] MyTest.test1
[       OK ] MyTest.test1 (0 ms)
[----------] 1 test from MyTest (0 ms total)

[----------] 20 tests from FPGrowthPatternGroup1
[ RUN      ] FPGrowthPatternGroup1.Test1
[       OK ] FPGrowthPatternGroup1.Test1 (1 ms)
[ RUN      ] FPGrowthPatternGroup1.Test2
[       OK ] FPGrowthPatternGroup1.Test2 (0 ms)
[ RUN      ] FPGrowthPatternGroup1.Test3
[       OK ] FPGrowthPatternGroup1.Test3 (1 ms)
[ RUN      ] FPGrowthPatternGroup1.Test4
[       OK ] FPGrowthPatternGroup1.Test4 (0 ms)
[ RUN      ] FPGrowthPatternGroup1.Test5
[       OK ] FPGrowthPatternGroup1.Test5 (1 ms)
[ RUN      ] FPGrowthPatternGroup1.Test6
[       OK ] FPGrowthPatternGroup1.Test6 (0 ms)
[ RUN      ] FPGrowthPatternGroup1.Test7
[       OK ] FPGrowthPatternGroup1.Test7 (1 ms)
[ RUN      ] FPGrowthPatternGroup1.Test8
[       OK ] FPGrowthPatternGroup1.Test8 (0 ms)
[ RUN      ] FPGrowthPatternGroup1.Test9
[       OK ] FPGrowthPatternGroup1.Test9 (0 ms)
[ RUN      ] FPGrowthPatternGroup1.Test10
[       OK ] FPGrowthPatternGroup1.Test10 (1 ms)
[ RUN      ] FPGrowthPatternGroup1.Test11
[       OK ] FPGrowthPatternGroup1.Test11 (0 ms)
[ RUN      ] FPGrowthPatternGroup1.Test12
[       OK ] FPGrowthPatternGroup1.Test12 (1 ms)
[ RUN      ] FPGrowthPatternGroup1.Test13
[       OK ] FPGrowthPatternGroup1.Test13 (0 ms)
[ RUN      ] FPGrowthPatternGroup1.Test14
[       OK ] FPGrowthPatternGroup1.Test14 (1 ms)
[ RUN      ] FPGrowthPatternGroup1.Test15
[       OK ] FPGrowthPatternGroup1.Test15 (0 ms)
[ RUN      ] FPGrowthPatternGroup1.Test16
[       OK ] FPGrowthPatternGroup1.Test16 (1 ms)
[ RUN      ] FPGrowthPatternGroup1.Test17
[       OK ] FPGrowthPatternGroup1.Test17 (0 ms)
[ RUN      ] FPGrowthPatternGroup1.Test18
[       OK ] FPGrowthPatternGroup1.Test18 (1 ms)
[ RUN      ] FPGrowthPatternGroup1.Test19
[       OK ] FPGrowthPatternGroup1.Test19 (0 ms)
[ RUN      ] FPGrowthPatternGroup1.Test20
[       OK ] FPGrowthPatternGroup1.Test20 (1 ms)
[----------] 20 tests from FPGrowthPatternGroup1 (10 ms total)

[----------] 20 tests from FPGrowthPatternGroup2
[ RUN      ] FPGrowthPatternGroup2.Test1
[       OK ] FPGrowthPatternGroup2.Test1 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test2
[       OK ] FPGrowthPatternGroup2.Test2 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test3
[       OK ] FPGrowthPatternGroup2.Test3 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test4
[       OK ] FPGrowthPatternGroup2.Test4 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test5
[       OK ] FPGrowthPatternGroup2.Test5 (1 ms)
[ RUN      ] FPGrowthPatternGroup2.Test6
[       OK ] FPGrowthPatternGroup2.Test6 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test7
[       OK ] FPGrowthPatternGroup2.Test7 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test8
[       OK ] FPGrowthPatternGroup2.Test8 (1 ms)
[ RUN      ] FPGrowthPatternGroup2.Test9
[       OK ] FPGrowthPatternGroup2.Test9 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test10
[       OK ] FPGrowthPatternGroup2.Test10 (1 ms)
[ RUN      ] FPGrowthPatternGroup2.Test11
[       OK ] FPGrowthPatternGroup2.Test11 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test12
[       OK ] FPGrowthPatternGroup2.Test12 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test13
[       OK ] FPGrowthPatternGroup2.Test13 (1 ms)
[ RUN      ] FPGrowthPatternGroup2.Test14
[       OK ] FPGrowthPatternGroup2.Test14 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test15
[       OK ] FPGrowthPatternGroup2.Test15 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test16
[       OK ] FPGrowthPatternGroup2.Test16 (1 ms)
[ RUN      ] FPGrowthPatternGroup2.Test17
[       OK ] FPGrowthPatternGroup2.Test17 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test18
[       OK ] FPGrowthPatternGroup2.Test18 (1 ms)
[ RUN      ] FPGrowthPatternGroup2.Test19
[       OK ] FPGrowthPatternGroup2.Test19 (0 ms)
[ RUN      ] FPGrowthPatternGroup2.Test20
[       OK ] FPGrowthPatternGroup2.Test20 (0 ms)
[----------] 20 tests from FPGrowthPatternGroup2 (7 ms total)

[----------] 19 tests from FPGrowthPatternGroup3
[ RUN      ] FPGrowthPatternGroup3.Test1
[       OK ] FPGrowthPatternGroup3.Test1 (1 ms)
[ RUN      ] FPGrowthPatternGroup3.Test2
[       OK ] FPGrowthPatternGroup3.Test2 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test3
[       OK ] FPGrowthPatternGroup3.Test3 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test4
[       OK ] FPGrowthPatternGroup3.Test4 (1 ms)
[ RUN      ] FPGrowthPatternGroup3.Test5
[       OK ] FPGrowthPatternGroup3.Test5 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test6
[       OK ] FPGrowthPatternGroup3.Test6 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test7
[       OK ] FPGrowthPatternGroup3.Test7 (1 ms)
[ RUN      ] FPGrowthPatternGroup3.Test8
[       OK ] FPGrowthPatternGroup3.Test8 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test9
[       OK ] FPGrowthPatternGroup3.Test9 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test10
[       OK ] FPGrowthPatternGroup3.Test10 (1 ms)
[ RUN      ] FPGrowthPatternGroup3.Test11
[       OK ] FPGrowthPatternGroup3.Test11 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test12
[       OK ] FPGrowthPatternGroup3.Test12 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test13
[       OK ] FPGrowthPatternGroup3.Test13 (1 ms)
[ RUN      ] FPGrowthPatternGroup3.Test14
[       OK ] FPGrowthPatternGroup3.Test14 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test15
[       OK ] FPGrowthPatternGroup3.Test15 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test16
[       OK ] FPGrowthPatternGroup3.Test16 (1 ms)
[ RUN      ] FPGrowthPatternGroup3.Test17
[       OK ] FPGrowthPatternGroup3.Test17 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test18
[       OK ] FPGrowthPatternGroup3.Test18 (0 ms)
[ RUN      ] FPGrowthPatternGroup3.Test19
[       OK ] FPGrowthPatternGroup3.Test19 (1 ms)
[----------] 19 tests from FPGrowthPatternGroup3 (7 ms total)

[----------] Global test environment tear-down
[==========] 60 tests from 4 test suites ran. (24 ms total)
[  PASSED  ] 60 tests.
