/*!
 * @file main.cpp
 * @description
 * This program implements a single line of code count for C/C++ programs.
 * @author	Add your name here
 * @date	September, 9th 2024.
 * @remark On 2022-09-9 changed ...
 */

 #include "main.hpp"
#include <string>

/// help message

std::string universal = "entrei em nada";

void usage(std::string_view msg) {
  std::cout << "Welcome to sloc cpp, version 1.0, (c) DIMAp/UFRN.\n\n";
  std::cout << "NAME\n";
  std::cout << "  sloc - single line of code counter.\n\n";
  std::cout << "SYNOPSIS\n";
  std::cout << "  sloc [-h | --help] [-r] [(-s | -S) f|t|c|b|s|a] <file | directory>\n\n";
  std::cout << "EXAMPLES\n";
  std::cout << "  sloc main.cpp sloc.cpp\n";
  std::cout << "     Counts loc, comments, blanks of the source files 'main.cpp' and 'sloc.cpp'\n\n";
  std::cout << "  sloc source\n";
  std::cout << "     Counts loc, comments, blanks of all C/C++ source files inside 'source'\n\n";
  std::cout << "  sloc -r -s c source\n";
  std::cout << "     Counts loc, comments, blanks of all C/C++ source files recursively inside 'source'\n";
  std::cout << "     and sort the result in ascending order by # of comment lines.\n\n";
  std::cout << "DESCRIPTION\n";
  std::cout << "  Sloc counts the individual number **lines of code** (LOC), comments, and blank\n";
  std::cout << "  lines found in a list of files or directories passed as the last argument\n";
  std::cout << "  (after options).\n";
  std::cout << "  After the counting process is concluded the program prints out to the standard\n";
  std::cout << "  output a table summarizing the information gathered, by each source file and/or\n";
  std::cout << "  directory provided.\n";
  std::cout << "  It is possible to inform which fields sloc should use to sort the data by, as\n";
  std::cout << "  well as if the data should be presented in ascending/descending numeric order.\n\n";
  std::cout << "OPTIONS:\n";
  std::cout << "  -h/--help\n";
  std::cout << "            Display this information.\n\n";
  std::cout << "  -r\n";
  std::cout << "            Look for files recursively in the directory provided.\n\n";
  std::cout << "  -s f|t|c|d|b|s|a\n";
  std::cout << "            Sort table in ASCENDING order by (f)ilename, (t) filetype,\n";
  std::cout << "            (c)omments, (d)oc comments, (b)lank lines, (s)loc, or (a)ll.\n";
  std::cout << "            Default is to show files in ordem of appearance.\n\n";
  std::cout << "  -S f|t|c|d|b|s|a\n";
  std::cout << "            Sort table in DESCENDING order by (f)ilename, (t) filetype,\n";
  std::cout << "            (c)omments, (d)oc comments, (b)lank lines, (s)loc, or (a)ll.\n";
  std::cout << "            Default is to show files in ordem of appearance.\n";
}

//== Aux functions

std::string percent(count_t part, count_t total) {
  if (total == 0) return "0%";
  double perc = (static_cast<double>(part) / total) * 100; //converts part to double, divides by total and multiplies by 100
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(1) << perc; //std::fixed rounds the number and std::setprecision(1) shows one decimal place
  return oss.str() + "%"; //extracts the contents of ostringstream as a std::string.
}

std::string value_with_percent(count_t value, count_t total) {
    return std::to_string(value) + " (" + percent(value, total) + ")";
}

// trim from left
inline std::string ltrim(const std::string& s, const char* t) {
  std::string clone{s}; //copy of the original string s
  size_t start = clone.find_first_not_of(t); //finds the first character not in t, i.e. the first visible character

  if (start != std::string::npos) { //if there is a visible character
    clone.erase(0, start); //erase from the first character (which is a space) to the first visible character
  } else { //if there is only space, i.e., start == std::string::npos
    clone.clear();
  }

  return clone;
}
// trim from right
inline std::string rtrim(const std::string& s, const char* t) {
  std::string clone{s}; //copy of the original string s
  size_t end = clone.find_last_not_of(t); //finds the last character not in t, i.e. the last visible character

  if (end != std::string::npos) {
    clone.erase(end + 1); //erase everything after the last visible character
  } else {
    clone.clear();
  }
  
  return clone;
}
// trim from left & right
inline std::string trim(const std::string& s, const char* t) {
  return ltrim(rtrim(s, t), t);
}

