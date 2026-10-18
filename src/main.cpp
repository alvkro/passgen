#include <cstdlib>
#include <optional>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <cmath>
#include <unordered_set>
#include <algorithm>

using value_type = unsigned short;

constexpr size_t default_pass_length{ 7 };

/**
 * @brief Armazena os conjuntos de caracteres de cada grupo disponível no gerador.
 */
struct stringGroups {
  std::string LOWER{ "abcdefghijklmnopqrstuvwxyz" };
  std::string UPPER{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
  std::string DIGITS{ "0123456789" };
  std::string LOGOGRAMS{ "#$%&@^`~" };
  std::string PUNCTUATION{ ".,;:" };
  std::string QUOTES{ "'\"" };
  std::string SLASHES{ "\\/|_-" };
  std::string MATH{ "*+!?=" };
  std::string BRACES{ "{}[]()" };
};

/**
 * @brief Armazena todas as opções de execução obtidas dos argumentos da linha de comando.
 */
struct RunningOptions {
  size_t pass_length{ default_pass_length };
  bool show_password_strength{ false };
  stringGroups active_groups;
  std::string password_pool{ "" };
  bool verbose{ false };
  value_type group_counter {0};
  std::unordered_set<std::string> selected_groups;
};

/**
 * @brief Encapsula um gerador Mersenne Twister para seleção aleatória de caracteres.
 */
struct Randomic {
  std::mt19937 gen;

  /// @brief Inicializa o gerador usando um dispositivo de entropia de hardware.
  Randomic() {
    std::random_device rd;
    gen.seed(rd());
  }

