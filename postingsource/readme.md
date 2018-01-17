# test ValuePostingSource
Add Python PostingSource example from Xappy to docs
https://trac.xapian.org/ticket/503

# complile
- linux:
g++ index.cpp -o index  support.cc -L/usr/local/xapian/lib `pkg-config --cflags --libs xapian-core` --std=c++14

./index

g++ search.cpp ExternalWeightPostingSource.cpp -o search -L/usr/local/xapian/lib `pkg-config --cflags --libs xapian-core` --std=c++14

./search


- mac:
g++ index.cpp -o index  support.cc -lxapian -std=c++14 

g++ search.cpp -o search -lxapian -std=c++14