//count total lines
count_t countTotalLines (const std::string& filename) {
  std::ifstream file(filename);
  int line = 0;
  std::string codeLines;

  while (std::getline(file, codeLines)) ++line;
  
  return line;
}

bool startLiteral(std::string line, char i){
  if (line[i - 1] == '\\' || (line[i - 1] == '\'' && line[i + 1] == '\'')){
    return false;
  }
  return true;
}

///Verify if the comment command is part of the code
bool isPartOfCode(std::string str, std::string line){
  
  bool leftIsPair;
  bool rightIsPair;
  int countLeft{0};
  int countRight{0};

  auto indexFirst{line.find(str)};
  auto indexLast{indexFirst + str.length() - 1};
  
  for (size_t i{0}; i < indexFirst; i++){
    if (line[i] == '"'){
      countLeft++;
    }
  }

  if (countLeft%2 == 0){
    leftIsPair = true;
  } else{
    leftIsPair = false;
  }

  for (; indexLast < line.length(); indexLast++){
    if (line[indexLast] == '"'){
      countRight++;
    }
  }

  if (countRight%2 == 0){
    rightIsPair = true;
  } else{
    rightIsPair = false;
  }
  
  if (rightIsPair && leftIsPair){
    return false;
  } 
  else if (!rightIsPair && !leftIsPair){
    return true;
  }
  return true;
}



///Update the current state
AttributeCount updateState(std::string line, CurrentCount &ts){
  AttributeCount atributes;
  line = trim(line, " ");
  size_t len = line.length();

      if (ts.current_state != ts.COMMENT && ts.current_state != ts.DOXY){

        bool blank{false};
      
      if (line.empty()) {
        blank = true;
        atributes.blank = 1;
        return atributes;
      }
      /*else if(!line.empty() && line[0] != '/'){
        atributes.loc = 1;
      }*/

    }
    if (ts.current_state == ts.COMMENT){
      atributes.com = 1;
    }
    if (ts.current_state == ts.DOXY){
      atributes.dox = 1;
    }
    if (ts.current_state == ts.LITERAL || ts.current_state == ts.CODE){
      atributes.loc = 1;
    }


    for (size_t i{0}; i < len; ++i){
      auto minline3 = line.substr(i, 3);
      auto minline2 = line.substr(i, 2);

      if (ts.current_state == ts.LITERAL){
        if (line[i] == '"'){
          if(startLiteral(line, i)){
            ts.current_state = ts.START;
            continue;
          }
        }
      }

      if (ts.current_state == ts.DOXY || ts.current_state == ts.COMMENT){

        if (minline2 == "*/"){
          ts.current_state = ts.START;
          i += 1;
          continue;
        }
      }
      
      if (ts.current_state == ts.CODE || ts.current_state == ts.START){

        if (line[i]  == '"'){
          if (startLiteral(line, line[i])){
            ts.current_state = ts.LITERAL;
            continue;
          }
          
        }


        else if (minline3 == "/*!" || minline3 == "/**"){
          ts.current_state = ts.DOXY;
          atributes.dox = 1;
          i += 2;
          continue;
        
        } 

        else if (minline3 == "//!" || minline3 == "///"){

          atributes.dox = 1; 
          ts.current_state = ts.START;
          break;

        }

        else if (minline2 == "/*"){

          ts.current_state = ts.COMMENT;
          atributes.com = 1;
          i += 1;
          continue;
          
        }
        else if (minline2 == "//"){

          atributes.com = 1;
          ts.current_state = ts.START;
          break;
          
        }
        else{
          ts.current_state = ts.CODE;
          atributes.loc = 1;
        }
      }
    }
    if (ts.current_state == ts.CODE){
      ts.current_state = ts.START;
    }
    return atributes;
}

