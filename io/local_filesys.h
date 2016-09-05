// =====================================================================================
// 
//       Filename:  local_filesys.h
// 
//    Description:  local file system head file
// 
//        Version:  1.0
//        Created:  09/01/2016 07:08:14 PM
//       Revision:  none
//       Compiler:  g++
// 
// 
// =====================================================================================

#ifndef LOCAL_FILESYS_H_
#define LOCAL_FILESYS_H_

#include "filesys.h"

class LocalFileSystem : public FileSystem {
 public:
    static vector<string> GetDirFiles(const string& dir_name);

    char* ReadLine(FILE* input);

    LocalFileSystem() {
        max_line_len_ = 1000;
        line_ = (char*) malloc(max_line_len_ * sizeof(char));
    }
 private:
    int max_line_len_;
    char* line_; 
};

#endif
