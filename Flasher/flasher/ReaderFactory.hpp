#pragma once

#include "ReaderParametersProviderBase.hpp"

#include <common/ICanChannelProvider.hpp>

#include <memory>

namespace flasher {

class ReaderBase;

class ReaderFactory {
public:
    static std::unique_ptr<ReaderBase> create(
        std::shared_ptr<common::ICanChannelProvider> channelProvider,
        const ReaderParametersProviderBase& params);
};

} // namespace flasher
