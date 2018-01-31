#include <ProfileWeight.h>

// #include "xapian/weight.h"
// #include <cmath>
// #include <cstring>

// #include "debuglog.h"
// #include "omassert.h"
// #include "serialise-double.h"

// #include "xapian/error.h"


ProfileWeight::ProfileWeight() {
    need_stat(WDF);
    need_stat(WDF_MAX);
}


void ProfileWeight::init(double factor)
{
    term_factor = factor
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
    return wdf * term_factor;
}


double ProfileWeight::get_maxpart() const {
    Xapian::termcount wdf_max = get_wdf_upper_bound();
    return wdf * term_factor;
}

// There is no extra per document component in the ProfileWeighting scheme.
double ProfileWeight::get_sumextra(Xapian::termcount, Xapian::termcount) const {
    return 0;
}

double ProfileWeight::get_maxextra() const {
    return 0;
}

