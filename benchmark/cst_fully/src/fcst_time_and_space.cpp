#include <sdsl/suffix_trees.hpp>
#include <fstream>
#include <chrono>

using namespace sdsl;
using namespace std::chrono;
using timer = std::chrono::high_resolution_clock;

typedef cst_fully<csa_wt<wt_huff<rrr_vector<63> >, 32, 32, text_order_sa_sampling<>, text_order_isa_sampling_support<> > > cst_type;

const size_t BURST_SIZE = 1000;
const size_t NUM_BURSTS = 10;

std::default_random_engine &get_generator() {
    static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    return generator;
}

template<class t_cst>
class node_sampler {
public:
    virtual ~node_sampler() {}

    virtual void reset() = 0;
    virtual std::vector<typename t_cst::node_type> get_burst(size_t sample_count) = 0;
};

template<class t_cst>
class node_sampler_uniform : public node_sampler<t_cst> {
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
class node_sampler_iterate_sl : public node_sampler<t_cst> {
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
class node_sampler_iterate_parent : public node_sampler<t_cst> {
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
void test_lca(const t_cst &cst, const std::vector<typename t_cst::node_type> &nodes,
                                const std::vector<typename t_cst::node_type> &nodes2) {
    auto it = nodes.begin();
    auto it2 = nodes2.begin();

    while(it != nodes.end()) {
        cst.lca(*it, *it2);
        ++it;
        ++it2;
    }
}

template<class t_cst>
void test_slink(const t_cst &cst, const std::vector<typename t_cst::node_type> &nodes) {
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        cst.sl(*it);
    }
}

template<class t_cst>
void test_first_child(const t_cst &cst, const std::vector<typename t_cst::node_type> &nodes) {
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        cst.select_child(*it, 1);
    }
}

template<class t_cst>
void test_depth(const t_cst &cst, const std::vector<typename t_cst::node_type> &nodes) {
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        cst.depth(*it);
    }
}
template<class t_cst>
void test_parent(const t_cst &cst, const std::vector<typename t_cst::node_type> &nodes) {
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        cst.parent(*it);
    }
}

template<class t_cst>
void run_benchmark(std::string cst_name, const t_cst &cst) {
    std::vector<std::pair<std::string, std::function<void(const t_cst&, const std::vector<typename t_cst::node_type> &)> > > operations;
    operations.push_back(std::make_pair("SLINK", test_slink<t_cst>));
    operations.push_back(std::make_pair("CHILD", test_first_child<t_cst>));
    operations.push_back(std::make_pair("DEPTH", test_depth<t_cst>));
    operations.push_back(std::make_pair("PARENT", test_parent<t_cst>));

    node_sampler_uniform<t_cst> sampler_uniform(cst);
    node_sampler_iterate_sl<t_cst> sampler_sl(cst);
    node_sampler_iterate_parent<t_cst> sampler_parent(cst);

    std::vector<std::pair<std::string, node_sampler<t_cst>* > > samplers;
    samplers.push_back(std::make_pair("U", &sampler_uniform));
    samplers.push_back(std::make_pair("SU", &sampler_sl));
    samplers.push_back(std::make_pair("PU", &sampler_parent));

    for(auto sampler: samplers) {
        std::string sampler_name = sampler.first;
        node_sampler<t_cst> *sampler_object = sampler.second;

        unsigned long long nanos = 0;

        for(int i = 0; i < NUM_BURSTS; i++) {
            sampler_object->reset();
            auto nodes = sampler_object->get_burst(BURST_SIZE);

            std::vector<typename t_cst::node_type> nodes2;
            for(auto v: nodes) {
                if(cst.rb(v) + 2 < cst.size()) {
                    nodes2.push_back(cst.lca(cst.rb(v) + 1, cst.rb(v) + 2));
                } else {
                    nodes2.push_back(cst.root());
                }
            }

            auto start = timer::now();
            test_lca(cst, nodes, nodes2);
            auto stop = timer::now();

            nanos += duration_cast<nanoseconds>(stop-start).count();
        }

        std::cout << "# " << cst_name << "_LCA_" << sampler_name << "_TIME = "
                  << nanos / BURST_SIZE / NUM_BURSTS << std::endl;
    }

    for(auto op: operations) {
        std::string op_name = op.first;
        auto op_function = op.second;

        for(auto sampler: samplers) {
            std::string sampler_name = sampler.first;
            node_sampler<t_cst> *sampler_object = sampler.second;

            unsigned long long nanos = 0;

            for(int i = 0; i < NUM_BURSTS; i++) {
                sampler_object->reset();
                auto nodes = sampler_object->get_burst(BURST_SIZE);

                auto start = timer::now();
                op_function(cst, nodes);
                auto stop = timer::now();

                nanos += duration_cast<nanoseconds>(stop-start).count();
            }

            std::cout << "# " << cst_name << "_" << op_name << "_" << sampler_name << "_TIME = "
                      << nanos / BURST_SIZE / NUM_BURSTS << std::endl;
        }
    }
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
    if(argc != 2) {
        std::cout << "Syntax: " << argv[0] << " [input file]" << std::endl;
        return 1;
    }

    const char* input_file = argv[1];

    auto start = timer::now();

    cst_type::construction_cst_type cst;
    construct(cst, input_file, 1);

    const size_t n = cst.size();
    const size_t delta = log(n) * log(log(n));
    cst_type fcst(cst, delta, false);

    auto stop = timer::now();

    std::cout << "# ALPHABET_SIZE = " << fcst.csa.sigma << std::endl;
    std::cout << "# DATA_SIZE = " << fcst.csa.text.size() << std::endl;
    std::cout << "# TREE_SIZE = " << cst.nodes() << std::endl;
    std::cout << "# SAMPLING_FACTOR = " << fcst.delta << std::endl;
    std::cout << "# SAMPLED_TREE_SIZE = " << fcst.sampled_nodes() << std::endl;

    std::cout << "# FCST_SIZE = " << size_in_bytes(fcst) << std::endl;
    std::cout << "# CST_SIZE = " << size_in_bytes(cst) << std::endl;
    std::cout << "# CSA_SIZE = " << size_in_bytes(fcst.csa) << std::endl;

    std::cout << "# CONSTRUCTION_TIME = " << duration_cast<seconds>(stop-start).count() << std::endl;

    run_benchmark("FCST", fcst);
    run_benchmark("CST", cst);

    return 0;
}
