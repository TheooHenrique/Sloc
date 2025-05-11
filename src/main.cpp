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
AttributeCount updateState(std::string line, Turnstile &ts){
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
      else if(!line.empty() && line[0] != '/'){
        atributes.loc = 1;
      }

    }
    if (ts.current_state == ts.COMMENT){
      atributes.com = 1;
    }
    if (ts.current_state == ts.DOXY){
      atributes.dox = 1;
    }



        for (size_t i{0}; i < len; ++i){
          auto minline3 = line.substr(i, 3);
          auto minline2 = line.substr(i, 2);

          if (ts.current_state == ts.DOXY || ts.current_state == ts.COMMENT){
            if (minline2 == "*/"){
              ts.current_state = ts.CODE;
              auto lineafterclosecomment = line.substr(i + 2, line.length());
              if (!lineafterclosecomment.empty()){
                AttributeCount recursiveAttributes = updateState(lineafterclosecomment, ts); 
                atributes.loc += recursiveAttributes.loc;
                break;
              }
            }
          }
          
          if (ts.current_state == ts.CODE || ts.current_state == ts.START){
            if (minline3 == "/*!" || minline3 == "/**"){
              if (!isPartOfCode(minline3, line)) {
                ts.current_state = ts.DOXY;
                atributes.dox = 1;
                i += 2;
                continue;
              }
            } 
  
            if (minline3 == "//!" || minline3 == "///"){
              if (!isPartOfCode(minline3, line)){
                atributes.dox = 1; 
                break;
              }
            }
  
            if (minline2 == "/*"){
              if (!isPartOfCode(minline2, line)){
                ts.current_state = ts.COMMENT;
                atributes.com = 1;
                i += 1;
                continue;
              }
            }
            if (minline2 == "//"){
              if (!isPartOfCode(minline2, line)){
                atributes.com = 1;
                break;
              }
            }
          }

        }
      return atributes;
    }

AttributeCount statesMachine(const std::string& filename, Turnstile ts, AttributeCount &atr){
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
  Turnstile ts;

  statesMachine(filename, ts, atr);
  atr.lines = countTotalLines(filename);

  // deixa assim por enquanto
  //info.n_comments = 0;
  //info.n_doc_comments = 0;

  return atr;
}
 
//validate arguments in CLI
void validate_arguments(int argc, char* argv[], RunningOpt& run_options) {
  for (size_t ct{1}; ct < argc; ++ct) {
    auto it {inputed_arguments_with_their_keys.find(argv[ct])}; //find the key argv[ct] in inputed_arguments_with_their_keys, which is, e.g., "-r" in case of recursive
    if (it != inputed_arguments_with_their_keys.end()){ //.find() returns .end() if nothing is found with that inputed argument
      enum_arguments arg {it -> second}; //iterator -> second returns the value of the dictionary

      switch(arg){ //if the value of the unordered_map is
        case RECURSIVE: run_options.recursive = true; break;
        case SORTDES: run_options.sort_descending = true; break;
        case SORTAS: run_options.sort_ascending = true; break;
        case HELP: run_options.help = true; break;
      }
    }

    if (run_options.help){
      usage();
      exit(0);
    }

    //Checking if sort arguments are correctly inputed
    if (run_options.sort_ascending || run_options.sort_descending){
      if (ct + 1 >= argc) { //treating memory leak
        std::cerr << "Missing value\n";
        usage();
        exit(1);
      }
      const char* nextArgument{argv[ct+1]};
      bool valid_input = true;
      if (strcmp(nextArgument, "f") == 0 && strcmp(nextArgument, "t") == 0 && strcmp(nextArgument, "c") == 0 && strcmp(nextArgument, "d") == 0 && strcmp(nextArgument, "b") == 0 && strcmp(nextArgument, "s") == 0 && strcmp(nextArgument, "a") == 0){
        valid_input = false;
        std::cerr << "Insert a parameter after sort argument! See the parameter list below:\n";
        usage();
        exit(0);
      }
    }
  }


  for (size_t ct{1}; ct < argc; ++ct) { //another of this loop so the files/directories can be analysed separated
    std::string argument(argv[ct]); //this will be the file or directory we analyse
    
    if(argument.length() >= 4) {
      std::string last4 {argument.end() - 4, argument.end()};
      std::string last2 {argument.end() - 2, argument.end()};
      if (last4 == ".cpp" || last4 == ".hpp" || last2 == ".c" || last2 == ".h"){
        if (run_options.added_files.find(argument) == run_options.added_files.end()) { //if this element is not found in added_files, e.g., if main.cpp is not found in added_files, then we need to add it with the insert
          run_options.input_list.push_back(argument); //add the file .cpp, .c, .h or .hpp in input_list
          run_options.added_files.insert(argument); //insert this added file to the set of added files
        }
        

        continue; //using continue for the loop to skip to the next iteration. Below is the recursion code, which will first check whether the argument is a directory or not. As it has already been seen in this part that it is a file, it makes no sense to check whether it is the directory
      } 
    }

    if (run_options.recursive) {
      if (fs::exists(argument) && fs::is_directory(argument)) {
        run_options.directory_list.push_back(argument); //adding this directory to this list in run_options
      }
    }
  }
}

