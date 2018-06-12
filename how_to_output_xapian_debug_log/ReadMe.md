# linux
on linux:
g++ index.cpp -o index  -L/usr/local/xapian/lib `pkg-config --cflags --libs xapian-core` --std=c++11
g++ search.cpp -o search  -L/usr/local/xapian/lib `pkg-config --cflags --libs xapian-core` --std=c++11


on mac:
g++ index.cpp -o index -lxapian -std=c++11
g++ search.cpp -o search -lxapian -std=c++11


# output debug log
rm -rf index_data
./index  
./index  # twice
export XAPIAN_DEBUG_LOG=-  # - means output log to stderr, also you can set a file path.
export XAPIAN_DEBUG_FLAGS=-  # - means output all category logs.
./search > xapian-log 2>&1 