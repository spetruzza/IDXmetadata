

#ifndef IDX_METADATA_LAYOUT_H_
#define IDX_METADATA_LAYOUT_H_

#include <string>

#include "idx_metadata.h"
#include "idx_metadata_datatypes.h"

namespace idx_metadata{

class IDX_Metadata;
class IDX_Metadata_Layout{

public:

  IDX_Metadata* metadata;

  IDX_Metadata_Layout(IDX_Metadata* idx_metadata)
  : metadata(idx_metadata){};

  virtual int load() = 0;
  virtual int save() = 0;
  virtual std::string get_file_path(int timestep, int level, CenterType ctype) = 0;

};

};

#endif