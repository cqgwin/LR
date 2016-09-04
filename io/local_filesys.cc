// =====================================================================================
// 
//       Filename:  local_filesys.cc
// 
//    Description:  local file system implementation
// 
//        Version:  1.0
//        Created:  09/01/2016 08:33:50 PM
//       Revision:  none
//       Compiler:  g++
// 
// 
// =====================================================================================
#include "local_filesys.h"


vector<string> LocalFileSystem::GetDirFiles(const string& dir_name) {
    DIR* dir;
    struct dirent *ptr;
    vector<string> file_list;
    if ((dir = opendir(dir_name.c_str())) == NULL) {
        printf("open dir error\n");
        return file_list;
    }
    while ((ptr = readdir(dir)) != NULL) {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
            continue;
        if (ptr->d_type == 8)
            file_list.push_back(ptr->d_name);
    } 
    return file_list;
}

char* LocalFileSystem::ReadLine(FILE* input) {
    if (fget(line_, max_line_len_, input) == NULL)
        return NULL;
    while (strrchr(line_, '\n') == NULL) {
        max_line_len_ *= 2;
        line = (char*) realloc(line_, max_line_len_);
        int len = (int) strlen(line_);
        if (fgets(line + len, max_line_len_ - len, input) == NULL)
            break;
    }
    return line_;
}
