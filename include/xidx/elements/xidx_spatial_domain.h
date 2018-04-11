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

#ifndef XIDX_SPATIAL_DOMAIN_H_
#define XIDX_SPATIAL_DOMAIN_H_

#include "xidx/xidx.h"

namespace xidx{

class SpatialDomain : public Domain{

public:
  
  SpatialDomain(std::string _name) : Domain(_name){
    type = DomainType::SPATIAL_DOMAIN_TYPE;
  };
  
  SpatialDomain(const SpatialDomain* dom) : Domain(dom->name){
    SetParent(dom->GetParent());
    topology = dom->topology;
    geometry = dom->geometry;
  }
  
  Topology topology;
  Geometry geometry;
  
  int SetTopology(Topology _topology) { topology = _topology; return 0; }
  
  int SetTopology(TopologyType type, uint32_t dims){
    topology.dimensions = ToIndexVector(string_format("%d", dims));
    topology.type = type;
    
    return 0;
  }
  
  int SetTopology(TopologyType type, int n_dims, uint32_t* dims){
    for(int i=0; i< n_dims; i++)
      topology.dimensions.push_back(dims[i]);
    
    topology.type = type;
    
    return 0;
  }
  
  int SetGeometry(Geometry _geometry) { geometry = _geometry; return 0; }

  int SetGeometry(GeometryType type, int n_dims, const double* ox_oy_oz,
                  const double* dx_dy_dz=NULL) {
    geometry.type = type;
    
    DataItem item_o(this);
    item_o.format_type = FormatType::XML_FORMAT;
    item_o.number_type = NumberType::FLOAT_NUMBER_TYPE;
    item_o.bit_precision = "32";
    item_o.endian_type = EndianType::LITTLE_ENDIANESS;
    DataItem item_d(this);
    item_d.format_type = FormatType::XML_FORMAT;
    item_d.number_type = NumberType::FLOAT_NUMBER_TYPE;
    item_d.bit_precision = "32";
    item_d.endian_type = EndianType::LITTLE_ENDIANESS;
    
    item_o.dimensions.push_back(n_dims);
    item_d.dimensions.push_back(n_dims);
    
    if(type == GeometryType::RECT_GEOMETRY_TYPE){
      n_dims *= 2; // two points per dimension
      for(int i=0; i< n_dims; i++)
        item_o.text += std::to_string(ox_oy_oz[i])+" ";
      trim(item_o.text);
      geometry.items.push_back(item_o);
    }
    else{
      for(int i=0; i< n_dims; i++){
        item_o.text += std::to_string(ox_oy_oz[i])+" ";
        item_d.text += std::to_string(dx_dy_dz[i])+" ";
      }
      trim(item_o.text);
      trim(item_d.text);
      geometry.items.push_back(item_o);
      geometry.items.push_back(item_d);
    }
    
    return 0;
  }
  
  virtual xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL) override{
    xmlNodePtr domain_node = Domain::Serialize(parent, text);
    xmlNodePtr topology_node = topology.Serialize(domain_node);
    
    xmlNodePtr geometry_node = geometry.Serialize(domain_node);
    
    return domain_node;
  };
  
  virtual int Deserialize(xmlNodePtr node, Parsable* _parent) override{
    Domain::Deserialize(node, _parent);

    SetParent(_parent);
    
    for (xmlNode* cur_node = node->children->next; cur_node; cur_node = cur_node->next) {
//      for (xmlNode* inner_node = cur_node->children->next; inner_node; inner_node = inner_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
          
          if(IsNodeName(cur_node, "Topology")){
            topology.Deserialize(cur_node, this);
          }
          else if(IsNodeName(cur_node, "Geometry")){
            geometry.Deserialize(cur_node, this);
          }
//        }
      }
    }

    return 0;
  };
  
  virtual std::string GetClassName() const override { return "SpatialDomain"; };
  
  virtual const IndexSpace<PHY_TYPE>& GetLinearizedIndexSpace() override{
    // TODO NOT IMPLEMENTED
    fprintf(stderr, "GetLinearizedIndexSpace() for SpatialDomain not implemented yet, please\
            use GetLinearizedIndexSpace(int index)\n");
    assert(false);
    return IndexSpace<PHY_TYPE>();
  };
  
  virtual size_t GetVolume() const override{
    size_t total = 1;
    
    for(int i=0; i<topology.dimensions.size(); i++)
      total *= topology.dimensions[i];
    
    return total;
  }


};

}
#endif
