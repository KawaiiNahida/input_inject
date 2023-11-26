#pragma once
/**
 * Constants that identify tool types.
 * Refer to the documentation on the MotionEvent class for descriptions of each tool type.
 */
enum {
    /** unknown */
    AMOTION_EVENT_TOOL_TYPE_UNKNOWN = 0,
    /** finger */
    AMOTION_EVENT_TOOL_TYPE_FINGER = 1,
    /** stylus */
    AMOTION_EVENT_TOOL_TYPE_STYLUS = 2,
    /** mouse */
    AMOTION_EVENT_TOOL_TYPE_MOUSE = 3,
    /** eraser */
    AMOTION_EVENT_TOOL_TYPE_ERASER = 4,
    /** palm */
    AMOTION_EVENT_TOOL_TYPE_PALM = 5,
};
/**
 * The pointer tool type.
 */
enum class ToolType {
    UNKNOWN = AMOTION_EVENT_TOOL_TYPE_UNKNOWN,
    FINGER = AMOTION_EVENT_TOOL_TYPE_FINGER,
    STYLUS = AMOTION_EVENT_TOOL_TYPE_STYLUS,
    MOUSE = AMOTION_EVENT_TOOL_TYPE_MOUSE,
    ERASER = AMOTION_EVENT_TOOL_TYPE_ERASER,
    PALM = AMOTION_EVENT_TOOL_TYPE_PALM,
    ftl_first = UNKNOWN,
    ftl_last = PALM,
};
/**
 * Constants that identify different gesture classification types.
 */
enum AMotionClassification : uint32_t {
    /**
     * Classification constant: None.
     *
     * No additional information is available about the current motion event stream.
     */
    AMOTION_EVENT_CLASSIFICATION_NONE = 0,
    /**
     * Classification constant: Ambiguous gesture.
     *
     * The user's intent with respect to the current event stream is not yet determined. Events
     * starting in #AMOTION_EVENT_CLASSIFICATION_AMBIGUOUS_GESTURE will eventually resolve into
     * either #AMOTION_EVENT_CLASSIFICATION_DEEP_PRESS or #AMOTION_EVENT_CLASSIFICATION_NONE.
     * Gestural actions, such as scrolling, should be inhibited until the classification resolves
     * to another value or the event stream ends.
     */
    AMOTION_EVENT_CLASSIFICATION_AMBIGUOUS_GESTURE = 1,
    /**
     * Classification constant: Deep press.
     *
     * The current event stream represents the user intentionally pressing harder on the screen.
     * This classification type should be used to accelerate the long press behaviour.
     */
    AMOTION_EVENT_CLASSIFICATION_DEEP_PRESS = 2,
    /**
     * Classification constant: touchpad two-finger swipe.
     *
     * The current event stream represents the user swiping with two fingers on a touchpad.
     */
    AMOTION_EVENT_CLASSIFICATION_TWO_FINGER_SWIPE = 3,
    /**
     * Classification constant: multi-finger swipe.
     *
     * The current event stream represents the user swiping with three or more fingers on a
     * touchpad. Unlike two-finger swipes, these are only to be handled by the system UI, which is
     * why they have a separate constant from two-finger swipes.
     */
    AMOTION_EVENT_CLASSIFICATION_MULTI_FINGER_SWIPE = 4,
    /**
     * Classification constant: pinch.
     *
     * The current event stream represents the user pinching with two fingers on a touchpad. The
     * gesture is centered around the current cursor position.
     */
    AMOTION_EVENT_CLASSIFICATION_PINCH = 5,
};


/**
 * Classifications of the current gesture, if available.
 */
