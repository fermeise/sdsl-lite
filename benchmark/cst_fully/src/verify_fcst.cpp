
#include <sdsl/suffix_trees.hpp>
#include <fstream>
#include <chrono>

using namespace sdsl;

typedef FCST_TYPE fcst_type;
typedef CST_TYPE cst_type;

const size_t NUM_REPETITIONS = 10000;
const size_t BURST_SIZE = 100;

std::default_random_engine &get_generator() {
    static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    return generator;
}

template<class t_cst>
class node_sampler_uniform {
private:
    const t_cst &m_cst;
    std::uniform_int_distribution<typename t_cst::size_type> m_distribution;

public:
    node_sampler_uniform(const t_cst &cst)
    : m_cst(cst),
      m_distribution(0, m_cst.size() - 2)
    {}

    std::vector<typename t_cst::node_type> get_data(size_t sample_count, size_t) {
        std::vector<typename t_cst::node_type> res;
        res.reserve(sample_count);

        for(size_t i = 0; i < sample_count; i++) {
            auto leaf = m_distribution(get_generator());
            res.push_back(m_cst.lca(m_cst.select_leaf(leaf + 1), m_cst.select_leaf(leaf + 2)));
        }

        return res;
    }
};

template<class t_cst, class t_cst_ref>
bool run_tests(const t_cst &cst, const t_cst_ref &cst_ref) {
    auto nodes = node_sampler_uniform<t_cst_ref>(cst_ref).get_data(NUM_REPETITIONS, BURST_SIZE);

    for(auto v_ref: nodes) {
        std::uniform_int_distribution<typename t_cst::size_type> distribution(cst_ref.lb(v_ref), cst_ref.rb(v_ref));

        auto v1_ref = cst_ref.select_leaf(distribution(get_generator()) + 1);
        auto v2_ref = cst_ref.select_leaf(distribution(get_generator()) + 1);

        auto v1 = cst.node(cst_ref.lb(v1_ref), cst_ref.rb(v1_ref));
        auto v2 = cst.node(cst_ref.lb(v2_ref), cst_ref.rb(v2_ref));

        auto u = cst.lca(v1, v2);
        auto u_ref = cst_ref.lca(v1_ref, v2_ref);

        if(cst.lb(u) != cst_ref.lb(u_ref) ||
           cst.rb(u) != cst_ref.rb(u_ref)) {
            std::cout << "# Error while calculating LCA("
                      << "[" << cst_ref.lb(v1_ref) << ", " << cst_ref.rb(v1_ref) << "], " << std::endl
                      << "[" << cst_ref.lb(v2_ref) << ", " << cst_ref.rb(v2_ref) << "])." << std::endl;
            std::cout << "# Result: [" << cst.lb(u) << ", " << cst.rb(u) << "]" << std::endl;
            std::cout << "# Expected: [" << cst_ref.lb(u_ref) << ", " << cst_ref.rb(u_ref) << "]" << std::endl;
            return false;
        }
    }

    std::cout << "# LCA: No errors." << std::endl;

    for(auto v_ref: nodes) {
        auto v = cst.node(cst_ref.lb(v_ref), cst_ref.rb(v_ref));

        auto d = cst_ref.depth(v_ref);
        if(d > 0) {
            auto c = cst.edge(v, d);
            auto c_ref = cst_ref.edge(v_ref, d);

            if(c != c_ref) {
                std::cout << "# Error while calculating LETTER(["
                          << cst_ref.lb(v_ref) << ", " << cst_ref.rb(v_ref) << "])." << std::endl;
                std::cout << "# Result: " << c << std::endl;
                std::cout << "# Expected: " << c_ref << std::endl;
                return false;
            }
        }
    }

    std::cout << "# LETTER: No errors." << std::endl;

    for(auto v_ref: nodes) {
        auto v = cst.node(cst_ref.lb(v_ref), cst_ref.rb(v_ref));

        auto u = cst.sl(v);
        auto u_ref = cst_ref.sl(v_ref);

        if(cst.lb(u) != cst_ref.lb(u_ref) ||
           cst.rb(u) != cst_ref.rb(u_ref)) {
            std::cout << "# Error while calculating SLINK(["
                      << cst_ref.lb(v_ref) << ", " << cst_ref.rb(v_ref) << "])." << std::endl;
            std::cout << "# Result: [" << cst.lb(u) << ", " << cst.rb(u) << "]" << std::endl;
            std::cout << "# Expected: [" << cst_ref.lb(u_ref) << ", " << cst_ref.rb(u_ref) << "]" << std::endl;
            return false;
        }
    }

    std::cout << "# SLINK: No errors." << std::endl;

    for(auto v_ref: nodes) {
        auto v = cst.node(cst_ref.lb(v_ref), cst_ref.rb(v_ref));

        std::uniform_int_distribution<typename t_cst::size_type> distribution(cst_ref.lb(v_ref), cst_ref.rb(v_ref));
        auto d = cst_ref.depth(v_ref);
        auto c = cst_ref.csa.text[cst_ref.csa[distribution(get_generator())] + d];

        auto u = cst.child(v, c);
        auto u_ref = cst_ref.child(v_ref, c);

        if(cst.lb(u) != cst_ref.lb(u_ref) ||
           cst.rb(u) != cst_ref.rb(u_ref)) {
            std::cout << "# Error while calculating CHILD(["
                      << cst_ref.lb(v_ref) << ", " << cst_ref.rb(v_ref) << "])." << std::endl;
            std::cout << "# Result: [" << cst.lb(u) << ", " << cst.rb(u) << "]" << std::endl;
            std::cout << "# Expected: [" << cst_ref.lb(u_ref) << ", " << cst_ref.rb(u_ref) << "]" << std::endl;
            return false;
        }
    }

    std::cout << "# CHILD: No errors." << std::endl;

    for(auto v_ref: nodes) {
        auto v = cst.node(cst_ref.lb(v_ref), cst_ref.rb(v_ref));

        auto d = cst.depth(v);
        auto d_ref = cst_ref.depth(v_ref);

        if(d != d_ref) {
            std::cout << "# Error while calculating DEPTH(["
                      << cst_ref.lb(v_ref) << ", " << cst_ref.rb(v_ref) << "])." << std::endl;
            std::cout << "# Result: " << d << std::endl;
            std::cout << "# Expected: " << d_ref << std::endl;
            return false;
        }
    }

    std::cout << "# DEPTH: No errors." << std::endl;

    for(auto v_ref: nodes) {
        auto v = cst.node(cst_ref.lb(v_ref), cst_ref.rb(v_ref));

        auto u = cst.parent(v);
        auto u_ref = cst_ref.parent(v_ref);

        if(cst.lb(u) != cst_ref.lb(u_ref) ||
           cst.rb(u) != cst_ref.rb(u_ref)) {
            std::cout << "# Error while calculating PARENT(["
                      << cst_ref.lb(v_ref) << ", " << cst_ref.rb(v_ref) << "])." << std::endl;
            std::cout << "# Result: [" << cst.lb(u) << ", " << cst.rb(u) << "]" << std::endl;
            std::cout << "# Expected: [" << cst_ref.lb(u_ref) << ", " << cst_ref.rb(u_ref) << "]" << std::endl;
            return false;
        }
    }

    std::cout << "# PARENT: No errors." << std::endl;

    return true;
}

int main(int argc, char** argv) {
    if(argc < 3) {
        std::cout << "Usage: " << argv[0] << " [fcst_file] [cst_file]" << std::endl;
        return 1;
    }

    const char* fcst_file = argv[1];
    const char* cst_file = argv[2];

    fcst_type fcst;
    cst_type cst;

    load_from_file(fcst, fcst_file);
    load_from_file(cst, cst_file);

    run_tests(fcst, cst);

    return 0;
}
