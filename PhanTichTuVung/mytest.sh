#! /bin/bash
./kplc ../test/example1.kpl | diff ../test/result1.txt -
./kplc ../test/example2.kpl | diff ../test/result2.txt -
./kplc ../test/example3.kpl | diff ../test/result3.txt -
./kplc ../test/example4.kpl | diff ../test/result4.txt -
./kplc ../test/example5.kpl | diff ../test/result5.txt -
./kplc ../test/example6.kpl | diff ../test/result6.txt -
