
#include <sdsl/suffix_trees.hpp>
#include <fstream>
#include <chrono>

using namespace sdsl;
using timer = std::chrono::high_resolution_clock;

typedef cst_fully<csa_wt<wt_huff<rrr_vector<63> >, 32, 32, text_order_sa_sampling<>, text_order_isa_sampling_support<> > > cst_type;

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Syntax: " << argv[0] << " [input file]" << std::endl;
        std::cout << "e.g. " << argv[0] << " english.1MB" << std::endl;
        return 1;
    }

    const char* input_file = argv[1];

    cst_type::construction_cst_type cst;

    auto start = timer::now();
    construct(cst, input_file, 1);
    auto stop = timer::now();
    std::cout << "Construction CST: " << (stop - start) << std::endl;

    start = timer::now();
    cst_type fcst(cst, 100, true);
    stop = timer::now();
    std::cout << "Construction FCST: " << (stop - start) << std::endl;

    std::ofstream out_fcst("structure.html");
    write_structure<HTML_FORMAT>(fcst, out_fcst);
    std::cout << "Output written to structure.html." << std::endl;

    return 0;
}
