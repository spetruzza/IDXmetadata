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

#ifndef XIDX_DOMAIN_H_
#define XIDX_DOMAIN_H_

#include "xidx/xidx.h"

namespace xidx{

class Domain : public Parsable{
public:
  enum DomainType{
    HYPER_SLAB_DOMAIN_TYPE = 0,
    LIST_DOMAIN_TYPE = 1,
    MULTIAXIS_DOMAIN_TYPE = 2,
    SPATIAL_DOMAIN_TYPE = 3,
    RANGE_DOMAIN_TYPE = 4
  };
  
  static inline const char* toString(DomainType v)
  {
    switch (v)
    {
      case HYPER_SLAB_DOMAIN_TYPE:         return "HyperSlab";
      case LIST_DOMAIN_TYPE:               return "List";
      case MULTIAXIS_DOMAIN_TYPE:          return "MultiAxisDomain";
      case SPATIAL_DOMAIN_TYPE:            return "Spatial";
      case RANGE_DOMAIN_TYPE:              return "Range";
      default:                             return "[Unknown]";
    }
  }

protected:
  std::vector<std::shared_ptr<Attribute>> attributes;
  // TODO this should be part of the IndexSpace definition
  int bound_size = 1;
  DomainType type;
  
public:
  
  Domain(const Domain& c) {
    setParent(c.getParent());
    name = c.name;
    type = c.type;
    attributes = c.attributes;
    data_items = c.data_items;
  };
  
  Domain(std::string _name) {
    name=_name;
  };

  DomainType getType() { return type; }

  std::vector<std::shared_ptr<DataItem> > data_items;

  int addDataItem(std::shared_ptr<DataItem> item){
    data_items.push_back(item);
    return 0;
  }
  
  virtual int addDataItem(std::string name, Parsable *parent){
    data_items.push_back(std::make_shared<DataItem>(new DataItem(name, parent)));
    return 0;
  }
  
  virtual int addAttribute(std::string name, std::string value){
    attributes.push_back(std::make_shared<Attribute>(new Attribute(name, value)));
    return 0;
  }
  
  std::vector<std::shared_ptr<Attribute>> getAttributes() const{ return attributes; }
  
  virtual xmlNodePtr serialize(xmlNode *parent, const char *text = NULL) override{
    //Parsable::serialize(parent);

    xmlNodePtr domain_node = xmlNewChild(parent, NULL, BAD_CAST "Domain", NULL);
    xmlNewProp(domain_node, BAD_CAST "Type", BAD_CAST toString(type));

    for(auto item: data_items)
      xmlNodePtr item_node = item->serialize(domain_node);
      
    for(auto att: attributes)
      xmlNodePtr item_att = att->serialize(domain_node);

    return domain_node;
  };
  
  virtual int deserialize(xmlNodePtr node, Parsable *_parent) override{
    //Parsable::deserialize(node); // TODO use the parent class to serialize name??
    setParent(_parent);
    
    assert(this->getParent()!=nullptr);
    
    const char* domain_type = xidx::getProp(node, "Type");

    for(int t=DomainType::HYPER_SLAB_DOMAIN_TYPE; t <= DomainType::RANGE_DOMAIN_TYPE; t++)
      if (strcmp(domain_type, toString(static_cast<DomainType>(t)))==0)
          type = static_cast<DomainType>(t);
    
    for(int t=DomainType::HYPER_SLAB_DOMAIN_TYPE; t <= DomainType::RANGE_DOMAIN_TYPE; t++)
      if (strcmp(domain_type, toString(static_cast<DomainType>(t)))==0)
        type = static_cast<DomainType>(t);
    
    int data_items_count=0;
    for (xmlNode* cur_node = node->children->next; cur_node; cur_node = cur_node->next) {
      
      if (cur_node->type == XML_ELEMENT_NODE) {
        
        if(isNodeName(cur_node, "DataItem")){
          if(data_items.size() > data_items_count){
            std::shared_ptr<DataItem> d = data_items[data_items_count];
            d->deserialize(cur_node, this);
          }
          else{
            std::shared_ptr<DataItem> d(new DataItem(this));
            d->deserialize(cur_node, this);
            data_items.push_back(d);
          }
          
          data_items_count++;
        }
        else if(isNodeName(cur_node, "Attribute")){
          std::shared_ptr<Attribute> att(new Attribute());
          att->deserialize(cur_node, this);
          attributes.push_back(att);
        }
      }
      
    }

    return 0;
  };
  
  virtual size_t getVolume() const{
    size_t total = 1;
    for(auto& item: this->data_items)
      for(int i=0; i < item->dimensions.size(); i++)
        total *= item->dimensions[i];
    return total;
  }
  
  virtual const IndexSpace& getLinearizedIndexSpace() = 0;
  
  virtual std::string getClassName() const override { return "Domain"; };

};

}
#endif