//== Collect Files
/*
void collect_files(std::vector<std::string> input_list, bool recursive, std::vector<FileInfo> db){ //check all .c, .cpp, .h, .hpp
  if (!recursive) {

  }
} */

void collect_files(RunningOpt& run_options) {
  if (!run_options.recursive || run_options.directory_list.empty()) return; //return gets out of the function

  for (const auto& directory : run_options.directory_list) { //for each directory in directory_list
    for (const auto& directory_file : fs::recursive_directory_iterator(directory)) {
      if (directory_file.is_regular_file()) { //if the file in the directory is a file, not another directory
        std::string file_path = directory_file.path().string(); //path() gets the... path... of the file, and .string() converts the path to string

        if (file_path.length() >= 4) {
          std::string last4(file_path.end() - 4, file_path.end());
          std::string last2(file_path.end() - 2, file_path.end());

          if (last4 == ".cpp" || last4 == ".hpp" || last2 == ".c" || last2 == ".h") {
              if (run_options.added_files.find(file_path) == run_options.added_files.end()) { //if the path of the file is not added
                run_options.input_list.push_back(file_path); //add the file path to the input_list
                run_options.added_files.insert(file_path); //now the path is added. this was made to avoid duplicate counting of a file when the user puts the file and also its directory
              }
          }
        }
      }
    }
  }
}

void print_summary(const std::vector<FileInfo>& db/*, Turnstile ts*/) {
  int total_files = 0;
  count_t total_lines = 0;
  count_t total_blank = 0;

  for (const auto& info : db) {
      std::cout << "--- Arquivo: " << info.filename << " ---\n";
      std::cout << "  Linhas totais:   " << info.n_lines << "\n";
      std::cout << "  Linhas de código: " << info.n_loc << "\n";
      std::cout << "  Linhas em branco: " << info.n_blank << "\n";
      std::cout << "  Linhas de comentário: " << info.n_comments << "\n";
      std::cout << "  Linhas de comentário doxygen: " << info.n_doc_comments << "\n";

      total_files++;
      total_lines += info.n_lines;
      total_blank += info.n_blank;
  }

  if (!db.empty()) {
      std::cout << "\n--- Resumo Geral ---\n";
      std::cout << "Total de arquivos processados: " << total_files << "\n";
      std::cout << "Total de linhas:               " << total_lines << "\n";
      std::cout << "Total de linhas em branco:     " << total_blank << "\n";
  }
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

  for (const auto& file :run_options.input_list){
    FileInfo current_file;
    AttributeCount result = process_file(file);

    
    current_file.filename = file;
    current_file.n_lines = result.lines;
    current_file.n_blank = result.blank;
    current_file.n_comments = result.com;
    current_file.n_doc_comments = result.dox;
    current_file.n_loc = result.loc;

    db.push_back(current_file);
  }

  

  /*tem q declarar essas funções.

  sort_files vai ordenar os arquivos. honestamente não entendi mt bem como seria a ordenação. vamos falando com nossos colegas e com o prof

  print_report calcula largura máxima das colunas (especialmente nome do arquivo), imprime cabeçalho da tabela, imprime cada linha de db. Se tiver mais de 1 arquivo, imprime linha de soma.

  if (run_options.sort_field != '\0') {
    sort_files(db, run_options.sort_field, run_options.sort_descending);
  }
  
  print_report(db);*/

  print_summary(db);

  ///test to check if FileInfo is working and if the db vector is getting infos
  FileInfo fc;
  fc.filename = "test.cpp";
  fc.type = CPP;
  fc.n_comments = 4;
  fc.n_blank = 5;
  fc.n_loc = 15;
  fc.n_lines = 20;

  db.push_back(fc);

  db.emplace_back("test.cpp", CPP, 3, 10, 15, 18);

  return EXIT_SUCCESS;
}