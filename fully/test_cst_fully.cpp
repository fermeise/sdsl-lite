
#include <sdsl/suffix_trees.hpp>
#include <gtest/gtest.h>

using namespace sdsl;

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

TEST(CstFullyWhiteboxTest, LsaLeafTest) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, true> fcst;

    construct_im(fcst, "abbbab", 1);

    // sampled tree should look like (0 1 2 (3) (4) 5 6)
    EXPECT_EQ(0, fcst.lsa_leaf(0));
    EXPECT_EQ(0, fcst.lsa_leaf(1));
    EXPECT_EQ(0, fcst.lsa_leaf(2));
    EXPECT_EQ(1, fcst.lsa_leaf(3));
    EXPECT_EQ(3, fcst.lsa_leaf(4));
    EXPECT_EQ(0, fcst.lsa_leaf(5));
    EXPECT_EQ(0, fcst.lsa_leaf(6));
}

TEST(CstFullyWhiteboxTest, LsaLeafTest2) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, true> fcst;

    construct_im(fcst, "Mississippi", 1);

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

TEST(CstFullyWhiteboxTest, LsaLeafTest3) {
    cst_fully<csa_wt<>, 2, bp_support_sada<>, sd_vector<>, dac_vector<>, true> fcst;

    construct_im(fcst, "Mississippi", 1);

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

TEST(CstFullyWhiteboxTest, LsaLeafTest4) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, false> fcst;

    construct_im(fcst, "Mississippi", 1);

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

TEST(CstFullyWhiteboxTest, SampledNodeTest) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, true> fcst;
    typedef typename decltype(fcst)::node_type node_type;

    construct_im(fcst, "abbbab", 1);

    EXPECT_EQ(node_type(0, 6), fcst.sampled_node(0));
    EXPECT_EQ(node_type(3, 3), fcst.sampled_node(1));
    EXPECT_EQ(node_type(4, 4), fcst.sampled_node(3));
}

TEST(CstFullyWhiteboxTest, SampledNodeTest2) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, true> fcst;
    typedef typename decltype(fcst)::node_type node_type;

    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ(node_type(0, 11), fcst.sampled_node(0));
    EXPECT_EQ(node_type(2, 2), fcst.sampled_node(1));
    EXPECT_EQ(node_type(4, 4), fcst.sampled_node(3));
    EXPECT_EQ(node_type(7, 7), fcst.sampled_node(5));
    EXPECT_EQ(node_type(8, 9), fcst.sampled_node(7));
    EXPECT_EQ(node_type(8, 8), fcst.sampled_node(8));
}

TEST(CstFullyWhiteboxTest, SampledNodeTest3) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, false> fcst;
    typedef typename decltype(fcst)::node_type node_type;

    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ(node_type(0, 11), fcst.sampled_node(0));
    EXPECT_EQ(node_type(8, 9), fcst.sampled_node(1));
}

TEST(CstFullyWhiteboxTest, SampledDepthTest) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, true> fcst;

    construct_im(fcst, "abbbab", 1);

    EXPECT_EQ(0, fcst.depth(0));
    EXPECT_EQ(2, fcst.depth(1));
    EXPECT_EQ(4, fcst.depth(3));
}

TEST(CstFullyWhiteboxTest, SampledDepthTest2) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, true> fcst;

    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ(0, fcst.depth(0));
    EXPECT_EQ(2, fcst.depth(1));
    EXPECT_EQ(8, fcst.depth(3));
    EXPECT_EQ(4, fcst.depth(5));
    EXPECT_EQ(2, fcst.depth(7));
    EXPECT_EQ(6, fcst.depth(8));
}

TEST(CstFullyWhiteboxTest, SampledDepthTest3) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, false> fcst;

    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ(0, fcst.depth(0));
    EXPECT_EQ(2, fcst.depth(1));
}

TEST(CstFullyWhiteboxTest, SampledLCATest) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, true> fcst;

    construct_im(fcst, "abbbab", 1);

    EXPECT_EQ(0, fcst.sampled_lca(0, 0));
    EXPECT_EQ(1, fcst.sampled_lca(1, 1));
    EXPECT_EQ(3, fcst.sampled_lca(3, 3));
    EXPECT_EQ(0, fcst.sampled_lca(0, 1));
    EXPECT_EQ(0, fcst.sampled_lca(0, 3));
    EXPECT_EQ(0, fcst.sampled_lca(1, 3));
}

