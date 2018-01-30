# on mac book

DIR=$(cd `dirname $0`; pwd)

# step1, gen data
echo "[step1] [`date`] Generating data ..."
cd ${DIR}/data
python gen_test_data.py 30000000  > data.csv # python2, sys.argv[1] means the number of documents

# step2, create index database
echo "[step2] [`date`] Index ..."
cd ${DIR}/index
g++ index.cpp -o index  support.cc -lxapian -std=c++14
rm -rf ${DIR}/db
./index ${DIR}/data/data.csv ${DIR}/db

# step3, search use posting source
echo "[step3] [`date`] search_use_postingsource ..."
cd ${DIR}/search_use_postingsource
g++ search.cpp ExternalWeightPostingSource.cpp  -o search -lxapian -std=c++14
time ./search ${DIR}/db t1

# step4, search use KeyMaker
echo "[step4] [`date`] search_use_keymaker ..."
cd ${DIR}/search_use_keymaker
g++ search.cpp -o search -lxapian -std=c++14
time ./search ${DIR}/db t1
