from ctypes import*

class SDL_CommonEvent(Structure):
	_fields_ = [
	('type',c_uint32), #
	('timestamp',c_uint32)] #


'''*
 *  Window state change event data (event.window.*)
'''
class SDL_WindowEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_WINDOWEVENT 
	('timestamp',c_uint32), #
	('windowID',c_uint32), #< The associated window 
	('event',c_uint8), #< ::SDL_WindowEventID 
	('padding1',c_uint8), #
	('padding2',c_uint8), #
	('padding3',c_uint8), #
	('data1',c_int32), #< event dependent data 
	('data2',c_int32)] #< event dependent data 



class SDL_Keysym(Structure):
	_fields_ = [
	('scancode',c_int32),      #< SDL physical key code - see ::SDL_Scancode for details 
	('sym',c_int32),            #< SDL virtual key code - see ::SDL_Keycode for details 
	('mod',c_uint16),                 #< current key modifiers 
	('unused',c_uint32)]

'''*
 *  Keyboard button event structure (event.key.*)
'''
class SDL_KeyboardEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_KEYDOWN or ::SDL_KEYUP 
	('timestamp',c_uint32), #
	('windowID',c_uint32), #< The window with keyboard focus, if any 
	('state',c_uint8), #< ::SDL_PRESSED or ::SDL_RELEASED 
	('repeat',c_uint8), #< Non-zero if this is a key repeat 
	('padding2',c_uint8), #
	('padding3',c_uint8), #
	('keysym',SDL_Keysym)] #< The key that was pressed or released 


#define SDL_TEXTEDITINGEVENT_TEXT_SIZE (32)
'''*
 *  Keyboard text editing event structure (event.edit.*)
'''
class SDL_TextEditingEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_TEXTEDITING 
	('timestamp',c_uint32), #
	('windowID',c_uint32), #< The window with keyboard focus, if any 
	('text',c_char*32), #< The editing text 
	('start',c_int32), #< The start cursor of selected editing text 
	('length',c_int32)] #< The length of selected editing text 


#define SDL_TEXTINPUTEVENT_TEXT_SIZE (32)
'''*
 *  Keyboard text input event structure (event.text.*)
'''
class SDL_TextInputEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_TEXTINPUT 
	('timestamp',c_uint32), #
	('windowID',c_uint32), #< The window with keyboard focus, if any 
	('text',c_char*32)] #< The input text 


'''*
 *  Mouse motion event structure (event.motion.*)
'''
class SDL_MouseMotionEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_MOUSEMOTION 
	('timestamp',c_uint32), #
	('windowID',c_uint32), #< The window with mouse focus, if any 
	('which',c_uint32), #< The mouse instance id, or SDL_TOUCH_MOUSEID 
	('state',c_uint32), #< The current button state 
	('x',c_int32), #< X coordinate, relative to window 
	('y',c_int32), #< Y coordinate, relative to window 
	('xrel',c_int32), #< The relative motion in the X direction 
	('yrel',c_int32)] #< The relative motion in the Y direction 

'''*
 *  Mouse button event structure (event.button.*)
'''
class SDL_MouseButtonEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_MOUSEBUTTONDOWN or ::SDL_MOUSEBUTTONUP 
	('timestamp',c_uint32), #
	('windowID',c_uint32), #< The window with mouse focus, if any 
	('which',c_uint32), #< The mouse instance id, or SDL_TOUCH_MOUSEID 
	('button',c_uint8), #< The mouse button index 
	('state',c_uint8), #< ::SDL_PRESSED or ::SDL_RELEASED 
	('clicks',c_uint8), #< 1 for single-click, 2 for double-click, etc. 
	('padding1',c_uint8), #
	('x',c_int32), #< X coordinate, relative to window 
	('y',c_int32)] #< Y coordinate, relative to window 


'''*
 *  Mouse wheel event structure (event.wheel.*)
'''
class SDL_MouseWheelEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_MOUSEWHEEL 
	('timestamp',c_uint32), #
	('windowID',c_uint32), #< The window with mouse focus, if any 
	('which',c_uint32), #< The mouse instance id, or SDL_TOUCH_MOUSEID 
	('x',c_int32), #< The amount scrolled horizontally, positive to the right and negative to the left 
	('y',c_int32)] #< The amount scrolled vertically, positive away from the user and negative toward the user 