AttributeCount statesMachine(const std::string& filename, CurrentCount ts, AttributeCount& atr){
  std::ifstream file(filename);
  std::string codeLines;

  while (std::getline(file, codeLines)) {
    AttributeCount atributes = updateState(codeLines, ts);
    atr.lines += atributes.lines;
    atr.blank += atributes.blank;
    atr.com += atributes.com;
    atr.dox += atributes.dox;
    atr.loc += atributes.loc;
  }
  return atr;
}

//function to apply the functions of totallines and blanklines to an object
AttributeCount process_file(const std::string& filename) {
  AttributeCount atr;
  CurrentCount ts;

  statesMachine(filename, ts, atr);
  atr.lines = countTotalLines(filename);

  return atr;
}
 
//validate arguments in CLI
void validate_arguments(int argc, char* argv[], RunningOpt& run_options) {
  for (size_t ct{1}; ct < argc; ++ct) {
    auto it { inputed_arguments_with_their_keys.find(argv[ct]) }; //find the key argv[ct] in inputed_arguments_with_their_keys, which is, e.g., "-r" in case of recursive
    if (it != inputed_arguments_with_their_keys.end()) { //.find() returns .end() if nothing is found with that inputed argument
      enum_arguments arg {it -> second}; //iterator -> second returns the value of the dictionary

      switch(arg){ //if the value of the unordered_map is
        case RECURSIVE: run_options.recursive = true; break;
        case SORTDES: run_options.sort_descending = true; run_options.should_sort = true; break;
        case SORTAS: run_options.sort_ascending = true; run_options.should_sort = true; break;
        case HELP: run_options.help = true; break;
      }

      if (run_options.help){
        usage();
        exit(0);
      }

      //Checking if sort arguments are correctly inputed
      if (arg == SORTAS || arg == SORTDES){
        if (ct + 1 >= argc) { //treating memory leak
          std::cerr << "Missing value\n";
          usage();
          exit(1);
        }
        const char* nextArgument { argv[ct+1] };
        auto it { sorters_with_their_keys.find(nextArgument) }; //find the key argv[ct+1] in sorters_with_their_keys, which is, e.g., "f" in case of sort by filename
        if (it != sorters_with_their_keys.end()) {
          run_options.sort_field = it -> second;
          ct++;
        } else {
          std::cerr << "Invalid sorter parameter: " << nextArgument << "\n";
          usage();
          exit(1);
        }
      }
    } else {
      std::string file_or_dir_inputed_by_the_user = argv[ct];

      if (!fs::exists(file_or_dir_inputed_by_the_user)) {
        std::cerr << "Sorry, unable to read \"" << file_or_dir_inputed_by_the_user << "\".\n";
      }

      if (fs::is_regular_file(file_or_dir_inputed_by_the_user)) {
        std::string extension = fs::path(file_or_dir_inputed_by_the_user).extension().string();
        if (extension == ".cpp" || extension == ".c" || extension == ".hpp" || extension == ".h") {
          run_options.input_list.push_back(file_or_dir_inputed_by_the_user);
        } else {
          std::cerr << "Sorry, \"" << extension << "\" files are not supported at this time.\n";
          exit(1);
        }
      } else if (fs::is_directory(file_or_dir_inputed_by_the_user)) {
        run_options.directory_list.push_back(file_or_dir_inputed_by_the_user);
      }
    }

    if (run_options.input_list.empty()) {
      bool found_any_supported_file {false};

      for (const auto& directory : run_options.directory_list) {
        for (const auto& directory_file : fs::recursive_directory_iterator(directory)) {
          if (directory_file.is_regular_file()) {
            std::string extension = directory_file.path().extension().string();
            if (extension == ".c" || extension == ".cpp" || extension == ".h" || extension == ".hpp") {
              found_any_supported_file = true;
              break;
            }
          }
        }

        if (found_any_supported_file) break;
      }

      if (!found_any_supported_file) {
        if (!run_options.directory_list.empty()) {
          std::cerr << "Sorry, unable to find any supported source file inside directory \"" << run_options.directory_list[0] << "\".\n";
        }
      }
    }
  }
}

