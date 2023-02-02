#define MDSPAN_INTERNAL_TEST

#include <experimental/mdspan>
#include <gtest/gtest.h>

namespace stdex = std::experimental;

// Compile time tests

// actual padding stride
// If extents_type::rank() equals zero or one, then padding_stride.
static_assert(stdex::layout_left_padded<0>::mapping<stdex::extents<std::size_t, 0>>::__actual_padding_stride == 0);
static_assert(stdex::layout_left_padded<2>::mapping<stdex::extents<std::size_t, 0>>::__actual_padding_stride == 2);
static_assert(stdex::layout_left_padded<2>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent>>::__actual_padding_stride == 2);
static_assert(stdex::layout_left_padded<2>::mapping<stdex::extents<std::size_t>>::__actual_padding_stride == 2);
static_assert(stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 3>>::__actual_padding_stride == stdex::dynamic_extent);
static_assert(stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 0>>::__actual_padding_stride == stdex::dynamic_extent);
static_assert(stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t>>::__actual_padding_stride == stdex::dynamic_extent);

// Else, if
// - padding_stride does not equal dynamic_extent and
// - extents_type::static_extent(0) does not equal dynamic_extent,
// then the size_t value which is the least multiple of padding_stride that is greater than or equal to extents_type::static_extent(0).
static_assert(stdex::layout_left_padded<2>::mapping<stdex::extents<std::size_t, 3, 7>>::__actual_padding_stride == 4);

// Otherwise, dynamic_extent.
static_assert(stdex::layout_left_padded<2>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent, 7>>::__actual_padding_stride == stdex::dynamic_extent);
static_assert(stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent, 7>>::__actual_padding_stride == stdex::dynamic_extent);
static_assert(stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 3, 7>>::__actual_padding_stride == stdex::dynamic_extent);

// inner extents type
// If extents_type::rank() equals zero or one, then inner-extents-type names the type extents_type.
namespace
{
template<class LayoutLeftPadded, class Extents, class DesiredExtents>
inline constexpr bool has_inner_extents_type = std::is_same_v<
    typename LayoutLeftPadded::template mapping<Extents>::__inner_extents_type,
    DesiredExtents >;
template<class LayoutLeftPadded, class Extents>
inline constexpr bool is_0_or_1_rank_inner_extents_type = has_inner_extents_type<LayoutLeftPadded, Extents, typename LayoutLeftPadded::template mapping<Extents>::extents_type>;
}
static_assert(is_0_or_1_rank_inner_extents_type<stdex::layout_left_padded<0>, stdex::extents<std::size_t, 0>>);
static_assert(is_0_or_1_rank_inner_extents_type<stdex::layout_left_padded<2>, stdex::extents<std::size_t>>);
static_assert(is_0_or_1_rank_inner_extents_type<stdex::layout_left_padded<2>, stdex::extents<std::size_t, 3>>);
static_assert(is_0_or_1_rank_inner_extents_type<stdex::layout_left_padded<2>, stdex::extents<std::size_t, stdex::dynamic_extent>>);
static_assert(is_0_or_1_rank_inner_extents_type<stdex::layout_left_padded<stdex::dynamic_extent>, stdex::extents<std::size_t, 3>>);
static_assert(is_0_or_1_rank_inner_extents_type<stdex::layout_left_padded<stdex::dynamic_extent>, stdex::extents<std::size_t, stdex::dynamic_extent>>);

// Otherwise, inner-extents-type names the type extents<index_type,actual-padding-stride, extents_type::static_extent(P_left)...>.
static_assert(has_inner_extents_type<stdex::layout_left_padded<0>, stdex::extents<std::size_t, 0, 5>, stdex::extents<std::size_t, 0, 5>>);
static_assert(has_inner_extents_type<stdex::layout_left_padded<2>, stdex::extents<std::size_t, 3, 5>, stdex::extents<std::size_t, 4, 5>>);
static_assert(has_inner_extents_type<stdex::layout_left_padded<2>, stdex::extents<std::size_t, stdex::dynamic_extent, 5>, stdex::extents<std::size_t, stdex::dynamic_extent, 5>>);
static_assert(has_inner_extents_type<stdex::layout_left_padded<2>, stdex::extents<std::size_t, 3, stdex::dynamic_extent>, stdex::extents<std::size_t, 4, stdex::dynamic_extent>>);
static_assert(has_inner_extents_type<stdex::layout_left_padded<stdex::dynamic_extent>, stdex::extents<std::size_t, 3, 5>, stdex::extents<std::size_t, stdex::dynamic_extent, 5>>);