  /**
   * @brief Retorna um índice aleatório no intervalo [0, string_size].
   * @param string_size Limite superior da distribuição.
   * @return Inteiro aleatório em [0, string_size].
   */
  auto randomEngine(value_type string_size) {
    std::uniform_int_distribution<> distrib(0, string_size);
    return distrib(gen);
  }
};

/**
 * @brief Valida se uma string representa um valor numérico válido.
 * @param value_str A string a ser convertida.
 * @return O valor convertido, ou vazio se inválido ou fora do intervalo.
 */
std::optional<value_type> validate_number(const std::string& value_str) {
  value_type temp{ 0 };
  try {
    temp = std::stoi(value_str);
  } catch (const std::invalid_argument& e) {
    return {};
  } catch (const std::out_of_range& e) {
    return {};
  }
  return temp;
}

/**
 * @brief Exibe as instruções de uso ou uma mensagem de erro na saída padrão.
 * @param msg Mensagem de erro opcional. Se vazia, exibe a tela de ajuda completa.
 */
void usage(std::string_view msg = "") {
  if (!msg.empty()) {
    std::cout << "Error: " << msg << std::endl;
  } else {
    std::cout << " This program generates passwords of specific length based on the selection of groups of characters.\n"
              << "Usage: passgen [<options>]\n"
              << "--len n            Size of the password (default is 7).\n"
              <<"-l, --lower        Add random characters from [a-z].\n"
              <<"-u, --upper        Add random characters from [A-Z].\n"
              << "-d, --digits       Add random characters from [0-9].\n"
              <<"-o, --logograms    Add random characters from [#$%&@^`~].\n"
              <<"-p, --punctuation  Add random characters from [.,;:].\n"
              << "-q, --quotes       Add random characters from [\"'].\n"
              << " -s, --slashes      Add random characters from [\\ /|_-].\n"
              <<"-m, --math         Add random characters from [*+!?=].\n"
              <<"-b, --braces       Add random characters from [{}[]()].\n"
              <<"-a, --all-groups   Add random characters from all the above groups.\n"
              <<"-v, --verbose      Prints extra information besides the password.\n"
              <<  "-h, --help         Show this help screen"<<std::endl;
  }
}

/**
 * @brief Processa os argumentos da linha de comando e preenche a struct de opções.
 * @param argc Quantidade de argumentos do main.
 * @param argv Vetor de argumentos do main.
 * @param run_options A struct de opções a ser preenchida.
 * @return A struct RunningOptions preenchida.
 */
RunningOptions validate_arguments(int argc, char* argv[], RunningOptions& run_options) {
  stringGroups string_groups;

  for (int i = 1; i < argc; ++i) {
    std::string argument{ argv[i] };
    if (argument == "--len") {
      if (i + 1 >= argc) {
        usage("Missing value for --len.");
        exit(0);
      }
      if (i + 1 < argc) {
        auto temp_val = validate_number(argv[++i]);
        if (!temp_val.has_value()) {
          usage("The value provided for --len is invalid or out of range.");
          exit(0);
        }
        value_type val = temp_val.value();
         if (val <= 0) {
          usage("Password length must be greater than 0.");
          exit(0);
        }
        run_options.pass_length = static_cast<size_t>(val);
      }
    }

    else if (argument == "--verbose" or argument == "-v") {
      run_options.show_password_strength = true;
      run_options.verbose = true;
    }
    else if (argument == "--lower" or argument == "-l") {
      if(run_options.selected_groups.insert("lower").second)
      run_options.group_counter++;
    }
    else if (argument == "--upper" or argument == "-u") {
      if(run_options.selected_groups.insert("upper").second)
      run_options.group_counter++;
    }
    else if (argument == "--digits" or argument == "-d") {
      if(run_options.selected_groups.insert("digits").second)
      run_options.group_counter++;
    }
    else if (argument == "--logograms" or argument == "-o") {
      if(run_options.selected_groups.insert("logograms").second)
      run_options.group_counter++;
    }
    else if (argument == "--punctuation" or argument == "-p") {
      if(run_options.selected_groups.insert("punctuation").second)
      run_options.group_counter++;
    }
    else if (argument == "--quotes" or argument == "-q") {
      if(run_options.selected_groups.insert("quotes").second)
      run_options.group_counter++;
    }
    else if (argument == "--slashes" or argument == "-s") {
      if(run_options.selected_groups.insert("slashes").second)
      run_options.group_counter++;
    }
    else if (argument == "--math" or argument == "-m") {
      if(run_options.selected_groups.insert("math").second)
      run_options.group_counter++;
    }
    else if (argument == "--braces" or argument == "-b") {
      if(run_options.selected_groups.insert("braces").second)
      run_options.group_counter++;
    }
    else if (argument == "--all-groups" or argument == "-a"){
      run_options.selected_groups.insert("braces");
      run_options.selected_groups.insert("math");
      run_options.selected_groups.insert("slashes");
      run_options.selected_groups.insert("quotes");
      run_options.selected_groups.insert("punctuation");
      run_options.selected_groups.insert("logograms");
      run_options.selected_groups.insert("digits");
      run_options.selected_groups.insert("upper");
      run_options.selected_groups.insert("lower");
      run_options.group_counter = run_options.selected_groups.size();
    }

    else if (argument == "--help" || argument == "-h") {
      usage();
      exit(0);
    } 

    else {
      usage("Invalid argument: " + argument);
      exit(0);
    }
  }

  if (run_options.selected_groups.empty()) {
    run_options.selected_groups.insert("lower");
  }

  run_options.password_pool.clear();

  for (const auto& group : run_options.selected_groups) {
      if (group == "lower") run_options.password_pool += run_options.active_groups.LOWER;
      if (group == "upper") run_options.password_pool += run_options.active_groups.UPPER;
      if (group == "digits") run_options.password_pool += run_options.active_groups.DIGITS;
      if (group == "logograms") run_options.password_pool += run_options.active_groups.LOGOGRAMS;
      if (group == "punctuation") run_options.password_pool += run_options.active_groups.PUNCTUATION;
      if (group == "quotes") run_options.password_pool += run_options.active_groups.QUOTES;
      if (group == "slashes") run_options.password_pool += run_options.active_groups.SLASHES;
      if (group == "math") run_options.password_pool += run_options.active_groups.MATH;
      if (group == "braces") run_options.password_pool += run_options.active_groups.BRACES;
  }

  if (run_options.pass_length < run_options.group_counter) {
      run_options.pass_length = run_options.group_counter;
  }

  return run_options;
}

/**
 * @brief Responsável pela geração e avaliação de qualidade da senha.
 */
struct Password {
  RunningOptions run_opt;
  std::string password_pool;
  stringGroups string_group;

  int entropy{0};
  value_type variability{0};
  std::string quality {""};

  /**
   * @brief Escolhe um caractere aleatório de um grupo de caracteres.
   * @param group O grupo de caracteres de onde selecionar.
   * @param rng   O gerador aleatório a ser utilizado.
   * @return Um caractere selecionado aleatoriamente.
   */
  char pick_random_char(const std::string& group, Randomic& rng) {
    auto idx = rng.randomEngine(group.size() - 1);
    return group[idx];
  }

