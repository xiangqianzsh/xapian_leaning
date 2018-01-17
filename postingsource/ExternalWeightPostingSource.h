#include <xapian.h>
#include <memory>

// only use for generate weight and max weight
class WeightSource {
public:
    WeightSource() {};
    virtual ~WeightSource() {};
    virtual double get_maxweight() = 0;
    virtual double get_weight(Xapian::Document & doc) const = 0;
};

class ExternalWeightPostingSource: public Xapian::PostingSource {
public:
    ExternalWeightPostingSource(std::shared_ptr<WeightSource> wtsourcePtr_);
    void init(const Xapian::Database & db_);
    Xapian::doccount get_termfreq_min() const;
    Xapian::doccount get_termfreq_est() const;
    Xapian::doccount get_termfreq_max() const;
    Xapian::docid get_docid() const;
    double get_maxweight();
    double get_weight() const;
    void next(double min_wt);
    bool at_end() const;
    void skip_to(Xapian::docid min_docid, double min_wt);
    bool check(Xapian::docid min_docid, double);

private:
    // wtsource implement get_weight() and get_maxweight() function.
    std::shared_ptr<WeightSource> wtsourcePtr;
    /// The database we're reading documents from.
    Xapian::Database db;
    /// Number of documents in the posting source.
    Xapian::doccount termfreq;
    /// Iterator over all documents.
    Xapian::PostingIterator it;
    /// Flag indicating if we've started (true if we have).
    bool started;
    /// The docid last passed to check() (0 if check() wasn't the last move).
    Xapian::docid check_docid;
};



