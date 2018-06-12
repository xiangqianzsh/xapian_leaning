#include <xapian.h>
#include <iostream>
 
using namespace std;
 
int main()
{
    try
    {
        std::vector<std::string> doc_strs = {
            "Community portal – Bulletin board, projects, resources and activities covering a wide range of Wikipedia areas.",
            "Help desk – Ask questions about using Wikipedia.",
            "Local embassy – For Wikipedia-related communication in languages other than English.",
            "Reference desk – Serving as virtual librarians, Wikipedia volunteers tackle your questions on a wide range of subjects.",
            "Site news – Announcements, updates, articles and press releases on Wikipedia and the Wikimedia Foundation.",
            "Village pump – For discussions about Wikipedia itself, including areas for technical issues and policies.",
        };
        //Open an Database for write
        Xapian::WritableDatabase db("index_data", Xapian::DB_CREATE_OR_OPEN);
 
        //Prepare TermGenerator, just split word by space, not chinese analysis
        Xapian::TermGenerator indexer;
 
        for (auto text : doc_strs) {
            //Make Document
            Xapian::Document doc;
            doc.set_data(text);
            indexer.set_document(doc);
            indexer.index_text(text, 1, "");     
            //Add Document to db
            db.add_document(doc);
            //Flush to disk
            db.commit();            
        }

    }
    catch(const Xapian::Error &e)
    {
        cout << e.get_description() << endl;
    }
    return 0;
}
