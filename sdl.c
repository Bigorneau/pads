#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

int open_joysticks()
{
	int i, size;
	SDL_Joystick *joy;

	/* initialize the joystick subsystem */
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	/* Check for joystick */
	size = SDL_NumJoysticks();
	if (size > 0) {
		for (i = 0; i <= size ;i++) {
			joy = SDL_JoystickOpen(i);
			if (joy) {
				printf("joystick %d:\n"
						"  name='%s',  axes=%d,"
						"  buttons=%d,  balls=%d,"
						"  hats=%d\n",
						i,
						SDL_JoystickNameForIndex(i),
						SDL_JoystickNumAxes(joy),
						SDL_JoystickNumButtons(joy),
						SDL_JoystickNumBalls(joy),
						SDL_JoystickNumHats(joy));
			}
		}
	}
	return 0;
}

int close_joysticks()
{
	int i, size;
	SDL_Joystick *joy;

	size = SDL_NumJoysticks();
	if (size > 0) {
		for (i = 0; i <= size ;i++) {
			joy = SDL_JoystickOpen(i);
			if (SDL_JoystickGetAttached(joy))
				SDL_JoystickClose(joy);
		}
	}
	return 0;
}

int event_quit(SDL_QuitEvent *quit)
{
	return 0;
}

int event_app_terminating()
{
	return 0;
}

int event_window(SDL_WindowEvent *window)
{
	return 0;
}

int event_system_window(SDL_SysWMEvent *syswm)
{
	return 0;
}

int event_keyboard(SDL_KeyboardEvent *keyboard, bool down)
{
	return 0;
}

int event_textedit(SDL_TextEditingEvent *edit)
{
	return 0;
}

int event_textinput(SDL_TextInputEvent *text)
{
	return 0;
}

int event_mousemotion(SDL_MouseMotionEvent *motion)
{
	return 0;
}

int event_mousebutton(SDL_MouseButtonEvent *button, bool down)
{
	return 0;
}

int event_mousewheel(SDL_MouseWheelEvent *wheel)
{
	return 0;
}

#define JOYAXIS_THRESHOLD_FACTOR 0.1
#define JOYAXIS_THRESHOLD (JOYAXIS_THRESHOLD_FACTOR * 32700) /* range: -32768 to 32767 */

int event_joyaxis(SDL_JoyAxisEvent *jaxis)
{
	if (!jaxis)
		return -1;
	if ((jaxis->value < -JOYAXIS_THRESHOLD) || (jaxis->value > JOYAXIS_THRESHOLD)) {
		printf("%s:  ", __FUNCTION__);
		printf("%u:  joystick=%d,  axis=%d,  value=%d\n",
				jaxis->timestamp, jaxis->which,
				jaxis->axis, jaxis->value);
	}
	return 0;
}

int event_joyball(SDL_JoyBallEvent *jball)
{
	if (!jball)
		return -1;
	printf("%s:  ", __FUNCTION__);
	printf("%u:  joystick=%d,  ball=%d,  xrel=%d,  yrel=%d\n",
			jball->timestamp, jball->which,
			jball->xrel, jball->yrel);
	return 0;
}

int event_joyhat(SDL_JoyHatEvent *jhat)
{
	if (!jhat)
		return -1;
	printf("%s:  ", __FUNCTION__);
	printf("%u:  joystick=%d,  hat=%d,  value=%d\n",
			jhat->timestamp, jhat->which, jhat->value);
	return 0;
}

int event_joybutton(SDL_JoyButtonEvent *jbutton, bool down)
{
	if (!jbutton)
		return -1;
	printf("%s:  ", __FUNCTION__);
	printf("%u:  joystick=%d,  hat=%d,  button=%d,  state=%d\n",
			jbutton->timestamp, jbutton->which,
			jbutton->button, jbutton->state);
	return 0;
}

int event_joydevice(SDL_JoyDeviceEvent *jdevice, bool added)
{
	if (!jdevice)
		return -1;
	printf("%s:  ", __FUNCTION__);
	printf("%u:  joystick=%d,  %d",
			jdevice->timestamp, jdevice->which,
			added?"ADDED":"REMOVED");
	return 0;
}

int event_controlleraxis(SDL_ControllerAxisEvent *caxis)
{
	if (!caxis)
		return -1;
	printf("%s:  ", __FUNCTION__);
	printf("%u:  joystick=%d,  axis=%d,  value=%d\n",
			caxis->timestamp, caxis->which, caxis->axis, caxis->value);
	return 0;
}

