#ifndef PARSE_H
#define PARSE_H


#include <fstream>
#include <memory>

#include "SA_data.h"

namespace std {


SA_data *parse(fstream &hardblock, fstream &net, fstream &pl, fp drc);


}  // namespace std



#endif /* PARSE_H */