// unpadded extent type
namespace
{
template<class LayoutLeftPadded, class Extents, class DesiredExtents>
inline constexpr bool has_unpadded_extents_type = std::is_same_v<
    typename LayoutLeftPadded::template mapping<Extents>::__unpadded_extent_type,
    DesiredExtents >;
}
// If extents_type::rank() equals zero, then unpadded-extent-type names the type extents<index_type>.
static_assert(has_unpadded_extents_type<stdex::layout_left_padded<0>, stdex::extents<std::size_t>, stdex::extents<std::size_t>>);
static_assert(has_unpadded_extents_type<stdex::layout_left_padded<2>, stdex::extents<std::size_t>, stdex::extents<std::size_t>>);
static_assert(has_unpadded_extents_type<stdex::layout_left_padded<stdex::dynamic_extent>, stdex::extents<std::size_t>, stdex::extents<std::size_t>>);

// Otherwise, unpadded-extent-type names the type extents<index_type, extents_type::static_extent(0)>.
static_assert(has_unpadded_extents_type<stdex::layout_left_padded<0>, stdex::extents<std::size_t, 0>, stdex::extents<std::size_t, 0>>);
static_assert(has_unpadded_extents_type<stdex::layout_left_padded<2>, stdex::extents<std::size_t, 3>, stdex::extents<std::size_t, 3>>);
static_assert(has_unpadded_extents_type<stdex::layout_left_padded<2>, stdex::extents<std::size_t, stdex::dynamic_extent>, stdex::extents<std::size_t, stdex::dynamic_extent>>);
static_assert(has_unpadded_extents_type<stdex::layout_left_padded<0>, stdex::extents<std::size_t, 0, 7>, stdex::extents<std::size_t, 0>>);
static_assert(has_unpadded_extents_type<stdex::layout_left_padded<2>, stdex::extents<std::size_t, 3, 5>, stdex::extents<std::size_t, 3>>);
static_assert(has_unpadded_extents_type<stdex::layout_left_padded<2>, stdex::extents<std::size_t, stdex::dynamic_extent, 13>, stdex::extents<std::size_t, stdex::dynamic_extent>>);

namespace
{
template<class LayoutLeftPadded, class InnerExtents, class TestExtents>
void test_inner_mapping_extent(const InnerExtents &inner_extents, const TestExtents &test_extents)
{
  auto mapping = typename LayoutLeftPadded::template mapping<InnerExtents>(inner_extents);
  ASSERT_EQ(mapping.__inner_mapping.extents(), test_extents);

  auto strs = mapping.strides();
  size_t prod = 1;
  for (typename decltype(mapping)::rank_type r = 0; r < TestExtents::rank(); ++r)
  {
    ASSERT_EQ(strs[r], prod);
    prod *= test_extents.extent(r);
  }

  ASSERT_EQ(prod, mapping.required_span_size());
}

template <class LayoutLeftPadded, class InnerExtents, class TestExtents, class Size>
void test_inner_mapping_extent(const InnerExtents &inner_extents, const TestExtents &test_extents, Size padding_value)
{
  auto mapping = typename LayoutLeftPadded::template mapping<InnerExtents>(inner_extents, padding_value);
  ASSERT_EQ(mapping.__inner_mapping.extents(), test_extents);

  auto strs = mapping.strides();
  size_t prod = 1;
  for (typename decltype(mapping)::rank_type r = 0; r < TestExtents::rank(); ++r)
  {
    ASSERT_EQ(strs[r], prod);
    prod *= test_extents.extent(r);
  }

  ASSERT_EQ(prod, mapping.required_span_size());
}

template<class LayoutLeftPadded, class Extents>
void test_0_or_1_rank_inner_mapping_extents(const Extents &extents)
{
  test_inner_mapping_extent<LayoutLeftPadded>(extents, extents);
}

template <class LayoutLeftPadded, class Extents, class Size>
void test_0_or_1_rank_inner_mapping_extents(const Extents &extents, Size padding_value)
{
  test_inner_mapping_extent<LayoutLeftPadded>(extents, extents, padding_value);
}
}

