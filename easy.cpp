#include<iostream>
#include<vector>
#include<string>
#include<filesystem>
#include<fstream>
#include<sstream>
#include<omp.h>

void my_assert(bool exp, std::string assert_message) {
    if (exp) return;
    std::cerr << "Assert failed: " << std::endl << assert_message << std::endl;
    abort();
}

bool is_positive_integer(std::string &s) { // includes 0
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    int res = stoi(s);
    return s == std::to_string(res);
}

bool is_integer(std::string &s) {
    if (s[0] == '-') {
        std::string t = s.substr(1, int(s.size()) - 1);
        return is_positive_integer(t);
    }
    return is_positive_integer(s);
}

bool is_txt_file(std::string &s) {
    int sz_s = int(s.size());
    if (sz_s < 4) return false;
    std::string suffix = s.substr(sz_s - 4, 4);
    return suffix == ".txt";
}

bool is_file_exist(std::string &s) {
    std::ifstream f(s.c_str());
    return f.good();
}


int read_int(std::ifstream &in) {
    std::string s;
    my_assert(bool(in >> s), "Expected integer but found end of file");
    my_assert(is_integer(s), s + " expected as integer");
    return stoi(s);
}

int read_positive_int(std::ifstream &in) {
    int res = read_int(in);
    std::string s = std::to_string(res);
    my_assert(is_positive_integer(s), s + " expected as positive integer");
    return res;
}

void check_end_of_file(std::ifstream &in) {
    std::string s;
    bool res = !(bool(in >> s));
    my_assert(res, "Expected end of file, but found " + s);
    return ;
}

int threads;

int sum(std::vector<int> &v) {
    int res = 0;
    #pragma omp parallel shared(v) reduction(+: res) num_threads(threads)
    {
        #pragma omp for
        for (size_t i = 0; i < v.size(); i++) {
            res += v[i];
        }
    }
    
    return res;
}


void solve(std::ifstream &in, std::ofstream &out) {
    int n = read_positive_int(in);
    std::vector <int> v(n);
    for (int i = 0; i < n; i++) {
        v[i] = read_int(in);
    }
    check_end_of_file(in);
    out << sum(v);
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    // {hw5.exe, [threads], [input_file], [output_file], [coef]}
    my_assert(int(args.size()) == 4, "Expected 3 arguments");
    my_assert(is_positive_integer(args[1]), " Expected " + args[1] + " as positive integer");
    my_assert(is_file_exist(args[2]), "Input file " + args[2] + " doesn't exist");
    my_assert(is_txt_file(args[2]), "Input file: " + args[2] + " expected as .txt file");
    my_assert(is_txt_file(args[3]), "Output file: " + args[3] + " expected as .txt file");
    
    threads = stoi(args[1]);
    std::string input_file = args[2];
    std::string output_file = args[3];

    std::ifstream in_file;
    in_file.open(input_file);
    std::ofstream out_file(output_file);

    solve(in_file, out_file);
    
    out_file.close();
    in_file.close();
    return 0;
}
/*

int main() {
    int value = 123;
    #pragma omp parallel
    {
        #pragma omp atomic
        value++;
        #pragma omp barrier
        #pragma omp critical(cout)
        {
            std::cout << value << std::endl;
        }
    }
    return 0;
}

*/