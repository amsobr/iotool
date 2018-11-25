/*
 * shellHelpers.cpp
 *
 *  Created on: 1 Jun 2015
 *      Author: keeper
 */

#include <iostream>
#include "shellHelpers.h"

namespace shellHelpers {

std::string execShellCommand(const std::string& commmand) {
  FILE* pipe = popen(commmand.c_str(), "r");

  if (!pipe) {
    return "ERROR";
  }

  char buffer[256] {};
  std::string result = "";

  while (!feof(pipe)) {
    if (fgets(buffer, sizeof buffer, pipe) != NULL) {
      result += buffer;
    }
  }

  pclose(pipe);
  return result;
}

} // namespace shellHelpers