TEST(LayoutLeftTests, construction)
{
  // Constructor only taking an extent
  // Direct-non-list-initializes inner-mapping with:
  // - ext, if extents_type::rank() is zero or one; else,
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<4>>(stdex::extents<std::size_t>{});
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<0>>(stdex::extents<std::size_t, 0>{});
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, 5>{});
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, stdex::dynamic_extent>{7});
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t>{});
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 0>{});
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 5>{});
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, stdex::dynamic_extent>{7});

  // - ext.extent(0), ext.extent(P_left)..., if padding_stride is dynamic_extent
  test_inner_mapping_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 0, 7>{}, stdex::extents<std::size_t, stdex::dynamic_extent, 7>{ 0 });
  test_inner_mapping_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 5, 7>{}, stdex::extents<std::size_t, stdex::dynamic_extent, 7>{ 5 });
  test_inner_mapping_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, stdex::dynamic_extent, 13>{7}, stdex::extents<std::size_t, stdex::dynamic_extent, 13>{7});

  // - S_left, ext.extent(P_left)..., where S_left is the least multiple of padding_stride greater than or equal to ext.extent(0)
  test_inner_mapping_extent<stdex::layout_left_padded<0>>(stdex::extents<std::size_t, 0, 7>{}, stdex::extents<std::size_t, 0, 7>{});
  test_inner_mapping_extent<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, 5, 7>{}, stdex::extents<std::size_t, 8, 7>{});
  test_inner_mapping_extent<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, stdex::dynamic_extent, 13>{7}, stdex::extents<std::size_t, stdex::dynamic_extent, 13>{8});

  // Constructor taking an extent and a dynamic value
  // Direct-non-list-initializes inner-mapping with:
  // - ext, if extents_type::rank() is zero or one; else,
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<4>>(stdex::extents<std::size_t>{}, 4);
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<0>>(stdex::extents<std::size_t, 0>{}, 0);
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, 5>{}, 4);
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, stdex::dynamic_extent>{7}, 4);
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t>{}, 3);
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 0>{}, 3255);
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 5>{}, 1337);
  test_0_or_1_rank_inner_mapping_extents<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, stdex::dynamic_extent>{7}, 6323);

  // - S_left, ext.extent(P_left)..., where S_left is the least multiple of padding_value greater than or equal to ext.extent(0)
  test_inner_mapping_extent<stdex::layout_left_padded<0>>(stdex::extents<std::size_t, 0, 7>{}, stdex::extents<std::size_t, 0, 7>{}, 0);
  test_inner_mapping_extent<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, 5, 7>{}, stdex::extents<std::size_t, 8, 7>{}, 4);
  test_inner_mapping_extent<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, stdex::dynamic_extent, 13>{7}, stdex::extents<std::size_t, stdex::dynamic_extent, 13>{8}, 4);
  test_inner_mapping_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 0, 7>{}, stdex::extents<std::size_t, stdex::dynamic_extent, 7>{0}, 2);
  test_inner_mapping_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 5, 7>{}, stdex::extents<std::size_t, stdex::dynamic_extent, 7>{8}, 4);
  test_inner_mapping_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, stdex::dynamic_extent, 13>{7}, stdex::extents<std::size_t, stdex::dynamic_extent, 13>{8}, 4);
}

namespace
{
  template <class PaddedLayout, class Extents>
  void test_extent(const Extents &input_extents)
  {
    auto mapping = typename PaddedLayout::template mapping<Extents>(input_extents);
    ASSERT_EQ(mapping.extents(), input_extents);
  }

  template <class PaddedLayout, class Extents, class Size>
  void test_extent(const Extents &input_extents, Size padding_value)
  {
    auto mapping = typename PaddedLayout::template mapping<Extents>(input_extents, padding_value);
    ASSERT_EQ(mapping.extents(), input_extents);
  }
}

TEST(LayoutLeftTests, extents)
{
  test_extent<stdex::layout_left_padded<4>>(stdex::extents<std::size_t>{});
  test_extent<stdex::layout_left_padded<0>>(stdex::extents<std::size_t, 0, 7>{});
  test_extent<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, 5, 7>{});
  test_extent<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, stdex::dynamic_extent, 13>{7});
  test_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 0, 7>{});
  test_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 5, 7>{});
  test_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, stdex::dynamic_extent, 13>{7});

  test_extent<stdex::layout_left_padded<0>>(stdex::extents<std::size_t, 0, 7>{}, 0);
  test_extent<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, 5, 7>{}, 4);
  test_extent<stdex::layout_left_padded<4>>(stdex::extents<std::size_t, stdex::dynamic_extent, 13>{7}, 4);
  test_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 0, 7>{}, 1);
  test_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, 5, 7>{}, 3);
  test_extent<stdex::layout_left_padded<stdex::dynamic_extent>>(stdex::extents<std::size_t, stdex::dynamic_extent, 13>{7}, 5);
}

