#ifndef STDDEF_H
#define STDDEF_H

// const strings
static const char DEF_FMT[] = ".osync";

// max
#define MAX_FILENAME_LENGTH 4096
#define MAX_PATH_LENGTH 8192

// stderrs
#define ERR_PARSE 5
#define OPN_FILE 4
#define MK_FILE 3
#define OPN_DIR 1
#define MK_DIR 2

// default errors
#define DEF_ERR 1
#define DEF_OK 0

// other
#define PREF_DIR "config_attributes"
#define TRACK_FILE "trackfile.txt"
#define CONFIG "config.osync"
#define STD_FOLDER ".osync"

// functions
int ThrowError(char*, int);
int Parse(char*);

#endif
