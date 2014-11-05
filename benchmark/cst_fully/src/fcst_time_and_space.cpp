#include <sdsl/suffix_trees.hpp>
#include <fstream>
#include <chrono>

using namespace sdsl;
using timer = std::chrono::high_resolution_clock;

typedef cst_fully<cst_sada<csa_wt<wt_huff<rrr_vector<63> >, 32, 32, text_order_sa_sampling<>, text_order_isa_sampling_support<>>, lcp_wt<> > > cst_type;

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Syntax: " << argv[0] << " [input file]" << std::endl;
        return 1;
    }

    const char* input_file = argv[1];

    cst_type::construction_cst_type cst;
    construct(cst, input_file, 1);
    cst_type fcst(cst, 100, false);

    std::cout << "# ALPHABET_SIZE = " << fcst.csa.sigma << std::endl;
    std::cout << "# DATA_SIZE = " << fcst.csa.text.size() << std::endl;
    std::cout << "# TREE_SIZE = " << cst.nodes() << std::endl;
    std::cout << "# SAMPLING_FACTOR = " << fcst.delta << std::endl;
    std::cout << "# SAMPLED_TREE_SIZE = " << fcst.sampled_nodes() << std::endl;

    std::cout << "# FCST_SIZE = " << size_in_bytes(fcst) << std::endl;
    std::cout << "# CST_SIZE = " << size_in_bytes(cst) << std::endl;
    std::cout << "# CSA_SIZE = " << size_in_bytes(fcst.csa) << std::endl;

    return 0;
}