TEST(CstFullyWhiteboxTest, SampledLCATest2) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, true> fcst;

    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ(0, fcst.sampled_lca(0, 0));
    EXPECT_EQ(1, fcst.sampled_lca(1, 1));
    EXPECT_EQ(3, fcst.sampled_lca(3, 3));
    EXPECT_EQ(5, fcst.sampled_lca(5, 5));
    EXPECT_EQ(7, fcst.sampled_lca(7, 7));
    EXPECT_EQ(8, fcst.sampled_lca(8, 8));
    EXPECT_EQ(0, fcst.sampled_lca(0, 3));
    EXPECT_EQ(0, fcst.sampled_lca(0, 7));
    EXPECT_EQ(0, fcst.sampled_lca(0, 8));
    EXPECT_EQ(0, fcst.sampled_lca(5, 7));
    EXPECT_EQ(0, fcst.sampled_lca(5, 8));
    EXPECT_EQ(7, fcst.sampled_lca(7, 8));
    EXPECT_EQ(7, fcst.sampled_lca(8, 7));
}

TEST(CstFullyWhiteboxTest, SampledLCATest3) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, true> fcst;

    construct_im(fcst, " as far as I c as j as fai", 1);

    // sampled tree should look like (01(2)(3)4567(8)(9)(10)11(12)131415(16)(17)((18)19)(20)2122((23)((24)25)26))
    EXPECT_EQ(fcst.sampled_lca(fcst.lsa_leaf(23), fcst.lsa_leaf(24)),
              fcst.sampled_lca(fcst.sampled_lca(fcst.lsa_leaf(23), fcst.lsa_leaf(24)), fcst.lsa_leaf(24)));
}

TEST(CstFullyWhiteboxTest, SampledLCATest4) {
    cst_fully<csa_wt<>, 4, bp_support_sada<>, sd_vector<>, dac_vector<>, false> fcst;

    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ(0, fcst.sampled_lca(0, 0));
    EXPECT_EQ(0, fcst.sampled_lca(0, 1));
    EXPECT_EQ(0, fcst.sampled_lca(1, 0));
    EXPECT_EQ(1, fcst.sampled_lca(1, 1));
}

TEST(CstFullySdsWhiteboxTest, LevelTest) {
    EXPECT_EQ(1, cst_fully_sds<>::level(0));
    EXPECT_EQ(1, cst_fully_sds<>::level(1));
    EXPECT_EQ(2, cst_fully_sds<>::level(2));
    EXPECT_EQ(2, cst_fully_sds<>::level(3));
    EXPECT_EQ(3, cst_fully_sds<>::level(4));
    EXPECT_EQ(3, cst_fully_sds<>::level(5));
    EXPECT_EQ(3, cst_fully_sds<>::level(6));
    EXPECT_EQ(3, cst_fully_sds<>::level(7));
    EXPECT_EQ(4, cst_fully_sds<>::level(8));
}

TEST(CstFullyBlindWhiteboxTest, SampledTreeTest) {
    cst_fully_blind<csa_wt<>, 4> cst;

    construct_im(cst, road_not_taken, 1);

    std::stringstream ss;
    ss << cst.bp_bin;
    EXPECT_EQ("1111111111110100011011101000001101000111101110100000110100001111010011"
              "0100011010000110111101000000111010011010000111011010001101000011111111"
              "1010001101000111010011101000001001111010001111110111101001101000111010"
              "0100001110100100011010001101000000111111111101001001001110100110100001"
              "1101001000111101001101000110100001110100111010000011101000011111110100"
              "1001101000110100001111101001001111101000100100000001111010011110100011"
              "1101001110100110100000001110111101001000011111010011101001101000011010"
              "0010000001111101111010011101001111010001101000000011111101111011010001"
              "1110100010001101000001101000011111101001001101000111101001111101001000"
              "10001101000000011101111010011010000011011110100000001000", ss.str());
}

template<typename t_cst>
class CstFullyBlackboxTest : public ::testing::Test {
};

