#include <xapian.h>
#include <sys/time.h>  // timeval
#include <iostream>
#include <string>
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

class InMemoryDoc {
public:
    void set_values_size(const int size) {
        slot_values.resize(size + 1);   // the first slot is not used.
    }

    std::string get_value(const int idx) const {
        return slot_values[idx];
    }
    
    void set_value(const int idx, const std::string value) {
        slot_values[idx] = value;
    }

private:
    std::vector<std::string> slot_values;
};

class InMemoryDB {
public:
    void load_from_db(const Xapian::Database db) {
        Xapian::docid doc_size = db.get_doccount();
        docs.resize(doc_size + 1);  // the first doc is not used.
        for (Xapian::docid i = 1; i <= doc_size; i++) {
            if (i % 1000000 == 0) {
                std::cout << "load docid " << i << std::endl;
            }
            auto doc = db.get_document(i);
            auto & memoryDoc = docs[i];
            memoryDoc.set_values_size(5);
            memoryDoc.set_value(1, doc.get_value(1));
            memoryDoc.set_value(2, doc.get_value(2));
            memoryDoc.set_value(3, doc.get_value(3));
            memoryDoc.set_value(4, doc.get_value(4));
            memoryDoc.set_value(5, doc.get_value(5));
        }  
    }

    InMemoryDoc & get_doc(Xapian::docid docid) {
        return docs[docid];
    }
private:
    std::vector<InMemoryDoc> docs;
};



class MyKeyMaker : public Xapian::KeyMaker {
public:
    MyKeyMaker(InMemoryDB * db_ptr) {
        this->db_ptr = db_ptr;
    }

private:
    std::string operator() (const Xapian::Document &doc) const {
        InMemoryDoc & inMemoryDoc = db_ptr->get_doc(doc.get_docid());
        return Xapian::sortable_serialise(
                0.5 * Xapian::sortable_unserialise(inMemoryDoc.get_value(1)) + 
                0.7 * Xapian::sortable_unserialise(inMemoryDoc.get_value(2)) + 
                0.2 * Xapian::sortable_unserialise(inMemoryDoc.get_value(3)) + 
                0.4 * Xapian::sortable_unserialise(inMemoryDoc.get_value(4)) + 
                0.9 * Xapian::sortable_unserialise(inMemoryDoc.get_value(5)) 
                );
    }

    InMemoryDB * db_ptr;
};

void search(const string & dbpath, const std::string & term, InMemoryDB & inMemoryDB,
       Xapian::doccount offset = 0, Xapian::doccount pagesize = 10) {
    // offset - defines starting point within result set.
    // pagesize - defines number of records to retrieve.

    // Open the database we're going to search.
    Xapian::Database db(dbpath);
    Xapian::Query query(term);

    // Use an Enquire object on the database to run the query.
    Xapian::Enquire enquire(db);
    enquire.set_query(query);
    enquire.set_weighting_scheme(Xapian::BoolWeight());
    MyKeyMaker myKeyMaker(&inMemoryDB);
    enquire.set_sort_by_key(&myKeyMaker, true);

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
    
    Xapian::Database db(dbpath);
    InMemoryDB inMemoryDB;
    inMemoryDB.load_from_db(db);

    while (true) {
        struct timeval startTime;
        gettimeofday(&startTime, NULL);
        
        search(dbpath, bool_term, inMemoryDB, 0, pagesize);
        
        struct timeval endTime;
        gettimeofday(&endTime,NULL);
        std::cout << "startTime: " << startTime.tv_sec << "(s), " << startTime.tv_usec << "(us)" << std::endl;
        std::cout << "endTime: " << endTime.tv_sec << "(s), " << endTime.tv_usec << "(us)" << std::endl;
        std::cout << "elapse time: " << timeElapse(startTime, endTime) << "(ms)" << std::endl;

        std::cout << std::endl << "=============another query===========" << std::endl
            << "Please input query_term  pagesize" << std::endl;
        std::cin >> bool_term >> pagesize;
    }

    return 0;
}

