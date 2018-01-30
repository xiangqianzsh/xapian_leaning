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
        termgenerator.index_text(data_map.at("content"), 0);  // booleam term 
        // Add document values.
        std::string & score = data_map.at("score");
        std::string & x1 = data_map.at("x1");
        std::string & x2 = data_map.at("x2");
        std::string & x3 = data_map.at("x3");
        std::string & x4 = data_map.at("x4");
        std::string & x5 = data_map.at("x5");

        int x1_int = static_cast<int>(10000 * std::atof(x1.c_str()));
        int x2_int = static_cast<int>(10000 * std::atof(x2.c_str()));
        int x3_int = static_cast<int>(10000 * std::atof(x3.c_str()));
        int x4_int = static_cast<int>(10000 * std::atof(x4.c_str()));
        int x5_int = static_cast<int>(10000 * std::atof(x5.c_str()));
        
        data_map["x1"] = std::to_string((x1_int));
        data_map["x2"] = std::to_string((x2_int));
        data_map["x3"] = std::to_string((x3_int));
        data_map["x4"] = std::to_string((x4_int));
        data_map["x5"] = std::to_string((x5_int));
       
        doc.add_term("x1", x1_int);
        doc.add_term("x2", x2_int);
        doc.add_term("x3", x3_int);
        doc.add_term("x4", x4_int);
        doc.add_term("x5", x5_int);

        if (!score.empty()) {
            doc.add_value(0, Xapian::sortable_serialise(atoi(score.c_str())));
        }
        
        doc.add_value(1, Xapian::sortable_serialise(x1_int));
        doc.add_value(2, Xapian::sortable_serialise(x2_int));
        doc.add_value(3, Xapian::sortable_serialise(x3_int));
        doc.add_value(4, Xapian::sortable_serialise(x4_int));
        doc.add_value(5, Xapian::sortable_serialise(x5_int));
       
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