#include "hookapi.h"
#include "types.h"

#include <cstdint>
#include <string>
#include <array>
#include "logger.h"
#include "magic_enum.hpp"

template<typename Function>
struct Defer {
    Function f;

    explicit Defer(Function f) : f(f) {}

    ~Defer() { f(); }
};

namespace android {
    struct InputDeviceIdentifier {
        inline InputDeviceIdentifier() :
                bus(0), vendor(0), product(0), version(0) {
        }

        // Information provided by the kernel.
        std::string name;
        std::string location;
        std::string uniqueId;
        uint16_t bus;
        uint16_t vendor;
        uint16_t product;
        uint16_t version;

    };

    struct InputDevice {
        class InputReaderContext *mContext;

        int32_t mId;
        int32_t mGeneration;
        int32_t mControllerNumber;
        InputDeviceIdentifier mIdentifier;

    };
    struct InputDeviceContext {
        InputDevice *mDevice;

        class InputReaderContext *mContext;

        class EventHubInterface *mEventHub;

        int32_t mId;
        int32_t mDeviceId;
    };

    struct TouchInputMapper {
        uint64_t _vptr;
        InputDeviceContext *mDeviceContext;

        inline PointerGestureMode &getCurrentGestureMode() {
            return *(PointerGestureMode *) ((char *) this + 2086 * 4);
        }

        inline BitSet32 &getCurrentFingerIdBits() {
            return *(BitSet32 *) ((char *) this + 1054 * 4);
        }
    };

    // check struct based on reverse-engineering to make sure the builds are compatible with current compiler behavior
    static_assert(offsetof(TouchInputMapper, mDeviceContext) == 8);
    static_assert(offsetof(InputDeviceContext, mDeviceId) == 28);
}
#define LOG_TAG "InputInject/CustomGesture"

