//----------------------------------------------------------------------------------------
/**
 * \file    config.h
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Declaration of function for the config file.
 */
 //----------------------------------------------------------------------------------------

#pragma once
#include <string>
#include "pgr.h"

/// @brief Load configuration from file
/// @param path path to config file
void loadConfig(const std::string& path);