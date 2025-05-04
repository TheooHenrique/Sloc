/*!
 * @file main.cpp
 * @description
 * This program implements a single line of code count for C/C++ programs.
 * @author	Add your name here
 * @date	September, 9th 2024.
 * @remark On 2022-09-9 changed ...
 */

 #include "main.hpp"

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
inline std::string ltrim(const std::string& s, const char* t = " \t\n\r\f\v") {
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
inline std::string rtrim(const std::string& s, const char* t = " \t\n\r\f\v") {
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
inline std::string trim(const std::string& s, const char* t = " \t\n\r\f\v") {
  return ltrim(rtrim(s, t), t);
}

//validate arguments in CLI
void validate_arguments(int argc, char* argv[], RunningOpt& run_options) {
  for (size_t ct{1}; ct < argc; ++ct) {
    if (strcmp(argv[ct], "--help") == 0 || strcmp(argv[ct], "-h") == 0){ //strcmp returns 0 if strings are equal
      usage();
      exit(0);
    } else if (strcmp(argv[ct], "-r") == 0) {
      run_options.recursive = true; //should look for files recursively in the directory provided. (eu nao sei oq isso faria de vdd, mas vamo indo ne, vamos descobrir uma hora)
    } else if (strcmp(argv[ct], "-s") == 0 || strcmp(argv[ct], "-S") == 0){ //é obrigatorio o user colocar isso?
      run_options.sort_ascending = (strcmp(argv[ct], "-s") == 0);
      run_options.sort_field = argv[++ct][0];
    } else { //if it's a file or directory, add to the list
      run_options.input_list.push_back(argv[ct]);
    }
  }

  if (run_options.input_list.empty()) {
    std::cerr << "Error: no input file or directory provided.\n";
    usage();
  }
}

//== Main entry

int main(int argc, char* argv[]) {

  RunningOpt run_options;
  validate_arguments(argc, argv, run_options);

  std::vector<FileInfo> db;
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