enum class MotionClassification : uint8_t {
    /**
     * No classification is available.
     */
    NONE = AMOTION_EVENT_CLASSIFICATION_NONE,
    /**
     * Too early to classify the current gesture. Need more events. Look for changes in the
     * upcoming motion events.
     */
    AMBIGUOUS_GESTURE = AMOTION_EVENT_CLASSIFICATION_AMBIGUOUS_GESTURE,
    /**
     * The current gesture likely represents a user intentionally exerting force on the touchscreen.
     */
    DEEP_PRESS = AMOTION_EVENT_CLASSIFICATION_DEEP_PRESS,
    /**
     * The current gesture represents the user swiping with two fingers on a touchpad.
     */
    TWO_FINGER_SWIPE = AMOTION_EVENT_CLASSIFICATION_TWO_FINGER_SWIPE,
    /**
     * The current gesture represents the user swiping with three or more fingers on a touchpad.
     * Unlike two-finger swipes, these are only to be handled by the system UI, which is why they
     * have a separate constant from two-finger swipes.
     */
    MULTI_FINGER_SWIPE = AMOTION_EVENT_CLASSIFICATION_MULTI_FINGER_SWIPE,
    /**
     * The current gesture represents the user pinching with two fingers on a touchpad. The gesture
     * is centered around the current cursor position.
     */
    PINCH = AMOTION_EVENT_CLASSIFICATION_PINCH,
};

enum {
    /** Bit mask of the parts of the action code that are the action itself. */
    AMOTION_EVENT_ACTION_MASK = 0xff,

    /**
     * Bits in the action code that represent a pointer index, used with
     * #AMOTION_EVENT_ACTION_POINTER_DOWN and AMOTION_EVENT_ACTION_POINTER_UP.
     * Shifting down by #AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT provides the
     * actual pointer index where the data for the pointer going up or down can
     * be found.
     */
    AMOTION_EVENT_ACTION_POINTER_INDEX_MASK = 0xff00,

    /** A pressed gesture has started, the motion contains the initial starting
     * location. */
    AMOTION_EVENT_ACTION_DOWN = 0,

    /**
     * A pressed gesture has finished, the motion contains the final release
     * location as well as any intermediate points since the last down or move
     * event.
     */
    AMOTION_EVENT_ACTION_UP = 1,

    /**
     * A change has happened during a press gesture (between
     * #AMOTION_EVENT_ACTION_DOWN and #AMOTION_EVENT_ACTION_UP).  The motion
     * contains the most recent point, as well as any intermediate points since
     * the last down or move event.
     */
    AMOTION_EVENT_ACTION_MOVE = 2,

    /**
     * The current gesture has been aborted.
     * You will not receive any more points in it.  You should treat this as
     * an up event, but not perform any action that you normally would.
     */
    AMOTION_EVENT_ACTION_CANCEL = 3,

    /**
     * A movement has happened outside of the normal bounds of the UI element.
     * This does not provide a full gesture, but only the initial location of
     * the movement/touch.
     */
    AMOTION_EVENT_ACTION_OUTSIDE = 4,

    /**
     * A non-primary pointer has gone down.
     * The bits in #AMOTION_EVENT_ACTION_POINTER_INDEX_MASK indicate which
     * pointer changed.
     */
    AMOTION_EVENT_ACTION_POINTER_DOWN = 5,

    /**
     * A non-primary pointer has gone up.
     * The bits in #AMOTION_EVENT_ACTION_POINTER_INDEX_MASK indicate which
     * pointer changed.
     */
    AMOTION_EVENT_ACTION_POINTER_UP = 6,

    /**
     * A change happened but the pointer is not down (unlike
     * #AMOTION_EVENT_ACTION_MOVE). The motion contains the most recent point,
     * as well as any intermediate points since the last hover move event.
     */
    AMOTION_EVENT_ACTION_HOVER_MOVE = 7,

    /**
     * The motion event contains relative vertical and/or horizontal scroll
     * offsets. Use {@link AMotionEvent_getAxisValue} to retrieve the
     * information from #AMOTION_EVENT_AXIS_VSCROLL and
     * #AMOTION_EVENT_AXIS_HSCROLL. The pointer may or may not be down when this
     * event is dispatched. This action is always delivered to the winder under
     * the pointer, which may not be the window currently touched.
     */
    AMOTION_EVENT_ACTION_SCROLL = 8,