typedef ::testing::Types<
    cst_fully<csa_wt<>, 2>,
    cst_fully<csa_wt<>, 4>,
    cst_fully<csa_wt<>, 6>,
    cst_fully<csa_wt<>, 8>,
    cst_fully<csa_wt<>, 10>,
    cst_fully_sds<csa_wt<>, 4>,
    cst_sada_blind<csa_wt<>, lcp_wt<> >,
    cst_fully_blind<csa_wt<>, 4>
    > BlackboxTestTypes;
typedef cst_sada<> ref_cst_type;

TYPED_TEST_CASE(CstFullyBlackboxTest, BlackboxTestTypes);

TYPED_TEST(CstFullyBlackboxTest, DepthTest) {
    typedef TypeParam cst_type;

    cst_type fcst;
    construct_im(fcst, "abbbab", 1);

    EXPECT_EQ(1, fcst.depth(fcst.node(0, 0)));
    EXPECT_EQ(0, fcst.depth(fcst.node(0, 3)));
    EXPECT_EQ(0, fcst.depth(fcst.node(0, 6)));
    EXPECT_EQ(3, fcst.depth(fcst.node(1, 1)));
    EXPECT_EQ(2, fcst.depth(fcst.node(1, 2)));
    EXPECT_EQ(0, fcst.depth(fcst.node(1, 3)));
    EXPECT_EQ(0, fcst.depth(fcst.node(1, 4)));
    EXPECT_EQ(0, fcst.depth(fcst.node(1, 6)));
    EXPECT_EQ(7, fcst.depth(fcst.node(2, 2)));
    EXPECT_EQ(0, fcst.depth(fcst.node(2, 3)));
    EXPECT_EQ(2, fcst.depth(fcst.node(3, 3)));
    EXPECT_EQ(1, fcst.depth(fcst.node(3, 4)));
    EXPECT_EQ(1, fcst.depth(fcst.node(3, 5)));
    EXPECT_EQ(1, fcst.depth(fcst.node(3, 6)));
    EXPECT_EQ(5, fcst.depth(fcst.node(5, 5)));
    EXPECT_EQ(2, fcst.depth(fcst.node(5, 6)));
}

TYPED_TEST(CstFullyBlackboxTest, LCATest) {
    typedef TypeParam cst_type;

    cst_type fcst;
    construct_im(fcst, "abbbab", 1);

    auto lca = [&](size_t l, size_t r) { return fcst.lca(fcst.node(l, l), fcst.node(r, r)); };

    EXPECT_EQ(fcst.node(0, 0), lca(0, 0));
    EXPECT_EQ(fcst.node(0, 6), lca(0, 3));
    EXPECT_EQ(fcst.node(0, 6), lca(0, 6));
    EXPECT_EQ(fcst.node(1, 1), lca(1, 1));
    EXPECT_EQ(fcst.node(1, 2), lca(1, 2));
    EXPECT_EQ(fcst.node(0, 6), lca(1, 3));
    EXPECT_EQ(fcst.node(0, 6), lca(1, 4));
    EXPECT_EQ(fcst.node(0, 6), lca(1, 6));
    EXPECT_EQ(fcst.node(2, 2), lca(2, 2));
    EXPECT_EQ(fcst.node(0, 6), lca(2, 3));
    EXPECT_EQ(fcst.node(3, 3), lca(3, 3));
    EXPECT_EQ(fcst.node(3, 6), lca(3, 4));
    EXPECT_EQ(fcst.node(3, 6), lca(3, 5));
    EXPECT_EQ(fcst.node(3, 6), lca(3, 6));
    EXPECT_EQ(fcst.node(5, 5), lca(5, 5));
    EXPECT_EQ(fcst.node(5, 6), lca(5, 6));
}

