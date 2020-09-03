// Copyright (c) 2020, Eiichiro Iwata. All rights reserved.
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../textgrid.hpp"
#include "./cmdline.h"

void AddCommandLineArguments(cmdline::parser& parser) {
  parser.add("rename-tier-names", '\0', "Rename tier names to their numbers.");
  parser.add("help", 'h', "Print the help message.");
  parser.footer("filename ...");
}

textgrid::TextGrid ReadTextGridFile(const std::string& filename) {
  std::ifstream ifs(filename.c_str());
  if (!ifs) {
    throw std::runtime_error("Cannot open file: filename = " + filename +
                             ", error = " + strerror(errno));
  }
  try {
    textgrid::TextGrid text_grid;
    ifs >> text_grid;
    return text_grid;
  } catch (textgrid::Exception& e) {
    throw std::runtime_error("Failed to parse TextGrid file: filename = " + filename +
                             ", error = " + std::string(e.what()));
  }
}

void MergeTextGrid(const cmdline::parser& parser,
                   const textgrid::TextGrid& text_grid,
                   textgrid::TextGrid& merged_text_grid) {
  for (auto&& tier : text_grid.GetAllTiers()) {
    if (!tier) {
      continue;
    }
    if (parser.exist("rename-tier-names")) {
      const std::string new_tier_name = std::to_string(merged_text_grid.GetNumberOfTiers() + 1);
      tier->SetName(new_tier_name);
    }
    merged_text_grid.AppendTier(tier);
    merged_text_grid.SetMinTime(std::min(merged_text_grid.GetMinTime(), tier->GetMinTime()));
    merged_text_grid.SetMaxTime(std::max(merged_text_grid.GetMaxTime(), tier->GetMaxTime()));
  }
}

int main(int argc, char* argv[]) {
  cmdline::parser parser;
  AddCommandLineArguments(parser);
  parser.parse_check(argc, argv);

  const std::vector<std::string> textgrid_files = parser.rest();

  try {
    textgrid::TextGrid merged_text_grid;
    for (size_t i = 0; i < textgrid_files.size(); ++i) {
      textgrid::TextGrid text_grid = ReadTextGridFile(textgrid_files[i]);
      MergeTextGrid(parser, text_grid, merged_text_grid);
    }
    std::cout << merged_text_grid << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  return 0;
}
