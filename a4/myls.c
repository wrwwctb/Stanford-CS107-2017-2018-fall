#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/*musl_scandir for debug
#include <errno.h>
#include <stdint.h>
int scandir(const char *path, struct dirent ***res,
      int (*sel)(const struct dirent *),
      int (*cmp)(const struct dirent **, const struct dirent **))
{
    DIR *d = opendir(path);
    struct dirent *de, **names = NULL, **tmp;
    size_t cnt = 0, len = 0;

    if (!d) return -1;

    while ((de = readdir(d))) {
        if (sel && !sel(de)) continue;
        if (cnt >= len) {
            len = 2*len+1;
            if (len > SIZE_MAX/sizeof(*names)) break;
            tmp = realloc(names, len * sizeof(*names));
            if (!tmp) break;
            names = tmp;
        }
        names[cnt] = malloc(de->d_reclen);
        if (!names[cnt]) break;
        memcpy(names[cnt++], de, de->d_reclen);
    }

    closedir(d);

    if (errno) {
        if (names) while (cnt-- > 0) free(names[cnt]);
        free(names);
        return -1;
    }
    if (cmp) qsort(names, cnt, sizeof *names, 
                 (int (*)(const void *, const void *))cmp);
    *res = names;
    return cnt;
}
*/
bool is_dir(int filetype)
{
    return filetype == DT_DIR;
}

int is_fold(const struct dirent *entry){
  return is_dir(entry->d_type);//non POSIX
}

int is_file(const struct dirent *entry){
  return !is_fold(entry);
}

int wi_dot(const struct dirent *entry){
  return (entry->d_name)[0] == '.';
}

int wo_dot(const struct dirent *entry){
  return !wi_dot(entry);
}

int fold_wo_dot(const struct dirent *entry){
  return wo_dot(entry) && is_fold(entry);
}

int file_wo_dot(const struct dirent *entry){
  return wo_dot(entry) && is_file(entry);
}

int dir_comp(const struct dirent **a, const struct dirent **b){
  return strcasecmp((*a)->d_name, (*b)->d_name);
}

void ls(const char *dir, bool show_all, bool sort_by_type)
{
/*
show_all sort_by_type   filter
t       t               folder, not folder
t       f               *
f       f               not .*
f       t               folder not .*   not folder not .*
*/
  
  struct dirent **lists[2] = {NULL, NULL};
  int nums[2] = {-2, -2};

  if (show_all == true && sort_by_type == true){
    nums[0] = scandir(dir, &lists[0], is_fold, dir_comp);
    nums[1] = scandir(dir, &lists[1], is_file, dir_comp);
  }else if (show_all == true && sort_by_type == false){
    nums[0] = scandir(dir, &lists[0], NULL, dir_comp);
  }else if (show_all == false && sort_by_type == false){
    nums[0] = scandir(dir, &lists[0], wo_dot, dir_comp);
  }else{//if (show_all == false && sort_by_type == true){
    nums[0] = scandir(dir, &lists[0], fold_wo_dot, dir_comp);
    nums[1] = scandir(dir, &lists[1], file_wo_dot, dir_comp);
  }

  for (int i = 0; i < 2; i++){
    if (nums[i] == -1){
      printf("./myls: cannot access %s: No such directory\n", dir);
      continue;
    }
    if (lists[i] == NULL || nums[i] == -2)
      continue;
    for (int j = 0; j < nums[i]; j++){
      struct dirent *entry = lists[i][j];
      if (is_fold(entry))
        printf("%s/\n", entry->d_name);
      else
        printf("%s\n", entry->d_name);
      free(entry);
    }
    free(lists[i]);
  }
}


int main(int argc, char *argv[])
{
    bool show_all = false;
    bool dirs_first = false;

    int opt;
    while ((opt = getopt(argc, argv, "az")) != -1) {
        switch (opt) {
            case 'a': show_all = true; break;
            case 'z': dirs_first = true; break;
            default: exit(1);
        }
    }
    if (optind < argc -1) {
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            ls(argv[i], show_all, dirs_first);
            printf("\n");
        }
    } else {
        ls(optind == argc -1? argv[optind] : ".", show_all, dirs_first);
    }
    return 0;
}
