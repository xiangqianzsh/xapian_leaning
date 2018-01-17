#include <xapian.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "support.h"

using namespace std;

void index(const string & datapath, const string & dbpath)
{
    // Hardcode field offsets for simplicity.
    const size_t FIELD_ORDER = 0;
    const size_t FIELD_NAME = 1;
    const size_t FIELD_SCORE = 2;

    // Create or open the database we're going to be writing to.
    Xapian::WritableDatabase db(dbpath, Xapian::DB_CREATE_OR_OPEN);

    // Set up a TermGenerator that we'll use in indexing.
    Xapian::TermGenerator termgenerator;
    termgenerator.set_stemmer(Xapian::Stem("en"));

    ifstream csv(datapath.c_str());
    vector<string> fields;
    csv_parse_line(csv, fields);

    // Check the CSV header line matches our hard-code offsets.
    if (fields.at(FIELD_NAME) != "name" ||
    fields.at(FIELD_SCORE) != "score") {
    // The CSV format doesn't match what we expect.
    cerr << "CSV format has changed!" << endl;
    exit(1);
    }

    while (csv_parse_line(csv, fields)) {
    // 'fields' is a vector mapping from field number to value.
    // We look up fields with the 'at' method so we get an exception
    // if that field isn't set.
    const string & order = fields.at(FIELD_ORDER);
    const string & name = fields.at(FIELD_NAME);
    const string & score = fields.at(FIELD_SCORE);

    // We make a document and tell the term generator to use this.
    Xapian::Document doc;
    termgenerator.set_document(doc);

// Start of example code.
    // Index each field with a suitable prefix.
    termgenerator.index_text(name, 1, "S");
    // Add document values.
    if (!score.empty()) {
        doc.add_value(1, Xapian::sortable_serialise(atoi(score.c_str())));
    }
// End of example code.

    // Store all the fields for display purposes.
    doc.set_data(name + "\t" + score);

    // We use the order to ensure each object ends up in the
    // database only once no matter how many times we run the
    // indexer.
    string idterm = "Q" + order;
    doc.add_boolean_term(idterm);
    db.replace_document(idterm, doc);
    }
}

int main(int argc, char** argv) {
    index("test.csv", "db");
    
    return 0;
}
