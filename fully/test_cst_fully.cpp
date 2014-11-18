
#include <sdsl/suffix_trees.hpp>
#include <gtest/gtest.h>

using namespace sdsl;

template<typename t_cst>
class CstFullyWhiteboxTest : public ::testing::Test {
};

typedef ::testing::Types<cst_fully<> > WhiteboxTestTypes;
TYPED_TEST_CASE(CstFullyWhiteboxTest, WhiteboxTestTypes);

TYPED_TEST(CstFullyWhiteboxTest, LsaLeafTest) {
    typedef TypeParam cst_type;

    cst_type fcst = cst_type::construct_im("abbbab", 4, true);

    // sampled tree should look like (0 1 2 (3) (4) 5 6)
    EXPECT_EQ(0, fcst.lsa_leaf(0));
    EXPECT_EQ(0, fcst.lsa_leaf(1));
    EXPECT_EQ(0, fcst.lsa_leaf(2));
    EXPECT_EQ(1, fcst.lsa_leaf(3));
    EXPECT_EQ(3, fcst.lsa_leaf(4));
    EXPECT_EQ(0, fcst.lsa_leaf(5));
    EXPECT_EQ(0, fcst.lsa_leaf(6));
}

TYPED_TEST(CstFullyWhiteboxTest, LsaLeafTest2) {
    typedef TypeParam cst_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4, true);

    // sampled tree should look like (0 1 (2) 3 (4) 5 6 (7) ((8) 9) 10 (11))
    EXPECT_EQ(0, fcst.lsa_leaf(0));
    EXPECT_EQ(0, fcst.lsa_leaf(1));
    EXPECT_EQ(1, fcst.lsa_leaf(2));
    EXPECT_EQ(0, fcst.lsa_leaf(3));
    EXPECT_EQ(3, fcst.lsa_leaf(4));
    EXPECT_EQ(0, fcst.lsa_leaf(5));
    EXPECT_EQ(0, fcst.lsa_leaf(6));
    EXPECT_EQ(5, fcst.lsa_leaf(7));
    EXPECT_EQ(8, fcst.lsa_leaf(8));
    EXPECT_EQ(7, fcst.lsa_leaf(9));
    EXPECT_EQ(0, fcst.lsa_leaf(10));
    EXPECT_EQ(11, fcst.lsa_leaf(11));
}

TYPED_TEST(CstFullyWhiteboxTest, LsaLeafTest3) {
    typedef TypeParam cst_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 2, true);

    // sampled tree should look like ((0)1((2)(3)(4)(5))(6)(7)((8)(9))((10)(11)))
    EXPECT_EQ(1, fcst.lsa_leaf(0));
    EXPECT_EQ(0, fcst.lsa_leaf(1));
    EXPECT_EQ(4, fcst.lsa_leaf(2));
    EXPECT_EQ(6, fcst.lsa_leaf(3));
    EXPECT_EQ(8, fcst.lsa_leaf(4));
    EXPECT_EQ(10, fcst.lsa_leaf(5));
    EXPECT_EQ(13, fcst.lsa_leaf(6));
    EXPECT_EQ(15, fcst.lsa_leaf(7));
    EXPECT_EQ(18, fcst.lsa_leaf(8));
    EXPECT_EQ(20, fcst.lsa_leaf(9));
    EXPECT_EQ(24, fcst.lsa_leaf(10));
    EXPECT_EQ(26, fcst.lsa_leaf(11));
}

TYPED_TEST(CstFullyWhiteboxTest, LsaLeafTest4) {
    typedef TypeParam cst_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4);

    // sampled tree should look like (0 1 2 3 4 5 6 7 (8 9) 10 11)
    EXPECT_EQ(0, fcst.lsa_leaf(0));
    EXPECT_EQ(0, fcst.lsa_leaf(1));
    EXPECT_EQ(0, fcst.lsa_leaf(2));
    EXPECT_EQ(0, fcst.lsa_leaf(3));
    EXPECT_EQ(0, fcst.lsa_leaf(4));
    EXPECT_EQ(0, fcst.lsa_leaf(5));
    EXPECT_EQ(0, fcst.lsa_leaf(6));
    EXPECT_EQ(0, fcst.lsa_leaf(7));
    EXPECT_EQ(1, fcst.lsa_leaf(8));
    EXPECT_EQ(1, fcst.lsa_leaf(9));
    EXPECT_EQ(0, fcst.lsa_leaf(10));
    EXPECT_EQ(0, fcst.lsa_leaf(11));
}

