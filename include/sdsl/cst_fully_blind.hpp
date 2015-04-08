
#ifndef INCLUDED_SDSL_CST_FULLY_BLIND
#define INCLUDED_SDSL_CST_FULLY_BLIND

#include "bit_vectors.hpp"
#include "vectors.hpp"
#include "bp_support.hpp"
#include "csa_iterators.hpp"
#include <fstream>

namespace sdsl {

template<class t_csa = csa_wt<>,
         uint32_t t_delta = 0,
         class t_bp_support = bp_support_sada<>,
         class t_b = sd_vector<>,
         class t_depth = dac_vector<>,
         bool t_sample_leaves = false
         >
class cst_fully_blind {
public:
    typedef cst_dfs_const_forward_iterator<cst_fully_blind> const_iterator;
    typedef typename t_csa::size_type                       size_type;
    typedef t_csa                                           csa_type;
    typedef typename t_csa::char_type                       char_type;
    typedef std::pair<size_type, size_type>                 node_type; // Nodes are represented by their left and right leafs (inclusive)
    typedef size_type                                       leaf_type; // Index of a leaf
    typedef size_type                                       bin_node_type; // Node in the sampled tree represented by its index in bp_bin
    typedef size_type                                       sampled_node_type; // Node in the sampled tree represented by its index in s
    typedef t_bp_support                                    bp_bin_support_type;
    typedef t_b                                             b_type;
    typedef typename t_b::select_0_type                     b_select_0_type;
    typedef typename t_b::select_1_type                     b_select_1_type;
    typedef int_vector<>                                    pd_type;
    typedef t_bp_support                                    s_support_type;
    typedef t_depth                                         depth_type;

    typedef typename t_csa::alphabet_category               alphabet_category;
    typedef bcst_tag                                        index_category;

private:
    size_type                      m_delta;
    csa_type                       m_csa;
    bit_vector                     m_bp_bin; // balanced parenthesis representation of the binary tree
    bp_bin_support_type            m_bp_bin_support;
    b_type                         m_b;
    b_select_0_type                m_b_select0;
    b_select_1_type                m_b_select1;
    pd_type                        m_pd; // Depth (mod log sigma) of nodes in the binary tree
    bit_vector                     m_in_st; // For each opening parenthesis in bp_bin: 1 if the node is in the suffix tree, 0 if it is a pseudonode
    bit_vector                     m_in_s; // For each parenthesis in bp_bin: 1 if the parenthesis is also in S, 0 otherwise
    bit_vector::rank_1_type        m_in_s_rank1;
    bit_vector::select_1_type      m_in_s_select1;
    bit_vector                     m_s; // S, the original sampled suffix tree (no child sampling, no pseudo nodes)
    s_support_type                 m_s_support;
    depth_type                     m_depth;

    void copy(const cst_fully_blind& cst) {
        m_delta          = cst.m_delta;
        m_csa            = cst.m_csa;
        m_bp_bin         = cst.m_bp_bin;
        m_bp_bin_support = cst.m_bp_bin_support;
        m_bp_bin_support.set_vector(&m_bp_bin);
        m_b              = cst.m_b;
        m_b_select0      = cst.m_b_select0;
        m_b_select0.set_vector(&m_b);
        m_b_select1      = cst.m_b_select1;
        m_b_select1.set_vector(&m_b);
        m_pd             = cst.m_pd;
        m_in_st          = cst.m_in_st;
        m_in_s           = cst.m_in_s;
        m_in_s_rank1     = cst.m_in_s_rank1;
        m_in_s_rank1.set_vector(&m_in_s);
        m_in_s_select1   = cst.m_in_s_select1;
        m_in_s_select1.set_vector(&m_in_s);
        m_s              = cst.m_s;
        m_s_support      = cst.m_s_support;
        m_s_support.set_vector(&m_s);
        m_depth          = cst.m_depth;
    }

public:
    const size_type                 &delta = m_delta;
    const csa_type                  &csa = m_csa;
    const bit_vector                &bp_bin = m_bp_bin;
    const bp_bin_support_type       &bp_bin_support = m_bp_bin_support;
    const b_type                    &b = m_b;
    const b_select_0_type           &b_select_0 = m_b_select0;
    const b_select_1_type           &b_select_1 = m_b_select1;
    const pd_type                   &pd = m_pd;
    const bit_vector                &in_st = m_in_st;
    const bit_vector                &in_s = m_in_s;
    const bit_vector::rank_1_type   &in_s_rank1 = m_in_s_rank1;
    const bit_vector::select_1_type &in_s_select1 = m_in_s_select1;
    const bit_vector                &s = m_s;
    const s_support_type            &s_support = m_s_support;
    const depth_type                &depth_sampling = m_depth;

//! Default constructor
    cst_fully_blind() {}

//! Copy constructor
    cst_fully_blind(const cst_fully_blind &cst) {
        copy(cst);
    }

//! Move constructor
    cst_fully_blind(cst_fully_blind &&cst) {
        *this = std::move(cst);
    }

//! Construct CST from file_map
    cst_fully_blind(cache_config &config);


