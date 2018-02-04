#include <ProfileWeight.h>
#include <iostream>

ProfileWeight::ProfileWeight() {
    need_stat(WDF);
    need_stat(WDF_MAX);
}


void ProfileWeight::init(double factor)
{
    // std::cout << "ProfileWeight::init() factor " << factor << std::endl;
    termweight = factor;
}

ProfileWeight * ProfileWeight::clone() const {
    return new ProfileWeight();
}

std::string ProfileWeight::name() const
{
    return "ProfileWeight";
}

std::string
ProfileWeight::short_name() const
{
    return "pfweight";
}


double ProfileWeight::get_sumpart(Xapian::termcount wdf, Xapian::termcount doclen,
             Xapian::termcount uniqterms) const {
    // std::cout << "ProfileWeigth::get_sumpart() wdf " << wdf << std::endl;
    return termweight * wdf;
}


double ProfileWeight::get_maxpart() const {
    Xapian::termcount wdf_max = get_wdf_upper_bound();
    // std::cout << "ProfileWeigth::get_maxpart() wdf_max " << wdf_max << std::endl;
    return termweight * wdf_max;
}

// There is no extra per document component in the ProfileWeighting scheme.
double ProfileWeight::get_sumextra(Xapian::termcount, Xapian::termcount) const {
    return 0;
}

double ProfileWeight::get_maxextra() const {
    return 0;
}


