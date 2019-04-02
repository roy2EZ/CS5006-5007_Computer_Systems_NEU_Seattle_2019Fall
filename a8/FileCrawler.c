/*
 *  Created by Adrienne Slaughter
 *  CS 5007 Spring 2019
 *  Northeastern University, Seattle
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  See <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#include "FileCrawler.h"
#include "DocIdMap.h"
#include "LinkedList.h"


void CrawlFilesToMap(const char *dir, DocIdMap map) {
  struct stat s;

  struct dirent **namelist;
  int n;
  n = scandir(dir, &namelist, 0, alphasort);

  // TODO: use namelist to find all the files and put them in map.
  // NOTE: There may be nested folders.
  // Be sure to lookup how scandir works. Don't forget about memory use.
  if (n == -1) {
    perror("scandir");
    exit(EXIT_FAILURE);
  }
  
  // verify dir is a directory
  int result;
  result = stat((const char *) dir, &s);
  if (result == -1 || !S_ISDIR(s.st_mode)) {
    return;
  }
  // try open the dir, if fail, return 0
  DIR *rd;
  rd = opendir(dir);
  if (rd == NULL) {
    return;
  }
 
  // now we are sure dir can be opened. continue to do check files in dir.
  
  // iterates namelist
  for (int i = 0; i < n; i++) {
    // if the namelist[i] is "." or "..", break that iteration in the loop and continue next iteration
    if ( strcmp(namelist[i]->d_name, ".") == 0 || strcmp(namelist[i]->d_name, "..") == 0) {
      continue;
    }
    
    int charsize;
    charsize = strlen(dir) + 1 + strlen(namelist[i]->d_name) + 1;
    char *newfile;
    newfile = (char *) malloc(charsize);
    if (dir[strlen(dir) - 1] == '/') {
      snprintf(newfile, charsize, "%s%s", dir, namelist[i]->d_name);
    } else {
      snprintf(newfile, charsize, "%s/%s", dir, namelist[i]->d_name);
    }

    int res;
    res = stat((const char *) newfile, &s);
    if (res == 0) {
      // if the newfile is a file, put it into map
      if (S_ISREG(s.st_mode)) {
        PutFileInMap(newfile, map);
      } else if (S_ISDIR(s.st_mode)) {
      DIR *newdir; 
      newdir = opendir(newfile);
        if (newdir == NULL) {
          free(newfile);
          continue;
        }
        CrawlFilesToMap(newfile, map);
        closedir(newdir);
      }
    }
  }
  closedir(rd);
}