TYPED_TEST(CstFullyWhiteboxTest, SampledNodeTest) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("abbbab", 4, true);

    EXPECT_EQ(node_type(0, 6), fcst.sampled_node(0));
    EXPECT_EQ(node_type(3, 3), fcst.sampled_node(1));
    EXPECT_EQ(node_type(4, 4), fcst.sampled_node(3));
}

TYPED_TEST(CstFullyWhiteboxTest, SampledNodeTest2) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4, true);

    EXPECT_EQ(node_type(0, 11), fcst.sampled_node(0));
    EXPECT_EQ(node_type(2, 2), fcst.sampled_node(1));
    EXPECT_EQ(node_type(4, 4), fcst.sampled_node(3));
    EXPECT_EQ(node_type(7, 7), fcst.sampled_node(5));
    EXPECT_EQ(node_type(8, 9), fcst.sampled_node(7));
    EXPECT_EQ(node_type(8, 8), fcst.sampled_node(8));
}

TYPED_TEST(CstFullyWhiteboxTest, SampledNodeTest3) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4);

    EXPECT_EQ(node_type(0, 11), fcst.sampled_node(0));
    EXPECT_EQ(node_type(8, 9), fcst.sampled_node(1));
}

TYPED_TEST(CstFullyWhiteboxTest, SampledDepthTest) {
    typedef TypeParam cst_type;

    cst_type fcst = cst_type::construct_im("abbbab", 4, true);

    EXPECT_EQ(0, fcst.depth(0));
    EXPECT_EQ(2, fcst.depth(1));
    EXPECT_EQ(4, fcst.depth(3));
}

TYPED_TEST(CstFullyWhiteboxTest, SampledDepthTest2) {
    typedef TypeParam cst_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4, true);

    EXPECT_EQ(0, fcst.depth(0));
    EXPECT_EQ(2, fcst.depth(1));
    EXPECT_EQ(8, fcst.depth(3));
    EXPECT_EQ(4, fcst.depth(5));
    EXPECT_EQ(2, fcst.depth(7));
    EXPECT_EQ(6, fcst.depth(8));
}

TYPED_TEST(CstFullyWhiteboxTest, SampledDepthTest3) {
    typedef TypeParam cst_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4);

    EXPECT_EQ(0, fcst.depth(0));
    EXPECT_EQ(2, fcst.depth(1));
}

TYPED_TEST(CstFullyWhiteboxTest, LCSATest) {
    typedef TypeParam cst_type;

    cst_type fcst = cst_type::construct_im("abbbab", 4, true);

    EXPECT_EQ(0, fcst.lcsa(0, 0));
    EXPECT_EQ(1, fcst.lcsa(1, 1));
    EXPECT_EQ(3, fcst.lcsa(3, 3));
    EXPECT_EQ(0, fcst.lcsa(0, 1));
    EXPECT_EQ(0, fcst.lcsa(0, 3));
    EXPECT_EQ(0, fcst.lcsa(1, 3));
}

TYPED_TEST(CstFullyWhiteboxTest, LCSATest2) {
    typedef TypeParam cst_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4, true);

    EXPECT_EQ(0, fcst.lcsa(0, 0));
    EXPECT_EQ(1, fcst.lcsa(1, 1));
    EXPECT_EQ(3, fcst.lcsa(3, 3));
    EXPECT_EQ(5, fcst.lcsa(5, 5));
    EXPECT_EQ(7, fcst.lcsa(7, 7));
    EXPECT_EQ(8, fcst.lcsa(8, 8));
    EXPECT_EQ(0, fcst.lcsa(0, 3));
    EXPECT_EQ(0, fcst.lcsa(0, 7));
    EXPECT_EQ(0, fcst.lcsa(0, 8));
    EXPECT_EQ(0, fcst.lcsa(5, 7));
    EXPECT_EQ(0, fcst.lcsa(5, 8));
    EXPECT_EQ(7, fcst.lcsa(7, 8));
    EXPECT_EQ(7, fcst.lcsa(8, 7));
}