TInstanceHook(void, hooks::LIBINPUT_READER,
              "_ZN7android16TouchInputMapper14dispatchMotionElljjiiiiiiPKNS_17PointerPropertiesEPKNS_13PointerCoordsEPKjNS_8BitSet32Eiffl",
              android::TouchInputMapper,
              nsecs_t when, nsecs_t readTime, uint32_t policyFlags, uint32_t source, int32_t action,
              int32_t actionButton, int32_t flags, int32_t metaState, int32_t buttonState,
              int32_t edgeFlags, PropertiesArray *properties, CoordsArray *coords,
              IdToIndexArray *idToIndex, ::android::BitSet32 idBits, int32_t changedId, float xPrecision,
              float yPrecision, nsecs_t downTime, MotionClassification classification) {


    static void *xiaomiTouchDevice = nullptr;
    static bool enableGestureTransform = true;
    if (xiaomiTouchDevice == nullptr && this->mDeviceContext->mDevice->mIdentifier.name == "Xiaomi Touch") {
        xiaomiTouchDevice = this->mDeviceContext->mDevice;
    }

    if (this->mDeviceContext->mDevice == xiaomiTouchDevice) {

        // ==================== Collect Info ====================
        static PointerGestureMode last_gesture = PointerGestureMode::NEUTRAL;

        auto curr_gesture = getCurrentGestureMode();
        auto finger_count = getCurrentFingerIdBits().count();

        LOGD("dispatchMotion(deviceId=%d deviceName=%s gestureMode=%s, last_gesture=%s count=%d)",
             this->mDeviceContext->mDeviceId,
             this->mDeviceContext->mDevice->mIdentifier.name.c_str(),
             std::string(magic_enum::enum_name(getCurrentGestureMode())).c_str(),
             std::string(magic_enum::enum_name(last_gesture)).c_str(),
             finger_count
        );

        auto hookInstance = this;

        auto handleModeSwitch = [=]() {
            // last_finger_count is set when press detected, used to identify press release gesture
            static uint32_t last_finger_count = 0;
            // last_press_time is used to detect press release time interval
            // only update when press detected
            static nsecs_t last_press_time = 0;

            // press gestures are activated when two or more fingers are placed on the touchpad
            if (curr_gesture == PointerGestureMode::PRESS) {
                last_finger_count = finger_count;
                last_press_time = when;
                LOGD("handleModeSwitch: press detected, when=%lld", when);
            }

            // press release gesture triggered when all fingers are released and last gesture is press
            if ((curr_gesture == PointerGestureMode::NEUTRAL || curr_gesture == PointerGestureMode::QUIET) &&
                last_gesture == PointerGestureMode::PRESS && last_finger_count == 3) {
                if ((when - last_press_time) <= 150 * 1000000LL) {         // 150 ms
                    static size_t tripleTapCounter = 0;
                    tripleTapCounter++;
                    // reset counter if last triple tap is too long ago
                    static nsecs_t last_triple_tap_time = 0;

                    LOGD("handleModeSwitch: last_triple_tap_time, when=%lld inv=%lld", when,
                         (when - last_triple_tap_time));
                    if ((when - last_triple_tap_time) <= 1500 * 1000000LL) {
                        if (tripleTapCounter >= 5) {
                            enableGestureTransform = !enableGestureTransform;
                            tripleTapCounter = 0;
                            LOGI("handleModeSwitch: REQUEST SWITCH CUSTOM GESTURE MODE, ENABLED=%d",
                                 enableGestureTransform);
                        }
                    } else {
                        tripleTapCounter = 0;
                        LOGI("handleModeSwitch: timeout, reset counter, when=%lld",
                             last_triple_tap_time);
                    }

                    last_triple_tap_time = when;


                } else {
                    LOGD("handleModeSwitch: CUSTOM_GESTURE NOT A TAP, when=%lld", when);
                }

            }
        };

        auto handlePressGesture = [&]() -> bool {
            // last_finger_count is set when press detected, used to identify press release gesture
            static uint32_t last_finger_count = 0;
            // last_press_time is used to detect press release time interval
            // only update when press detected
            static nsecs_t last_press_time = 0;

            // press gesture detected when two or more fingers are putted on the touchpad
            if (curr_gesture == PointerGestureMode::PRESS) {
                last_finger_count = finger_count;
                last_press_time = when;
                LOGD("handlePressGesture: press detected, when=%lld", when);
                return true;
            }

            if ((curr_gesture == PointerGestureMode::NEUTRAL || curr_gesture == PointerGestureMode::QUIET) &&
                last_gesture == PointerGestureMode::PRESS && last_finger_count == 2) {
                if (when - last_press_time <= 150 * 1000000LL) {         // 150 ms
                    LOGD("handlePressGesture: press release detected, when=%lld", when);
                    //transform pointer type to mouse, use a new properties array to avoid modifying the original one
                    auto new_properties = *properties;
                    new_properties.at(0).toolType = ToolType::MOUSE;
                    //emulate press tap
                    hookInstance->original(hookInstance, when, readTime, policyFlags, source,
                                           AMOTION_EVENT_ACTION_DOWN, 0, flags, metaState,
                                           AMOTION_EVENT_BUTTON_SECONDARY,
                                           edgeFlags, &new_properties, coords, idToIndex, idBits, changedId,
                                           xPrecision, yPrecision, downTime, classification);
                    hookInstance->original(hookInstance, when, readTime, policyFlags, source,
                                           AMOTION_EVENT_ACTION_BUTTON_PRESS, AMOTION_EVENT_BUTTON_SECONDARY, flags,
                                           metaState,
                                           AMOTION_EVENT_BUTTON_SECONDARY,
                                           edgeFlags, &new_properties, coords, idToIndex, idBits, changedId,
                                           xPrecision, yPrecision, downTime, classification);

                    //emulate press release
                    hookInstance->original(hookInstance, when, readTime, policyFlags, source,
                                           AMOTION_EVENT_ACTION_BUTTON_RELEASE, AMOTION_EVENT_BUTTON_SECONDARY, flags,
                                           metaState,
                                           0,
                                           edgeFlags, &new_properties, coords, idToIndex, idBits, changedId,
                                           xPrecision, yPrecision, downTime, classification);
                    hookInstance->original(hookInstance, when, readTime, policyFlags, source,
                                           AMOTION_EVENT_ACTION_UP, 0, flags, metaState, 0,
                                           edgeFlags, &new_properties, coords, idToIndex, idBits, changedId,
                                           xPrecision, yPrecision, downTime, classification);

                    LOGI("handlePressGesture: RIGHT_TAP, when=%lld", when);
                    return true;

                } else {
                    LOGD("handlePressGesture: NOT A TAP, when=%lld", when);
                    return true;
                }

            }
            return false;
        };
        auto handleSwipeGesture = [&]() -> bool {
            static float last_x = 0;
            static float last_y = 0;
            static float last_diff_x = 0;
            static float last_diff_y = 0;
            static float swipe_x = 0;
            static float swipe_y = 0;
            float curr_x = coords->at(0).getAxisValue(AMOTION_EVENT_AXIS_X);
            float curr_y = coords->at(0).getAxisValue(AMOTION_EVENT_AXIS_Y);
            float diff_x = last_x - curr_x, diff_y = last_y - curr_y;
            Defer _d([&]() {
                last_x = curr_x;
                last_y = curr_y;
                last_diff_x = diff_x == 0 ? last_diff_x : diff_x;
                last_diff_y = diff_y == 0 ? last_diff_y : diff_y;
            });

            if (curr_gesture == PointerGestureMode::SWIPE) {
                //transform pointer type to mouse, use a new properties array to avoid modifying the original one
                auto new_properties = *properties;
                new_properties.at(0).toolType = ToolType::MOUSE;

                auto speedTransform = [](float speed) -> float {
                    float s = abs(speed);
                    float sign = speed > 0 ? 1.0f : -1.0f;
                    if (s < 0.2)
                        return 0;
                    if (s < 0.5)
                        return sign * (s * 1.1f);
                    if (s < 2)
                        return sign * (s * 1.25f);
                    if (s > 80)
                        return sign * 8.0f;
                    return sign * 2.5f;
                };
                // avoid huge scroll when gesture triggered first time in this session
                if (last_gesture != PointerGestureMode::SWIPE) {
                    diff_x = 0;
                    diff_y = 0;
                    swipe_x = last_x;
                    swipe_y = last_y;
                } else {
                    static size_t counter = 0;
                    static float speedSum_x = 0;
                    static float speedSum_y = 0;
                    counter++;
                    speedSum_x += speedTransform(diff_x);
                    speedSum_y += speedTransform(diff_y);

                    if (counter >= 2) {

                        coords->at(0).setAxisValue(AMOTION_EVENT_AXIS_VSCROLL, speedSum_x * 0.2f);
                        coords->at(0).setAxisValue(AMOTION_EVENT_AXIS_HSCROLL, speedSum_y * 0.2f);

                        // lock scroll pointer to the first position
                        coords->at(0).setAxisValue(AMOTION_EVENT_AXIS_X, swipe_x);
                        coords->at(0).setAxisValue(AMOTION_EVENT_AXIS_Y, swipe_y);
                        hookInstance->original(hookInstance, when, readTime, policyFlags, source,
                                               AMOTION_EVENT_ACTION_HOVER_MOVE,
                                               actionButton,
                                               flags,
                                               metaState, buttonState,
                                               edgeFlags, &new_properties, coords, idToIndex, idBits, changedId,
                                               xPrecision,
                                               yPrecision,
                                               downTime,
                                               classification);
                        hookInstance->original(hookInstance, when, readTime, policyFlags, source,
                                               AMOTION_EVENT_ACTION_SCROLL, actionButton,
                                               flags,
                                               metaState, buttonState,
                                               edgeFlags, &new_properties, coords, idToIndex, idBits, changedId,
                                               xPrecision,
                                               yPrecision,
                                               downTime,
                                               classification);
                        counter = 0;
                        speedSum_x = 0;
                        speedSum_y = 0;
                        LOGD("handleSwipeGesture: scroll dx:%0.3f dy:%0.3f a:%0.3f b:%0.3f", diff_x, diff_y,
                             coords->at(0).getAxisValue(AMOTION_EVENT_AXIS_VSCROLL),
                             coords->at(0).getAxisValue(AMOTION_EVENT_AXIS_HSCROLL));
                    }
                }
                return true;
            }

            return false;
        };

        bool cancel_gesture = false;
        if (enableGestureTransform) {
            cancel_gesture = handlePressGesture() || cancel_gesture;
            cancel_gesture = handleSwipeGesture() || cancel_gesture;
        }
        handleModeSwitch();

        // ==================== update state ====================
        last_gesture = curr_gesture;
        if (cancel_gesture) {
            LOGD("inject: gesture canceled, when=%lld action=%d", when, action);
            return;
        } else {
            return original(this, when, readTime, policyFlags, source, action, actionButton, flags, metaState,
                            buttonState,
                            edgeFlags, properties, coords, idToIndex, idBits, changedId, xPrecision, yPrecision,
                            downTime,
                            classification);
        }
    }

    return original(this, when, readTime, policyFlags, source, action, actionButton, flags, metaState, buttonState,
                    edgeFlags, properties, coords, idToIndex, idBits, changedId, xPrecision, yPrecision, downTime,
                    classification);
}


