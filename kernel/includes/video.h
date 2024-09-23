#ifndef VIDEO_H
#define VIDEO_H

#include <cstdint>

extern int cursor_x;
extern int cursor_y;
extern const int VGA_WIDTH;
extern const int VGA_HEIGHT;
extern uint16_t *VideoMemory;

#ifdef __cplusplus
extern "C"
{
#endif

	void clear_screen();
	void move_cursor();
	void kernel_printf(const char *format, ...);
	void test_message(const char *message, bool success);

#ifdef __cplusplus
}
#endif

#endif