  /**
   * @brief Gera uma senha com base nos grupos ativos e no comprimento desejado.
   *
   * Garante ao menos um caractere de cada grupo selecionado e preenche
   * as posições restantes aleatoriamente a partir do pool completo.
   *
   * @param run_options As opções de execução atuais.
   * @return A senha gerada.
   */
  std::string generate_password(const RunningOptions& run_options) {
    std::string password;
    Randomic rng;

    std::vector<std::string> selected_groups;

    if (run_options.password_pool.find_first_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos)
        selected_groups.push_back(run_options.active_groups.LOWER);
    if (run_options.password_pool.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos)
        selected_groups.push_back(run_options.active_groups.UPPER);
    if (run_options.password_pool.find_first_of("0123456789") != std::string::npos)
        selected_groups.push_back(run_options.active_groups.DIGITS);
    if (run_options.password_pool.find_first_of("#$%&@^`~") != std::string::npos)
        selected_groups.push_back(run_options.active_groups.LOGOGRAMS);
    if (run_options.password_pool.find_first_of(".,;:") != std::string::npos)
        selected_groups.push_back(run_options.active_groups.PUNCTUATION);
    if (run_options.password_pool.find_first_of("\"'") != std::string::npos)
        selected_groups.push_back(run_options.active_groups.QUOTES);
    if (run_options.password_pool.find_first_of("\\/|_-") != std::string::npos)
        selected_groups.push_back(run_options.active_groups.SLASHES);
    if (run_options.password_pool.find_first_of("*+!?=") != std::string::npos)
        selected_groups.push_back(run_options.active_groups.MATH);
    if (run_options.password_pool.find_first_of("{}[]()") != std::string::npos)
        selected_groups.push_back(run_options.active_groups.BRACES);

    for (const auto& group : selected_groups) {
        if (password.size() < run_options.pass_length)
            password.push_back(pick_random_char(group, rng));
    }

    while (password.size() < run_options.pass_length) {
        auto idx = rng.randomEngine(run_options.password_pool.size() - 1);
        password.push_back(run_options.password_pool[idx]);
    }

    return password;
  }

  /**
   * @brief Avalia a qualidade da senha com base na entropia.
   *
   * A entropia é calculada como E = L * log2(variabilidade), onde L é o
   * comprimento da senha e variabilidade é a quantidade de caracteres distintos.
   *
   * @param password     A senha gerada a ser avaliada.
   * @param entropy      Saída: entropia calculada em bits.
   * @param variability  Saída: número de caracteres distintos na senha.
   * @param quality      Saída: classificação qualitativa (Muito Fraca a Excelente).
   */
  void password_quality(std::string_view password, int& entropy, value_type& variability, std::string& quality){
    auto L { password.size() };

    std::unordered_set<char> unique(password.begin(), password.end());
    variability = unique.size();

    entropy = static_cast<int>(L * std::log2(variability));

    if (entropy <= 25) quality = "Very Weak";
    else if (entropy <= 45) quality = "Weak";
    else if (entropy <= 80) quality = "Good";
    else if (entropy <= 100) quality = "Strong";
    else if (entropy <= 120) quality = "Very Strong";
    else quality = "Excellent";
  }
};

/**
 * @brief Ponto de entrada do programa. Processa os argumentos, gera e exibe a senha.
 * @param argc Quantidade de argumentos.
 * @param argv Vetor de argumentos.
 * @return EXIT_SUCCESS em caso de sucesso.
 */
int main(int argc, char* argv[]){
  Password pass;
  RunningOptions run_options;
  Randomic getRandomic;

  std::cout << "-- Welcome to passgen v 1.0, © DIMAp --\n\n";

  validate_arguments(argc, argv, run_options);
  pass.run_opt = run_options;

  auto password = pass.generate_password(run_options);

  std::shuffle(password.begin(), password.end(), getRandomic.gen);

  std::cout << ">>> The generated password is:\n" << password << "\n\n";

  if (run_options.verbose) {
    pass.password_quality(password, pass.entropy, pass.variability, pass.quality);

    std::cout << ">>> Variability (distinct characters in the password) is: "
              << pass.variability << std::endl;
    std::cout << "Password quality: " << pass.quality << std::endl;
    std::cout << "Entropy: " << pass.entropy << " bits" << std::endl;
  }

  return EXIT_SUCCESS;
}