#ifndef SLOC_HPP
#define SLOC_HPP

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <filesystem>
#include <fstream> //ifstream
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>

//alias
namespace fs = std::filesystem;
using count_t = unsigned long; // Integer type for counting lines.

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

/// This enumeration lists all the supported
enum enum_arguments : std::uint8_t {
  RECURSIVE = 0,        //recursive
  SORTDES,              //sort descending
  SORTAS,               //sort ascending
  HELP,                 //help
};
  

//== Classes

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

///The class that determine the states of the code
class CurrentCount {
  public:
    //States
    enum current_count_state : std::uint8_t {START, CODE, POSSIBCOMMENT, COMMENT, POSSIBDOXY, DOXY, LITERAL};
    //Current State
    std::uint8_t current_state {START};
};


//== Structs

struct AttributeCount{
  short lines{0};
  short blank{0};
  short loc{0};
  short com{0};
  short dox{0};
};

/// The running options provided via CLI.
struct RunningOpt {
  bool recursive { false };
  std::optional<sorting_arg> sort_field;
  bool should_sort {false};
  bool sort_ascending {false};
  bool sort_descending {false};
  bool help { false };
  std::vector<std::string> input_list;  //!< list of filenames
  std::vector<std::string> directory_list; //!< list of directories
  std::unordered_set<std::string> added_files;
};

//== Unordered Maps

///Declaring dictionary that relates the arguments with the keys
const std::unordered_map<std::string, enum_arguments> inputed_arguments_with_their_keys = {
  {"-r", RECURSIVE},
  {"-s", SORTAS},
  {"-S", SORTDES},
  {"-h", HELP}, {"--help", HELP}
};

///Declaring dictionary that relates the sorting values with the inputs
const std::unordered_map<std::string, sorting_arg> sorters_with_their_keys = {
  {"f", f},
  {"t", t},
  {"c", c},
  {"d", d},
  {"b", b},
  {"s", s},
  {"a", a},
};

//== Functions

std::string percent(count_t part, count_t total);

std::string value_with_percent(count_t value, count_t total);

std::string language_to_string (lang_type_e lang_type);

inline std::string ltrim(const std::string& s, const char* t = " \t\n\r\f\v");

inline std::string rtrim(const std::string& s, const char* t = " \t\n\r\f\v");

inline std::string trim(const std::string& s, const char* t = " \t\n\r\f\v");
  
lang_type_e return_language_by_extension (const std::string& filename);

void statesMachine(const std::string& filename, CurrentCount ts, FileInfo &info);

void updateState(std::string line, CurrentCount ts, FileInfo &info);

void usage(std::string_view msg = "");

void validate_arguments(int argc, char* argv[], RunningOpt& run_options);

count_t countTotalLines (const std::string& filename);

bool compare_files(const FileInfo& firstFile, const FileInfo& secondFile, std::optional<sorting_arg> sort_field, bool sort_ascending);

bool isPartOfCode(std::string str, std::string line);

AttributeCount process_file(const std::string& filename);

#endif