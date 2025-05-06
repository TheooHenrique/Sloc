#ifndef SLOC_HPP
#define SLOC_HPP

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

//== Enumerations

/// This enumeration lists all the supported languages.
enum lang_type_e : std::uint8_t {
    C = 0,  //!< C language
    CPP,    //!< C++ language
    H,      //!< C/C++ header
    HPP,    //!< C++ header
    UNDEF,  //!< Undefined type.
  };

/// This enumeration lists all the supported arguments for sorting

enum sorting_arg : std::uint8_t {
  f = 0,     //filename
  t,         //filetype
  c,         //comments
  d,         //doc comments
  b,         //blank lines
  s,         //sloc
  a,         //all
};

enum enum_arguments : std::uint8_t {
  RECURSIVE = 0,        //recursive
  SORTDES,              //sort descending
  SORTAS,               //sort ascending
  HELP,                 //help
};
  
  //== Class/Struct declaration
  
  /// Integer type for counting lines.
  using count_t = unsigned long;
  
  /// Stores the file information we are collecting.
  class FileInfo {
  public:
    std::string filename;   //!< the filename.
    lang_type_e type;       //!< the language type.
    count_t n_blank;        //!< # of blank lines in the file.
    count_t n_comments;     //!< # of comment lines.
    count_t n_doc_comments; //!< # of doc comments
    count_t n_loc;          //!< # lines of code.
    count_t n_lines;        //!< # of lines.

    
  
    /// constructor
    FileInfo(std::string fn = "",
             lang_type_e t = UNDEF,
             count_t nb = 0,
             count_t nc = 0,
             count_t nl = 0,
             count_t ni = 0)
        : filename{ std::move(fn) }, type{ t }, n_blank{ nb }, n_comments{ nc }, n_loc{ nl },
          n_lines{ ni } {
      /* empty*/
    }
  };
  
  /// The running options provided via CLI.
  struct RunningOpt {
    bool recursive = false;
    char sort_field = '\0';
    bool sort_ascending = false;
    bool sort_descending = false;
    bool help = false;
    std::vector<std::string> input_list;  //!< This might be a list of filenames or a directories.
  };

  void usage(std::string_view msg = "");

  inline std::string ltrim(const std::string& s, const char* t = " \t\n\r\f\v");

  inline std::string rtrim(const std::string& s, const char* t = " \t\n\r\f\v");

  inline std::string trim(const std::string& s, const char* t = " \t\n\r\f\v");

  void validate_arguments(int argc, char* argv[], RunningOpt& run_options);

  
  const std::unordered_map<std::string, enum_arguments> inputed_arguments_with_their_keys = {
    {"-r", RECURSIVE},
    {"-s", SORTAS},
    {"-S", SORTDES},
    {"-h", HELP}, {"--help", HELP}
  };

    ///Declaring dictionary that relates the sorting values an the inputs
    const std::unordered_map<std::string, sorting_arg> sorters_with_their_keys = {
      {"f", f},
      {"t", t},
      {"c", c},
      {"d", d},
      {"b", b},
      {"s", s},
      {"a", a},
    };


#endif