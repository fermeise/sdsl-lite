#include <sdsl/suffix_trees.hpp>
#include <fstream>
#include <chrono>

using namespace sdsl;
using namespace std::chrono;
using timer = std::chrono::high_resolution_clock;

typedef CST_TYPE cst_type;

const size_t BURST_SIZE = 1000;
const size_t NUM_BURSTS = 10;

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

    void reset() {
    }

    std::vector<typename t_cst::node_type> get_burst(size_t sample_count) {
        std::vector<typename t_cst::node_type> res;
        res.reserve(sample_count);

        for(size_t i = 0; i < sample_count; i++) {
            auto leaf = m_distribution(get_generator());
            res.push_back(m_cst.lca(leaf, leaf + 1));
        }

        return res;
    }
};

template<class t_cst>
class node_sampler_iterate_sl {
private:
    const t_cst &m_cst;
    std::uniform_int_distribution<typename t_cst::size_type> m_distribution;
    typename t_cst::node_type m_last_node;

public:
    node_sampler_iterate_sl(const t_cst &cst)
    : m_cst(cst),
      m_distribution(0, m_cst.size() - 2),
      m_last_node(cst.root())
    {}

    void reset() {
        m_last_node = m_cst.root();
    }

    std::vector<typename t_cst::node_type> get_burst(size_t sample_count) {
        std::vector<typename t_cst::node_type> res;
        res.reserve(sample_count);

        for(size_t i = 0; i < sample_count; i++) {
            if(m_last_node == m_cst.root()) {
                auto leaf = m_distribution(get_generator());
                m_last_node = m_cst.lca(leaf, leaf + 1);
            } else {
                m_last_node = m_cst.sl(m_last_node);
            }

            res.push_back(m_last_node);
        }

        return res;
    }
};

template<class t_cst>
class node_sampler_iterate_parent {
private:
    const t_cst &m_cst;
    std::uniform_int_distribution<typename t_cst::size_type> m_distribution;
    typename t_cst::node_type m_last_node;

public:
    node_sampler_iterate_parent(const t_cst &cst)
    : m_cst(cst),
      m_distribution(0, m_cst.size() - 2),
      m_last_node(cst.root())
    {}

    void reset() {
        m_last_node = m_cst.root();
    }

    std::vector<typename t_cst::node_type> get_burst(size_t sample_count) {
        std::vector<typename t_cst::node_type> res;
        res.reserve(sample_count);

        for(size_t i = 0; i < sample_count; i++) {
            if(m_last_node == m_cst.root()) {
                auto leaf = m_distribution(get_generator());
                m_last_node = m_cst.lca(leaf, leaf + 1);
            } else {
                m_last_node = m_cst.parent(m_last_node);
            }

            res.push_back(m_last_node);
        }

        return res;
    }
};

template<class t_cst>
std::pair<typename t_cst::node_type, typename t_cst::node_type>
lca_argument(const t_cst& cst, const typename t_cst::node_type v) {
    typename t_cst::node_type w;
    if(cst.rb(v) + 2 < cst.size()) {
        w = cst.lca(cst.rb(v) + 1, cst.rb(v) + 2);
    } else {
        w = cst.root();
    }

    return std::make_pair(v, w);
}

template<class t_cst>
void test_lca(const t_cst &cst, const std::pair<typename t_cst::node_type, typename t_cst::node_type> &arg) {
    cst.lca(arg.first, arg.second);
}

template<class t_cst>
std::pair<typename t_cst::node_type, typename t_cst::size_type>
letter_argument(const t_cst& cst, const typename t_cst::node_type v) {
    auto d = cst.depth(v);

    if(d == 0) {
        std::uniform_int_distribution<typename t_cst::size_type> distribution(0, cst.size() - 1);

        auto c = cst.csa.F[distribution(get_generator())];

        return std::make_pair(cst.child(v, c), 1);
    }

    return std::make_pair(v, d);
}

template<class t_cst>
void test_letter(const t_cst &cst, const std::pair<typename t_cst::node_type, typename t_cst::size_type> &arg) {
    cst.edge(arg.first, arg.second);
}

template<class t_cst>
typename t_cst::node_type slink_argument(const t_cst&, const typename t_cst::node_type v) {
    return v;
}

template<class t_cst>
void test_slink(const t_cst &cst, const typename t_cst::node_type &v) {
    cst.sl(v);
}

template<class t_cst>
std::pair<typename t_cst::node_type, typename t_cst::char_type>
child_argument(const t_cst &cst, const typename t_cst::node_type v) {
    std::uniform_int_distribution<typename t_cst::size_type> distribution(cst.lb(v), cst.rb(v));

    auto d = cst.depth(v);
    auto c = cst.csa.text[cst.csa[distribution(get_generator())] + d];

    return std::make_pair(v, c);
}