'''*
 *  Joystick axis motion event structure (event.jaxis.*)
'''
class SDL_JoyAxisEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_JOYAXISMOTION 
	('timestamp',c_uint32), #
	('which',c_int32), #< The joystick instance id 
	('axis',c_uint8), #< The joystick axis index 
	('padding1',c_uint8), #
	('padding2',c_uint8), #
	('padding3',c_uint8), #
	('value',c_int16), #< The axis value (range: -32768 to 32767) 
	('padding4',c_uint16)] #


'''*
 *  Joystick trackball motion event structure (event.jball.*)
'''
class SDL_JoyBallEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_JOYBALLMOTION 
	('timestamp',c_uint32), #
	('which',c_int32), #< The joystick instance id 
	('ball',c_uint8), #< The joystick trackball index 
	('padding1',c_uint8), #
	('padding2',c_uint8), #
	('padding3',c_uint8), #
	('xrel',c_int16), #< The relative motion in the X direction 
	('yrel',c_int16)] #< The relative motion in the Y direction 


'''*
 *  Joystick hat position change event structure (event.jhat.*)
'''
class SDL_JoyHatEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_JOYHATMOTION 
	('timestamp',c_uint32), #
	('which',c_int32), #< The joystick instance id 
	('hat',c_uint8), #< The joystick hat index 
	('value',c_uint8), #< The hat position value.
	('padding1',c_uint8), #
	('padding2',c_uint8)] #

'''*
 *  Joystick button event structure (event.jbutton.*)
'''
class SDL_JoyButtonEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_JOYBUTTONDOWN or ::SDL_JOYBUTTONUP 
	('timestamp',c_uint32), #
	('which',c_int32), #< The joystick instance id 
	('button',c_uint8), #< The joystick button index 
	('state',c_uint8), #< ::SDL_PRESSED or ::SDL_RELEASED 
	('padding1',c_uint8), #
	('padding2',c_uint8)] #

'''*
 *  Joystick device event structure (event.jdevice.*)
'''
class SDL_JoyDeviceEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_JOYDEVICEADDED or ::SDL_JOYDEVICEREMOVED 
	('timestamp',c_uint32), #
	('which',c_int32)] #< The joystick device index for the ADDED event, instance id for the REMOVED event 


'''*
 *  Game controller axis motion event structure (event.caxis.*)
'''
class SDL_ControllerAxisEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_CONTROLLERAXISMOTION 
	('timestamp',c_uint32), #
	('which',c_int32), #< The joystick instance id 
	('axis',c_uint8), #< The controller axis (SDL_GameControllerAxis) 
	('padding1',c_uint8), #
	('padding2',c_uint8), #
	('padding3',c_uint8), #
	('value',c_int16), #< The axis value (range: -32768 to 32767) 
	('padding4',c_uint16)] #


'''*
 *  Game controller button event structure (event.cbutton.*)
'''
class SDL_ControllerButtonEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_CONTROLLERBUTTONDOWN or ::SDL_CONTROLLERBUTTONUP 
	('timestamp',c_uint32), #
	('which',c_int32), #< The joystick instance id 
	('button',c_uint8), #< The controller button (SDL_GameControllerButton) 
	('state',c_uint8), #< ::SDL_PRESSED or ::SDL_RELEASED 
	('padding1',c_uint8), #
	('padding2',c_uint8)] #



'''*
 *  Controller device event structure (event.cdevice.*)
'''
class SDL_ControllerDeviceEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_CONTROLLERDEVICEADDED, ::SDL_CONTROLLERDEVICEREMOVED, or ::SDL_CONTROLLERDEVICEREMAPPED 
	('timestamp',c_uint32), #
	('which',c_int32)] #< The joystick device index for the ADDED event, instance id for the REMOVED or REMAPPED event 


'''*
 *  Touch finger event structure (event.tfinger.*)
'''
class SDL_TouchFingerEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_FINGERMOTION or ::SDL_FINGERDOWN or ::SDL_FINGERUP 
	('timestamp',c_uint32), #
	('touchId',c_int64), #< The touch device id 
	('fingerId',c_int64), #
	('x',c_float), #< Normalized in the range 0...1 
	('y',c_float), #< Normalized in the range 0...1 
	('dx',c_float), #< Normalized in the range 0...1 
	('dy',c_float), #< Normalized in the range 0...1 
	('pressure',c_float)] #< Normalized in the range 0...1 



