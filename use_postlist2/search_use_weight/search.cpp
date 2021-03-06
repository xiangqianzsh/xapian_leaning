#include <xapian.h>
#include <sys/time.h>  // timeval
#include <iostream>
#include <string>
#include <ProfileWeight.h>
using namespace std;

// return millisecond 毫秒 
int timeElapse(const timeval startTime, const timeval endTime) {
    // struct timeval {
    //     time_t tv_sec; /* seconds */
    //     suseconds_t tv_usec; /* microseconds */
    // };

    // millisecond 毫秒
    // microsecond 微秒
    // 1 second = 10^3 millisecond
    // 1 millisecond = 10^3 microsecond
    struct timeval diffTime;
    if (endTime.tv_usec < startTime.tv_usec) {
        diffTime.tv_sec = endTime.tv_sec - startTime.tv_sec - 1;
        diffTime.tv_usec = 1000000 + endTime.tv_usec - startTime.tv_usec;
    } else {
        diffTime.tv_sec = endTime.tv_sec - startTime.tv_sec;
        diffTime.tv_usec = endTime.tv_usec - startTime.tv_usec;
    }

    int diffMilliseconds = diffTime.tv_sec * 1000 + diffTime.tv_usec / 1000;
    return diffMilliseconds;
}

void search(const string & dbpath, const std::string & term,
       Xapian::doccount offset = 0, Xapian::doccount pagesize = 10) {
    // offset - defines starting point within result set.
    // pagesize - defines number of records to retrieve.

    // Open the database we're going to search.
    Xapian::Database db(dbpath);
    Xapian::Query query_bool = Xapian::Query(Xapian::Query::OP_SCALE_WEIGHT, Xapian::Query(term), 0);
    
    std::vector<Xapian::Query> query_vec;
    query_vec.push_back(Xapian::Query(Xapian::Query::OP_SCALE_WEIGHT, Xapian::Query("x1"), 0.5));
    query_vec.push_back(Xapian::Query(Xapian::Query::OP_SCALE_WEIGHT, Xapian::Query("x2"), 0.7));
    query_vec.push_back(Xapian::Query(Xapian::Query::OP_SCALE_WEIGHT, Xapian::Query("x3"), 0.2));
    query_vec.push_back(Xapian::Query(Xapian::Query::OP_SCALE_WEIGHT, Xapian::Query("x4"), 0.4));
    query_vec.push_back(Xapian::Query(Xapian::Query::OP_SCALE_WEIGHT, Xapian::Query("x5"), 0.9));
    Xapian::Query query_weight = Xapian::Query(Xapian::Query::OP_OR, query_vec.begin(), query_vec.end());

    Xapian::Query query = Xapian::Query(Xapian::Query::OP_AND, query_bool, query_weight);

    // Use an Enquire object on the database to run the query.
    Xapian::Enquire enquire(db);
    enquire.set_query(query);
    enquire.set_weighting_scheme(ProfileWeight());

    // And print out something about each match.
    Xapian::MSet mset = enquire.get_mset(offset, pagesize);

    for (Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m) {
        Xapian::docid did = *m;
        const string& data = m.get_document().get_data();

        cout << "did: " << did
            << ", weight: " << m.get_weight()
            << ", doc_data: " << data << endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " DBPATH QUERYTERM PAGESIZE" << endl;
        return 1;
    }
    const char * dbpath = argv[1];
    char * bool_term = argv[2];
    int pagesize = std::stoi(argv[3]);

    struct timeval startTime;
    gettimeofday(&startTime, NULL);
    
    search(dbpath, bool_term, 0, pagesize);
    
    struct timeval endTime;
    gettimeofday(&endTime,NULL);
    std::cout << "startTime: " << startTime.tv_sec << "(s), " << startTime.tv_usec << "(us)" << std::endl;
    std::cout << "endTime: " << endTime.tv_sec << "(s), " << endTime.tv_usec << "(us)" << std::endl;
    std::cout << "elapse time: " << timeElapse(startTime, endTime) << "(ms)" << std::endl;
}
