#include <xapian>


/// Xapian::Weight subclass implementing the tf-idf weighting scheme.
class ProfileWeight : public Xapian::Weight {
    double term_factor;
    void init(double factor);
  public:
    ProfileWeight();


    std::string name() const;

    std::string short_name() const;

    // std::string serialise() const;
    // ProfileWeight * unserialise(const std::string & serialised) const;

    double get_sumpart(Xapian::termcount wdf,
               Xapian::termcount doclen,
               Xapian::termcount uniqterm) const;
    double get_maxpart() const;

    double get_sumextra(Xapian::termcount doclen,
            Xapian::termcount uniqterms) const;
    double get_maxextra() const;

    // ProfileWeight * create_from_parameters(const char * params) const;
};
