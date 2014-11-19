
#include <sdsl/suffix_trees.hpp>
#include <fstream>
#include <chrono>

using namespace sdsl;
using namespace std::chrono;
using timer = std::chrono::high_resolution_clock;

typedef cst_fully<csa_wt<wt_huff<rrr_vector<63> >, 32, 32, text_order_sa_sampling<>, text_order_isa_sampling_support<> > > cst_type;

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Syntax: " << argv[0] << " [input file]" << std::endl;
        std::cout << "e.g. " << argv[0] << " english.1MB" << std::endl;
        return 1;
    }

    const char* input_file = argv[1];

    memory_monitor::start();

    cst_type fcst;
    auto start = timer::now();
    construct(fcst, input_file, 1);
    auto stop = timer::now();
    std::cout << "construction cst time in milliseconds: " << duration_cast<milliseconds>(stop-start).count() << std::endl;

    memory_monitor::stop();

    std::cout << "peak usage = " << memory_monitor::peak() / (1024*1024) << " MB" << std::endl;

    std::ofstream cstofs("construction.html");
    std::cout << "writing memory usage visualization to construction.html" << std::endl;
    memory_monitor::write_memory_log<HTML_FORMAT>(cstofs);
    cstofs.close();

    std::ofstream out_fcst("structure.html");
    std::cout << "writing structure visualization to structure.html" << std::endl;
    write_structure<HTML_FORMAT>(fcst, out_fcst);

    return 0;
}
