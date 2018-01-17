#include <xapian.h>

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>

#include "support.h"
#include "ExternalWeightPostingSource.h"
using namespace std;


class TestWeightSource: public WeightSource {
public:
    TestWeightSource(double scale_) {
        this->scale = scale_;
    }
    double get_maxweight() {
        return 10000.;
    }
    double get_weight(Xapian::Document & doc) const {
        double weight = Xapian::sortable_unserialise(doc.get_value(1)) * scale;
        // std::cout << "weight: " << weight << std::endl;
        return weight;
    }

private:
    double scale;
};


static void
search(const string & dbpath,
       Xapian::doccount offset = 0, Xapian::doccount pagesize = 10)
{
    int score_slot = 1;
    // offset - defines starting point within result set.
    // pagesize - defines number of records to retrieve.

    // Open the database we're going to search.
    Xapian::Database db(dbpath);

    Xapian::Query query("Sa1");

    // // test ok using Xapian::ValueWeightPostingSource
    // Xapian::ValueWeightPostingSource vps(score_slot);
    // Xapian::Query query_vps(&vps);
    // query = Xapian::Query(query.OP_AND_MAYBE, query, query_vps);
    // cout << "query serialise: " << query.serialise() << endl;


    std::shared_ptr<WeightSource> twtsPtr(new TestWeightSource(0.5));
    ExternalWeightPostingSource extwps(twtsPtr);
    Xapian::Query query_extwps(&extwps);
    query = Xapian::Query(query.OP_AND_MAYBE, query, query_extwps);

    // Use an Enquire object on the database to run the query.
    Xapian::Enquire enquire(db);
    enquire.set_query(query);
    enquire.set_weighting_scheme(Xapian::BoolWeight());

    // And print out something about each match.
    Xapian::MSet mset = enquire.get_mset(offset, pagesize);

    for (Xapian::MSetIterator m = mset.begin(); m != mset.end(); ++m) {

    Xapian::docid did = *m;
    const string& data = m.get_document().get_data();

    cout << "did: " << did
        << ", weight: " << m.get_weight()
        << ", score: " << Xapian::sortable_unserialise(m.get_document().get_value(score_slot))
        << ", data: " << data << endl;

    }
}

int main(int argc, char** argv) {
    search("db");
}
