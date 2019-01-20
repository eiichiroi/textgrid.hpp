// Copyright (c) 2019, Eiichiro Iwata. All rights reserved.
#include <gtest/gtest.h>

#include "../textgrid.hpp"

namespace textgrid {

TEST(TextGridTest, Construct) {
  TextGrid text_grid;
  EXPECT_EQ(0, text_grid.GetMinTime());
  EXPECT_EQ(0, text_grid.GetMaxTime());
  EXPECT_EQ(0, text_grid.GetNumberOfTiers());
}

TEST(TextGridTest, AppendIntervalTier) {
  TextGrid text_grid(100, 200);
  EXPECT_EQ(100, text_grid.GetMinTime());
  EXPECT_EQ(200, text_grid.GetMaxTime());
  EXPECT_EQ(0, text_grid.GetNumberOfTiers());

  text_grid.AppendTier<IntervalTier>("New tier name");
  EXPECT_EQ(1, text_grid.GetNumberOfTiers());
  {
    const std::shared_ptr<Tier> tier = text_grid.GetTier(0);
    EXPECT_EQ("New tier name", tier->GetName());
    EXPECT_EQ(100, tier->GetMinTime());
    EXPECT_EQ(200, tier->GetMaxTime());
    EXPECT_EQ(1, tier->GetNumberOfAnnotations());
  }
  {  // Get as std::shared_ptr<IntervalTier> type.
    if (auto tier = text_grid.GetTierAs<IntervalTier>(0)) {
      EXPECT_EQ("New tier name", tier->GetName());
      EXPECT_EQ(100, tier->GetMinTime());
      EXPECT_EQ(200, tier->GetMaxTime());
      ASSERT_EQ(1, tier->GetNumberOfIntervals());
      const Interval& interval = tier->GetInterval(0);
      EXPECT_EQ(100, interval.min_time);
      EXPECT_EQ(200, interval.max_time);
      EXPECT_EQ("", interval.text);
    } else {
      EXPECT_TRUE(false);  // Test fail if reaching here.
    }
  }
  {  // Get as std::shared_ptr<PointTier> type, but it return nullptr.
    if (auto tier = text_grid.GetTierAs<PointTier>(0)) {
      EXPECT_TRUE(false);  // Test fail if reaching here.
    } else {
      EXPECT_TRUE(true);
    }
  }
}

}  // namespace textgrid
