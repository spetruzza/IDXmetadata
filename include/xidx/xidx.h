/*
 * Copyright (c) 2017 University of Utah
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef XIDX_H_
#define XIDX_H_

#include <cassert>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>

namespace xidx{
  typedef uint32_t INDEX_TYPE;
  typedef double PHY_TYPE;
}

#include "xidx_config.h"
#include "elements/xidx_parsable.h"
#include "xidx_data_source.h"
#include "elements/xidx_attribute.h"
#include "elements/xidx_types.h"
#include "elements/xidx_dataitem.h"

#include "xidx_index_space.h"
#include "elements/xidx_domain.h"
#include "elements/xidx_list_domain.h"
#include "elements/xidx_hyperslab_domain.h"
#include "elements/xidx_topology.h"
#include "elements/xidx_geometry.h"
#include "elements/xidx_spatial_domain.h"

#include "elements/xidx_variable.h"

namespace xidx {
typedef HyperSlabDomain TemporalHyperSlabDomain;
typedef ListDomain<PHY_TYPE> TemporalListDomain;
  
//template<typename T>
//using Axis = ListDomain<T>;
//class Axis : public Variable{
//  friend Variable;
//public:
//  Axis(Parsable* parent): Variable(parent){};
//  Axis(std::string name) : Variable(name){};
//};

typedef Variable Axis;
  
}

#include "elements/xidx_multiaxis_domain.h"
#include "elements/xidx_group.h"

#include "xidx_file.h"


#endif