template<class t_cst>
void test_child(const t_cst &cst, const std::pair<typename t_cst::node_type, typename t_cst::char_type> &arg) {
    cst.child(arg.first, arg.second);
}

template<class t_cst>
typename t_cst::node_type depth_argument(const t_cst&, const typename t_cst::node_type v) {
    return v;
}

template<class t_cst>
void test_depth(const t_cst &cst, const typename t_cst::node_type &v) {
    cst.depth(v);
}

template<class t_cst>
typename t_cst::node_type parent_argument(const t_cst&, const typename t_cst::node_type v) {
    return v;
}

template<class t_cst>
void test_parent(const t_cst &cst, const typename t_cst::node_type &v) {
    cst.parent(v);
}

template<class t_cst,
         class t_test_func,
         class t_prepare_func>
void run_benchmark(const std::string &cst_name, const t_cst &cst,
                   const std::string &op_name, t_test_func test_func, t_prepare_func prepare_func) {
    run_benchmark(cst_name, cst, op_name, test_func, prepare_func, "U", node_sampler_uniform<t_cst>(cst));
    run_benchmark(cst_name, cst, op_name, test_func, prepare_func, "SU", node_sampler_iterate_sl<t_cst>(cst));
    run_benchmark(cst_name, cst, op_name, test_func, prepare_func, "PU", node_sampler_iterate_parent<t_cst>(cst));
}

template<class t_cst,
         class t_test_func,
         class t_prepare_func,
         class t_sampler>
void run_benchmark(const std::string cst_name, const t_cst &cst,
                   const std::string op_name, t_test_func test_func, t_prepare_func prepare_func,
                   const std::string sampler_name, t_sampler sampler) {
    unsigned long long nanos = 0;

    for(int i = 0; i < NUM_BURSTS; i++) {
        auto nodes = sampler.get_burst(BURST_SIZE);

        std::vector<decltype(prepare_func(cst, cst.root()))> args;
        args.reserve(nodes.size());

        for(auto v: nodes) {
            args.push_back(prepare_func(cst, v));
        }

        auto start = timer::now();
        for(auto arg: args) {
            test_func(cst, arg);
        }
        auto stop = timer::now();

        nanos += duration_cast<nanoseconds>(stop-start).count();
    }

    std::cout << "# " << cst_name << "_" << op_name << "_" << sampler_name
              << "_TIME = " << nanos / BURST_SIZE / NUM_BURSTS << std::endl;
}

template<class t_cst>
void run_benchmark(std::string cst_name, const t_cst &cst) {
    run_benchmark(cst_name, cst, "LCA", test_lca<t_cst>, lca_argument<t_cst>);
    run_benchmark(cst_name, cst, "LETTER", test_letter<t_cst>, letter_argument<t_cst>);
    run_benchmark(cst_name, cst, "SLINK", test_slink<t_cst>, slink_argument<t_cst>);
    run_benchmark(cst_name, cst, "CHILD", test_child<t_cst>, child_argument<t_cst>);
    run_benchmark(cst_name, cst, "DEPTH", test_depth<t_cst>, depth_argument<t_cst>);
    run_benchmark(cst_name, cst, "PARENT", test_parent<t_cst>, parent_argument<t_cst>);
}

// Returns ceil(log(n))
size_t log(size_t n) {
    size_t m = 1;
    size_t res = 0;
    while(n > m) {
        m *= 2;
        res++;
    }
    return res;
}

int main(int argc, char** argv) {
    if(argc < 3) {
        std::cout << "Usage: " << argv[0] << " [fcst_file] [cst_file]" << std::endl;
        return 1;
    }

    const char* fcst_file = argv[1];
    const char* cst_file = argv[2];

    cst_type fcst;
    cst_type::construction_cst_type cst;

    load_from_file(fcst, fcst_file);
    load_from_file(cst, cst_file);

    std::cout << "# ALPHABET_SIZE = " << fcst.csa.sigma << std::endl;
    std::cout << "# DATA_SIZE = " << fcst.csa.text.size() << std::endl;
    std::cout << "# TREE_SIZE = " << cst.nodes() << std::endl;
    std::cout << "# SAMPLING_FACTOR = " << fcst.delta << std::endl;
    std::cout << "# SAMPLED_TREE_SIZE = " << fcst.sampled_nodes() << std::endl;

    std::cout << "# FCST_SIZE = " << size_in_bytes(fcst) << std::endl;
    std::cout << "# CST_SIZE = " << size_in_bytes(cst) << std::endl;
    std::cout << "# CSA_SIZE = " << size_in_bytes(fcst.csa) << std::endl;

    run_benchmark("FCST", fcst);
    run_benchmark("CST", cst);

    return 0;
}
