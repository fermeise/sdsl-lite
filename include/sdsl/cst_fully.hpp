
#ifndef INCLUDED_SDSL_CST_FULLY
#define INCLUDED_SDSL_CST_FULLY

#include "bit_vectors.hpp"
#include "vectors.hpp"
#include "bp_support.hpp"
#include "csa_iterators.hpp"
#include <fstream>
#include <chrono>

using timer = std::chrono::high_resolution_clock;

namespace sdsl {

template<class t_csa = csa_wt<>,
         class t_s_support = bp_support_sada<>,
         class t_b = sd_vector<>,
         class t_depth = dac_vector<>
         >
class cst_fully {
public:
    typedef cst_dfs_const_forward_iterator<cst_fully> const_iterator;
    typedef typename t_csa::size_type                 size_type;
    typedef t_csa                                     csa_type;
    typedef typename t_csa::char_type                 char_type;
    typedef std::pair<size_type, size_type>           node_type; // Nodes are represented by their left and right leafs (inclusive)
    typedef size_type                                 leaf_type; // Index of a leaf
    typedef size_type                                 sampled_node_type; // Node in the sampled tree represented by its index in s
    typedef t_s_support                               s_support_type;
    typedef t_b                                       b_type;
    typedef typename t_b::select_0_type               b_select_0_type;
    typedef typename t_b::select_1_type               b_select_1_type;
    typedef t_depth                                   depth_type;
    typedef cst_sada<t_csa, lcp_wt<> >                construction_cst_type; // CST used for the construction of the FCST
    typedef csa_iterator<t_csa>                       csa_iterator_type;

    typedef typename t_csa::alphabet_category         alphabet_category;
    typedef cst_tag                                   index_category;

private:
    size_type                      m_delta;
    csa_type                       m_csa;
    bit_vector                     m_s;
    s_support_type                 m_s_support;
    b_type                         m_b;
    b_select_0_type                m_b_select0;
    b_select_1_type                m_b_select1;
    depth_type                     m_depth;

    void copy(const cst_fully& cst) {
        m_delta     = cst.m_delta;
        m_csa       = cst.m_csa;
        m_s         = cst.m_s;
        m_s_support = cst.m_s_support;
        m_s_support.set_vector(&m_s);
        m_b         = cst.m_b;
        m_b_select0 = cst.m_b_select0;
        m_b_select0.set_vector(&m_b);
        m_b_select1 = cst.m_b_select1;
        m_b_select1.set_vector(&m_b);
        m_depth     = cst.m_depth;
    }

public:
    const size_type   &delta = m_delta;
    const csa_type    &csa = m_csa;
    const bit_vector  &s = m_s;
    const b_type      &b = m_b;

    static cst_fully construct(const std::string &file, size_type delta, bool sample_leaves = false) {
        construction_cst_type cst;
        sdsl::construct(cst, file, 1);
        return cst_fully(cst, delta, sample_leaves);
    }

