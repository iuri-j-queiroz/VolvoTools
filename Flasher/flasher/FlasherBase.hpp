#pragma once

#include "FlasherState.hpp"
#include "FlasherCallbackHolder.hpp"

#include <common/ICanChannelProvider.hpp>
#include <common/CarPlatform.hpp>
#include <common/VBF.hpp>

#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace common {
class ICanChannel;
} // namespace common

namespace flasher {

class FlasherBase: public FlasherCallbackHolder {
public:
    static size_t getProgressFromVBF(const common::VBF& vbf);

    FlasherBase(std::shared_ptr<common::ICanChannelProvider> channelProvider, common::CarPlatform carPlatform, uint32_t ecuId);
    virtual ~FlasherBase();

    FlasherState getCurrentState() const;
    size_t getCurrentProgress() const;
    size_t getMaximumProgress() const;

    common::CarPlatform getCarPlatform() const { return _carPlatform; }
    uint32_t getEcuId() const { return _ecuId; }

    void start();

protected:
    virtual void startImpl(const std::vector<std::unique_ptr<common::ICanChannel>>& channels) = 0;

    void setCurrentState(FlasherState state);
    void setCurrentProgress(size_t currentProgress);
    void incCurrentProgress(size_t delta);
    void setMaximumProgress(size_t maximumProgress);

    void runOnThread(std::function<void()> callable);

    const common::CarPlatform _carPlatform;
    const uint32_t _ecuId;

private:
    std::shared_ptr<common::ICanChannelProvider> _channelProvider;
    mutable std::mutex _mutex;
    size_t _currentProgress;
    size_t _maximumProgress;
    FlasherState _currentState;

    std::thread _flasherThread;
    bool _stopRequested;
};

} // namespace flasher
