#ifndef GM_S_LITTLE_HELPERS_FILE_HELPERS_H
#define GM_S_LITTLE_HELPERS_FILE_HELPERS_H

typedef struct binary_data_t binary_data_t;

struct binary_data_t {
  long size;
  unsigned char *data;
};

binary_data_t * read_file(const char *filename);

#endif //GM_S_LITTLE_HELPERS_FILE_HELPERS_H