TYPED_TEST(CstFullyWhiteboxTest, LCSATest3) {
    typedef TypeParam cst_type;

    cst_type fcst = cst_type::construct_im(" as far as I c as j as fai", 4, true);

    // sampled tree should look like (01(2)(3)4567(8)(9)(10)11(12)131415(16)(17)((18)19)(20)2122((23)((24)25)26))
    EXPECT_EQ(fcst.lcsa(fcst.lsa_leaf(23), fcst.lsa_leaf(24)),
              fcst.lcsa(fcst.lcsa(fcst.lsa_leaf(23), fcst.lsa_leaf(24)), fcst.lsa_leaf(24)));
}

TYPED_TEST(CstFullyWhiteboxTest, LCSATest4) {
    typedef TypeParam cst_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4);

    EXPECT_EQ(0, fcst.lcsa(0, 0));
    EXPECT_EQ(0, fcst.lcsa(0, 1));
    EXPECT_EQ(0, fcst.lcsa(1, 0));
    EXPECT_EQ(1, fcst.lcsa(1, 1));
}

template<typename t_cst>
class CstFullyBlackboxTest : public ::testing::Test {
};

typedef ::testing::Types<cst_fully<>, cst_fully_sds<> > BlackboxTestTypes;
TYPED_TEST_CASE(CstFullyBlackboxTest, BlackboxTestTypes);

TYPED_TEST(CstFullyBlackboxTest, DepthTest) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("abbbab", 4);

    EXPECT_EQ(1, fcst.depth(node_type(0, 0)));
    EXPECT_EQ(0, fcst.depth(node_type(0, 3)));
    EXPECT_EQ(0, fcst.depth(node_type(0, 6)));
    EXPECT_EQ(3, fcst.depth(node_type(1, 1)));
    EXPECT_EQ(2, fcst.depth(node_type(1, 2)));
    EXPECT_EQ(0, fcst.depth(node_type(1, 3)));
    EXPECT_EQ(0, fcst.depth(node_type(1, 4)));
    EXPECT_EQ(0, fcst.depth(node_type(1, 6)));
    EXPECT_EQ(7, fcst.depth(node_type(2, 2)));
    EXPECT_EQ(0, fcst.depth(node_type(2, 3)));
    EXPECT_EQ(2, fcst.depth(node_type(3, 3)));
    EXPECT_EQ(1, fcst.depth(node_type(3, 4)));
    EXPECT_EQ(1, fcst.depth(node_type(3, 5)));
    EXPECT_EQ(1, fcst.depth(node_type(3, 6)));
    EXPECT_EQ(5, fcst.depth(node_type(5, 5)));
    EXPECT_EQ(2, fcst.depth(node_type(5, 6)));
}

TYPED_TEST(CstFullyBlackboxTest, LCATest) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("abbbab", 4);

    auto lca = [&](size_t l, size_t r) { return fcst.lca(node_type(l, l), node_type(r, r)); };

    EXPECT_EQ(node_type(0, 0), lca(0, 0));
    EXPECT_EQ(node_type(0, 6), lca(0, 3));
    EXPECT_EQ(node_type(0, 6), lca(0, 6));
    EXPECT_EQ(node_type(1, 1), lca(1, 1));
    EXPECT_EQ(node_type(1, 2), lca(1, 2));
    EXPECT_EQ(node_type(0, 6), lca(1, 3));
    EXPECT_EQ(node_type(0, 6), lca(1, 4));
    EXPECT_EQ(node_type(0, 6), lca(1, 6));
    EXPECT_EQ(node_type(2, 2), lca(2, 2));
    EXPECT_EQ(node_type(0, 6), lca(2, 3));
    EXPECT_EQ(node_type(3, 3), lca(3, 3));
    EXPECT_EQ(node_type(3, 6), lca(3, 4));
    EXPECT_EQ(node_type(3, 6), lca(3, 5));
    EXPECT_EQ(node_type(3, 6), lca(3, 6));
    EXPECT_EQ(node_type(5, 5), lca(5, 5));
    EXPECT_EQ(node_type(5, 6), lca(5, 6));
}

