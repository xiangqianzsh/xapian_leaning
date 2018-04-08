# introduction
We have a search service based on xapian, when receving a request, we create serveral threads to create xapian-query and search in the corresponding databases, and then merge all results together.

In some case, we use Xapian::Query::MatchAll to create the query, but it always has `Segmentation fault (core dumped)`. It looks like some pointers are double freed.

You will get a core file if you run `./match_all` command several times.
```
[admin@localhost xapian_test]$ ./match_all
In thread In thread In thread 4In thread 35

In thread 7
In thread 6
In thread In thread 81


2
In thread 9
In thread 10
Segmentation fault (core dumped)
```

Error information looks like this When using gdb to detect the core file. 
```
#0  0x00007efe6f57de1b in ~intrusive_ptr (this=<optimized out>, __in_chrg=<optimized out>) at ./include/xapian/intrusive_ptr.h:112
112         if( px != 0 && --px->_refs == 0 ) delete px;
#8  0x0000000000401cca in Xapian::Internal::intrusive_ptr<Xapian::Query::Internal>::~intrusive_ptr() ()
#9  0x00000000004019cc in Xapian::Query::~Query() ()
```

# compile
g++ match_all.cpp -o match_all -lxapian -std=c++14