'''*
 *  Multiple Finger Gesture Event (event.mgesture.*)
'''
class SDL_MultiGestureEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_MULTIGESTURE 
	('timestamp',c_uint32), #
	('touchId',c_int64), #< The touch device id 
	('dTheta',c_float), #
	('dDist',c_float), #
	('x',c_float), #
	('y',c_float), #
	('numFingers',c_uint16), #
	('padding',c_uint16)] #



'''*
 * Dollar Gesture Event (event.dgesture.*)
'''
class SDL_DollarGestureEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_DOLLARGESTURE 
	('timestamp',c_uint32), #
	('touchId',c_int64), #< The touch device id 
	('gestureId',c_int64), #
	('numFingers',c_uint32), #
	('error',c_float), #
	('x',c_float), #< Normalized center of gesture 
	('y',c_float)] #< Normalized center of gesture 



'''*
 *  An event used to request a file open by the system (event.drop.*)
 *         This event is disabled by default, you can enable it with SDL_EventState()
 *  \note If you enable this event, you must free the filename in the event.
'''
class SDL_DropEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_DROPFILE 
	('timestamp',c_uint32), #
	('file', c_char_p)] #< The file name, which should be freed with SDL_free() 



'''*
 *  The "quit requested" event
'''
class SDL_QuitEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_QUIT 
	('timestamp',c_uint32)] #


'''*
 *  OS Specific event
'''
class SDL_OSEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_QUIT 
	('timestamp',c_uint32)] #


'''*
 *  A user-defined event type (event.user.*)
'''
class SDL_UserEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_USEREVENT through ::SDL_LASTEVENT-1 
	('timestamp',c_uint32), #
	('windowID',c_uint32), #< The associated window if any 
	('code',c_int32), #< User defined event code 
	('data1',c_void_p), #< User defined data pointer 
	('data2',c_void_p)] #< User defined data pointer 

class SDL_SysWMEvent(Structure):
	_fields_ = [
	('type',c_uint32), #< ::SDL_SYSWMEVENT 
	('timestamp',c_uint32), #
	('msg',c_void_p)] #< driver dependent data, defined in SDL_syswm.h 


'''*
 *  General event structure
'''
class SDL_Event(Union):
	_fields_ = [
	('type',c_uint32), #< Event type, shared with all events 
	('common', SDL_CommonEvent), #< Common event data 
	('window', SDL_WindowEvent), #< Window event data 
	('key', SDL_KeyboardEvent), #< Keyboard event data 
	('edit', SDL_TextEditingEvent), #< Text editing event data 
	('text', SDL_TextInputEvent), #< Text input event data 
	('motion', SDL_MouseMotionEvent), #< Mouse motion event data 
	('button', SDL_MouseButtonEvent), #< Mouse button event data 
	('wheel', SDL_MouseWheelEvent), #< Mouse wheel event data 
	('jaxis', SDL_JoyAxisEvent), #< Joystick axis event data 
	('jball', SDL_JoyBallEvent), #< Joystick ball event data 
	('jhat', SDL_JoyHatEvent), #< Joystick hat event data 
	('jbutton', SDL_JoyButtonEvent), #< Joystick button event data 
	('jdevice', SDL_JoyDeviceEvent), #< Joystick device change event data 
	('caxis', SDL_ControllerAxisEvent), #< Game Controller axis event data 
	('cbutton', SDL_ControllerButtonEvent), #< Game Controller button event data 
	('cdevice', SDL_ControllerDeviceEvent), #< Game Controller device event data 
	('quit', SDL_QuitEvent), #< Quit request event data 
	('user', SDL_UserEvent), #< Custom event data 
	('syswm', SDL_SysWMEvent), #< System dependent window event data 
	('tfinger', SDL_TouchFingerEvent), #< Touch finger event data 
	('mgesture', SDL_MultiGestureEvent), #< Gesture event data 
	('dgesture', SDL_DollarGestureEvent), #< Gesture event data 
	('drop', SDL_DropEvent), #< Drag and drop event data 
	('padding',c_uint8*56)] #
