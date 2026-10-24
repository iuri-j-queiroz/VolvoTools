#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace common {

class ICanChannel;

/**
 * Creates CAN channels for a car platform.
 *
 * J2534 is one implementation (`J2534ChannelProvider`, Windows). Android transports
 * (vLinker/ELM327, DiCE) provide their own implementation, which is why flashers, readers
 * and loggers depend on this interface instead of the J2534 device object.
 */
class ICanChannelProvider {
public:
    virtual ~ICanChannelProvider() = default;

    /** All channels required to reach the given ECU (for example high-speed + medium-speed buses). */
    virtual std::vector<std::unique_ptr<ICanChannel>> getAllChannels(uint32_t ecuId = 0) const = 0;

    /** A single channel for the given ECU. */
    virtual std::unique_ptr<ICanChannel> getChannelForEcu(uint32_t ecuId) const = 0;

    /** Human-readable device description for logs and the UI. */
    virtual std::string getDeviceInfo() const { return {}; }
};

} // namespace common
