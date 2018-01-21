#include <xapian.h>
#include "ExternalWeightPostingSource.h"
using namespace std;

ExternalWeightPostingSource::ExternalWeightPostingSource(std::shared_ptr<WeightSource> wtsourcePtr_)
        :wtsourcePtr(wtsourcePtr_) {};

void ExternalWeightPostingSource::init(const Xapian::Database & db_) {
    this->db = db_;
    termfreq = db_.get_doccount();
    started = false;
    check_docid = 0;
}

Xapian::doccount ExternalWeightPostingSource::get_termfreq_min() const {
    return 0;
}
Xapian::doccount ExternalWeightPostingSource::get_termfreq_est() const {
    return termfreq;
}
Xapian::doccount ExternalWeightPostingSource::get_termfreq_max() const {
    return termfreq;        
}

Xapian::docid ExternalWeightPostingSource::get_docid() const {
    if (check_docid != 0) return check_docid;
    return *it;
}

double ExternalWeightPostingSource::get_maxweight() {
    return wtsourcePtr->get_maxweight();
}

double ExternalWeightPostingSource::get_weight() const {
    Xapian::Document doc = this->db.get_document(get_docid());
    return wtsourcePtr->get_weight(doc);
}

void ExternalWeightPostingSource::next(double min_wt) {
    if (!started) {
        started = true;
        it = db.postlist_begin(string());
    } else {
        ++it;
    }

    if (it == db.postlist_end(string())) return;

    if (check_docid) {
        it.skip_to(check_docid + 1);
        check_docid = 0;
    }

    if (min_wt > get_maxweight()) {
        it = db.postlist_end(string());
    }
}

bool ExternalWeightPostingSource::at_end() const {
    if (check_docid != 0) return false;
    return started && it == db.postlist_end(string());
}

void ExternalWeightPostingSource::skip_to(Xapian::docid min_docid, double min_wt) {
    if (!started) {
        started = true;
        it = db.postlist_begin(string());

        if (it == db.postlist_end(string())) return;
    }

    if (check_docid) {
        if (min_docid < check_docid)
            min_docid = check_docid + 1;
        check_docid = 0;
    }

    if (min_wt > get_maxweight()) {
        it = db.postlist_end(string());
        return;
    }
    it.skip_to(min_docid);
}

bool ExternalWeightPostingSource::check(Xapian::docid min_docid, double) {
    // We're guaranteed not to be called if the document doesn't
    // exist, so just remember the docid passed, and return true.
    check_docid = min_docid;
    return true;
}