    /** The pointer is not down but has entered the boundaries of a window or
     * view. */
    AMOTION_EVENT_ACTION_HOVER_ENTER = 9,

    /** The pointer is not down but has exited the boundaries of a window or
     * view. */
    AMOTION_EVENT_ACTION_HOVER_EXIT = 10,

    /* One or more buttons have been pressed. */
    AMOTION_EVENT_ACTION_BUTTON_PRESS = 11,

    /* One or more buttons have been released. */
    AMOTION_EVENT_ACTION_BUTTON_RELEASE = 12,
};

/**
 * Constants that identify each individual axis of a motion event.
 * @anchor AMOTION_EVENT_AXIS
 */
enum {
    /**
     * Axis constant: X axis of a motion event.
     *
     * - For a touch screen, reports the absolute X screen position of the center of
     * the touch contact area.  The units are display pixels.
     * - For a touch pad, reports the absolute X surface position of the center of the touch
     * contact area. The units are device-dependent.
     * - For a mouse, reports the absolute X screen position of the mouse pointer.
     * The units are display pixels.
     * - For a trackball, reports the relative horizontal displacement of the trackball.
     * The value is normalized to a range from -1.0 (left) to 1.0 (right).
     * - For a joystick, reports the absolute X position of the joystick.
     * The value is normalized to a range from -1.0 (left) to 1.0 (right).
     */
    AMOTION_EVENT_AXIS_X = 0,
    /**
     * Axis constant: Y axis of a motion event.
     *
     * - For a touch screen, reports the absolute Y screen position of the center of
     * the touch contact area.  The units are display pixels.
     * - For a touch pad, reports the absolute Y surface position of the center of the touch
     * contact area. The units are device-dependent.
     * - For a mouse, reports the absolute Y screen position of the mouse pointer.
     * The units are display pixels.
     * - For a trackball, reports the relative vertical displacement of the trackball.
     * The value is normalized to a range from -1.0 (up) to 1.0 (down).
     * - For a joystick, reports the absolute Y position of the joystick.
     * The value is normalized to a range from -1.0 (up or far) to 1.0 (down or near).
     */
    AMOTION_EVENT_AXIS_Y = 1,
    /**
     * Axis constant: Pressure axis of a motion event.
     *
     * - For a touch screen or touch pad, reports the approximate pressure applied to the surface
     * by a finger or other tool.  The value is normalized to a range from
     * 0 (no pressure at all) to 1 (normal pressure), although values higher than 1
     * may be generated depending on the calibration of the input device.
     * - For a trackball, the value is set to 1 if the trackball button is pressed
     * or 0 otherwise.
     * - For a mouse, the value is set to 1 if the primary mouse button is pressed
     * or 0 otherwise.
     */
    AMOTION_EVENT_AXIS_PRESSURE = 2,
    /**
     * Axis constant: Size axis of a motion event.
     *
     * - For a touch screen or touch pad, reports the approximate size of the contact area in
     * relation to the maximum detectable size for the device.  The value is normalized
     * to a range from 0 (smallest detectable size) to 1 (largest detectable size),
     * although it is not a linear scale. This value is of limited use.
     * To obtain calibrated size information, see
     * {@link AMOTION_EVENT_AXIS_TOUCH_MAJOR} or {@link AMOTION_EVENT_AXIS_TOOL_MAJOR}.
     */
    AMOTION_EVENT_AXIS_SIZE = 3,
    /**
     * Axis constant: TouchMajor axis of a motion event.
     *
     * - For a touch screen, reports the length of the major axis of an ellipse that
     * represents the touch area at the point of contact.
     * The units are display pixels.
     * - For a touch pad, reports the length of the major axis of an ellipse that
     * represents the touch area at the point of contact.
     * The units are device-dependent.
     */
    AMOTION_EVENT_AXIS_TOUCH_MAJOR = 4,
    /**
     * Axis constant: TouchMinor axis of a motion event.
     *
     * - For a touch screen, reports the length of the minor axis of an ellipse that
     * represents the touch area at the point of contact.
     * The units are display pixels.
     * - For a touch pad, reports the length of the minor axis of an ellipse that
     * represents the touch area at the point of contact.
     * The units are device-dependent.
     *
     * When the touch is circular, the major and minor axis lengths will be equal to one another.
     */
    AMOTION_EVENT_AXIS_TOUCH_MINOR = 5,
    /**
     * Axis constant: ToolMajor axis of a motion event.
     *
     * - For a touch screen, reports the length of the major axis of an ellipse that
     * represents the size of the approaching finger or tool used to make contact.
     * - For a touch pad, reports the length of the major axis of an ellipse that
     * represents the size of the approaching finger or tool used to make contact.
     * The units are device-dependent.
     *
     * When the touch is circular, the major and minor axis lengths will be equal to one another.
     *
     * The tool size may be larger than the touch size since the tool may not be fully
     * in contact with the touch sensor.
     */
    AMOTION_EVENT_AXIS_TOOL_MAJOR = 6,
    /**
     * Axis constant: ToolMinor axis of a motion event.
     *
     * - For a touch screen, reports the length of the minor axis of an ellipse that
     * represents the size of the approaching finger or tool used to make contact.
     * - For a touch pad, reports the length of the minor axis of an ellipse that
     * represents the size of the approaching finger or tool used to make contact.
     * The units are device-dependent.
     *
     * When the touch is circular, the major and minor axis lengths will be equal to one another.
     *
     * The tool size may be larger than the touch size since the tool may not be fully
     * in contact with the touch sensor.
     */
    AMOTION_EVENT_AXIS_TOOL_MINOR = 7,
    /**
     * Axis constant: Orientation axis of a motion event.
     *
     * - For a touch screen or touch pad, reports the orientation of the finger
     * or tool in radians relative to the vertical plane of the device.
     * An angle of 0 radians indicates that the major axis of contact is oriented
     * upwards, is perfectly circular or is of unknown orientation.  A positive angle
     * indicates that the major axis of contact is oriented to the right.  A negative angle
     * indicates that the major axis of contact is oriented to the left.
     * The full range is from -PI/2 radians (finger pointing fully left) to PI/2 radians
     * (finger pointing fully right).
     * - For a stylus, the orientation indicates the direction in which the stylus
     * is pointing in relation to the vertical axis of the current orientation of the screen.
     * The range is from -PI radians to PI radians, where 0 is pointing up,
     * -PI/2 radians is pointing left, -PI or PI radians is pointing down, and PI/2 radians
     * is pointing right.  See also #AMOTION_EVENT_AXIS_TILT.
     */
    AMOTION_EVENT_AXIS_ORIENTATION = 8,
    /**
     * Axis constant: Vertical Scroll axis of a motion event.
     *
     * - For a mouse, reports the relative movement of the vertical scroll wheel.
     * The value is normalized to a range from -1.0 (down) to 1.0 (up).
     *
     * This axis should be used to scroll views vertically.
     */
    AMOTION_EVENT_AXIS_VSCROLL = 9,
    /**
     * Axis constant: Horizontal Scroll axis of a motion event.
     *
     * - For a mouse, reports the relative movement of the horizontal scroll wheel.
     * The value is normalized to a range from -1.0 (left) to 1.0 (right).
     *
     * This axis should be used to scroll views horizontally.
     */
    AMOTION_EVENT_AXIS_HSCROLL = 10,
    /**
     * Axis constant: Z axis of a motion event.
     *
     * - For a joystick, reports the absolute Z position of the joystick.
     * The value is normalized to a range from -1.0 (high) to 1.0 (low).
     * <em>On game pads with two analog joysticks, this axis is often reinterpreted
     * to report the absolute X position of the second joystick instead.</em>
     */
    AMOTION_EVENT_AXIS_Z = 11,
    /**
     * Axis constant: X Rotation axis of a motion event.
     *
     * - For a joystick, reports the absolute rotation angle about the X axis.
     * The value is normalized to a range from -1.0 (counter-clockwise) to 1.0 (clockwise).
     */
    AMOTION_EVENT_AXIS_RX = 12,
    /**
     * Axis constant: Y Rotation axis of a motion event.
     *
     * - For a joystick, reports the absolute rotation angle about the Y axis.
     * The value is normalized to a range from -1.0 (counter-clockwise) to 1.0 (clockwise).
     */
    AMOTION_EVENT_AXIS_RY = 13,
    /**
     * Axis constant: Z Rotation axis of a motion event.
     *
     * - For a joystick, reports the absolute rotation angle about the Z axis.
     * The value is normalized to a range from -1.0 (counter-clockwise) to 1.0 (clockwise).
     * On game pads with two analog joysticks, this axis is often reinterpreted
     * to report the absolute Y position of the second joystick instead.
     */
    AMOTION_EVENT_AXIS_RZ = 14,
    /**
     * Axis constant: Hat X axis of a motion event.
     *
     * - For a joystick, reports the absolute X position of the directional hat control.
     * The value is normalized to a range from -1.0 (left) to 1.0 (right).
     */
    AMOTION_EVENT_AXIS_HAT_X = 15,
    /**
     * Axis constant: Hat Y axis of a motion event.
     *
     * - For a joystick, reports the absolute Y position of the directional hat control.
     * The value is normalized to a range from -1.0 (up) to 1.0 (down).
     */
    AMOTION_EVENT_AXIS_HAT_Y = 16,
    /**
     * Axis constant: Left Trigger axis of a motion event.
     *
     * - For a joystick, reports the absolute position of the left trigger control.
     * The value is normalized to a range from 0.0 (released) to 1.0 (fully pressed).
     */
    AMOTION_EVENT_AXIS_LTRIGGER = 17,
    /**
     * Axis constant: Right Trigger axis of a motion event.
     *
     * - For a joystick, reports the absolute position of the right trigger control.
     * The value is normalized to a range from 0.0 (released) to 1.0 (fully pressed).
     */
    AMOTION_EVENT_AXIS_RTRIGGER = 18,
    /**
     * Axis constant: Throttle axis of a motion event.
     *
     * - For a joystick, reports the absolute position of the throttle control.
     * The value is normalized to a range from 0.0 (fully open) to 1.0 (fully closed).
     */
    AMOTION_EVENT_AXIS_THROTTLE = 19,
    /**
     * Axis constant: Rudder axis of a motion event.
     *
     * - For a joystick, reports the absolute position of the rudder control.
     * The value is normalized to a range from -1.0 (turn left) to 1.0 (turn right).
     */
    AMOTION_EVENT_AXIS_RUDDER = 20,
    /**
     * Axis constant: Wheel axis of a motion event.
     *
     * - For a joystick, reports the absolute position of the steering wheel control.
     * The value is normalized to a range from -1.0 (turn left) to 1.0 (turn right).
     */
    AMOTION_EVENT_AXIS_WHEEL = 21,
    /**
     * Axis constant: Gas axis of a motion event.
     *
     * - For a joystick, reports the absolute position of the gas (accelerator) control.
     * The value is normalized to a range from 0.0 (no acceleration)
     * to 1.0 (maximum acceleration).
     */
    AMOTION_EVENT_AXIS_GAS = 22,
    /**
     * Axis constant: Brake axis of a motion event.
     *
     * - For a joystick, reports the absolute position of the brake control.
     * The value is normalized to a range from 0.0 (no braking) to 1.0 (maximum braking).
     */
    AMOTION_EVENT_AXIS_BRAKE = 23,
    /**
     * Axis constant: Distance axis of a motion event.
     *
     * - For a stylus, reports the distance of the stylus from the screen.
     * A value of 0.0 indicates direct contact and larger values indicate increasing
     * distance from the surface.
     */
    AMOTION_EVENT_AXIS_DISTANCE = 24,
    /**
     * Axis constant: Tilt axis of a motion event.
     *
     * - For a stylus, reports the tilt angle of the stylus in radians where
     * 0 radians indicates that the stylus is being held perpendicular to the
     * surface, and PI/2 radians indicates that the stylus is being held flat
     * against the surface.
     */
    AMOTION_EVENT_AXIS_TILT = 25,
    /**
     * Axis constant:  Generic scroll axis of a motion event.
     *
     * - This is used for scroll axis motion events that can't be classified as strictly
     *   vertical or horizontal. The movement of a rotating scroller is an example of this.
     */
    AMOTION_EVENT_AXIS_SCROLL = 26,
    /**
     * Axis constant: The movement of x position of a motion event.
     *
     * - For a mouse, reports a difference of x position between the previous position.
     * This is useful when pointer is captured, in that case the mouse pointer doesn't
     * change the location but this axis reports the difference which allows the app
     * to see how the mouse is moved.
     */
    AMOTION_EVENT_AXIS_RELATIVE_X = 27,
    /**
     * Axis constant: The movement of y position of a motion event.
     *
     * Same as #AMOTION_EVENT_AXIS_RELATIVE_X, but for y position.
     */
    AMOTION_EVENT_AXIS_RELATIVE_Y = 28,
    /**
     * Axis constant: Generic 1 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_1 = 32,
    /**
     * Axis constant: Generic 2 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_2 = 33,
    /**
     * Axis constant: Generic 3 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_3 = 34,
    /**
     * Axis constant: Generic 4 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_4 = 35,
    /**
     * Axis constant: Generic 5 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_5 = 36,
    /**
     * Axis constant: Generic 6 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_6 = 37,
    /**
     * Axis constant: Generic 7 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_7 = 38,
    /**
     * Axis constant: Generic 8 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_8 = 39,
    /**
     * Axis constant: Generic 9 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_9 = 40,
    /**
     * Axis constant: Generic 10 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_10 = 41,
    /**
     * Axis constant: Generic 11 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_11 = 42,
    /**
     * Axis constant: Generic 12 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_12 = 43,
    /**
     * Axis constant: Generic 13 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_13 = 44,
    /**
     * Axis constant: Generic 14 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_14 = 45,
    /**
     * Axis constant: Generic 15 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_15 = 46,
    /**
     * Axis constant: Generic 16 axis of a motion event.
     * The interpretation of a generic axis is device-specific.
     */
    AMOTION_EVENT_AXIS_GENERIC_16 = 47,
    /**
     * Axis constant: X gesture offset axis of a motion event.
     *
     * - For a touch pad, reports the distance that a swipe gesture has moved in the X axis, as a
     *   proportion of the touch pad's size. For example, if a touch pad is 1000 units wide, and a
     *   swipe gesture starts at X = 500 then moves to X = 400, this axis would have a value of
     *   -0.1.
     *
     * These values are relative to the state from the last event, not accumulated, so developers
     * should make sure to process this axis value for all batched historical events.
     */
    AMOTION_EVENT_AXIS_GESTURE_X_OFFSET = 48,
    /**
     * Axis constant: Y gesture offset axis of a motion event.
     *
     * The same as {@link AMOTION_EVENT_AXIS_GESTURE_X_OFFSET}, but for the Y axis.
     */
    AMOTION_EVENT_AXIS_GESTURE_Y_OFFSET = 49,
    /**
     * Axis constant: X scroll distance axis of a motion event.
     *
     * - For a touch pad, reports the distance that should be scrolled in the X axis as a result of
     *   the user's two-finger scroll gesture, in display pixels.
     *
     * These values are relative to the state from the last event, not accumulated, so developers
     * should make sure to process this axis value for all batched historical events.
     */
    AMOTION_EVENT_AXIS_GESTURE_SCROLL_X_DISTANCE = 50,
    /**
     * Axis constant: Y scroll distance axis of a motion event.
     *
     * The same as {@link AMOTION_EVENT_AXIS_GESTURE_SCROLL_X_DISTANCE}, but for the Y axis.
     */
    AMOTION_EVENT_AXIS_GESTURE_SCROLL_Y_DISTANCE = 51,
    /**
     * Axis constant: pinch scale factor of a motion event.
     *
     * - For a touch pad, reports the change in distance between the fingers when the user is making
     *   a pinch gesture, as a proportion of that distance when the gesture was last reported. For
     *   example, if the fingers were 50 units apart and are now 52 units apart, the scale factor
     *   would be 1.04.
     *
     * These values are relative to the state from the last event, not accumulated, so developers
     * should make sure to process this axis value for all batched historical events.
     */
    AMOTION_EVENT_AXIS_GESTURE_PINCH_SCALE_FACTOR = 52,