    static cst_fully construct_im(const char *str, size_type delta, bool sample_leaves = false) {
        construction_cst_type cst;
        sdsl::construct_im(cst, str, 1);
        return cst_fully(cst, delta, sample_leaves);
    }

//! Default constructor
    cst_fully() {}

//! Copy constructor
    cst_fully(const cst_fully &cst) {
        copy(cst);
    }

//! Move constructor
    cst_fully(cst_fully &&cst) {
        *this = std::move(cst);
    }

//! Construct CST from file_map
    cst_fully(cache_config &config, size_type delta = 0, bool sample_leaves = false) {
        construction_cst_type cst(config);

        if(delta == 0) {
            const size_type n = cst.size();
            if(n >= 3) {
                delta = (log2(n-1)+1) * (log2(log2(n-1))+1);
            } else {
                delta = 2;
            }
        }

        cst_fully(cst, delta, sample_leaves);
    }

//! Construct CST from CST
    cst_fully(const construction_cst_type &cst, size_type delta, bool sample_leaves)
        : m_delta(delta),
          m_csa(cst.csa)
    {
        size_type delta_half = delta / 2;

        bit_vector is_sampled(cst.nodes(), false);
        is_sampled[cst.id(cst.root())] = true; // always sample root
        size_type sample_count = 1;

        if(sample_leaves) {
            auto event = memory_monitor::event("scan-leaves");
            for(auto it = csa_iterator_type::begin(cst.csa); it != csa_iterator_type::end(cst.csa); ++it) {
                const size_type d = it.depth();
                if(d + delta_half <= cst.size() and
                   d % delta_half == 0) {
                    const auto node = cst.select_leaf(*it + 1);
                    const size_type id = cst.id(node);
                    if(!is_sampled[id]) {
                        is_sampled[id] = true;
                        sample_count++;
                    }
                }
            }
        }

        {
            auto event = memory_monitor::event("scan-nodes");
            for(auto it = cst.begin(); it != cst.end(); ++it) {
                if(it.visit() == 1 and cst.is_leaf(*it) == false) {
                    const auto node = *it;
                    const size_type d = cst.depth(node);
                    if(d % delta_half == 0) {
                        auto v = node;
                        for(size_type i = 0; i < delta_half; i++) {
                            v = cst.sl(v);
                        }
                        const size_type id = cst.id(v);
                        if(!is_sampled[id]) {
                            is_sampled[id] = true;
                            sample_count++;
                        }
                    }
                }
            }
        }

        bit_vector tmp_b;
        int_vector<64> tmp_depth;
        std::stack<bool> sampledStack;

        m_s.resize(2 * sample_count);
        tmp_b.resize(2 * sample_count + cst.size());
        tmp_depth.resize(sample_count);

        {
            auto event = memory_monitor::event("node-sampling");

            size_type s_idx = 0;
            size_type b_idx = 0;
            size_type sample_idx = 0;

            for(auto it = cst.begin(); it != cst.end(); ++it) {
                auto node = *it;
                if(cst.is_leaf(node)) {
                    if(is_sampled[cst.id(node)]) {
                        m_s[s_idx++] = 1;
                        m_s[s_idx++] = 0;
                        tmp_b[b_idx++] = 1;
                        tmp_b[b_idx++] = 0;
                        tmp_b[b_idx++] = 1;
                        tmp_depth[sample_idx++] = cst.depth(node) / delta_half;
                    } else {
                        tmp_b[b_idx++] = 0;
                    }
                } else {
                    if(it.visit() == 1) {
                        sampledStack.push(is_sampled[cst.id(node)]);
                        if(sampledStack.top()) {
                            m_s[s_idx++] = 1;
                            tmp_b[b_idx++] = 1;
                            tmp_depth[sample_idx++] = cst.depth(node) / delta_half;
                        }
                    } else {
                        if(sampledStack.top()) {
                            m_s[s_idx++] = 0;
                            tmp_b[b_idx++] = 1;
                        }
                        sampledStack.pop();
                    }
                }
            }
        }

        {
            auto event = memory_monitor::event("ss-depth");
            util::init_support(m_s_support, &m_s);
            m_b = b_type(tmp_b);
            util::init_support(m_b_select0, &m_b);
            util::init_support(m_b_select1, &m_b);
            m_depth = depth_type(tmp_depth);
        }
    }

    size_type size() const {
        return m_csa.size();
    }

    static size_type max_size() {
        return t_csa::max_size();
    }

    bool empty() const {
        return m_csa.empty();
    }

    void swap(cst_fully &cst) {
        if(this != &cst) {
            std::swap(m_delta, cst.m_delta);
            m_csa.swap(cst.m_csa);
            m_s.swap(cst.m_s);
            util::swap_support(m_s_support, cst.m_s_support, &m_s, &(cst.m_s));
            m_b.swap(cst.m_b);
            util::swap_support(m_b_select0, cst.m_b_select0, &m_b, &(cst.m_b));
            util::swap_support(m_b_select1, cst.m_b_select1, &m_b, &(cst.m_b));
            m_depth.swap(cst.m_depth);
        }
    }

