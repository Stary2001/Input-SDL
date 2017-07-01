#include "replay.h"

// initial valeus
char *REPLAY_FILENAME_PRE = "replay_";
char *REPLAY_PLAY_NAME = "...";
char *REPLAY_MSG = "";
const char *REPLAY_FILENAME_DEFAULT_PRE = "replay_";
const char *REPLAY_FILENAME_SUFFIX = ".replay";
const char *REPLAY_FOLDER = "./replay";
int RECORDING_TURN_ON = -1;
int REPLAY_PLAYING = -1;

FILE *file_replay_writer = NULL;
FILE *file_replay_reader = NULL;

FILE* get_new_file_replay()
{
	char *filename = NULL;
	uint32_t count = 0;
	do {
		asprintf(&filename, "%s/%s%d%s", REPLAY_FOLDER, REPLAY_FILENAME_PRE, count, REPLAY_FILENAME_SUFFIX);
		count++;
	} while(access(filename, W_OK) == 0);

	return fopen(filename, "wb");
}

struct command_raw create_raw_cmd(uint32_t hid_state, uint32_t circle_state, uint32_t cstick_state, uint32_t touch_state, uint32_t special_buttons)
{
	struct command_raw r;
	r.hid_state = hid_state;
	r.circle_state = circle_state;
	r.cstick_state = cstick_state;
	r.touch_state = touch_state;
	r.special_buttons = special_buttons;

	return r;
}

void save_command_replay(struct command_raw cmd_raw)
{
	char cmd[20];
	memcpy(cmd, &cmd_raw.hid_state, 4);
	memcpy(cmd + 4, &cmd_raw.touch_state, 4);
	memcpy(cmd + 8, &cmd_raw.circle_state, 4);
	memcpy(cmd + 12, &cmd_raw.cstick_state, 4);
	memcpy(cmd + 16, &cmd_raw.special_buttons, 4);

	fwrite(cmd, sizeof(cmd), 1, file_replay_writer);
}

void toggle_replay_recording()
{
	RECORDING_TURN_ON *= -1;

	if (RECORDING_TURN_ON == 1) {
		file_replay_writer = get_new_file_replay();
	}
	else {
		fclose(file_replay_writer);
		asprintf(&REPLAY_MSG, "%s", "INFO: Replay saved.");
	}
}

void toggle_replay_playing()
{
	REPLAY_PLAYING *= -1;
	strcpy(REPLAY_MSG, "");

	if (REPLAY_PLAYING == 1) {
		char *filename = NULL;
		asprintf(&filename, "%s/%s%s", REPLAY_FOLDER, REPLAY_PLAY_NAME, REPLAY_FILENAME_SUFFIX);
		if (!(file_replay_reader = fopen(filename, "rb"))) {
			toggle_replay_playing();
			asprintf(&REPLAY_MSG, "%s", "Error: Replay file does not exists!");
		}
	}
	else {
		fclose(file_replay_reader);
	}
}

int is_replay_recording()
{
	return RECORDING_TURN_ON == 1 ? 1 : 0;
}

int is_replay_playing()
{
	return REPLAY_PLAYING == 1 ? 1 : 0;
}

void modify_replay_default_name(char new_key)
{
	char *new_name = (char*) malloc(1*strlen(REPLAY_FILENAME_PRE));
	memcpy(new_name, REPLAY_FILENAME_PRE, strlen(REPLAY_FILENAME_PRE)+1);

	if(strcmp(REPLAY_FILENAME_PRE, "replay_") == 0) { // first time editing
		strcpy(new_name, "");
	}

	// add new char
	asprintf(&REPLAY_FILENAME_PRE, "%s%c\0", new_name, new_key);
}

void delete_replay_default_name()
{
	size_t len = strlen(REPLAY_FILENAME_PRE);
	memmove(REPLAY_FILENAME_PRE, REPLAY_FILENAME_PRE, len-1);
	REPLAY_FILENAME_PRE[len-1] = '\0';
}

void modify_replay_playing_name(char new_key)
{
	char *new_name = (char*) malloc(1*strlen(REPLAY_PLAY_NAME));
	memcpy(new_name, REPLAY_PLAY_NAME, strlen(REPLAY_PLAY_NAME)+1);

	if(strcmp(REPLAY_PLAY_NAME, "...") == 0) { // first time editing
		strcpy(new_name, "");
	}

	// add new char
	asprintf(&REPLAY_PLAY_NAME, "%s%c\0", new_name, new_key);
}

void delete_replay_playing_name()
{
	size_t len = strlen(REPLAY_PLAY_NAME);
	memmove(REPLAY_PLAY_NAME, REPLAY_PLAY_NAME, len-1);
	REPLAY_PLAY_NAME[len-1] = '\0';
}

void restore_replay_default_name()
{
	strcpy(REPLAY_FILENAME_PRE, REPLAY_FILENAME_DEFAULT_PRE);
}

void get_command_replay(char **v[20])
{
	char cmd[20];
	if(fread(cmd, sizeof(cmd), 1, file_replay_reader) != 1) {
		toggle_replay_playing();
		asprintf(&REPLAY_MSG, "%s", "INFO: Replay played with success.");
	}

	memcpy(v, &cmd, sizeof(cmd));
}
