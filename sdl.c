#include <stdio.h>
#include <unistd.h>

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

#define JOYAXIS_THRESHOLD_FACTOR 0.1
#define JOYAXIS_THRESHOLD (JOYAXIS_THRESHOLD_FACTOR * 32700) /* range: -32768 to 32767 */

int event_joyaxis(SDL_JoyAxisEvent *jaxis)
{
	if (!jaxis)
		return -1;
	if ((jaxis->value < -JOYAXIS_THRESHOLD) || (jaxis->value > JOYAXIS_THRESHOLD))
		printf("%u:  joystick=%d,  axis=%d,"
				"  padding1=%d,  padding2=%d,"
				"  padding3=%d,  padding4=%d,"
				"  value=%d\n",
				jaxis->timestamp, jaxis->which,
				jaxis->axis, jaxis->padding1,
				jaxis->padding2, jaxis->padding3,
				jaxis->padding4, jaxis->value);
	return 0;
}

int listen_events()
{
	SDL_Event event;
	while (1) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				return 0;
			break;
			case SDL_JOYAXISMOTION:
				event_joyaxis(&event.jaxis);
			break;
			case SDL_JOYBALLMOTION:
			case SDL_JOYHATMOTION:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
			case SDL_JOYDEVICEADDED:
			case SDL_JOYDEVICEREMOVED:
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