    size_type size() const {
        return m_csa.size();
    }

    static size_type max_size() {
        return t_csa::max_size();
    }

    bool empty() const {
        return m_csa.empty();
    }

    void swap(cst_fully_blind &cst) {
        if(this != &cst) {
            std::swap(m_delta, cst.m_delta);
            m_csa.swap(cst.m_csa);
            m_bp_bin.swap(cst.m_bp_bin);
            util::swap_support(m_bp_bin_support, cst.m_bp_bin_support, &m_bp_bin, &(cst.m_bp_bin));
            m_b.swap(cst.m_b);
            util::swap_support(m_b_select0, cst.m_b_select0, &m_b, &(cst.m_b));
            util::swap_support(m_b_select1, cst.m_b_select1, &m_b, &(cst.m_b));
            m_pd.swap(cst.m_pd);
            m_in_st.swap(cst.m_in_st);
            m_in_s.swap(cst.m_in_s);
            util::swap_support(m_in_s_rank1, cst.m_in_s_rank1, &m_in_s, &(cst.m_in_s));
            util::swap_support(m_in_s_select1, cst.m_in_s_select1, &m_in_s, &(cst.m_in_s));
            m_s.swap(cst.m_s);
            util::swap_support(m_s_support, cst.m_s_support, &m_s, &(cst.m_s));
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
    cst_fully_blind& operator=(const cst_fully_blind &cst) {
        if(this != &cst) {
            copy(cst);
        }
        return *this;
    }

//! Move Assignment Operator.
    cst_fully_blind& operator=(cst_fully_blind &&cst) {
        if(this != &cst) {
            m_delta          = cst.m_delta;
            m_csa            = std::move(cst.m_csa);
            m_bp_bin         = std::move(cst.m_bp_bin);
            m_bp_bin_support = std::move(cst.m_bp_bin_support);
            m_bp_bin_support.set_vector(&m_bp_bin);
            m_b              = std::move(cst.m_b);
            m_b_select0      = std::move(cst.m_b_select0);
            m_b_select0.set_vector(&m_b);
            m_b_select1      = std::move(cst.m_b_select1);
            m_b_select1.set_vector(&m_b);
            m_pd             = std::move(cst.m_pd);
            m_in_st          = std::move(cst.m_in_st);
            m_in_s           = std::move(cst.m_in_s);
            m_in_s_rank1     = std::move(cst.m_in_s_rank1);
            m_in_s_rank1.set_vector(&m_in_s);
            m_in_s_select1   = std::move(cst.m_in_s_select1);
            m_in_s_select1.set_vector(&m_in_s);
            m_s              = std::move(cst.m_s);
            m_s_support      = std::move(cst.m_s_support);
            m_s_support.set_vector(&m_s);
            m_depth          = std::move(cst.m_depth);
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
        written_bytes += m_bp_bin.serialize(out, child, "bp_bin");
        written_bytes += m_bp_bin_support.serialize(out, child, "bp_bin_support");
        written_bytes += m_b.serialize(out, child, "b");
        written_bytes += m_b_select0.serialize(out, child, "b_select0");
        written_bytes += m_b_select1.serialize(out, child, "b_select1");
        written_bytes += m_pd.serialize(out, child, "pd");
        written_bytes += m_in_st.serialize(out, child, "in_st");
        written_bytes += m_in_s.serialize(out, child, "in_s");
        written_bytes += m_in_s_rank1.serialize(out, child, "in_s_rank1");
        written_bytes += m_in_s_select1.serialize(out, child, "in_s_select1");
        written_bytes += m_s.serialize(out, child, "s");
        written_bytes += m_s_support.serialize(out, child, "s_support");
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
        m_bp_bin.load(in);
        m_bp_bin_support.load(in, &m_bp_bin);
        m_b.load(in);
        m_b_select0.load(in, &m_b);
        m_b_select1.load(in, &m_b);
        m_pd.load(in);
        m_in_st.load(in);
        m_in_s.load(in);
        m_in_s_rank1.load(in, &m_in_s);
        m_in_s_select1.load(in, &m_in_s);
        m_s.load(in);
        m_s_support.load(in, &m_s);
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

//! Return the i-th leaf (1-based from left to right) of the suffix tree.
    /*!
     * \param i 1-based position of the leaf. \f$1\leq i\leq csa.size()\f$.
     * \return The i-th leave.
     * \par Time complexity
     *     \f$ \Order{1} \f$
     * \pre \f$ 1 \leq i \leq csa.size() \f$
     */
    node_type select_leaf(size_type i) const {
        assert(i > 0 and i <= m_csa.size());
        return node_type(i - 1, i - 1);
    }

//! Get the node in the suffix tree which corresponds to the sa-interval [lb..rb]
    node_type node(size_type lb, size_type rb) const {
        return node_type(lb, rb);
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

    sampled_node_type bin_to_s(bin_node_type w) const {
        return m_in_s_rank1.rank(w) + m_in_s[w] - 1;
    }

    bin_node_type s_to_bin(sampled_node_type u) const {
        return m_in_s_select1.select(u + 1);
    }

//! Returns the index of the last bracket in S before the leaf with index l.
        /*!
         * \param l The index of leaf l.
         * \return The index of the last bracket in S before the leaf with index l.
         */
    sampled_node_type pred(leaf_type l) const {
        return bin_to_s(m_b_select0.select(l + 1) - l - 1);
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
        size_type b_left = m_b_select1.select(s_to_bin(u) + 1);
        size_type b_right = m_b_select1.select(s_to_bin(u_end) + 1);

        return node_type(b_left - s_to_bin(u),
                         b_right - s_to_bin(u_end) - 1);
    }

    node_type bin_node(bin_node_type w) const {
        assert(m_bp_bin[w] == 1);
        size_type w_end = m_bp_bin_support.find_close(w);
        size_type b_left = m_b_select1.select(w + 1);
        size_type b_right = m_b_select1.select(w_end + 1);

        return node_type(b_left - w,
                         b_right - w_end - 1);
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
//! In case SDepth(SLink^i([l, r])) + i is maximal for different values of i, the smallest value of i is chosen.
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

//! Calculate the depth of the LCA of two leaves l and r.
//! In case SDepth(SLink^i([l, r])) + i is maximal for different values of i, the largest value of i is chosen.
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
    size_type depth_lca_2(leaf_type l, leaf_type r,
                        size_type &res_i, sampled_node_type &res_u, std::vector<char_type> &res_label) const {
        assert(l<r);

        size_type max_d = 0;
        size_type max_d_i = 0;
        sampled_node_type max_d_node = 0;

        for(size_type i = 0; i < m_delta; i++) {
            sampled_node_type node = lcsa(lsa_leaf(l), lsa_leaf(r));
            size_type d = i + depth(node);

            // TODO: Just for testing
            if(d >= max_d) {
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

        const auto cc = m_csa.char2comp[c];
        if (cc==0 and c!= 0) {
            return root();
        }

        size_type l = lb(v);
        size_type r = rb(v);

        size_type i;
        sampled_node_type u;
        std::vector<char_type> label(delta, 0);
        depth_lca_2(l, r, i, u, label);

        // fancy child search
        if(u == sampled_root()) {
            l = 0;
            r = size() - 1;

            backward_search(m_csa, l, r, c, l, r);
        } else {
            const size_type bits_per_char = bits::hi(m_csa.sigma - 1) + 1;
            bin_node_type w = s_to_bin(u);
            do {
                size_type pd = m_pd[m_bp_bin_support.rank(w) - 1];
                bool is_right_node = (cc & (1 << (bits_per_char - pd - 1)));
                if(is_right_node) {
                    bin_node_type new_w = m_bp_bin_support.find_close(w + 1) + 1;
                    if(m_bp_bin[new_w]) {
                        w = new_w;
                    } else {
                        // w has only one child
                        w++;
                    }
                } else {
                    w++;
                }
            } while(m_in_st[m_bp_bin_support.rank(w) - 1] == 0);

            node_type u_c = bin_node(w);
            l = lb(u_c);
            r = rb(u_c);

            size_type char_pos = get_char_pos(l, depth(u), m_csa);
            if(char_pos < m_csa.C[cc] || char_pos >= m_csa.C[cc + 1]) {
                return root();
            }
        }

        for(size_type k = 0; k < i; k++) {
            backward_search(m_csa, l, r, label[i - k - 1], l, r);
        }

        if(l > r) {
            return root();
        } else {
            return node_type(l, r);
        }
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

        size_type d = depth(v);
        size_type char_pos = get_char_pos(v.first, d, m_csa);
        char_type c = m_csa.F[char_pos];
        node_type res = child(v, c);
        while(i > 1) {
            if(res.second >= v.second) {
                return root();
            }
            char_pos = get_char_pos(res.second + 1, d, m_csa);
            c = m_csa.F[char_pos];
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
        if(v.second >= p.second) {
            return root();
        }
        size_type d = depth(p);
        size_type char_pos = get_char_pos(v.second + 1, d, m_csa);
        char_type c = m_csa.F[char_pos];
        return child(p, c);
    }

    char_type edge(node_type v, size_type d) const {
        assert(d >= 1 and d <= depth(v));
        size_type char_pos = get_char_pos(v.first, d - 1, m_csa);
        return m_csa.F[char_pos];
    }

    //! Get the number of nodes in the sampled tree.
        /*!
         * \return The number of nodes in the sampled tree.
         * \par Time complexity
         *   \f$ \Order{1} \f$
         */
    size_type sampled_nodes() const {
        return m_bp_bin.size() / 2;
    }
};

template<class t_csa, uint32_t t_delta, class t_s_support, class t_b, class t_depth, bool t_sample_leaves>
cst_fully_blind<t_csa, t_delta, t_s_support, t_b, t_depth, t_sample_leaves>::cst_fully_blind(cache_config &config) {
    // 1. Construct blind CST
    cst_sada_blind<t_csa, lcp_dac<> > cst(config);

    if(t_delta > 0) {
        m_delta = t_delta;
    } else {
        const size_type n = cst.size();
        m_delta = (bits::hi(n-1)+1) * (bits::hi(bits::hi(n-1))+1);
        if(m_delta < 2) {
            m_delta = 2;
        }
    }

    size_type delta_half = m_delta / 2;
    typedef csa_iterator<t_csa> csa_iterator_type;

    bit_vector is_sampled(cst.nodes_bin(), false);
    is_sampled[cst.id_bin(cst.root())] = true; // always sample root
    size_type sample_count = 1;

    bit_vector is_sampled_s(cst.nodes_bin(), false);
    is_sampled_s[cst.id_bin(cst.root())] = true; // always sample root
    size_type sample_count_s = 1;

    // 2a. Scan and mark leaves to be sampled
    if(t_sample_leaves) {
        auto event = memory_monitor::event("scan-leaves");
        for(auto it = csa_iterator_type::begin(cst.csa); it != csa_iterator_type::end(cst.csa); ++it) {
            const size_type d = it.depth();
            if(d + delta_half <= cst.size() and
               d % delta_half == 0) {
                const auto node = cst.select_leaf(*it + 1);
                const size_type id = cst.id_bin(node);
                if(!is_sampled[id]) {
                    is_sampled[id] = true;
                    sample_count++;
                }
                if(!is_sampled_s[id]) {
                    is_sampled_s[id] = true;
                    sample_count_s++;
                }
            }
        }
    }

    // 2b. Scan an mark inner nodes to be sampled
    {
        auto event = memory_monitor::event("scan-nodes");
        for(auto it = cst.begin(); it != cst.end(); ++it) {
            if(it.visit() == 1 and cst.is_leaf(*it) == false and *it != cst.root()) {
                const auto node = *it;
                const size_type d = cst.depth(node);
                if(d % delta_half == 0) {
                    auto v = cst.sl_i(node, delta_half);
                    const size_type id = cst.id_bin(v);
                    if(!is_sampled[id]) {
                        is_sampled[id] = true;
                        sample_count++;
                    }
                    if(!is_sampled_s[id]) {
                        is_sampled_s[id] = true;
                        sample_count_s++;
                    }
                    for(auto child = cst.select_child(node, 1); child != cst.root(); child = cst.sibling(child)) {
                        auto c = cst.edge(child, d + 1);
                        auto sampled_child = cst.child(v, c);
                        const size_type sampled_child_id = cst.id_bin(sampled_child);
                        if(!is_sampled[sampled_child_id]) {
                            is_sampled[sampled_child_id] = true;
                            sample_count++;
                        }
                    }
                }
            }
        }
    }

    // 3. Choose sampled pseudo-nodes (so the sampled tree forms a binary tree)
    {
        // TODO: Use bit_vector as stack
        auto event = memory_monitor::event("bin-tree");
        bit_vector stack;
        stack.resize(cst.nodes_bin());
        stack[0] = 0;
        size_t stack_idx = 0;
        bool node_has_two_children = false;

        for (auto it=cst.begin_bin(), end=cst.end_bin(); it!=end; ++it) {
            bool in_cst = cst.is_suffix_node(*it);

            if(cst.is_leaf(*it)) {
                if(is_sampled[cst.id_bin(*it)]) {
                    node_has_two_children = stack[stack_idx];
                    stack[stack_idx] = 1;
                }
            } else {
                if(1 == it.visit()) {
                    stack[++stack_idx] = 0;
                }
                if(2 == it.visit()) {
                    if(in_cst && is_sampled[cst.id_bin(*it)]) {
                        stack_idx--;
                        node_has_two_children = stack[stack_idx];
                        stack[stack_idx] = 1;
                    } else if(node_has_two_children) {
                        is_sampled[cst.id_bin(*it)] = 1;
                        sample_count++;
                        stack_idx--;
                        node_has_two_children = stack[stack_idx];
                        stack[stack_idx] = 1;
                    } else if(stack[stack_idx] == 1) {
                        stack_idx--;
                        node_has_two_children = stack[stack_idx];
                        stack[stack_idx] = 1;
                    } else {
                        stack_idx--;
                    }
                }
            }
        }
    }

    // 4. Create sampled tree data structures
    const size_type width = bits::hi(cst.csa.sigma-1)+1;

    bit_vector tmp_b;
    int_vector<64> tmp_depth;

    m_bp_bin.resize(2 * sample_count);
    tmp_b.resize(2 * sample_count + cst.size());
    m_pd.width(bits::hi(width-1)+1);
    m_pd.resize(sample_count);
    m_in_st.resize(sample_count);
    m_in_s.resize(2 * sample_count);
    m_s.resize(2 * sample_count_s);
    tmp_depth.resize(sample_count_s);

    {
        auto event = memory_monitor::event("node-sampling");
        size_type bp_bin_idx = 0;
        size_type b_idx = 0;
        size_type pd_idx = 0;
        size_type in_st_idx = 0;
        size_type in_s_idx = 0;
        size_type s_idx = 0;
        size_type depth_idx = 0;

        for (auto it=cst.begin_bin(), end=cst.end_bin(); it!=end; ++it) {
            bool in_cst = cst.is_suffix_node(*it);

            if(it.visit() == 1 && is_sampled[cst.id_bin(*it)]) {
                m_bp_bin[bp_bin_idx++] = 1;
                tmp_b[b_idx++] = 1;
                if(cst.is_leaf(*it)) {
                    m_pd[pd_idx++] = 0;
                } else {
                    size_type d = cst.depth_bin(*it);
                    m_pd[pd_idx++] = d % width;
                }
                m_in_st[in_st_idx++] = (in_cst ? 1 : 0);
                if(in_cst && is_sampled_s[cst.id_bin(*it)]) {
                    m_in_s[in_s_idx++] = 1;
                    m_s[s_idx++] = 1;
                    size_type d = cst.depth(*it);
                    tmp_depth[depth_idx++] = d / delta_half;
                } else {
                    m_in_s[in_s_idx++] = 0;
                }
            }
            if(cst.is_leaf(*it)) {
                tmp_b[b_idx++] = 0;
            }
            if((cst.is_leaf(*it) || it.visit() == 2) && is_sampled[cst.id_bin(*it)]) {
                m_bp_bin[bp_bin_idx++] = 0;
                tmp_b[b_idx++] = 1;
                if(in_cst && is_sampled_s[cst.id_bin(*it)]) {
                    m_in_s[in_s_idx++] = 1;
                    m_s[s_idx++] = 0;
                } else {
                    m_in_s[in_s_idx++] = 0;
                }
            }
        }
    }

    {
        auto event = memory_monitor::event("ss-depth");
        m_csa = std::move(cst.csa);
        util::init_support(m_bp_bin_support, &m_bp_bin);
        m_b = b_type(tmp_b);
        util::init_support(m_b_select0, &m_b);
        util::init_support(m_b_select1, &m_b);
        util::init_support(m_in_s_rank1, &m_in_s);
        util::init_support(m_in_s_select1, &m_in_s);
        util::init_support(m_s_support, &m_s);
        m_depth = depth_type(tmp_depth);
    }
}

}// end namespace sdsl

#endif // INCLUDED_SDSL_CST_FULLY_BLIND
