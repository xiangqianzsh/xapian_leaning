# on mac book
DIR=$(cd `dirname $0`; pwd)
DATA_DIR=${DIR}/data
DATA_PATH=${DIR}/data/data.csv
DB_DIR=${DIR}/db_tmp
INDEX_DIR=${DIR}/index

# step1, gen data
echo "[step1] [`date`] Generating data ..."
cd ${DATA_DIR}
# python gen_test_data.py 30000000  > ${DATA_PATH} # python2, sys.argv[1] means the number of documents
python gen_test_data.py 3000  > ${DATA_PATH} # python2, sys.argv[1] means the number of documents

# step2, create index database
echo "[step2] [`date`] Index ..."
cd ${INDEX_DIR}
g++ index.cpp -o index  support.cc  -lxapian --std=c++14
rm -rf ${DB_DIR}
./index ${DATA_PATH} ${DB_DIR}

# step3, search_use_weight
echo "[step3] [`date`] search_use_weight ..."
cd ${DIR}/search_use_weight
g++ search.cpp ProfileWeight.cpp  -o search -lxapian -std=c++14 -I.
time ./search ${DB_DIR} t1 10

# step4, search_use_keymaker
echo "[step4] [`date`] search_use_keymaker ..."
cd ${DIR}/search_use_keymaker
g++ search.cpp -o search -lxapian -std=c++14
time ./search ${DB_DIR} t1 10

#  step5, search_use_keymaker_inmmeory
echo "[step5] [`date`] search_use_keymaker_inmmeory ..."
cd ${DIR}/search_use_keymaker_inmmeory
g++ search.cpp -o search -lxapian -std=c++14
time ./search ${DB_DIR} t1 10


# step6, search_use_keymaker_inmmeory_use_double
echo "[step6] [`date`] search_use_keymaker_inmmeory_use_double ..."
cd ${DIR}/search_use_keymaker
g++ search.cpp -o search -lxapian -std=c++14
time ./search ${DB_DIR} t1 10