// Copyright (c) 2020, Eiichiro Iwata. All rights reserved.
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "../textgrid.hpp"

struct Utterance {
  int32_t id;
  double start_time;
  double end_time;
  int32_t channel_id;
  std::string transcription;
};

void ReassignIds(std::vector<Utterance>& utterances) {
  for (size_t i = 0; i < utterances.size(); ++i) {
    utterances[i].id = i + 1;
  }
}

std::vector<Utterance> ConvertToUtterances(const textgrid::TextGrid& text_grid) {
  std::vector<Utterance> utterances;
  for (size_t tier_index = 0; tier_index < text_grid.GetNumberOfTiers(); ++tier_index) {
    auto tier = text_grid.GetTierAs<textgrid::IntervalTier>(tier_index);
    if (!tier) {
      continue;
    }
    for (auto&& interval : tier->GetAllIntervals()) {
      if (interval.text.empty() || interval.text == "#") {
        continue;
      }
      const int32_t id = static_cast<int32_t>(utterances.size()) + 1;
      const int32_t channel_id = tier_index + 1;
      Utterance utterance {
        id,
        interval.min_time,
        interval.max_time,
        channel_id,
        interval.text,
      };
      utterances.push_back(std::move(utterance));
    }
  }

  std::sort(utterances.begin(), utterances.end(),
            [](const Utterance& lhs, const Utterance& rhs) {
              return lhs.start_time < rhs.start_time;
            });

  ReassignIds(utterances);

  return utterances;
}

int main() {
  const std::string kDelimiter = ",";
  const char kByteOrderMark[] = {
    static_cast<char>(0xEF),
    static_cast<char>(0xBB),
    static_cast<char>(0xBF),
  };

  try {
    textgrid::TextGrid text_grid;
    std::cin >> text_grid;

    std::vector<Utterance> utterances = ConvertToUtterances(text_grid);

    std::cout << kByteOrderMark;
    for (auto&& utterance : utterances) {
      std::cout << utterance.id << kDelimiter
                << std::fixed << std::setprecision(3)
                << utterance.start_time << kDelimiter
                << utterance.end_time << kDelimiter
                << utterance.channel_id << kDelimiter
                << utterance.transcription << std::endl;
    }
  } catch (const textgrid::Exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  return 0;
}
