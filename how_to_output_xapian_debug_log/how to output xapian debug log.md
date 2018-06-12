
# 如何输出xapian-debug的log
xapian内部关系调用比较复杂, 想要了解内部的函数调用关系比较有难度, 好在内部使用LOGCALL_CTOR等输出了很多log, 如何打印这些log呢? 命令如下
```
cd xapian-core-1.4.5  # xapin-source downloaded
# install
mkdir BUILD
cd BUILD
../configure --enable-log=yes --enable-assertions=yes

# execute on a demo
export XAPIAN_DEBUG_LOG=-  # - means output log to stderr, also you can set a file path.
export XAPIAN_DEBUG_FLAGS=-  # - means output all category logs.
./search_demo_exe
```

log examples
```
logging)
D 1109    [0x7fb14b001ab8] void GlassTable::open(-1, Glass::RootInfo, 12) returned
D 1109    [0x7fb14b0018e0] void GlassTable::open(-1, Glass::RootInfo, 12)
D 1109     [0x7fb14b0018e0] void GlassTable::close(false)
D 1109     [0x7fb14b0018e0] void GlassTable::close(false) returned
D 1109     [0x7fb14b0018e0] bool GlassTable::do_open_to_read(&Glass::RootInfo, 12)
D 1109     [0x7fb14b0018e0] bool GlassTable::do_open_to_read(&Glass::RootInfo, 12) returned (not marked up for return logging)
D 1109    [0x7fb14b0018e0] void GlassTable::open(-1, Glass::RootInfo, 12) returned
D 1109    [0x7fb14b0016b0] void GlassTable::open(-1, Glass::RootInfo, 12)
D 1109     [0x7fb14b0016b0] void GlassTable::close(false)
D 1109     [0x7fb14b0016b0] void GlassTable::close(false) returned
```

参考:
xapian-core-1.4.5/INSTALL
xapian-core-1.4.5/HACKING


mac 安装xapian
https://blog.csdn.net/livefun/article/details/18043429
[Xapian-discuss] how to debug xapian?
http://grokbase.com/t/xapian/xapian-discuss/09b3ba97br/how-to-debug-xapian
Ticket #156: HACKING
https://trac.xapian.org/attachment/ticket/156/HACKING

