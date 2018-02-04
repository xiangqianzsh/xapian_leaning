# introduction
This is a demo for testing the performance of Weight and KeyMaker.

I generate 30 million documents, all documents contain term of t1, 1/2 of the documents contain term of t2, 1/3 of the documents contain term of t3, ..., 1/20 of the documents contain term of t20. For simple, let the score of each document equals to the docid of the documents, x1 to x5 is a random number from uniform(0, 1). The data looks like this
```
id,content,score,x1,x2,x3,x4,x5
1,t1,1,0.134364244112,0.847433736937,0.763774618977,0.255069025739,0.495435087092
2,t1 t2,2,0.449491064789,0.651592972723,0.788723351136,0.0938595867742,0.028347476522
3,t1 t3,3,0.83576510392,0.432767067905,0.762280082458,0.00210605335111,0.445387194055
4,t1 t2 t4,4,0.721540032341,0.22876222127,0.945270695554,0.901427457611,0.0305899830336
5,t1 t5,5,0.0254458609935,0.541412472793,0.939149162779,0.381204237688,0.216599397131
6,t1 t2 t3 t6,6,0.422116575583,0.0290407875749,0.221691666273,0.437887593651,0.495812241382
```

While index, we set t1 to t20 as booleam term, add set int(10000 * x1) to the 1st value slot, int(10000 * x2) to the 2nd value slot, ... set int(10000 * x5) to the 5-th value slot.

While searching, we first use one term (for example t1) to choose some documents, and then sort them descending using `0.5 * doc.get_value(1)) + 0.7 * doc.get_value(2)) + 0.2 * doc.get_value(3)) + 0.4 * doc.get_value(4)) + 0.9 * doc.get_value(5))`, i.e. `10000 * (0.5 * x1 + 0.7 * x2 + 0.2 * x3 + 0.4 * x4 + 0.9 * x5)`.

We compare the preformance of four methods: search_use_weight, search_use_keymaker, search_use_keymaker_inmmeory and search_use_keymaker_inmmeory_use_double.

# 结论
1. 测试的性能不是太稳定, 可能是与计算机做的一些cache有关.
2. search_use_keymaker_inmmeory_use_double目前看是比较占优的.
2. search_use_keymaker_inmmeory_use_double的性能大概是search_use_keymaker_inmmeory的1.2-2倍左右.
3. 查t1(全部3000万文档)时, search_use_keymaker_inmmeory比search_use_weight用时要多, 但随着查询文档的数据减少, search_use_keymaker_inmmeory逐渐变好, 查t10(十分一的文档)时打平, 查t20时反超. 原因还不清楚.
4. 从代码的log里看出`ProfileWeight::get_sumpart()`函数每次都被调用, 原因是目前x1-x5的倒排里均包含了所有文档, 当前剪枝策略不会跳过任何文档.

# 其它
如果需要计算的倒排不是非全链的, 那么剪枝策略就可以生效, 速度就可以提高.
或是倒排索引能记录局部最大值信息, 可以设计基于局部最大值的剪枝策略.



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
