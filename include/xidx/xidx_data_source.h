

#ifndef XIDX_DATA_SOURCE_H_
#define XIDX_DATA_SOURCE_H_

#include "xidx.h"

namespace xidx{

class DataSource : public Parsable{

private:
  std::string url;

public:

  DataSource(std::string path) : url(path){};

  std::string GetUrl(){ return url; }
  
  int SetFilePath(std::string path){ url = path; return 0; }
  
  xmlNodePtr Serialize(xmlNode* parent, const char* text=NULL){
    xmlNodePtr variable_node = xmlNewChild(parent, NULL, BAD_CAST "DataSource", NULL);
    xmlNewProp(variable_node, BAD_CAST "Name", BAD_CAST name.c_str());
    xmlNewProp(variable_node, BAD_CAST "Url", BAD_CAST url.c_str());
    
    return variable_node;
  }
  
  int Deserialize(xmlNodePtr node){
    if(!xidx::IsNodeName(node,"DataSource"))
      return -1;
    
    name = xidx::GetProp(node, "Name");
    url = xidx::GetProp(node, "Url");
    
    return 0;
  }
  
};

};

#endif