TEST(LayoutLeftTests, properties)
{
  // is_always_exhaustive
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t>>{}.is_always_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3>>{}.is_always_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent>>{}.is_always_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 3>>{}.is_always_exhaustive()));

  ASSERT_FALSE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3, 4>>{}.is_always_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 4, 6>>{}.is_always_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 8, 6>>{}.is_always_exhaustive()));
  ASSERT_FALSE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent, 4>>{}.is_always_exhaustive()));
  ASSERT_FALSE((stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 4, 5>>{}.is_always_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<0>::mapping<stdex::extents<std::size_t, 0, 6>>{}.is_always_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 0, 6>>{}.is_always_exhaustive()));

  // is_exhaustive
  // Sanity check -- if it is always exhaustive it should be exhaustive ^-^
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t>>{}.is_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3>>{}.is_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent>>{}.is_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 3>>{}.is_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 4, 6>>{}.is_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 8, 6>>{}.is_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<0>::mapping<stdex::extents<std::size_t, 0, 6>>{}.is_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 0, 6>>{}.is_exhaustive()));

  // is_exhaustive with dynamic values
  ASSERT_TRUE((stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 4, 6>>{stdex::extents<std::size_t, 4, 6>{}, 4}.is_exhaustive()));
  ASSERT_TRUE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent, 6>>{stdex::extents<std::size_t, stdex::dynamic_extent, 6>{8}}.is_exhaustive()));
  ASSERT_FALSE((stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 6, 6>>{stdex::extents<std::size_t, 6, 6>{}, 4}.is_exhaustive()));
  ASSERT_FALSE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent, 6>>{stdex::extents<std::size_t, stdex::dynamic_extent, 6>{7}}.is_exhaustive()));

  // Equality
  ASSERT_EQ((stdex::layout_left_padded<0>::mapping<stdex::extents<std::size_t, 0>>{}), (stdex::layout_left_padded<0>::mapping<stdex::extents<std::size_t, 0>>{}));
  ASSERT_EQ((stdex::layout_left_padded<0>::mapping<stdex::extents<std::size_t, 0>>{}), (stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 0>>{}));
  ASSERT_EQ((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3>>{}), (stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3>>{}));
  ASSERT_NE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3>>{}), (stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 5>>{}));
  ASSERT_EQ((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent>>{stdex::extents<std::size_t, stdex::dynamic_extent>{5}}),
            (stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent>>{stdex::extents<std::size_t, stdex::dynamic_extent>{5}}));
  ASSERT_NE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent>>{stdex::extents<std::size_t, stdex::dynamic_extent>{3}}),
            (stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent>>{stdex::extents<std::size_t, stdex::dynamic_extent>{5}}));
  ASSERT_EQ((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3, 7>>{}), (stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3, 7>>{}));
  ASSERT_NE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3, 7>>{}), (stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3, 4>>{}));
  ASSERT_NE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3, 7>>{}), (stdex::layout_left_padded<8>::mapping<stdex::extents<std::size_t, 3, 7>>{}));
  ASSERT_EQ((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent, 3>>{stdex::extents<std::size_t, stdex::dynamic_extent, 3>{5}}),
            (stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent, 3>>{stdex::extents<std::size_t, stdex::dynamic_extent, 3>{5}}));
  ASSERT_NE((stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent, 3>>{stdex::extents<std::size_t, stdex::dynamic_extent, 3>{3}}),
            (stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, stdex::dynamic_extent, 3>>{stdex::extents<std::size_t, stdex::dynamic_extent, 3>{5}}));
  ASSERT_EQ((stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 3, 7>>{{}, 4}), (stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3, 7>>{}));
  ASSERT_EQ((stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 3, 7>>{{}, 4}), (stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 3, 7>>{{}, 4}));
  ASSERT_NE((stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 3, 7>>{{}, 4}), (stdex::layout_left_padded<4>::mapping<stdex::extents<std::size_t, 3, 4>>{}));
  ASSERT_NE((stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 3, 7>>{{}, 4}), (stdex::layout_left_padded<stdex::dynamic_extent>::mapping<stdex::extents<std::size_t, 3, 7>>{{}, 8}));
}
