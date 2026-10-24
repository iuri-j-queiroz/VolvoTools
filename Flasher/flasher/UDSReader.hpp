#pragma once

#include "ReaderBase.hpp"

#include <common/CanIdProvider.hpp>

namespace flasher {

class UDSReader : public ReaderBase {
public:
    UDSReader(std::shared_ptr<common::ICanChannelProvider> channelProvider, common::CarPlatform carPlatform, uint32_t ecuId,
              ReadRanges ranges, uint64_t pin);

protected:
    void startImpl(const std::vector<std::unique_ptr<common::ICanChannel>>& channels) override;

private:
    uint64_t _pin;
    std::unique_ptr<common::CanIdProvider> _canIdProvider;
};

} // namespace flasher
