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
std::string universal{"entrei em nada"};
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

//count total lines (substituir esse código pra ele pegar as linhas de código, e não apenas todas)
count_t countTotalLines (const std::string& filename) {
  std::ifstream file(filename);
  int line = 0;
  std::string codeLines;

  while (std::getline(file, codeLines)) ++line;
  
  return line;
}




///Verify if the comment command is part of the code
bool isPartOfCode(std::string str, std::string line){
for (uint8_t i{0}; i < line.length(); i++ ){
  size_t indexStart{line.find(str)};
  size_t indexEnd{indexStart + str.length()};

  if (line[indexStart - 1] == '"' && line[indexEnd + 1] == '"'){
    return true;
  }
}
return false;
}
/*
*/


///Update the current state
void updateState(std::string line, Turnstile ts, FileInfo info){
  int blankLine;
  uint8_t len{static_cast<uint8_t>(line.length())};
  //universal = "EU ENTREI AQUI";
  for (size_t i{0}; i < len; ++i){
    auto minline = line.substr(i, 3);

    if (minline == "/*"){
      //universal = "EU ENTREI AQUI";
      //break;
      if (!isPartOfCode(minline, line)){ts.current_state = ts.COMMENT;}
      i += 2;
      continue;
      info.n_comments++;
    }

    if (minline == "/*!" || minline == "/**"){
      if (!isPartOfCode(minline, line)) {ts.current_state = ts.DOXY;}
      i += 2;
      info.n_doc_comments++;
      continue;
    } 


    if (ts.current_state == ts.COMMENT || ts.current_state == ts.DOXY){
      if (!isPartOfCode(minline, line)){ts.current_state = ts.CODE;}
      i += 2;
      info.n_loc++;
      continue;
    }
    
    if (ts.current_state != ts.COMMENT && ts.current_state != ts.DOXY){
      bool blank{true};
      universal = "EU ENTREI AQUI";
      for (char character : line) {
        if (!line.empty()) {blank = false; break;}
      }
      
      if (blank) info.n_blank++;
    }

  }
}

void statesMachine(const std::string& filename, Turnstile ts, FileInfo info){
  std::ifstream file(filename);
  std::string codeLines;

  while (std::getline(file, codeLines)) {
    updateState(codeLines, ts, info);
  }
}

/*
//count blank lines
count_t countBlankLines (const std::string& filename) {
  std::ifstream file(filename);
  int blankLine{0};
  std::string blankCodeLines;

  while (std::getline(file, blankCodeLines)) {
    bool blank = true;

    for (char line : blankCodeLines) {
      if (!std::isspace(line)) {blank = false; break;}
    }
    
    if (blank) ++blankLine;
  }

  return blankLine;
}*/

//count comment lines


//function to apply the functions of totallines and blanklines to an object
FileInfo process_file(const std::string& filename) {
  FileInfo info(filename);
  Turnstile ts;

  statesMachine(filename, ts, info);
  info.n_lines = countTotalLines(filename);

  // deixa assim por enquanto
  info.n_comments = 0;
  info.n_doc_comments = 0;
  info.n_loc = info.n_lines - info.n_blank - info.n_comments;

  return info;
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

    std::string argument(argv[ct]);
    if(argument.length() >= 4) {
      std::string last4 {argument.end() - 4, argument.end()};
      std::string last2 {argument.end() - 2, argument.end()};
      if (last4 == ".cpp" || last4 == ".hpp" || last2 == ".c" || last2 == ".h"){
        run_options.input_list.push_back(argv[ct]);
      }
    }
  }

  if (run_options.input_list.empty()) {
    std::cerr << "Error: no input file or directory provided.\n";
    usage();
  }
}

//== Collect Files

void collect_files(std::vector<std::string> input_list, bool recursive, std::vector<FileInfo> db){ //check all .c, .cpp, .h, .hpp
  if (!recursive) {

  }
} 

void print_summary(const std::vector<FileInfo>& db/*, Turnstile ts*/) {
  int total_files = 0;
    count_t total_lines = 0;
    count_t total_blank = 0;

    for (const auto& info : db) {
        std::cout << "--- Arquivo: " << info.filename << " ---\n";
        std::cout << "  Linhas totais:   " << info.n_lines << "\n";
        std::cout << "  Linhas em branco: " << info.n_blank << "\n";
        std::cout << "  Linhas de comentário: " << info.n_comments << "\n";
        //std::cout << "  Estado: " << ts.current_state << "\n";
        std::cout << "  FUNCIONA PFV: " << universal << "\n";

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

  std::vector<FileInfo> db;

  for (const auto& file : run_options.input_list) {
    db.push_back(process_file(file)); //adding each new file object to db
  }

  /*tem q declarar essas funções. collect_files vai pegar os arquivos .c, .cpp, .h e .hpp.

  count_lines vai contar n_blank, n_comments, n_doc_comments, n_loc e n_lines

  sort_files vai ordenar os arquivos. honestamente não entendi mt bem como seria a ordenação. vamos falando com nossos colegas e com o prof

  print_report calcula largura máxima das colunas (especialmente nome do arquivo), imprime cabeçalho da tabela, imprime cada linha de db. Se tiver mais de 1 arquivo, imprime linha de soma.

  collect_files(run_options.input_list, run_options.recursive, db); //check all .c, .cpp, .h, .hpp

  for (auto& data : db) {
    count_lines(data);
  }

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