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

// alias
namespace fs = std::filesystem;

/// @brief Integer type for counting lines.
using count_t = unsigned long;

//== Enumerations

/**
 * @enum lang_type_e
 * @brief Enumeration of supported languages
 */
enum lang_type_e : std::uint8_t {
    C = 0,  //!< C language
    CPP,    //!< C++ language
    H,      //!< C/C++ header
    HPP,    //!< C++ header
    UNDEF,  //!< Undefined type.
};

/**
 * @enum sorting_arg
 * @brief Enumeration of sorting criteria for results.
 */
enum sorting_arg : std::uint8_t {
  f = 0,     //filename
  t,         //filetype
  c,         //comments
  d,         //doc comments
  b,         //blank lines
  s,         //sloc
  a,         //all
};

/**
 * @enum enum_arguments
 * @brief Enumeration of supported command line arguments
 */
enum enum_arguments : std::uint8_t {
  RECURSIVE = 0,        //recursive
  SORTDES,              //sort descending
  SORTAS,               //sort ascending
  HELP,                 //help
};
  

//== Classes

/**
 * @class FileInfo
 * @brief Stores statistics about a source code file
 */
class FileInfo {
public:
  std::string filename;   //!< the filename.
  lang_type_e type;       //!< the language type.
  count_t n_blank;        //!< # of blank lines in the file.
  count_t n_comments;     //!< # of comment lines.
  count_t n_doc_comments; //!< # of doc comments
  count_t n_loc;          //!< # lines of code.
  count_t n_lines;        //!< # of lines.    

  /**
   * @brief Construct a new FileInfo object.
   * @param fn Filename (default empty)
   * @param t  Language type (default UNDEF)
   * @param nb Blank lines count (default 0)
   * @param nc Comment lines count (default 0)
   * @param nl Lines of code count (default 0)
   * @param ni Total lines count (default 0)
   */
  FileInfo(std::string fn = "",
            lang_type_e t = UNDEF,
            count_t nb = 0,
            count_t nc = 0,
            count_t nl = 0,
            count_t ni = 0)
      : filename{ std::move(fn) }, type{ t }, n_blank{ nb }, n_comments{ nc }, n_loc{ nl },
        n_lines{ ni } {
  }
};

/**
 * @class CurrentCount
 * @brief State machine for parsing source code.
*/
class CurrentCount {
  public:
    /// @brief Possible states for the parser.
    enum current_count_state : std::uint8_t {
      START,         //!< Initial state
      CODE,          //!< Processing code
      POSSIBCOMMENT, //!< Possible comment start
      COMMENT,       //!< Inside regular comment
      POSSIBDOXY,    //!< Possible documentation comment start
      DOXY,          //!< Inside documentation comment
      LITERAL        //!< Inside string literal
    };

    std::uint8_t current_state {START}; //!< Current parsing state
};


//== Structs

/**
 * @struct AttributeCount
 * @brief Temporary storage for line counts during processing.
 */
struct AttributeCount{
  short lines{0}; //!< Total lines processed
  short blank{0}; //!< Blank lines count
  short loc{0};   //!< Lines of code count
  short com{0};   //!< Regular comments count
  short dox{0};   //!< Documentation comments count
};

/**
 * @struct RunningOpt
 * @brief Runtime options from command line.
 */
struct RunningOpt {
  bool recursive { false };                    //!< Recursive directory search
  std::optional<sorting_arg> sort_field;       //!< Field to sort by
  bool should_sort { false };                  //!< Whether to sort results
  bool sort_ascending { false };               //!< Sort in ascending order
  bool sort_descending { false };              //!< Sort in descending order
  bool help { false };                         //!< Show help message
  std::vector<std::string> input_list;         //!< list of input files
  std::vector<std::string> directory_list;     //!< list of input directories
  std::unordered_set<std::string> added_files; //!< Files already processed
};

//== Unordered Maps

/// @brief Mapping of CLI flags to their corresponding enum values.
const std::unordered_map<std::string, enum_arguments> inputed_arguments_with_their_keys = {
  {"-r", RECURSIVE},
  {"-s", SORTAS},
  {"-S", SORTDES},
  {"-h", HELP}, {"--help", HELP}
};

/// @brief Mapping sorting criteria to their enum values.
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

/**
 * @brief Process a file and count its lines.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see process_file()
 */
AttributeCount process_file(const std::string& filename);

/**
 * @brief Process a file through the state machine.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see statesMachine()
 */
AttributeCount statesMachine(const std::string& filename, CurrentCount ts, AttributeCount& atr);

/**
 * @brief Update the counting state based on line content.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see updateState()
 */
AttributeCount updateState(std::string line, CurrentCount &ts);

/**
 * @brief Compare two files for sorting.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see compare_files()
 */
bool compare_files(const FileInfo& firstFile, const FileInfo& secondFile, std::optional<sorting_arg> sort_field, bool sort_ascending);

/**
 * @brief Check if a string is part of code (not in comments).
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see isPartOfCode()
 */
bool isPartOfCode(std::string str, std::string line);

/**
 * @brief Check if a quote starts a string literal.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see startLiteral()
 */
bool startLiteral(std::string line, char i);

/**
 * @brief Count total lines in a file.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see countTotalLines()
 */
count_t countTotalLines (const std::string& filename);

/**
 * @brief Determine language by file extension.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see return_language_by_extension()
 */
lang_type_e return_language_by_extension (const std::string& filename);

/**
 * @brief Convert language enum to string.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see language_to_string()
 */
std::string language_to_string (lang_type_e lang_type);

/**
 * @brief Calculate percentage string.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see percent()
 */
std::string percent(count_t part, count_t total);

/**
 * @brief Format value with percentage.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see value_with_percent()
 */
std::string value_with_percent(count_t value, count_t total);

/**
 * @brief Trim whitespace from left of string.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see ltrim()
 */
inline std::string ltrim(const std::string& s, const char* t = " \t\n\r\f\v");

/**
 * @brief Trim whitespace from right of string.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see rtrim()
 */
inline std::string rtrim(const std::string& s, const char* t = " \t\n\r\f\v");

/**
 * @brief Trim whitespace from both ends of string.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see trim()
 */
inline std::string trim(const std::string& s, const char* t = " \t\n\r\f\v");

/**
 * @brief Collect files from directories based on options.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see collect_files()
 */
void collect_files(RunningOpt& run_options);

/**
 * @brief Print summary table of line counts.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see print_summary()
 */
void print_summary(const std::vector<FileInfo>& db, const RunningOpt& run_options);

/**
 * @brief Print usage information.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see usage()
 */
void usage(std::string_view msg = "");

/**
 * @brief Validate and process command line arguments.
 * 
 * Detailed documentation for this function is provided in the implementation file.
 * 
 * @see validate_arguments()
 */
void validate_arguments(int argc, char* argv[], RunningOpt& run_options);

#endif