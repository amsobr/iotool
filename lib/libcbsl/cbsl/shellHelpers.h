/*
 * shellHelpers.h
 *
 *  Created on: 1 Jun 2015
 *      Author: keeper
 */

#ifndef LIB_WCOMMON_SHELLHELPERS_H_
#define LIB_WCOMMON_SHELLHELPERS_H_

#include <sys/time.h>
#include <cstdio>
#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdio.h>

namespace shellHelpers {

std::string execShellCommand(const std::string& commmand);

} // namespace shellHelpers

#endif /* LIB_WCOMMON_SHELLHELPERS_H_ */
