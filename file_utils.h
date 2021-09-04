#ifndef _FILE_UTILS_H
#define _FILE_UTILS_H

#include "FS.h"
#include "SPIFFS.h"

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

void FS_listDir(fs::FS &fs, const char * dirname, uint8_t levels);
int16_t FS_readFile(fs::FS &fs, const char * path, char output_buffer[]);
void FS_writeFile(fs::FS &fs, const char * path, const char * message);
void FS_appendFile(fs::FS &fs, const char * path, const char * message);
void FS_renameFile(fs::FS &fs, const char * path1, const char * path2);
void FS_deleteFile(fs::FS &fs, const char * path);
bool FS_setup();
void FS_test();

#endif
