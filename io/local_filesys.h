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
//         Author:  qingguochen (), qingguochen@tencent.com
//        Company:  
// 
// =====================================================================================

#ifndef LOCAL_FILESYS_H_
#define LOCAL_FILESYS_H_

#include "io/filesys.h"

class LocalFileSystem : public FileSystem {
 public:
    inline static LocalFileSystem *GetInstance(void) {
         static LocalFileSystem instance;
         return &instance;
    }
    static GetDirFiles(const string& dir_name);
    char* ReadLine(FILE* input);
 private:
    LocalFileSystem() {
        max_line_len_ = 1000;
        line_ = (char*) malloc(max_line_len_ * sizeof(char));
    }
    static int max_line_len_;
    static char* line_; 
};

#endif