TYPED_TEST(CstFullyBlackboxTest, LCATest2) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4);

    auto lca = [&](size_t l, size_t r) { return fcst.lca(node_type(l, l), node_type(r, r)); };

    EXPECT_EQ(node_type(0,  0), lca(0,  0));
    EXPECT_EQ(node_type(0, 11), lca(0,  2));
    EXPECT_EQ(node_type(0, 11), lca(0,  8));
    EXPECT_EQ(node_type(0, 11), lca(0,  8));
    EXPECT_EQ(node_type(2,  2), lca(2,  2));
    EXPECT_EQ(node_type(2,  5), lca(2,  3));
    EXPECT_EQ(node_type(2,  5), lca(2,  4));
    EXPECT_EQ(node_type(2,  5), lca(2,  5));
    EXPECT_EQ(node_type(0, 11), lca(2,  6));
    EXPECT_EQ(node_type(0, 11), lca(2,  8));
    EXPECT_EQ(node_type(0, 11), lca(2, 10));
    EXPECT_EQ(node_type(2,  5), lca(3,  4));
    EXPECT_EQ(node_type(2,  5), lca(3,  5));
    EXPECT_EQ(node_type(0, 11), lca(3,  6));
    EXPECT_EQ(node_type(0, 11), lca(3,  9));
    EXPECT_EQ(node_type(4,  5), lca(4,  5));
    EXPECT_EQ(node_type(0, 11), lca(4,  6));
    EXPECT_EQ(node_type(8,  8), lca(8,  8));
    EXPECT_EQ(node_type(8,  9), lca(8,  9));
    EXPECT_EQ(node_type(8, 11), lca(8, 10));
    EXPECT_EQ(node_type(8, 11), lca(8, 11));
    EXPECT_EQ(node_type(9,  9), lca(9,  9));
    EXPECT_EQ(node_type(8, 11), lca(9, 10));
    EXPECT_EQ(node_type(8, 11), lca(9, 11));
}

TYPED_TEST(CstFullyBlackboxTest, SuffixLinkTest) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4);

    EXPECT_EQ(node_type( 5,  5), fcst.sl(node_type( 1,  1)));
    EXPECT_EQ(node_type(11, 11), fcst.sl(node_type( 5,  5)));
    EXPECT_EQ(node_type( 9,  9), fcst.sl(node_type(11, 11)));
    EXPECT_EQ(node_type( 4,  4), fcst.sl(node_type( 9,  9)));
    EXPECT_EQ(node_type(10, 10), fcst.sl(node_type( 4,  4)));
    EXPECT_EQ(node_type( 8,  8), fcst.sl(node_type(10, 10)));
    EXPECT_EQ(node_type( 3,  3), fcst.sl(node_type( 8,  8)));
    EXPECT_EQ(node_type( 7,  7), fcst.sl(node_type( 3,  3)));
    EXPECT_EQ(node_type( 6,  6), fcst.sl(node_type( 7,  7)));
    EXPECT_EQ(node_type( 2,  2), fcst.sl(node_type( 6,  6)));
    EXPECT_EQ(node_type( 0,  0), fcst.sl(node_type( 2,  2)));
    EXPECT_EQ(node_type( 1,  1), fcst.sl(node_type( 0,  0)));

    EXPECT_EQ(node_type( 0, 11), fcst.sl(node_type( 0, 11)));
    EXPECT_EQ(node_type( 0, 11), fcst.sl(node_type( 2,  5)));
    EXPECT_EQ(node_type(10, 11), fcst.sl(node_type( 4,  5)));
    EXPECT_EQ(node_type( 0, 11), fcst.sl(node_type( 6,  7)));
    EXPECT_EQ(node_type( 0, 11), fcst.sl(node_type( 8, 11)));
    EXPECT_EQ(node_type( 2,  5), fcst.sl(node_type( 8,  9)));
    EXPECT_EQ(node_type( 8,  9), fcst.sl(node_type(10, 11)));
}