    /**
     * Note: This is not an "Axis constant". It does not represent any axis, nor should it be used
     * to represent any axis. It is a constant holding the value of the largest defined axis value,
     * to make some computations (like iterating through all possible axes) cleaner.
     * Please update the value accordingly if you add a new axis.
     */
    AMOTION_EVENT_MAXIMUM_VALID_AXIS_VALUE = AMOTION_EVENT_AXIS_GESTURE_PINCH_SCALE_FACTOR,

    // NOTE: If you add a new axis here you must also add it to several other files.
    //       Refer to frameworks/base/core/java/android/view/MotionEvent.java for the full list.
    //       Update AMOTION_EVENT_MAXIMUM_VALID_AXIS_VALUE accordingly as well.
};

enum class PointerGestureMode : uint32_t {
    // No fingers, button is not pressed.
    // Nothing happening.
    NEUTRAL,

    // No fingers, button is not pressed.
    // Tap detected.
    // Emits DOWN and UP events at the pointer location.
    TAP,

    // Exactly one finger dragging following a tap.
    // Pointer follows the active finger.
    // Emits DOWN, MOVE and UP events at the pointer location.
    //
    // Detect double-taps when the finger goes up while in TAP_DRAG mode.
    TAP_DRAG,

    // Button is pressed.
    // Pointer follows the active finger if there is one.  Other fingers are ignored.
    // Emits DOWN, MOVE and UP events at the pointer location.
    BUTTON_CLICK_OR_DRAG,

