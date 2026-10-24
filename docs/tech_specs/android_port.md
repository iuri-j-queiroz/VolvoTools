# Android port

Branch `android` keeps the protocol code shared with `master` while making the core buildable
and usable on Android (NDK, C++20). The Windows executables (`VolvoFlasher`, `VolvoLogger`)
remain unchanged in behaviour.

## What changed

### 1. `ICanChannelProvider` — transport-independent channel creation

Flashers, readers and the logger used to take `j2534::J2534&` in their constructors, which made
them unusable without a J2534 device. They now take a
`std::shared_ptr<common::ICanChannelProvider>`:

```cpp
class ICanChannelProvider {
public:
    virtual std::vector<std::unique_ptr<ICanChannel>> getAllChannels(uint32_t ecuId = 0) const = 0;
    virtual std::unique_ptr<ICanChannel> getChannelForEcu(uint32_t ecuId) const = 0;
    virtual std::string getDeviceInfo() const { return {}; }
};
```

- `J2534ChannelProvider` implements it (Windows, unchanged behaviour).
- Android transports implement it in the app (`ElmCanChannel` / `DiceCanChannel`).
- Affected: `FlasherBase`, `ReaderBase`, `Logger`, all `D2*`/`UDS*`/`KWP` flashers and readers,
  and the factories. The CLI creates a `J2534ChannelProvider` at each operation.

This is the transport abstraction described in `transport_abstraction.md`, finished for the
device-creation seam. `CanMessagesTransceiver`, `CanAlarmClock`, `UDSPinFinder` and the
`PASSTHRU_MSG`-based `UDSMessage`/`TP20Message` classes are not needed by flashers/loggers and
are excluded from the Android build.

### 2. Build guards

`VOLVOTOOLS_ANDROID` guards the Windows-only parts:

- `Common/src/Util.cpp`: registry access (`getAvailableDevices`) and all J2534 channel/filter
  helpers. Portable helpers (`makeUDSResponseCanId`, VIN/platform parsing, config loading,
  `readMessageCheckAndGet`, ...) are kept.
- The Android build excludes `J2534ChannelProvider.cpp`, `J2534ChannelAdapter.cpp`,
  `CanAlarmClock.cpp`, `CanMessagesTransceiver.cpp`, `UDSPinFinder.cpp`,
  `UDSMessage.cpp`, `TP20Message.cpp` and `Flasher/src/pin/*` (not needed for D2 ME9).

Portability fixes required by clang (MSVC was permissive):

- `Common/src/Util.cpp`: explicit casts in `getPinArray`.
- `D2ProtocolCommonSteps.cpp`: `localtime_s` on Windows, `localtime_r` elsewhere.

### 3. Dependencies

- `fast-cpp-csv-parser` (`csv.h`) is vendored in `third_party/fast-cpp-csv-parser` (BSD-3) and the
  Conan dependency was removed. `LogParameters.cpp` includes `<csv.h>`; the include directory is
  added in `Logger/CMakeLists.txt`.
- Android builds need Boost headers (Spirit X3 in `VBFParser.cpp`), yaml-cpp and the
  easylogging++ header + `easylogging++.cc`. They are provided by the app's CMake, not Conan.
- `.gitmodules` uses HTTPS so CI and contributors do not need SSH keys.

### 4. Platform fixes for P1_UDS (C30 2008, Elsys 2)

- `Logger/src/Logger.cpp`: `P1_UDS` is accepted for D2 logging (`ECM_ME` and `getLoggerType`).
- `Flasher/src/SBLProviderCommon.cpp`: the ME9 SBL (`P1_ME9_SBL`) is mapped for `P1_UDS`.

## Verification

Compiled for Android `arm64-v8a` (NDK 27.2, CMake 3.22.1, C++20) as static libraries
`common`, `flasher`, `logger`, and linked into a test executable. Windows builds are expected to
behave as before; the CLI call sites were updated to construct a `J2534ChannelProvider`.

## Not done yet

- `CanMessagesTransceiver` / `CanAlarmClock` / `UDSPinFinder` still use J2534 types; port them to
  `ICanChannel` before using them on Android (PIN cracking for UDS platforms).
- No Android `ICanChannelProvider` implementation in the fork: it lives in the app repository
  (`android/native`) so the fork stays independent of the app.