TYPED_TEST(CstFullyBlackboxTest, ParentTest) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4);

    EXPECT_EQ(node_type( 0, 11), fcst.parent(node_type( 0,  0)));
    EXPECT_EQ(node_type( 0, 11), fcst.parent(node_type( 1,  1)));
    EXPECT_EQ(node_type( 2,  5), fcst.parent(node_type( 2,  2)));
    EXPECT_EQ(node_type( 2,  5), fcst.parent(node_type( 3,  3)));
    EXPECT_EQ(node_type( 4,  5), fcst.parent(node_type( 4,  4)));
    EXPECT_EQ(node_type( 4,  5), fcst.parent(node_type( 5,  5)));
    EXPECT_EQ(node_type( 6,  7), fcst.parent(node_type( 6,  6)));
    EXPECT_EQ(node_type( 6,  7), fcst.parent(node_type( 7,  7)));
    EXPECT_EQ(node_type( 8,  9), fcst.parent(node_type( 8,  8)));
    EXPECT_EQ(node_type( 8,  9), fcst.parent(node_type( 9,  9)));
    EXPECT_EQ(node_type(10, 11), fcst.parent(node_type(10, 10)));
    EXPECT_EQ(node_type(10, 11), fcst.parent(node_type(11, 11)));

    EXPECT_EQ(node_type( 0, 11), fcst.parent(node_type( 0, 11)));
    EXPECT_EQ(node_type( 0, 11), fcst.parent(node_type( 2,  5)));
    EXPECT_EQ(node_type( 2,  5), fcst.parent(node_type( 4,  5)));
    EXPECT_EQ(node_type( 0, 11), fcst.parent(node_type( 6,  7)));
    EXPECT_EQ(node_type( 8, 11), fcst.parent(node_type(10, 11)));
    EXPECT_EQ(node_type( 8, 11), fcst.parent(node_type( 8,  9)));
    EXPECT_EQ(node_type( 0, 11), fcst.parent(node_type( 8, 11)));
}

TYPED_TEST(CstFullyBlackboxTest, ChildTest) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4);

    EXPECT_EQ(node_type( 0,  0), fcst.child(node_type( 0, 11), '\0'));
    EXPECT_EQ(node_type( 1,  1), fcst.child(node_type( 0, 11), 'M'));
    EXPECT_EQ(node_type( 2,  5), fcst.child(node_type( 0, 11), 'i'));
    EXPECT_EQ(node_type( 6,  7), fcst.child(node_type( 0, 11), 'p'));
    EXPECT_EQ(node_type( 8, 11), fcst.child(node_type( 0, 11), 's'));
    EXPECT_EQ(node_type( 2,  2), fcst.child(node_type( 2,  5), '\0'));
    EXPECT_EQ(node_type( 3,  3), fcst.child(node_type( 2,  5), 'p'));
    EXPECT_EQ(node_type( 4,  5), fcst.child(node_type( 2,  5), 's'));
    EXPECT_EQ(node_type( 6,  6), fcst.child(node_type( 6,  7), 'i'));
    EXPECT_EQ(node_type( 7,  7), fcst.child(node_type( 6,  7), 'p'));
    EXPECT_EQ(node_type( 8,  9), fcst.child(node_type( 8, 11), 'i'));
    EXPECT_EQ(node_type(10, 11), fcst.child(node_type( 8, 11), 's'));
    EXPECT_EQ(node_type( 8,  8), fcst.child(node_type( 8,  9), 'p'));
    EXPECT_EQ(node_type( 9,  9), fcst.child(node_type( 8,  9), 's'));
    EXPECT_EQ(node_type(10, 10), fcst.child(node_type(10, 11), 'p'));
    EXPECT_EQ(node_type(11, 11), fcst.child(node_type(10, 11), 's'));

    EXPECT_EQ(fcst.root(),       fcst.child(node_type( 2,  5), 'i'));
    EXPECT_EQ(fcst.root(),       fcst.child(node_type( 6,  7), 's'));
    EXPECT_EQ(fcst.root(),       fcst.child(node_type( 8, 11), 'p'));
}

