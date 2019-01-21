// Copyright (c) 2019, Eiichiro Iwata. All rights reserved.
#include "../textgrid.hpp"

#include <iostream>

#include "./cmdline.h"

int ReadAndWriteStepByStep(std::istream& in, std::ostream& out, std::ostream& err,
                           bool in_short = false) {
  try {
    textgrid::Parser parser(in);
    textgrid::TextGrid text_grid = parser.Parse();
    if (in_short) {
      textgrid::ShortWriter writer(out);
      text_grid.Accept(writer);
    } else {
      textgrid::Writer writer(out);
      text_grid.Accept(writer);
    }
    return 0;
  } catch (const textgrid::Exception& e) {
    err << e.what() << std::endl;
    return -1;
  }
}

void DefineCommandLineArguments(cmdline::parser& parser) {
  parser.add("short", '\0', "Output in short format or not.");
}

int main(int argc, char* argv[]) {
  cmdline::parser parser;
  DefineCommandLineArguments(parser);
  parser.parse_check(argc, argv);

  return ReadAndWriteStepByStep(std::cin, std::cout, std::cerr, parser.exist("short"));
}
