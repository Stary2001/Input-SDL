#ifndef _REPLAY_H
#define _REPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

extern char *REPLAY_FILENAME_PRE;
extern char *REPLAY_PLAY_NAME;
extern char *REPLAY_MSG;
extern const char *REPLAY_FILENAME_DEFAULT_PRE;
extern const char *REPLAY_FILENAME_SUFFIX;
extern const char *REPLAY_FOLDER;
extern int RECORDING_TURN_ON;
extern int REPLAY_PLAYING;

extern FILE *file_replay_writer;
extern FILE *file_replay_reader;

struct command_raw
{
	uint32_t hid_state;
	uint32_t circle_state;
	uint32_t cstick_state;
	uint32_t touch_state;
	uint32_t special_buttons;
};


FILE* get_new_file_replay();
struct timespec get_current_time();
void save_command_replay(struct command_raw cmd_raw);
void toggle_replay_recording();
int is_replay_recording();
void modify_replay_default_name(char new_key);
void delete_replay_default_name();
void modify_replay_playing_name(char new_key);
void delete_replay_playing_name();
void restore_replay_default_name();
void get_command_replay(char **v[20]);

struct command_raw create_raw_cmd(uint32_t hid_state, uint32_t circle_state, uint32_t cstick_state, uint32_t touch_state, uint32_t special_buttons);

#endif