TYPED_TEST(CstFullyBlackboxTest, LCATest2) {
    typedef TypeParam cst_type;

    cst_type fcst;
    construct_im(fcst, "Mississippi", 1);

    auto lca = [&](size_t l, size_t r) { return fcst.lca(fcst.node(l, l), fcst.node(r, r)); };

    EXPECT_EQ(fcst.node(0,  0), lca(0,  0));
    EXPECT_EQ(fcst.node(0, 11), lca(0,  2));
    EXPECT_EQ(fcst.node(0, 11), lca(0,  8));
    EXPECT_EQ(fcst.node(0, 11), lca(0,  8));
    EXPECT_EQ(fcst.node(2,  2), lca(2,  2));
    EXPECT_EQ(fcst.node(2,  5), lca(2,  3));
    EXPECT_EQ(fcst.node(2,  5), lca(2,  4));
    EXPECT_EQ(fcst.node(2,  5), lca(2,  5));
    EXPECT_EQ(fcst.node(0, 11), lca(2,  6));
    EXPECT_EQ(fcst.node(0, 11), lca(2,  8));
    EXPECT_EQ(fcst.node(0, 11), lca(2, 10));
    EXPECT_EQ(fcst.node(2,  5), lca(3,  4));
    EXPECT_EQ(fcst.node(2,  5), lca(3,  5));
    EXPECT_EQ(fcst.node(0, 11), lca(3,  6));
    EXPECT_EQ(fcst.node(0, 11), lca(3,  9));
    EXPECT_EQ(fcst.node(4,  5), lca(4,  5));
    EXPECT_EQ(fcst.node(0, 11), lca(4,  6));
    EXPECT_EQ(fcst.node(8,  8), lca(8,  8));
    EXPECT_EQ(fcst.node(8,  9), lca(8,  9));
    EXPECT_EQ(fcst.node(8, 11), lca(8, 10));
    EXPECT_EQ(fcst.node(8, 11), lca(8, 11));
    EXPECT_EQ(fcst.node(9,  9), lca(9,  9));
    EXPECT_EQ(fcst.node(8, 11), lca(9, 10));
    EXPECT_EQ(fcst.node(8, 11), lca(9, 11));
}

TYPED_TEST(CstFullyBlackboxTest, SuffixLinkTest) {
    typedef TypeParam cst_type;

    cst_type fcst;
    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ(fcst.node( 5,  5), fcst.sl(fcst.node( 1,  1)));
    EXPECT_EQ(fcst.node(11, 11), fcst.sl(fcst.node( 5,  5)));
    EXPECT_EQ(fcst.node( 9,  9), fcst.sl(fcst.node(11, 11)));
    EXPECT_EQ(fcst.node( 4,  4), fcst.sl(fcst.node( 9,  9)));
    EXPECT_EQ(fcst.node(10, 10), fcst.sl(fcst.node( 4,  4)));
    EXPECT_EQ(fcst.node( 8,  8), fcst.sl(fcst.node(10, 10)));
    EXPECT_EQ(fcst.node( 3,  3), fcst.sl(fcst.node( 8,  8)));
    EXPECT_EQ(fcst.node( 7,  7), fcst.sl(fcst.node( 3,  3)));
    EXPECT_EQ(fcst.node( 6,  6), fcst.sl(fcst.node( 7,  7)));
    EXPECT_EQ(fcst.node( 2,  2), fcst.sl(fcst.node( 6,  6)));
    EXPECT_EQ(fcst.node( 0,  0), fcst.sl(fcst.node( 2,  2)));
    EXPECT_EQ(fcst.node( 1,  1), fcst.sl(fcst.node( 0,  0)));

    EXPECT_EQ(fcst.node( 0, 11), fcst.sl(fcst.node( 0, 11)));
    EXPECT_EQ(fcst.node( 0, 11), fcst.sl(fcst.node( 2,  5)));
    EXPECT_EQ(fcst.node(10, 11), fcst.sl(fcst.node( 4,  5)));
    EXPECT_EQ(fcst.node( 0, 11), fcst.sl(fcst.node( 6,  7)));
    EXPECT_EQ(fcst.node( 0, 11), fcst.sl(fcst.node( 8, 11)));
    EXPECT_EQ(fcst.node( 2,  5), fcst.sl(fcst.node( 8,  9)));
    EXPECT_EQ(fcst.node( 8,  9), fcst.sl(fcst.node(10, 11)));
}

