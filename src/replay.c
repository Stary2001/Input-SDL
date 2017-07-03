#include "replay.h"

// initial valeus
char replay_filename_prefix[50] = "replay_";
char replay_play_name[50] = "...";
char replay_msg[100] = "";
const char *REPLAY_FILENAME_DEFAULT_PRE = "replay_";
const char *REPLAY_FILENAME_SUFFIX = ".replay";
const char *REPLAY_FOLDER = "./replay";
int recording_turn_on = 0;
int replay_playing = 0;

FILE *file_replay_writer = NULL;
FILE *file_replay_reader = NULL;

FILE* get_new_file_replay()
{
	char filename[150];
	int count_char;
	uint32_t count = 0;
	do {
		count_char = snprintf(filename, sizeof(filename), "%s/%s%d%s",
								REPLAY_FOLDER, replay_filename_prefix, count, REPLAY_FILENAME_SUFFIX);
		count++;

		if (count_char > sizeof(filename)) {
			snprintf(replay_msg, sizeof(replay_msg), "%s", "Error: Replay's name too big.");
			return NULL;
		}
	} while(access(filename, W_OK) == 0);

	return fopen(filename, "wb");
}

void save_command_replay(char **v[20])
{
	char cmd[20] = "";

	memcpy(cmd, v, sizeof(cmd));
	fwrite(cmd, sizeof(cmd), 1, file_replay_writer);
}

void toggle_replay_recording()
{
	recording_turn_on = !recording_turn_on;
	replay_msg[0] = '\0';

	if (recording_turn_on == 1) {
		if (!(file_replay_writer = get_new_file_replay())) {
			recording_turn_on = !recording_turn_on;
		}
	}
	else {
		fclose(file_replay_writer);
		snprintf(replay_msg, sizeof(replay_msg), "%s", "Info: Replay saved.");
	}
}

void toggle_replay_playing()
{
	replay_playing = !replay_playing;
	replay_msg[0] = '\0';

	if (replay_playing == 1) {
		char filename[150] = "";
		int count_char;
		count_char = snprintf(filename, sizeof(filename), "%s/%s%s", REPLAY_FOLDER, replay_play_name, REPLAY_FILENAME_SUFFIX);

		if (count_char > sizeof(filename)) {
			toggle_replay_playing();
			snprintf(replay_msg, sizeof(replay_msg), "%s", "Error: Can't play replay, name too big.");
			return;
		}

		if (!(file_replay_reader = fopen(filename, "rb"))) {
			toggle_replay_playing();
			snprintf(replay_msg, sizeof(replay_msg), "%s", "Error: Replay file does not exist!");
			return;
		}

		snprintf(replay_msg, sizeof(replay_msg), "%s", "Playing replay...");
	}
	else {
		fclose(file_replay_reader);
	}
}

int is_replay_recording()
{
	return recording_turn_on;
}

int is_replay_playing()
{
	return replay_playing;
}

void modify_replay_default_name(char new_key)
{
	if(strcmp(replay_filename_prefix, REPLAY_FILENAME_DEFAULT_PRE) == 0) { // first time editing
		replay_filename_prefix[0] = '\0';
	}

	size_t len = strlen(replay_filename_prefix);
	snprintf(replay_filename_prefix+len, sizeof(replay_filename_prefix)-len, "%c", new_key);
}

void delete_replay_default_name()
{
	size_t len = strlen(replay_filename_prefix);
	replay_filename_prefix[len-1] = '\0';
}

void modify_replay_playing_name(char new_key)
{
	if(strcmp(replay_play_name, "...") == 0) { // first time editing
		replay_play_name[0] = '\0';
	}

	size_t len = strlen(replay_play_name);
	snprintf(replay_play_name+len, sizeof(replay_play_name)-len, "%c", new_key);
}

void delete_replay_playing_name()
{
	size_t len = strlen(replay_play_name);
	replay_play_name[len-1] = '\0';
}

void restore_replay_default_name()
{
	strcpy(replay_filename_prefix, REPLAY_FILENAME_DEFAULT_PRE);
}

void get_command_replay(char **v[20])
{
	char cmd[20];
	if(fread(cmd, sizeof(cmd), 1, file_replay_reader) != 1) {
		toggle_replay_playing();
		snprintf(replay_msg, sizeof(replay_msg), "%s", "Info: Replay played with success.");
	}

	memcpy(v, &cmd, sizeof(cmd));
}
