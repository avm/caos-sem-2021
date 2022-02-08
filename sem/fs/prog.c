#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include<string.h>
int link_path(const char *name_from, const char *name_to, int fd_from, int fd_to) {
  struct stat name_from_stat;
  if (fstatat(fd_from, name_from, &name_from_stat, 0)) {
    perror(name_from);
    return 1;
  }
  if (!S_ISDIR(name_from_stat.st_mode)) {
    linkat(fd_from, name_from, fd_to, name_to, 0);
    return 0;
  }
  if (mkdirat(fd_to, name_to, name_from_stat.st_mode)) {
    perror("mkdir failed");
    return 1;
  }
  int src_dir;
  src_dir = openat(fd_from, name_from, O_RDONLY);
  if(src_dir < 0) {
    perror(name_from);
    return 1;
  }
  DIR* src_list = fdopendir(src_dir);
  if(src_list == NULL) {
    perror("something");
    return 1;
  }
  struct dirent* dirent_;
  int new_dst_dir =  openat(fd_to, name_to, O_RDONLY);
  if(new_dst_dir < 0) {
    perror(name_to);
    return 1;
  }
  while((dirent_ = readdir(src_list)) != NULL) {
    if(strcmp(dirent_->d_name, ".") == 0) {
      continue;
    }
    if(strcmp(dirent_->d_name, "..") == 0) {
      continue;
    }
   if(link_path(dirent_->d_name, dirent_->d_name, src_dir, new_dst_dir) == 1) {
     return 1;
   }
  
  }
  closedir(src_list);
  close(new_dst_dir);
  close(src_dir);
  return 0;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr, "too few arguments\n");
    return 1;
  }
  int cur_fd = open(".", O_RDONLY);
  return link_path(argv[1], argv[2], cur_fd, cur_fd);
}
