
#ifndef INCLUDED_CSA_ITERATORS
#define INCLUDED_CSA_ITERATORS

using namespace sdsl;

template<class t_csa,
         class t_extract_category = typename t_csa::extract_category>
class csa_iterator {
public:
    typedef typename t_csa::size_type               size_type;
    typedef t_csa                                   csa_type;
    typedef csa_iterator<t_csa, t_extract_category> iterator;

private:
    const csa_type &m_csa;
    size_type       m_leaf;
    size_type       m_depth;
    bool            m_valid;

public:
    csa_iterator(const csa_type &csa, size_type leaf, size_type depth, bool valid)
    : m_csa(csa),
      m_leaf(leaf),
      m_depth(depth),
      m_valid(valid)
    {}

    static iterator begin(const csa_type &csa) {
        return iterator(csa, csa.psi[0], csa.size(), !csa.empty());
    }

    static iterator end(const csa_type &csa) {
        return iterator(csa, 0, 1, false);
    }

    size_type depth() const {
        return m_depth;
    }

    const size_type operator*() const {
        return m_leaf;
    }

    iterator operator++() {
        if(m_depth == 1) {
            m_valid = false;
            return *this;
        }

        m_leaf = m_csa.psi[m_leaf];
        m_depth--;
        return *this;
    }

    void operator++(int) {
        ++(*this);
    }

    bool operator==(const iterator &it) const {
        return (m_depth == it.m_depth) and (m_valid == it.m_valid);
    }

    bool operator!=(const iterator &it) const {
        return !(*this==it);
    }
};

template<class t_csa>
class csa_iterator<t_csa, lf_tag> {
public:
    typedef typename t_csa::size_type   size_type;
    typedef t_csa                       csa_type;
    typedef csa_iterator<t_csa, lf_tag> iterator;

private:
    const csa_type &m_csa;
    size_type       m_leaf;
    size_type       m_depth;
    bool            m_valid;

public:
    csa_iterator(const csa_type &csa, size_type leaf, size_type depth, bool valid)
    : m_csa(csa),
      m_leaf(leaf),
      m_depth(depth),
      m_valid(valid)
    {}

    static iterator begin(const csa_type &csa) {
        return iterator(csa, 0, 1, !csa.empty());
    }

    static iterator end(const csa_type &csa) {
        return iterator(csa, 0, csa.size(), false);
    }

    size_type depth() const {
        return m_depth;
    }

    const size_type operator*() const {
        return m_leaf;
    }

    iterator operator++() {
        if(m_depth == m_csa.size()) {
            m_valid = false;
            return *this;
        }

        m_leaf = m_csa.lf[m_leaf];
        m_depth++;
        return *this;
    }

    void operator++(int) {
        ++(*this);
    }

    bool operator==(const iterator &it) const {
        return (m_depth == it.m_depth) and (m_valid == it.m_valid);
    }

    bool operator!=(const iterator &it) const {
        return !(*this==it);
    }
};

#endif // INCLUDED_CSA_ITERATORS