    const_iterator begin() const {
        if(m_b.size() == 0) {
            return end();
        }
        return const_iterator(this, root(), false, true);
    }

    const_iterator end() const {
        return const_iterator(this, root(), true, false);
    }

//! Copy Assignment Operator.
    cst_fully& operator=(const cst_fully &cst) {
        if(this != &cst) {
            copy(cst);
        }
        return *this;
    }

//! Move Assignment Operator.
    cst_fully& operator=(cst_fully &&cst) {
        if(this != &cst) {
            m_delta     = cst.m_delta;
            m_csa       = std::move(cst.m_csa);
            m_s         = std::move(cst.m_s);
            m_s_support = std::move(cst.m_s_support);
            m_s_support.set_vector(&m_s);
            m_b         = std::move(cst.m_b);
            m_b_select0 = std::move(cst.m_b_select0);
            m_b_select0.set_vector(&m_b);
            m_b_select1 = std::move(cst.m_b_select1);
            m_b_select1.set_vector(&m_b);
            m_depth     = std::move(cst.m_depth);
        }
        return *this;
    }

//! Serialize to a stream.
    /*! \param out Outstream to write the data structure.
     *  \return The number of written bytes.
     */
    size_type serialize(std::ostream& out, structure_tree_node* v=nullptr, std::string name="") const {
        structure_tree_node* child = structure_tree::add_child(v, name, util::class_name(*this));
        size_type written_bytes = 0;
        out.write((char*) &m_delta, sizeof(m_delta));
        written_bytes += sizeof(m_delta);
        written_bytes += m_csa.serialize(out, child, "csa");
        written_bytes += m_s.serialize(out, child, "s");
        written_bytes += m_s_support.serialize(out, child, "s_support");
        written_bytes += m_b.serialize(out, child, "b");
        written_bytes += m_b_select0.serialize(out, child, "b_select0");
        written_bytes += m_b_select1.serialize(out, child, "b_select1");
        written_bytes += m_depth.serialize(out, child, "depth");
        structure_tree::add_size(child, written_bytes);
        return written_bytes;
    }

//! Load from a stream.
    /*! \param in Inputstream to load the data structure from.
     */
    void load(std::istream& in) {
        in.read((char*) &m_delta, sizeof(m_delta));
        m_csa.load(in);
        m_s.load(in);
        m_s_support.load(in, &m_s);
        m_b.load(in);
        m_b_select0.load(in, &m_b);
        m_b_select1.load(in, &m_b);
        m_depth.load(in);
    }

//! Returns the root of the suffix tree.
    node_type root() const {
        return node_type(0, m_csa.size() - 1);
    }

//! Returns the root of the sampled tree.
    sampled_node_type sampled_root() const {
        return 0;
    }

//! Returns true iff node v is a leaf.
    bool is_leaf(node_type v) const {
        return v.first == v.second;
    }

//! Returns the leftmost leaf (left boundary) of a node.
    leaf_type lb(node_type v) const {
        return v.first;
    }

//! Returns the rightmost leaf (right boundary) of a node.
    leaf_type rb(node_type v) const {
        return v.second;
    }

//! Returns true iff v is an ancestor of w.
    bool ancestor(node_type v, node_type w) const {
        return v.first <= w.first && v.second >= w.second;
    }

//! Returns the index of the last bracket in S before the leaf with index l.
        /*!
         * \param v The index of leaf l.
         * \return The index of the last bracket in S before the leaf with index l.
         */
    sampled_node_type pred(leaf_type v) const {
        return m_b_select0.select(v + 1) - v - 1;
    }

//! Returns the LSA (lowest sampled ancestor) for a leaf with index l.
        /*!
         * \param v The index of leaf l.
         * \return The LSA for the leaf with index l.
         * \par Time complexity
         *   \f$ \Order{1} \f$
         */
    sampled_node_type lsa_leaf(leaf_type l) const {
        sampled_node_type p = pred(l);
        if(m_s[p]) {
            return p;
        } else {
            return m_s_support.enclose(m_s_support.find_open(p));
        }
    }

//! Returns the node in the suffix tree corresponding to the node u in the sampled tree.
        /*!
         * \param v The node u in the sampled tree.
         * \return The node in the suffix tree corresponding to the node u in the sampled tree.
         * \par Time complexity
         *   \f$ \Order{1} \f$
         */
    node_type sampled_node(sampled_node_type u) const {
        assert(m_s[u] == 1);
        size_type u_end = m_s_support.find_close(u);
        size_type b_left = m_b_select1.select(u + 1);
        size_type b_right = m_b_select1.select(u_end + 1);

        return node_type(b_left - u,
                         b_right - u_end - 1);
    }

//! Returns the LCSA (lowest common sampled ancestor) for two sampled nodes.
        /*!
         * \param u The sampled node u.
         * \param q The sampled node q.
         * \return The LCSA of u and q.
         * \par Time complexity
         *   \f$ \Order{\rrenclose} \f$
         */
    sampled_node_type lcsa(sampled_node_type u, sampled_node_type q) const {
        assert(m_s[u] == 1 and m_s[q] == 1);
        if(u > q) {
            std::swap(u, q);
        } else if(u == q) {
            return u;
        }
        if(u == sampled_root()) {
            return sampled_root();
        }
        if(m_s_support.find_close(u) > q) {
            return u;
        }

        return m_s_support.double_enclose(u, q);
    }

//! Returns the depth of a sampled node u.
        /*!
         * \param u A sampled node u.
         * \return The depth of sampled node u.
         * \par Time complexity
         *   \f$ \Order{1} \f$
         */
    size_type depth(sampled_node_type u) const {
        assert(m_s[u] == 1);

        size_type idx = m_s_support.rank(u) - 1;
        return m_depth[idx] * (m_delta / 2);
    }

//! Returns the depth of a node v.
        /*!
         * \param v The node v.
         * \return The depth of node v.
         * \par Time complexity
         *   \f$ \Order( \delta ) \f$ for inner nodes,
         *   \f$ \Order( \saaccess ) \f$ for leaves.
         */
    size_type depth(node_type v) const {
        if(is_leaf(v)) {
            return m_csa.size() - m_csa[v.first];
        }

        size_type i;
        sampled_node_type u;
        std::vector<char_type> c;
        c.reserve(delta);
        return depth_lca(v.first, v.second, i, u, c);
    }

//! Calculate the LCA of two nodes v and w.
        /*!
         * \param v The node v.
         * \param w The node w.
         * \return The LCA of v and w.
         * \par Time complexity
         *   \f$ \Order( \delta \cdot ( 1 + t_{rank\_bwt} ) ) \f$
         */
    node_type lca(node_type v, node_type w) const {
        leaf_type l = std::min(v.first, w.first);
        leaf_type r = std::max(v.second, w.second);

        if(l == r) {
            return node_type(l, r);
        } else {
            return lca(l, r);
        }
    }

//! Calculate the LCA of two leaves l and r.
        /*!
         * \param l The index of leaf l.
         * \param r The index of leaf r. \f$ r > l \f$
         * \return The LCA of l and r.
         * \par Time complexity
         *   \f$ \Order( \delta \cdot ( 1 + t_{rank\_bwt} ) ) \f$
         */
    node_type lca(leaf_type l, leaf_type r) const {
        assert(l<r);

        size_type i;
        sampled_node_type u;
        std::vector<char_type> c(delta, 0);
        depth_lca(l, r, i, u, c);

        node_type v = sampled_node(u);
        leaf_type lb = v.first;
        leaf_type rb = v.second;

        for(size_type k = 0; k < i; k++) {
            backward_search(m_csa, lb, rb, c[i - k - 1], lb, rb);
        }

        return node_type(lb, rb);
    }

//! Calculate the depth of the LCA of two leaves l and r.
        /*!
         * \param l The index of leaf l.
         * \param r The index of leaf r. \f$ r > l \f$
         * \param res_i The index i for the ancestor used to determine the depth (return value).
         * \param res_u The ancestor used to determine the depth (return value).
         * \param res_label The label from the found sampled node to the actual LCA.
         * \return The depth of the LCA of l and r.
         * \par Time complexity
         *   \f$ \Order( \delta ) \f$
         */
    size_type depth_lca(leaf_type l, leaf_type r,
                        size_type &res_i, sampled_node_type &res_u, std::vector<char_type> &res_label) const {
        assert(l<r);

        size_type max_d = 0;
        size_type max_d_i = 0;
        sampled_node_type max_d_node = 0;

        for(size_type i = 0; i < m_delta; i++) {
            sampled_node_type node = lcsa(lsa_leaf(l), lsa_leaf(r));
            size_type d = i + depth(node);

            if(d > max_d) {
                max_d = d;
                max_d_i = i;
                max_d_node = node;
            }

            char_type c = m_csa.F[l];
            char_type comp = csa.char2comp[c];
            res_label[i] = c;

            // break if LCA of lb and rb is root
            if(l < m_csa.C[comp] || r >= m_csa.C[comp + 1]) {
                break;
            }

            l = m_csa.psi[l];
            r = m_csa.psi[r];
        }

        res_i = max_d_i;
        res_u = max_d_node;

        return max_d;
    }

//! Compute the suffix link of a node v.
        /*!
         * \param v The node v.
         * \return The suffix link of node v or root() if v equals root().
         * \par Time complexity
         *   \f$ \Order( \delta \cdot ( 1 + t_{rank\_bwt} ) ) \f$
         */
    node_type sl(node_type v) const {
        if(v == root()) {
            return root();
        } else if(is_leaf(v)) {
            size_t leaf = m_csa.psi[v.first];
            return node_type(leaf, leaf);
        }

        return lca(m_csa.psi[v.first], m_csa.psi[v.second]);
    }

//! Calculate the parent node of a node v.
        /*!
         * \param v The node v.
         * \return The parent node of v or root() if v equals root().
         * \par Time complexity
         *   \f$ \Order( \delta \cdot ( 1 + t_{rank\_bwt} ) ) \f$
         */
    node_type parent(node_type v) const {
        const leaf_type l = v.first;
        const leaf_type r = v.second;

        node_type left_parent = root();
        node_type right_parent = root();

        if(l > 0) {
            left_parent = lca(l-1, r);
        }
        if(r < m_csa.size() - 1) {
            right_parent = lca(l, r+1);
        }
        return ancestor(right_parent, left_parent) ? left_parent : right_parent;
    }