TYPED_TEST(CstFullyBlackboxTest, ParentTest) {
    typedef TypeParam cst_type;

    cst_type fcst;
    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ(fcst.node( 0, 11), fcst.parent(fcst.node( 0,  0)));
    EXPECT_EQ(fcst.node( 0, 11), fcst.parent(fcst.node( 1,  1)));
    EXPECT_EQ(fcst.node( 2,  5), fcst.parent(fcst.node( 2,  2)));
    EXPECT_EQ(fcst.node( 2,  5), fcst.parent(fcst.node( 3,  3)));
    EXPECT_EQ(fcst.node( 4,  5), fcst.parent(fcst.node( 4,  4)));
    EXPECT_EQ(fcst.node( 4,  5), fcst.parent(fcst.node( 5,  5)));
    EXPECT_EQ(fcst.node( 6,  7), fcst.parent(fcst.node( 6,  6)));
    EXPECT_EQ(fcst.node( 6,  7), fcst.parent(fcst.node( 7,  7)));
    EXPECT_EQ(fcst.node( 8,  9), fcst.parent(fcst.node( 8,  8)));
    EXPECT_EQ(fcst.node( 8,  9), fcst.parent(fcst.node( 9,  9)));
    EXPECT_EQ(fcst.node(10, 11), fcst.parent(fcst.node(10, 10)));
    EXPECT_EQ(fcst.node(10, 11), fcst.parent(fcst.node(11, 11)));

    EXPECT_EQ(fcst.node( 0, 11), fcst.parent(fcst.node( 0, 11)));
    EXPECT_EQ(fcst.node( 0, 11), fcst.parent(fcst.node( 2,  5)));
    EXPECT_EQ(fcst.node( 2,  5), fcst.parent(fcst.node( 4,  5)));
    EXPECT_EQ(fcst.node( 0, 11), fcst.parent(fcst.node( 6,  7)));
    EXPECT_EQ(fcst.node( 8, 11), fcst.parent(fcst.node(10, 11)));
    EXPECT_EQ(fcst.node( 8, 11), fcst.parent(fcst.node( 8,  9)));
    EXPECT_EQ(fcst.node( 0, 11), fcst.parent(fcst.node( 8, 11)));
}

TYPED_TEST(CstFullyBlackboxTest, ChildTest) {
    typedef TypeParam cst_type;

    cst_type fcst;
    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ(fcst.node( 0,  0), fcst.child(fcst.node( 0, 11), '\0'));
    EXPECT_EQ(fcst.node( 1,  1), fcst.child(fcst.node( 0, 11), 'M'));
    EXPECT_EQ(fcst.node( 2,  5), fcst.child(fcst.node( 0, 11), 'i'));
    EXPECT_EQ(fcst.node( 6,  7), fcst.child(fcst.node( 0, 11), 'p'));
    EXPECT_EQ(fcst.node( 8, 11), fcst.child(fcst.node( 0, 11), 's'));
    EXPECT_EQ(fcst.node( 2,  2), fcst.child(fcst.node( 2,  5), '\0'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 2,  5), 'M'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 2,  5), 'i'));
    EXPECT_EQ(fcst.node( 3,  3), fcst.child(fcst.node( 2,  5), 'p'));
    EXPECT_EQ(fcst.node( 4,  5), fcst.child(fcst.node( 2,  5), 's'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 6,  7), 'M'));
    EXPECT_EQ(fcst.node( 6,  6), fcst.child(fcst.node( 6,  7), 'i'));
    EXPECT_EQ(fcst.node( 7,  7), fcst.child(fcst.node( 6,  7), 'p'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 6,  7), 's'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 8, 11), 'M'));
    EXPECT_EQ(fcst.node( 8,  9), fcst.child(fcst.node( 8, 11), 'i'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 8, 11), 'p'));
    EXPECT_EQ(fcst.node(10, 11), fcst.child(fcst.node( 8, 11), 's'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 8,  9), 'M'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 8,  9), 'i'));
    EXPECT_EQ(fcst.node( 8,  8), fcst.child(fcst.node( 8,  9), 'p'));
    EXPECT_EQ(fcst.node( 9,  9), fcst.child(fcst.node( 8,  9), 's'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node(10, 11), 'M'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node(10, 11), 'i'));
    EXPECT_EQ(fcst.node(10, 10), fcst.child(fcst.node(10, 11), 'p'));
    EXPECT_EQ(fcst.node(11, 11), fcst.child(fcst.node(10, 11), 's'));

    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 2,  5), 'a'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 6,  7), 'q'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 8, 11), 'w'));
    EXPECT_EQ(fcst.root(),       fcst.child(fcst.node( 0, 11), 'z'));
}

