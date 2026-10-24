#pragma once

#include "ReaderBase.hpp"

#include <memory>

namespace flasher {

class D2ReaderChecksum : public ReaderBase {
public:
    D2ReaderChecksum(std::shared_ptr<common::ICanChannelProvider> channelProvider, common::CarPlatform carPlatform, uint32_t ecuId,
             ReadRanges ranges);

protected:
    void startImpl(const std::vector<std::unique_ptr<common::ICanChannel>>& channels) override;

};

} // namespace flasher
