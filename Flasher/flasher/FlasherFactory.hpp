#pragma once

#include "FlasherParametersProviderBase.hpp"

#include <common/ICanChannelProvider.hpp>

#include <memory>

namespace flasher {

class FlasherBase;

class FlasherFactory {
public:
    static std::unique_ptr<FlasherBase> create(
        std::shared_ptr<common::ICanChannelProvider> channelProvider,
        const FlasherParametersProviderBase& params);

    static bool isD2Platform(common::CarPlatform p);
    static bool isUDSPlatform(common::CarPlatform p);
};

} // namespace flasher
