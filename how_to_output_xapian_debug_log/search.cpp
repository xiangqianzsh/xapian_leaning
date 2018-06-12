#include <xapian.h>
#include <iostream>
#include <vector>
 
using namespace std;
 
#define QUERY "title:新闻 AND content:男篮"
 
#define INDEX_PATH "./index_data"
 
#define F_DOCID 1
 
int main()
{
    try
    {
        //Query
        string query_str(QUERY);
 
        //Open an Database for read
        Xapian::Database db(string(INDEX_PATH));
 
        //Open Search Handle
        Xapian::Enquire enquire(db);
        enquire.set_weighting_scheme(Xapian::BoolWeight());
        
        /*
        //Parser Query
        Xapian::QueryParser qp;
        qp.add_prefix("title", "T");
        qp.add_prefix("content", "C");
        Xapian::Query query = qp.parse_query(query_str);
        cout << "Query is " << query.get_description() << endl;
        */
        
        std::vector<std::string> vec = {"questions", "about", "using", "Wikipedia"};
        Xapian::Query query_or = Xapian::Query(Xapian::Query::OP_OR, vec.begin(), vec.end());
        std::vector<std::string> vec2 = {"discussions", "virtual"};
        Xapian::Query query_not = Xapian::Query(Xapian::Query::OP_OR, vec2.begin(), vec2.end());

        Xapian::Query query = Xapian::Query(Xapian::Query::OP_AND_NOT, query_or, query_not);

        // Xapian::Query query3 = Xapian::Query(Xapian::Query::OP_AND, query2, query1);
        // Xapian::Query query4 = Xapian::Query(Xapian::Query::OP_AND_NOT, query1, query2);
        // Xapian::Query query5 = Xapian::Query(Xapian::Query::OP_AND_NOT, query2, query1);
        
        

        //Find top 10 results
        enquire.set_query(query);
        Xapian::MSet result = enquire.get_mset(0, 10);
        cout << result.get_matches_estimated() << " results found" << endl;
 
        //Print results
        for(Xapian::MSetIterator itr = result.begin(); itr!=result.end(); itr++)
        {
            Xapian::Document doc = itr.get_document();
            cout << itr.get_rank() << ": docid " << doc.get_value(F_DOCID) << ", data " << doc.get_data() << endl;
        }
    }
    catch(const Xapian::Error &e)
    {
        cout << e.get_description() << endl;
    }
    return 0;
}