TYPED_TEST(CstFullyBlackboxTest, SelectChildTest) {
    typedef TypeParam cst_type;

    cst_type fcst;
    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ(fcst.node( 0,  0), fcst.select_child(fcst.node( 0, 11), 1));
    EXPECT_EQ(fcst.node( 1,  1), fcst.select_child(fcst.node( 0, 11), 2));
    EXPECT_EQ(fcst.node( 2,  5), fcst.select_child(fcst.node( 0, 11), 3));
    EXPECT_EQ(fcst.node( 6,  7), fcst.select_child(fcst.node( 0, 11), 4));
    EXPECT_EQ(fcst.node( 8, 11), fcst.select_child(fcst.node( 0, 11), 5));
    EXPECT_EQ(fcst.root(),       fcst.select_child(fcst.node( 0, 11), 6));
    EXPECT_EQ(fcst.node( 2,  2), fcst.select_child(fcst.node( 2,  5), 1));
    EXPECT_EQ(fcst.node( 3,  3), fcst.select_child(fcst.node( 2,  5), 2));
    EXPECT_EQ(fcst.node( 4,  5), fcst.select_child(fcst.node( 2,  5), 3));
    EXPECT_EQ(fcst.root(),       fcst.select_child(fcst.node( 2,  5), 4));
    EXPECT_EQ(fcst.node( 4,  4), fcst.select_child(fcst.node( 4,  5), 1));
    EXPECT_EQ(fcst.node( 5,  5), fcst.select_child(fcst.node( 4,  5), 2));
    EXPECT_EQ(fcst.root(),       fcst.select_child(fcst.node( 4,  5), 3));
    EXPECT_EQ(fcst.node( 6,  6), fcst.select_child(fcst.node( 6,  7), 1));
    EXPECT_EQ(fcst.node( 7,  7), fcst.select_child(fcst.node( 6,  7), 2));
    EXPECT_EQ(fcst.root(),       fcst.select_child(fcst.node( 6,  7), 3));
    EXPECT_EQ(fcst.node( 8,  9), fcst.select_child(fcst.node( 8, 11), 1));
    EXPECT_EQ(fcst.node(10, 11), fcst.select_child(fcst.node( 8, 11), 2));
    EXPECT_EQ(fcst.root(),       fcst.select_child(fcst.node( 8, 11), 3));
    EXPECT_EQ(fcst.node( 8,  8), fcst.select_child(fcst.node( 8,  9), 1));
    EXPECT_EQ(fcst.node( 9,  9), fcst.select_child(fcst.node( 8,  9), 2));
    EXPECT_EQ(fcst.root(),       fcst.select_child(fcst.node( 8,  9), 3));
    EXPECT_EQ(fcst.node(10, 10), fcst.select_child(fcst.node(10, 11), 1));
    EXPECT_EQ(fcst.node(11, 11), fcst.select_child(fcst.node(10, 11), 2));
    EXPECT_EQ(fcst.root(),       fcst.select_child(fcst.node(10, 11), 3));
}