void collect_files(RunningOpt& run_options) {
  if (run_options.directory_list.empty()) return; //return gets out of the function

  std::unordered_set<std::string> unique_files;
  for (const auto& file : run_options.input_list) {
    unique_files.insert(fs::absolute(file).string()); //absolute returns the absolute path of the file
  }

  if (run_options.recursive) {
    for (const auto& directory : run_options.directory_list) { //for each directory in directory_list
    for (const auto& directory_file : fs::recursive_directory_iterator(directory)) { //recursive_directory_iterator and directory_iterator both return directory_entry
      if (directory_file.is_regular_file()) { //if the file in the directory is a file, not another directory
        std::string file_path = directory_file.path().string(); //path() gets the... path... of the file, and .string() converts the path to string
        std::string absolute_path = fs::absolute(file_path).string();

        if (unique_files.find(absolute_path) != unique_files.end()) { //.find returns end(). so if the return is != from .end(), it is found
          continue;
        }

        if (file_path.length() >= 4) {
          std::string last4(file_path.end() - 4, file_path.end());
          std::string last2(file_path.end() - 2, file_path.end());

          if (last4 == ".cpp" || last4 == ".hpp" || last2 == ".c" || last2 == ".h") {
            run_options.input_list.push_back(file_path);
            unique_files.insert(absolute_path);
          }
        }
      }
    }
  }
  } else {
      for (const auto& directory : run_options.directory_list) { //for each directory in directory_list
        for (const auto& directory_file : fs::directory_iterator(directory)) {
          if (directory_file.is_regular_file()) { //if the file in the directory is a file, not another directory
            std::string file_path = directory_file.path().string(); //path() gets the... path... of the file, and .string() converts the path to string
            std::string absolute_path = fs::absolute(file_path).string();

            if (unique_files.find(absolute_path) != unique_files.end()) { //if the path of a file is found, is because the file is already here, so we go to the next iteraction. this way, there is no chance for the file to be count twice
              continue;
            }

            if (file_path.length() >= 4) {
              std::string last4(file_path.end() - 4, file_path.end());
              std::string last2(file_path.end() - 2, file_path.end());

              if (last4 == ".cpp" || last4 == ".hpp" || last2 == ".c" || last2 == ".h") {
                run_options.input_list.push_back(file_path);
                unique_files.insert(absolute_path);
              }
            }
          }
        }
    }
  }
}

bool compare_files(const FileInfo& firstFile, const FileInfo& secondFile, std::optional<sorting_arg> sort_field, bool sort_ascending) {

  switch (sort_field.value()) { //gets the value in sort_field, e.g., t, f, etc...
    case f:
      return sort_ascending ? (firstFile.filename < secondFile.filename) : (firstFile.filename > secondFile.filename);
    case t:
      return sort_ascending ? (firstFile.type < secondFile.type) : (firstFile.type > secondFile.type);
    case c:
      return sort_ascending ? (firstFile.n_comments < secondFile.n_comments) : (firstFile.n_comments > secondFile.n_comments);
    case d:
      return sort_ascending ? (firstFile.n_doc_comments < secondFile.n_doc_comments) : (firstFile.n_doc_comments > secondFile.n_doc_comments);
    case b:
      return sort_ascending ? (firstFile.n_blank < secondFile.n_blank) : (firstFile.n_blank > secondFile.n_blank);
    case s:
      return sort_ascending ? (firstFile.n_loc < secondFile.n_loc) : (firstFile.n_loc > secondFile.n_loc);
    case a:
      return sort_ascending ? (firstFile.n_lines < secondFile.n_lines) : (firstFile.n_lines > secondFile.n_loc);
    default:
      return false;
  }
}

std::string language_to_string (lang_type_e lang_type) {
  switch (lang_type) {
    case C: return "C";
    case CPP: return "C++";
    case H: return "C/C++ header";
    case HPP: return "C++ header";
    default: return "Undefined type";
  }
}

