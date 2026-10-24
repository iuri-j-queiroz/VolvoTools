#pragma once

#include "FlasherCallback.hpp"
#include "FlasherBase.hpp"
#include "FlasherConfigs.hpp"

#include <common/CarPlatform.hpp>
#include <common/VBF.hpp>

namespace flasher {

    class UDSFlasher: public FlasherBase {
    public:
        UDSFlasher(std::shared_ptr<common::ICanChannelProvider> channelProvider, common::CarPlatform carPlatform, uint32_t ecuId,
                   UDSFlasherConfig&& config);
        ~UDSFlasher();

    private:
        void startImpl(const std::vector<std::unique_ptr<common::ICanChannel>>& channels) override;

        const UDSFlasherConfig _config;
    };

} // namespace flasher
