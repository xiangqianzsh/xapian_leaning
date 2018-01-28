# introduction
This is a demo for testing the performance of PostingSource and KeyMaker.

I generate 30 million documents, all documents contain term of t1, 1/2 of the documents contain term of t2, 1/3 of the documents contain term of t3, ..., 1/20 of the documents contain term of t20. For simple, let the score of each document equals to the docid of the documents. The data looks like this
```
id,content,score
1,t1,1
2,t1 t2,2
3,t1 t3,3
4,t1 t2 t4,4
5,t1 t5,5
6,t1 t2 t3 t6,6
7,t1 t7,7
8,t1 t2 t4 t8,8
9,t1 t3 t9,9
```

While searching, we first use one term (for example t1) to choose some documents, and then sort them descending using 0.5 * doc.get_value(1), i.e 0.5 * the score of the documents.

I was testing on my own mac book, the performance difference is very large.
```
for term t1:
using PostingSource:
    real    1m8.778s
    user    0m47.718s
    sys 0m18.062s
using KeyMaker
    real    0m10.737s
    user    0m10.379s
    sys 0m0.160s

for term t2:
using PostingSource:
    real    0m36.338s
    user    0m24.102s
    sys 0m9.935s
using KeyMaker
    real    0m5.607s
    user    0m5.440s
    sys 0m0.110s
```

# structure
use `sh run_mac.sh` to generate in your own computer and create index database.
```
.
├── data   # generate data
│   ├── data.csv
│   └── gen_test_data.py
├── db    # database
│   ├── docdata.glass
│   ├── flintlock
│   ├── iamglass
│   ├── position.glass
│   ├── postlist.glass
│   └── termlist.glass
├── index   # index
│   ├── index
│   ├── index.cpp
│   ├── support.cc
│   └── support.h
├── readme.md
├── run_mac.sh   # main scripts
├── search_use_keymaker
│   ├── search
│   └── search.cpp
└── search_use_postingsource
    ├── ExternalWeightPostingSource.cpp
    ├── ExternalWeightPostingSource.h
    ├── search
    └── search.cpp

```

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

g++ search.cpp ExternalWeightPostingSource.cpp  -o search -lxapian -std=c++14

g++ search.cpp -o search -lxapian -std=c++14