TYPED_TEST(CstFullyBlackboxTest, SelectChildTest) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4);

    EXPECT_EQ(node_type( 0,  0), fcst.select_child(node_type( 0, 11), 1));
    EXPECT_EQ(node_type( 1,  1), fcst.select_child(node_type( 0, 11), 2));
    EXPECT_EQ(node_type( 2,  5), fcst.select_child(node_type( 0, 11), 3));
    EXPECT_EQ(node_type( 6,  7), fcst.select_child(node_type( 0, 11), 4));
    EXPECT_EQ(node_type( 8, 11), fcst.select_child(node_type( 0, 11), 5));
    EXPECT_EQ(fcst.root(),       fcst.select_child(node_type( 0, 11), 6));
    EXPECT_EQ(node_type( 2,  2), fcst.select_child(node_type( 2,  5), 1));
    EXPECT_EQ(node_type( 3,  3), fcst.select_child(node_type( 2,  5), 2));
    EXPECT_EQ(node_type( 4,  5), fcst.select_child(node_type( 2,  5), 3));
    EXPECT_EQ(fcst.root(),       fcst.select_child(node_type( 2,  5), 4));
    EXPECT_EQ(node_type( 4,  4), fcst.select_child(node_type( 4,  5), 1));
    EXPECT_EQ(node_type( 5,  5), fcst.select_child(node_type( 4,  5), 2));
    EXPECT_EQ(fcst.root(),       fcst.select_child(node_type( 4,  5), 3));
    EXPECT_EQ(node_type( 6,  6), fcst.select_child(node_type( 6,  7), 1));
    EXPECT_EQ(node_type( 7,  7), fcst.select_child(node_type( 6,  7), 2));
    EXPECT_EQ(fcst.root(),       fcst.select_child(node_type( 6,  7), 3));
    EXPECT_EQ(node_type( 8,  9), fcst.select_child(node_type( 8, 11), 1));
    EXPECT_EQ(node_type(10, 11), fcst.select_child(node_type( 8, 11), 2));
    EXPECT_EQ(fcst.root(),       fcst.select_child(node_type( 8, 11), 3));
    EXPECT_EQ(node_type( 8,  8), fcst.select_child(node_type( 8,  9), 1));
    EXPECT_EQ(node_type( 9,  9), fcst.select_child(node_type( 8,  9), 2));
    EXPECT_EQ(fcst.root(),       fcst.select_child(node_type( 8,  9), 3));
    EXPECT_EQ(node_type(10, 10), fcst.select_child(node_type(10, 11), 1));
    EXPECT_EQ(node_type(11, 11), fcst.select_child(node_type(10, 11), 2));
    EXPECT_EQ(fcst.root(),       fcst.select_child(node_type(10, 11), 3));
}

