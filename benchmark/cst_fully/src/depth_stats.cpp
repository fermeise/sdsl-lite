
#include <sdsl/suffix_trees.hpp>
#include <iostream>

using namespace sdsl;

typedef CST_TYPE cst_type;
typedef cst_sada<cst_type::csa_type, lcp_dac<> > fast_cst_type;

int main(int argc, char **argv) {
    if(argc < 3) {
        std::cout << "Usage: " << argv[0] << " input_file tmp_dir" << std::endl;
        std::cout << " Generates a CST and outputs statistics about the distribution of depth." << std::endl;
        std::cout << " Temporary files are stored in tmp_dir." << std::endl;
        return 1;
    }

    const char* input_file = argv[1];
    const char* tmp_dir = argv[2];

    fast_cst_type cst;
    cache_config config(true, tmp_dir);
    construct(cst, input_file, config, fast_cst_type::alphabet_category::WIDTH == 8 ? 1 : 0);

    size_t max_depth = 0;
    size_t total_depth = 0;
    size_t node_count = 0;

    std::vector<size_t> depth_cnt(128, 0);
    std::vector<size_t> depth_cnt_exp(64, 0);
    std::vector<size_t> depth_cnt_exp_sample(64, 0);

    for(auto it = cst.begin(); it != cst.end(); ++it) {
        if(it.visit() == 1 && !cst.is_leaf(*it)) {
            const size_t depth = cst.depth(*it);
            if(depth < 128) {
                depth_cnt[depth]++;
            }
            if(depth > 0) {
                depth_cnt_exp[bits::hi(depth)]++;
            }
            if(depth == (1u << bits::hi(depth))) {
                depth_cnt_exp_sample[bits::hi(depth)]++;
            }
            if(depth > max_depth) {
                max_depth = depth;
            }
            total_depth += depth;
            node_count++;
        }
    }

    std::cout << "Number of nodes = " << node_count << std::endl;
    for(size_t i = 0; i < 128; i++) {
        std::cout << "Number of nodes with depth " << i << " = " << depth_cnt[i] << std::endl;
    }
    for(size_t i = 0; i < 64 && depth_cnt_exp[i] > 0; i++) {
        std::cout << "Number of nodes with depth between " << (1 << i) << " and " << (1 << (i + 1))-1 << " = " << depth_cnt_exp[i] << std::endl;
    }
    for(size_t i = 0; i < 64 && depth_cnt_exp_sample[i] > 0; i++) {
        std::cout << "Number of nodes with depth " << (1 << i) << " = " << depth_cnt_exp_sample[i] << std::endl;
    }
    std::cout << "Maximum depth = " << max_depth << std::endl;
    std::cout << "Average depth = " << total_depth / node_count << std::endl;

    return 0;
}
