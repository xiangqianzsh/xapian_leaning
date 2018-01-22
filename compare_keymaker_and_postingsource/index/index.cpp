#include <xapian.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "support.h"

using namespace std;

void display(const std::vector<string> & vec) {
    for (auto str : vec) {
        std::cout << str << ", ";
    }
    std::cout << std::endl;
}


std::string get_data_map_str(std::map<std::string, std::string> & data_map) {
    std::string ret;
    bool is_begin = true;
    for (auto it = data_map.begin(); it != data_map.end(); it++) {
        if (is_begin) {
            ret += (it->first + ":" + it->second);
            is_begin = false;
        } else {
            ret += (", " + it->first + ":" + it->second);
        }
    }
    return ret;
}

void set_data_map(const std::vector<string> & header,
    const std::vector<string> & fields,
    std::map<std::string, std::string> * data_map_ptr) {
    for (int i = 0; i < header.size(); i++) {
        auto & key = header[i];
        auto & value = fields[i];
        (*data_map_ptr)[key] = value;
    }

}

void index(const string & datapath, const string & dbpath)
{

    // Create or open the database we're going to be writing to.
    Xapian::WritableDatabase db(dbpath, Xapian::DB_CREATE_OR_OPEN);

    // Set up a TermGenerator that we'll use in indexing.
    Xapian::TermGenerator termgenerator;
    // termgenerator.set_stemmer(Xapian::Stem("en"));

    ifstream csv(datapath.c_str());
    // header
    vector<string> header;
    csv_parse_line(csv, header);
    display(header);
    size_t header_size = header.size();

    std::map<std::string, std::string> data_map;
    vector<string> fields;
    int count = 0;
    while (csv_parse_line(csv, fields)) {
        if ((++count) % 1000000 == 0) {
            std::cout << "INFO Current record id " << count << std::endl;
        }
        if (fields.size() != header_size) {
            std::cout << "Error fields.size() != header_size" << std::endl;
            continue;
        }


        set_data_map(header, fields, &data_map);
        // std::cout << "New line " << get_data_map_str(data_map) << std::endl;

        Xapian::Document doc;
        termgenerator.set_document(doc);

        // Index each field with a suitable prefix.
        termgenerator.index_text(data_map.at("content"), 1);
        // Add document values.
        std::string & score = data_map.at("score");
        if (!score.empty()) {
            doc.add_value(1, Xapian::sortable_serialise(atoi(score.c_str())));
        }

        // Store all the fields for display purposes.
        doc.set_data(get_data_map_str(data_map));

        // We use the id to ensure each object ends up in the
        // database only once no matter how many times we run the
        // indexer.
        string idterm = "Q" + data_map.at("id");
        doc.add_boolean_term(idterm);
        db.replace_document(idterm, doc);
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " DATAPATH DBPATH..." << endl;
        return 1;
    }
    const char * datapath = argv[1];
    const char * dbpath = argv[2];
    index(datapath, dbpath);
    
    return 0;
}
