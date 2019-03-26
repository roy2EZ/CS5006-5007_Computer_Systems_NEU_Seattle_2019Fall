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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "htll/LinkedList.h"
#include "MovieIndex.h"
#include "Movie.h"
#include "MovieReport.h"
#include "./FileParser.h"


int main(int argc, char* argv[]) {
  LinkedList movie_list  = ReadFile("data/test");
  // TODO: Parse command-line args to index based on the correct field
  Index index = BuildMovieIndex(movie_list, Year);

  // TODO: Output report to file, rather than terminal (see MovieReport.h)
  PrintReport(index);

  DestroyIndex(index);

  return 0;
}

