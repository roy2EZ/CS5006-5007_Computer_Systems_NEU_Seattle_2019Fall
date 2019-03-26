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
#include <ctype.h>

#include "MovieIndex.h"
#include "htll/LinkedList.h"
#include "htll/Hashtable.h"
#include "Movie.h"
#include "MovieSet.h"

void DestroyMovieSetWrapper(void *movie_set) {
  DestroyMovieSet((MovieSet)movie_set);
}
void toLower(char *str, int len) {
  for (int i = 0; i < len; i++) {
    str[i] = tolower(str[i]);
  }
}

int AddMovieTitleToIndex(Index index,
                         Movie *movie,
                         uint64_t doc_id,
                         int row_id) {
// Put in the index
  HTKeyValue kvp;

  // TODO: How to choose?
  int numFields = 1000;

  char *token[numFields];
  char *rest = movie->title;

  int i = 0;
  token[i] = strtok_r(rest, " ", &rest);
  while (token[i] != NULL) {
    toLower(token[i], strlen(token[i]));
    i++;
    token[i] = strtok_r(rest, " ", &rest);
  }

  for (int j = 0; j < i; j++) {
    // If this key is already in the hashtable, get the MovieSet.
    // Otherwise, create a MovieSet and put it in.
    int result = LookupInHashtable(index,
                          FNVHash64((unsigned char*)token[j],
                                    (unsigned int)strlen(token[j])), &kvp);
    HTKeyValue old_kvp;

    if (result < 0) {
      kvp.value = CreateMovieSet(token[j]);
      kvp.key = FNVHash64((unsigned char*)token[j], strlen(token[j]));
      PutInHashtable(index, kvp, &old_kvp);
    }

    AddMovieToSet((MovieSet)kvp.value, doc_id, row_id);
  }

  return 0;

}

int AddMovieToIndex(Index index, Movie *movie, enum IndexField field) {

  // Put in the index
  HTKeyValue kvp;
  HTKeyValue old_kvp;

  // If this key is already in the hashtable, get the MovieSet.
  // Otherwise, create a MovieSet and put it in.
  int result = LookupInHashtable(index,
                                 ComputeKey(movie, field),
                                 &kvp);

  if (result < 0) {
    char* doc_set_name;
    char year_str[10];
    switch (field) {
      case Type:
        doc_set_name = movie->type;
        break;
      case Year:
        snprintf(year_str, sizeof(year_str), "%d", movie->year);
        doc_set_name = year_str;
        break;
      case Id:
        doc_set_name = movie->id;
      break;
      case Genre:
        // TODO: What to do if there are multiple genres?
        doc_set_name = movie->genres;
    }
    kvp.value = CreateMovieSet(doc_set_name);  // Should be something like "1974", or "Documentary"
    kvp.key = ComputeKey(movie, field);
    PutInHashtable(index, kvp, &old_kvp);
  }

  //AddMovieToSet((MovieSet)kvp.value, movie); // TODO: Something needs to happen here

  return 0;
}



uint64_t ComputeKey(Movie* movie, enum IndexField which_field) {
  switch(which_field) {
    case Year:
      return FNVHashInt64(movie->year);
      break;
    case Type:
      return FNVHash64((unsigned char*)movie->type, strlen(movie->type));
      break;
    case Id:
      return FNVHash64((unsigned char*)movie->id, strlen(movie->id));
      break;
    case Genre:
      // TODO: how to deal with multiple genres??
      return FNVHash64((unsigned char*)movie->genres, strlen(movie->genres));
  }
  return -1u;
}


MovieSet GetMovieSet(Index index, const char *term) {
  HTKeyValue kvp;
  char lower[strlen(term)+1];
  strcpy(lower, term);
  toLower(lower, strlen(lower));
  int result = LookupInHashtable(index,
                                 FNVHash64((unsigned char*)lower,
                                           (unsigned int)strlen(lower)),
                                 &kvp);
  if (result < 0) {
    printf("term couldn't be found: %s \n", term);
    return NULL;
  }
  return (MovieSet)kvp.value;
}


int DestroyIndex(Index index) {
  DestroyHashtable(index, &DestroyMovieSetWrapper);
  return 0;
}

Index CreateIndex() {
  return CreateHashtable(128);
}