//referring to lang_type_e enum
lang_type_e return_language_by_extension (const std::string& filename) {
  fs::path file(filename);

  std::string extension = file.extension().string();

  if (extension == ".cpp") return CPP;
  if (extension == ".c") return C;
  if (extension == ".hpp") return HPP;
  if (extension == ".h") return H;

  return UNDEF;
}

void print_summary(const std::vector<FileInfo>& db, const RunningOpt& run_options) {
  if (db.empty()) { //if there are not files to be printed
    std::cout << "No files processed.\n";
    return;
  }

  std::vector<FileInfo> sorted_db { db }; //creates a copy of the vector, and this copy will be sorted

  if (run_options.should_sort) {
    std::sort(sorted_db.begin(), sorted_db.end(), [&run_options](const FileInfo& a, const FileInfo& b) {
      return compare_files(a, b, run_options.sort_field, run_options.sort_ascending); //if compare_files return true, 'a' comes before 'b' after sorting
    });
  }

  //calculate column widths
  size_t max_filename_length {0};

  for (const auto& info : sorted_db) {
    max_filename_length = std::max(max_filename_length, info.filename.size()); //returns the greater of values
  }

  constexpr size_t MIN_FILENAME_WIDTH {20};
  size_t filename_width = std::max(max_filename_length, MIN_FILENAME_WIDTH);

  //print summary
  std::cout << "Files processed: " << sorted_db.size() << "\n";

  std::string separator(filename_width + 14 + 16 + 16 + 14 + 14 + 10 + 6, '-');

  std::cout << separator << "\n";
  std::cout << std::left << std::setw(filename_width + 1) << "Filename" << std::setw(14) << "Language" << std::setw(16) << "Comments" << std::setw(16) << "Doc Comments" << std::setw(14) << "Blank" << std::setw(14) << "Code" << "# of lines\n";
  std::cout << separator << "\n";

  //print data
  count_t total_comments = 0, total_doc_comments = 0, total_blank = 0, total_code = 0, total_lines = 0;

  for (const auto& info : sorted_db) {
    std::cout << std::left << std::setw(filename_width + 1) << info.filename << std::setw(14) << language_to_string(info.type) << std::setw(16) << value_with_percent(info.n_comments, info.n_lines) << std::setw(16) << value_with_percent(info.n_doc_comments, info.n_lines) << std::setw(14) << value_with_percent(info.n_blank, info.n_lines) << std::setw(14) << value_with_percent(info.n_loc, info.n_lines) << info.n_lines << "\n";

    //add totals
    total_comments += info.n_comments;
    total_doc_comments += info.n_doc_comments;
    total_blank += info.n_blank;
    total_code += info.n_loc;
    total_lines += info.n_lines;
  }

  //print all
  if (sorted_db.size() > 1) {
    std::cout << separator << "\n";
    std::cout << std::left << std::setw(filename_width + 1) << "SUM" << std::setw(14) << "" << std::setw(16) << total_comments << std::setw(16) << total_doc_comments << std::setw(14) << total_blank << std::setw(14) << total_code << total_lines << "\n";
  }

  std::cout << separator << "\n";
}



//== Main entry

int main(int argc, char* argv[]) {
  RunningOpt run_options;
  validate_arguments(argc, argv, run_options);

  collect_files(run_options);

  if (run_options.input_list.empty() && run_options.directory_list.empty()) {
    std::cerr << "Error: no input file or directory provided.\n";
    usage();
  }

  std::vector<FileInfo> db;

  for (const auto& file : run_options.input_list){
    FileInfo current_file;
    AttributeCount result = process_file(file);

    current_file.filename = file;
    current_file.type = return_language_by_extension(file);
    current_file.n_lines = result.lines;
    current_file.n_blank = result.blank;
    current_file.n_comments = result.com;
    current_file.n_doc_comments = result.dox;
    current_file.n_loc = result.loc;

    db.push_back(current_file);
  }

  print_summary(db, run_options);

  return EXIT_SUCCESS;
}