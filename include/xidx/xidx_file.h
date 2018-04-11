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

#ifndef XIDX_FILE_H_
#define XIDX_FILE_H_

#include "xidx.h"

namespace xidx{
  
  //class GroupList : public XidxList {};
  
class XidxFile{

private:
  std::shared_ptr<Group> root_group;
  std::string file_path;

  bool loaded;

public:

  XidxFile(std::string path) : file_path(path){ };

  int Load(){
    LIBXML_TEST_VERSION;
    
    xmlDocPtr doc; /* the resulting document tree */
    
    doc = xmlReadFile(file_path.c_str(), NULL, 0);
    if (doc == NULL) {
      fprintf(stderr, "Failed to parse %s\n", file_path.c_str());
      return 1;
    }
    
    xmlNode* root_element = xmlDocGetRootElement(doc);
    
    if(!root_element || !(root_element->children) || !(root_element->children->next))
      return 1;
    
    for (xmlNode* cur_node = root_element->children->next; cur_node; cur_node = cur_node->next) {
      
      
      if(IsNodeName(cur_node,"Group")){
        root_group = std::make_shared<Group>(new Group("root"));
        root_group->Deserialize(cur_node, nullptr);//(Parsable*)(root_group->get()));
      }
    }
    
    xmlFreeDoc(doc);
    
    return 0;

  }
  
  int Save(){
    
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
    
    LIBXML_TEST_VERSION;
    
    /*
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "Xidx");
    xmlDocSetRootElement(doc, root_node);
    xmlNewProp(root_node, BAD_CAST "xmlns:xi", BAD_CAST "http://www.w3.org/2001/XInclude");
    xmlNewProp(root_node, BAD_CAST "Version", BAD_CAST "2.0");
    
    // xmlAddDocEntity(doc, BAD_CAST "main_idx_file", XML_INTERNAL_GENERAL_ENTITY, NULL, NULL, BAD_CAST "idx_file.idx");
    
    /*
     * Creates a DTD declaration. Isn't mandatory.
     */
    xmlCreateIntSubset(doc, BAD_CAST "Xidx", NULL, BAD_CAST "Xidx.dtd");
    
    root_group->Serialize(root_node);
    
    /*
     * Dumping document to stdio or file
     */
    xmlSaveFormatFileEnc(file_path.c_str(), doc, "UTF-8", 1);
    
    /*free the document */
    xmlFreeDoc(doc);
    
    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
    
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    
    return 0; 
  }
  
  int Save(std::string path){
    file_path = path;
    return Save();
  };

  // std::string get_idx_file_path(int timestep, int level, CenterType ctype);
  // std::string get_md_file_path(){ return file_path; }
  // int set_md_file_path(const char* new_path){ set_correct_path(new_path); return 0; }

  // int clear(){ groups.clear(); return 0; }

  int SetRootGroup(std::shared_ptr<Group> _root_group){
    root_group = _root_group;
    return 0;
  }
  
  std::shared_ptr<Group> GetRootGroup(){
    return root_group;
  }
  
  inline size_t GetNumberOfGroups() const { return root_group->GetGroups().size(); };
};

}

#endif
