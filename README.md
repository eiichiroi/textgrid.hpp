# textgrid.hpp - a C++ TextGrid parser / writer

## Introduction

`textgrid.hpp` is a C++ library for interacting with Praat TextGrid files.

It has the following features:

* Single header
* C++11 (or later)
* No external dependencies (only use standard libraries)
* Support only ASCII and UTF-8
* Checking invariants is not implemented yet

Notes on Praat and TextGrid file format:

* [Praat](http://www.fon.hum.uva.nl/praat/) is one of the most widely used speech analysis software.
* Praat read and write [TextGrid file formats](http://www.fon.hum.uva.nl/praat/manual/TextGrid_file_formats.html)

## Examples

Now, there are only 3 examples to read from stdin and write to stdout.

I will write how to use TextGrid, Tier, IntervalTier, PointTier, Interval, Point objects.

### Read TextGrid file from std::cin and Write to std::cout with operators

```C++
void ReadAndWriteViaStdio() {
  textgrid::TextGrid text_grid;
  try {
    std::cin >> text_grid;
    std::cout << text_grid;
  } catch (const textgrid::Exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
```

### Read TextGrid file from std::cin and Write to std::cout in 1 line

```C++
void ReadAndWriteViaStdioInOneLine() {
  try {
    textgrid::Parser(std::cin).Parse().Accept(textgrid::Writer(std::cout));
  } catch (const textgrid::Exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
```

### Read TextGrid file from std::cin and Write to std::cout step by step

```C++
int ReadAndWriteStepByStep(std::istream& in,
                           std::ostream& out,
                           std::ostream& err,
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
```

See also in the examples/textgrid_print.cpp.

## Build and Test

### Build example commands in the examples directory

```Shell
$ make
```

### Test the style of codes by cpplint

```Shell
$ make check-style
```

or

``` Shell
$ make check-style-by-cpplint
```

### Fix the style of codes by clang-format

``` Shell
$ make check-style-by-clang-format
```

### Build and run tests

```Shell
$ make check
```

## Todo

* Write example usage of TextGrid objects (Tier, IntervalTier, PointTier, Interval, Point) in README.md.
* Write comments.
* Write tests.
* Write more example commands.
* Write more utilities derived from TextGridVisitor.
* Add building interfaces to create TextGrid objects from scratch.
  * IntervalTier should have interfaces to divide the specific interval to 2 intervals.
* Check strong invariants in parsing TextGrid files.

## License

This is licensed under the [MIT License](https://opensource.org/licenses/MIT).

Copyright (c) 2019, Eiichiro Iwata. All rights reserved.