    // Exactly one finger, button is not pressed.
    // Pointer follows the active finger.
    // Emits HOVER_MOVE events at the pointer location.
    //
    // Detect taps when the finger goes up while in HOVER mode.
    HOVER,

    // Exactly two fingers but neither have moved enough to clearly indicate
    // whether a swipe or freeform gesture was intended.  We consider the
    // pointer to be pressed so this enables clicking or long-pressing on buttons.
    // Pointer does not move.
    // Emits DOWN, MOVE and UP events with a single stationary pointer coordinate.
    PRESS,

    // Exactly two fingers moving in the same direction, button is not pressed.
    // Pointer does not move.
    // Emits DOWN, MOVE and UP events with a single pointer coordinate that
    // follows the midpoint between both fingers.
    SWIPE,

    // Two or more fingers moving in arbitrary directions, button is not pressed.
    // Pointer does not move.
    // Emits DOWN, POINTER_DOWN, MOVE, POINTER_UP and UP events that follow
    // each finger individually relative to the initial centroid of the finger.
    FREEFORM,

    // Waiting for quiet time to end before starting the next gesture.
    QUIET,
};

/**
 * Constants that identify buttons that are associated with motion events.
 * Refer to the documentation on the MotionEvent class for descriptions of each button.
 */
enum {
    /** primary */
    AMOTION_EVENT_BUTTON_PRIMARY = 1 << 0,
    /** secondary */
    AMOTION_EVENT_BUTTON_SECONDARY = 1 << 1,
    /** tertiary */
    AMOTION_EVENT_BUTTON_TERTIARY = 1 << 2,
    /** back */
    AMOTION_EVENT_BUTTON_BACK = 1 << 3,
    /** forward */
    AMOTION_EVENT_BUTTON_FORWARD = 1 << 4,
    AMOTION_EVENT_BUTTON_STYLUS_PRIMARY = 1 << 5,
    AMOTION_EVENT_BUTTON_STYLUS_SECONDARY = 1 << 6,
};