int event_controllerbutton(SDL_ControllerButtonEvent *cbutton, bool down)
{
	if (!cbutton)
		return -1;
	printf("%s:  ", __FUNCTION__);
	printf("%u:  joystick=%d,  button=%d,  state=%d,  %s\n",
			cbutton->timestamp, cbutton->which, cbutton->button,
			cbutton->state, down?"DOWN":"UP");
	return 0;
}

int event_controllerdevice(SDL_ControllerDeviceEvent *cdevice, bool added)
{
	if (!cdevice)
		return -1;
	printf("%s:  ", __FUNCTION__);
	printf("%u:  joystick=%d,  %s\n",
			cdevice->timestamp, cdevice->which,
			added?"ADDED":"REMOVED");

	return 0;
}

int event_controller_remapped(SDL_ControllerDeviceEvent *cdevice)
{
	if (!cdevice)
		return -1;
	printf("%s:  ", __FUNCTION__);
	printf("%u:  joystick=%d,  REMAPPED\n",
			cdevice->timestamp, cdevice->which);
	return 0;
}

int event_dropfile(SDL_DropEvent *drop)
{
	return 0;
}

int event_user(SDL_UserEvent *user)
{
	return 0;
}

int listen_events()
{
	SDL_Event event;
	while (1) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				event_quit(&event.quit);
				return 0;
			case SDL_APP_TERMINATING:
				event_app_terminating();
				break;
			/* android and iOS */
			case SDL_APP_LOWMEMORY:
			case SDL_APP_WILLENTERBACKGROUND:
			case SDL_APP_DIDENTERBACKGROUND:
			case SDL_APP_WILLENTERFOREGROUND:
			case SDL_APP_DIDENTERFOREGROUND:
			break;
			/* window */
			case SDL_WINDOWEVENT:
				event_window(&event.window);
				break;
			break;
			case SDL_SYSWMEVENT:
				event_system_window(&event.syswm);
				break;
			break;
			/* keyboard */
			case SDL_KEYDOWN:
				event_keyboard(&event.key, true);
				break;
			case SDL_KEYUP:
				event_keyboard(&event.key, false);
				break;
			case SDL_TEXTEDITING:
				event_textedit(&event.edit);
				break;
			case SDL_TEXTINPUT:
				event_textinput(&event.text);
				break;
			break;
			/* mouse */
			case SDL_MOUSEMOTION:
				event_mousemotion(&event.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
				event_mousebutton(&event.button, true);
				break;
			case SDL_MOUSEBUTTONUP:
				event_mousebutton(&event.button, false);
				break;
			case SDL_MOUSEWHEEL:
				event_mousewheel(&event.wheel);
				break;
			break;
			/* joystick */
			case SDL_JOYAXISMOTION:
				event_joyaxis(&event.jaxis);
				break;
			break;
			case SDL_JOYBALLMOTION:
				event_joyball(&event.jball);
				break;
			case SDL_JOYHATMOTION:
				event_joyhat(&event.jhat);
				break;
			case SDL_JOYBUTTONDOWN:
				event_joybutton(&event.jbutton, true);
				break;
			case SDL_JOYBUTTONUP:
				event_joybutton(&event.jbutton, false);
				break;
			case SDL_JOYDEVICEADDED:
				event_joydevice(&event.jdevice, true);
				break;
			case SDL_JOYDEVICEREMOVED:
				event_joydevice(&event.jdevice, false);
				break;
			break;
			/* game controller */
			case SDL_CONTROLLERAXISMOTION:
				event_controlleraxis(&event.caxis);
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				event_controllerbutton(&event.cbutton, true);
				break;
			case SDL_CONTROLLERBUTTONUP:
				event_controllerbutton(&event.cbutton, false);
				break;
			case SDL_CONTROLLERDEVICEADDED:
				event_controllerdevice(&event.cdevice, true);
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				event_controllerdevice(&event.cdevice, false);
				break;
			case SDL_CONTROLLERDEVICEREMAPPED:
				event_controller_remapped(&event.cdevice);
				break;
			/* touch */
			case SDL_FINGERDOWN:
			case SDL_FINGERUP:
			case SDL_FINGERMOTION:
				break;
			/* clipboard */
			case SDL_CLIPBOARDUPDATE:
				break;
			/* drag and drop */
			case SDL_DROPFILE:
				event_dropfile(&event.drop);
				break;
			/* user */
			case SDL_USEREVENT:
				event_user(&event.user);
				break;
			default:
				printf("type=%d\n", event.type);
				break;
			}
			usleep(2000);
		}
		usleep(200);
	}
	return 0;
}

int main(void)
{
	open_joysticks();
	listen_events();
	close_joysticks();
	return 0;
}