    node_type child(node_type v, char_type c) const {
        if(is_leaf(v)) {
            return root();
        }

        size_type d = depth(v);

        return child_1(v, c, d);
    }

//! Get the child w of node v which edge label (v,w) starts with character c.
        /*!
         * \param v A node v.
         * \param c First character of the edge label from v to the desired child.
         * \return The child node w which edge label (v,w) starts with c or root() if it does not exist.
         * \par Time complexity
         *       \f$ \Order{ \log m \cdot (\saaccess+\isaaccess) } \f$
                 where \f$ m \f$ is the number of leaves under node v.
         */
    node_type child_1(node_type v, char_type c, size_type d) const {
        leaf_type res_lb;
        leaf_type res_rb;

        {
            leaf_type left = v.first;
            leaf_type right = v.second;

            while(left < right) {
                leaf_type sample_pos = (left + right) / 2;
                char_type sample = m_csa.text[m_csa[sample_pos] + d];
                if(sample == c) {
                    right = sample_pos;
                } else if(sample < c) {
                    left = sample_pos + 1;
                } else {
                    right = sample_pos - 1;
                }
            }

            if(m_csa.text[m_csa[left] + d] != c) {
                return root();
            }

            res_lb = left;
        }

        {
            leaf_type left = v.first;
            leaf_type right = v.second;

            while(left < right) {
                leaf_type sample_pos = (left + right + 1) / 2;
                char_type sample = m_csa.text[m_csa[sample_pos] + d];
                if(sample == c) {
                    left = sample_pos;
                } else if(sample < c) {
                    left = sample_pos + 1;
                } else {
                    right = sample_pos - 1;
                }
            }

            res_rb = left;
        }

        return node_type(res_lb, res_rb);
    }

//! Get the child w of node v which edge label (v,w) starts with character c.
        /*!
         * \param v A node v.
         * \param c First character of the edge label from v to the desired child.
         * \return The child node w which edge label (v,w) starts with c or root() if it does not exist.
         * \par Time complexity
         *       \f$ \Order{ m \cdot (\psiaccess + t_{rank\_bwt}) } \f$,
                 where \f$ m \f$ is the number of leaves under node v.
         */
    node_type child_2(node_type v, char_type c, size_type d) const {
        std::vector<char_type> label(d, 0);

        leaf_type l = v.first;

        for(size_type i = 0; i < d; i++) {
            label[i] = m_csa.F[l];
            l = m_csa.psi[l];
        }

        l = 0;
        leaf_type r = size() - 1;

        backward_search(m_csa, l, r, c, l, r);
        for(size_type k = 0; k < d; k++) {
            backward_search(m_csa, l, r, label[d - k - 1], l, r);
        }

        if(l > r) {
            return root();
        } else {
            return node_type(l, r);
        }
    }

//! Get the child w of node v which edge label (v,w) starts with character c.
        /*!
         * \param v A node v.
         * \param c First character of the edge label from v to the desired child.
         * \return The child node w which edge label (v,w) starts with c or root() if it does not exist.
         * \par Time complexity
         *       \f$ \Order{ \saaccess + \theta \cdot t_{rank\_bwt} + \log m + log \theta \cdot (\saaccess+\isaaccess) } \f$,
                 where \f$ \theta \f$ is the SA and ISA sampling factor and \f$ m \f$ is the number of leaves under node v.
         */
    node_type child_3(node_type v, char_type c, size_type d) const {
        // Assert that SA order SA sampling and text oder ISA sampling
        // with the same sampling factor is used.

        if(v == root()) {
            leaf_type& l = v.first;
            leaf_type& r = v.second;
            backward_search(m_csa, l, r, c, l, r);
            return v;
        }

        const size_type theta = std::min(d + 1, static_cast<size_type>(csa_type::sa_sample_dens));

        std::vector<char_type> label(theta, 0);

        leaf_type l = m_csa.isa[m_csa[v.first] + d - 1];
        for(size_type i = 0; i < theta; i++) {
            label[theta - i - 1] = m_csa.F[l];
            l = m_csa.lf[l];
        }

        l = 0;
        leaf_type r = size() - 1;

        std::vector<node_type> D(theta, root());

        backward_search(m_csa, l, r, c, l, r);
        D[0] = node_type(l, r);
        for(size_type i = 1; i < theta; i++) {
            backward_search(m_csa, l, r, label[theta - i], l, r);
            D[i] = node_type(l, r);
        }

        if(d < theta) {
            std::tie(l, r) = D[d];
            if(l > r) {
                return root();
            } else {
                return node_type(l, r);
            }
        }

        leaf_type res_lb;
        leaf_type res_rb;

        {
            leaf_type left = v.first;
            leaf_type right = v.second;

            while(true) {
                size_type ls = (left + theta - 1) / theta;
                size_type rs = right / theta;

                if(ls >= rs) {
                    break;
                }
                size_type sample_pos = (ls + rs) / 2;
                size_type text_pos = m_csa.sa_sample[sample_pos * theta];
                size_type d2 = ((text_pos + d) / theta) * theta - text_pos;
                leaf_type sample = m_csa.isa_sample[text_pos + d2];

                if(sample >= D[d - d2].first) {
                    if(sample <= D[d - d2].second) {
                        right = sample_pos * theta;
                    } else {
                        right = sample_pos * theta - 1;
                    }
                } else {
                    left = sample_pos * theta + 1;
                }
            }

            while(left < right) {
                leaf_type sample_pos = (left + right) / 2;
                leaf_type sample = m_csa.isa[m_csa[sample_pos] + d];
                if(sample >= D[0].first) {
                    if(sample <= D[0].second) {
                        right = sample_pos;
                    } else {
                        right = sample_pos - 1;
                    }
                } else {
                    left = sample_pos + 1;
                }
            }

            if(m_csa.text[m_csa[left] + d] != c) {
                return root();
            }

            res_lb = left;
        }

        {
            leaf_type left = v.first;
            leaf_type right = v.second;

            while(true) {
                size_type ls = (left + theta - 1) / theta;
                size_type rs = right / theta;

                if(ls >= rs) {
                    break;
                }
                size_type sample_pos = (ls + rs + 1) / 2;
                size_type text_pos = m_csa.sa_sample[sample_pos * theta];
                size_type d2 = ((text_pos + d) / theta) * theta - text_pos;
                leaf_type sample = m_csa.isa_sample[text_pos + d2];

                if(sample >= D[d - d2].first) {
                    if(sample <= D[d - d2].second) {
                        left = sample_pos * theta;
                    } else {
                        right = sample_pos * theta - 1;
                    }
                } else {
                    left = sample_pos * theta + 1;
                }
            }

            while(left < right) {
                leaf_type sample_pos = (left + right + 1) / 2;
                leaf_type sample = m_csa.isa[m_csa[sample_pos] + d];
                if(sample >= D[0].first) {
                    if(sample <= D[0].second) {
                        left = sample_pos;
                    } else {
                        right = sample_pos - 1;
                    }
                } else {
                    left = sample_pos + 1;
                }
            }

            res_rb = left;
        }

        return node_type(res_lb, res_rb);
    }

//! Get the i-th child of a node v.
        /*!
         * \param v A valid tree node of the cst.
         * \param i 1-based Index of the child which should be returned. \f $i \geq 1 \f$.
         * \return The i-th child node of v or root() if v has no i-th child.
         */
    node_type select_child(node_type v, size_type i) const {
        if(is_leaf(v)) {
            return root();
        }
        // TODO: Depth is unnecessarily calculated twice
        size_type d = depth(v);
        char_type c = m_csa.text[m_csa[v.first] + d];
        node_type res = child(v, c);
        while(i > 1) {
            if(res.second >= v.second) {
                return root();
            }
            c = m_csa.text[m_csa[res.second + 1] + d];
            res = child(v, c);
            i--;
        }

        return res;
    }

//! Returns the next sibling of node v.
        /*!
         * \param v A valid node v of the suffix tree.
         * \return The next (right) sibling of node v or root() if v has no next sibling.
         */
    node_type sibling(node_type v) const {
        node_type p = parent(v);
        size_type d = depth(p);
        if(v.second >= p.second) {
            return root();
        }
        char_type c = m_csa.text[m_csa[v.second + 1] + d];
        return child(p, c);
    }

    char_type edge(node_type v, size_type d) const {
        assert(d >= 1 and d <= depth(v));
        return m_csa.text[m_csa[v.first] + d - 1];
    }

//! Get the number of nodes in the sampled tree.
        /*!
         * \return The number of nodes in the sampled tree.
         * \par Time complexity
         *   \f$ \Order{1} \f$
         */
    size_type sampled_nodes() const {
        return m_s.size() / 2;
    }

//! Return floor(log_2(n))
    static size_type log2(size_type n) {
        size_type res = 0;
        while(n >>= 1) res++;
        return res;
    }
};

template<class t_csa,
         class t_s_support,
         class t_b,
         class t_depth
         >
std::ostream& operator<< (std::ostream &out, const cst_fully<t_csa, t_s_support, t_b, t_depth> &cst) {
    typedef typename cst_fully<t_csa, t_s_support, t_b, t_depth>::size_type size_type;

    size_type leaf_idx = 0;
    size_type s_idx = 0;

    for(auto value: cst.b) {
        if(value) {
            out << (cst.s[s_idx++] ? "(" : ")");
        } else {
            out << leaf_idx++;
        }
    }

    return out;
}

}// end namespace sdsl

#endif // INCLUDED_SDSL_CST_FULLY
