#pragma once

#include "ReaderBase.hpp"

namespace flasher {

class D2ReaderTF80 : public ReaderBase {
public:
    D2ReaderTF80(std::shared_ptr<common::ICanChannelProvider> channelProvider, common::CarPlatform carPlatform, uint32_t ecuId,
                 ReadRanges ranges);

protected:
    void startImpl(const std::vector<std::unique_ptr<common::ICanChannel>>& channels) override;
};

} // namespace flasher