TYPED_TEST(CstFullyBlackboxTest, SiblingTest) {
    typedef TypeParam cst_type;

    cst_type fcst;
    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ(fcst.node( 1,  1), fcst.sibling(fcst.node( 0,  0)));
    EXPECT_EQ(fcst.node( 2,  5), fcst.sibling(fcst.node( 1,  1)));
    EXPECT_EQ(fcst.node( 6,  7), fcst.sibling(fcst.node( 2,  5)));
    EXPECT_EQ(fcst.node( 3,  3), fcst.sibling(fcst.node( 2,  2)));
    EXPECT_EQ(fcst.node( 4,  5), fcst.sibling(fcst.node( 3,  3)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(fcst.node( 4,  5)));
    EXPECT_EQ(fcst.node( 5,  5), fcst.sibling(fcst.node( 4,  4)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(fcst.node( 5,  5)));
    EXPECT_EQ(fcst.node( 8, 11), fcst.sibling(fcst.node( 6,  7)));
    EXPECT_EQ(fcst.node( 7,  7), fcst.sibling(fcst.node( 6,  6)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(fcst.node( 7,  7)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(fcst.node( 8, 11)));
    EXPECT_EQ(fcst.node(10, 11), fcst.sibling(fcst.node( 8,  9)));
    EXPECT_EQ(fcst.node( 9,  9), fcst.sibling(fcst.node( 8,  8)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(fcst.node( 9,  9)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(fcst.node(10, 11)));
    EXPECT_EQ(fcst.node(11, 11), fcst.sibling(fcst.node(10, 10)));
    EXPECT_EQ(fcst.root(),       fcst.sibling(fcst.node(11, 11)));
}

TYPED_TEST(CstFullyBlackboxTest, EdgeTest) {
    typedef TypeParam cst_type;

    cst_type fcst;
    construct_im(fcst, "Mississippi", 1);

    EXPECT_EQ('i', fcst.edge(fcst.node( 2,  5), 1));
    EXPECT_EQ('p', fcst.edge(fcst.node( 6,  7), 1));
    EXPECT_EQ('s', fcst.edge(fcst.node( 8, 11), 1));
    EXPECT_EQ('s', fcst.edge(fcst.node( 8,  9), 1));
    EXPECT_EQ('i', fcst.edge(fcst.node( 8,  9), 2));
    EXPECT_EQ('s', fcst.edge(fcst.node(10, 11), 1));
    EXPECT_EQ('s', fcst.edge(fcst.node(10, 11), 2));
    EXPECT_EQ('i', fcst.edge(fcst.node(10, 11), 3));

    EXPECT_EQ('\0',fcst.edge(fcst.node( 0,  0), 1));
    EXPECT_EQ('M', fcst.edge(fcst.node( 1,  1), 1));
    EXPECT_EQ('i', fcst.edge(fcst.node( 1,  1), 2));
    EXPECT_EQ('s', fcst.edge(fcst.node( 1,  1), 3));
    EXPECT_EQ('p', fcst.edge(fcst.node( 1,  1), 10));
    EXPECT_EQ('i', fcst.edge(fcst.node( 1,  1), 11));
    EXPECT_EQ('\0',fcst.edge(fcst.node( 1,  1), 12));
}

TYPED_TEST(CstFullyBlackboxTest, IteratorTest) {
    typedef TypeParam cst_type;

    ref_cst_type cst;
    construct_im(cst, road_not_taken, 1);

    cst_type fcst;
    construct_im(fcst, road_not_taken, 1);

    auto it = fcst.begin();
    auto cst_it = cst.begin();

    while(it != fcst.end() && cst_it != cst.end()) {
        auto node = *it;
        auto sl = fcst.sl(node);
        auto parent = fcst.parent(node);
        auto cst_node = *cst_it;
        auto cst_ls = cst.sl(cst_node);
        auto cst_parent = cst.parent(cst_node);

        ASSERT_EQ(cst.lb(cst_node), fcst.lb(node));
        ASSERT_EQ(cst.rb(cst_node), fcst.rb(node));
        ASSERT_EQ(cst.depth(cst_node), fcst.depth(node));
        ASSERT_EQ(cst.lb(cst_ls), fcst.lb(sl));
        ASSERT_EQ(cst.rb(cst_ls), fcst.rb(sl));
        ASSERT_EQ(cst.lb(cst_parent), fcst.lb(parent));
        ASSERT_EQ(cst.rb(cst_parent), fcst.rb(parent));

        ++it;
        ++cst_it;
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

    ref_cst_type cst;
    construct_im(cst, text.c_str(), 1);

    cst_type fcst;
    construct_im(fcst, text.c_str(), 1);

    auto it = fcst.begin();
    auto cst_it = cst.begin();

    while(it != fcst.end() && cst_it != cst.end()) {
        if(!fcst.is_leaf(*it)) {
            auto node = *it;
            auto cst_node = *cst_it;

            ASSERT_EQ(cst.lb(cst_node), fcst.lb(node));
            ASSERT_EQ(cst.rb(cst_node), fcst.rb(node));
            ASSERT_EQ(cst.depth(cst_node), fcst.depth(node));
        }
        ++it;
        ++cst_it;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