TYPED_TEST(CstFullyBlackboxTest, SiblingTest) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4);

    EXPECT_EQ(node_type( 1,  1), fcst.sibling(node_type( 0,  0)));
    EXPECT_EQ(node_type( 2,  5), fcst.sibling(node_type( 1,  1)));
    EXPECT_EQ(node_type( 6,  7), fcst.sibling(node_type( 2,  5)));
    EXPECT_EQ(node_type( 3,  3), fcst.sibling(node_type( 2,  2)));
    EXPECT_EQ(node_type( 4,  5), fcst.sibling(node_type( 3,  3)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(node_type( 4,  5)));
    EXPECT_EQ(node_type( 5,  5), fcst.sibling(node_type( 4,  4)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(node_type( 5,  5)));
    EXPECT_EQ(node_type( 8, 11), fcst.sibling(node_type( 6,  7)));
    EXPECT_EQ(node_type( 7,  7), fcst.sibling(node_type( 6,  6)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(node_type( 7,  7)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(node_type( 8, 11)));
    EXPECT_EQ(node_type(10, 11), fcst.sibling(node_type( 8,  9)));
    EXPECT_EQ(node_type( 9,  9), fcst.sibling(node_type( 8,  8)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(node_type( 9,  9)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(node_type(10, 11)));
    EXPECT_EQ(node_type(11, 11), fcst.sibling(node_type(10, 10)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(node_type(11, 11)));
}

TYPED_TEST(CstFullyBlackboxTest, EdgeTest) {
    typedef TypeParam cst_type;
    typedef typename cst_type::node_type node_type;

    cst_type fcst = cst_type::construct_im("Mississippi", 4);

    EXPECT_EQ('i', fcst.edge(node_type( 2,  5), 1));
    EXPECT_EQ('p', fcst.edge(node_type( 6,  7), 1));
    EXPECT_EQ('s', fcst.edge(node_type( 8, 11), 1));
    EXPECT_EQ('s', fcst.edge(node_type( 8,  9), 1));
    EXPECT_EQ('i', fcst.edge(node_type( 8,  9), 2));
    EXPECT_EQ('s', fcst.edge(node_type(10, 11), 1));
    EXPECT_EQ('s', fcst.edge(node_type(10, 11), 2));
    EXPECT_EQ('i', fcst.edge(node_type(10, 11), 3));

    EXPECT_EQ('\0',fcst.edge(node_type( 0,  0), 1));
    EXPECT_EQ('M', fcst.edge(node_type( 1,  1), 1));
    EXPECT_EQ('i', fcst.edge(node_type( 1,  1), 2));
    EXPECT_EQ('s', fcst.edge(node_type( 1,  1), 3));
    EXPECT_EQ('p', fcst.edge(node_type( 1,  1), 10));
    EXPECT_EQ('i', fcst.edge(node_type( 1,  1), 11));
    EXPECT_EQ('\0',fcst.edge(node_type( 1,  1), 12));
}

const char road_not_taken[] = "TWO roads diverged in a yellow wood,\
And sorry I could not travel both\
And be one traveler, long I stood\
And looked down one as far as I could\
To where it bent in the undergrowth;\
\
Then took the other, as just as fair,\
And having perhaps the better claim,\
Because it was grassy and wanted wear;\
Though as for that the passing there\
Had worn them really about the same,\
\
And both that morning equally lay\
In leaves no step had trodden black.\
Oh, I kept the first for another day!\
Yet knowing how way leads on to way,\
I doubted if I should ever come back.\
\
I shall be telling this with a sigh\
Somewhere ages and ages hence:\
Two roads diverged in a wood, and I—\
I took the one less traveled by.\
And that has made all the difference.";

TYPED_TEST(CstFullyBlackboxTest, IteratorTest) {
    typedef TypeParam cst_type;

    typename cst_type::construction_cst_type cst;
    construct_im(cst, road_not_taken, 1);

    for(int delta = 2; delta <= 40; delta++) {
        cst_type fcst(cst, delta, false);

        auto it = fcst.begin();
        auto cst_it = cst.begin();

        while(it != fcst.end() && cst_it != cst.end()) {
            auto node = *it;
            auto sl = fcst.sl(node);
            auto parent = fcst.parent(node);
            auto cst_node = *cst_it;
            auto cst_ls = cst.sl(cst_node);
            auto cst_parent = cst.parent(cst_node);

            EXPECT_EQ(cst.lb(cst_node), fcst.lb(node));
            EXPECT_EQ(cst.rb(cst_node), fcst.rb(node));
            EXPECT_EQ(cst.depth(cst_node), fcst.depth(node));
            EXPECT_EQ(cst.lb(cst_ls), fcst.lb(sl));
            EXPECT_EQ(cst.rb(cst_ls), fcst.rb(sl));
            EXPECT_EQ(cst.lb(cst_parent), fcst.lb(parent));
            EXPECT_EQ(cst.rb(cst_parent), fcst.rb(parent));

            ++it;
            ++cst_it;
        }
    }
}

std::string fib(int n) {
    std::string a = "a";
    std::string b = "b";

    if(n == 0) return "";
    if(n == 1) return a;

    while(n > 2) {
        a += b;
        std::swap(a, b);
        n--;
    }

    return b;
}

TYPED_TEST(CstFullyBlackboxTest, IteratorTest2) {
    typedef TypeParam cst_type;

    const std::string text = fib(20);

    typename cst_type::construction_cst_type cst;
    construct_im(cst, text.c_str(), 1);

    cst_type fcst(cst, 4, false);

    auto it = fcst.begin();
    auto cst_it = cst.begin();

    while(it != fcst.end() && cst_it != cst.end()) {
        if(!fcst.is_leaf(*it)) {
            auto node = *it;
            auto cst_node = *cst_it;

            EXPECT_EQ(cst.lb(cst_node), fcst.lb(node));
            EXPECT_EQ(cst.rb(cst_node), fcst.rb(node));
            EXPECT_EQ(cst.depth(cst_node), fcst.depth(node));
        }
        ++it;
        ++cst_it;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
