#ifndef _REPLAY_H
#define _REPLAY_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

extern char replay_filename_prefix[50];
extern char replay_play_name[50];
extern char replay_msg[100];
extern const char *REPLAY_FILENAME_DEFAULT_PRE;
extern const char *REPLAY_FILENAME_SUFFIX;
extern const char *REPLAY_FOLDER;
extern int recording_turn_on;
extern int replay_playing;

extern FILE *file_replay_writer;
extern FILE *file_replay_reader;


FILE* get_new_file_replay();
struct timespec get_current_time();
void save_command_replay(char **v[20]);
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
