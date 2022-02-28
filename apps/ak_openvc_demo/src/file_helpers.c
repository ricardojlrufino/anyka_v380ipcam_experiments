#include <stdio.h>
#include <malloc.h>
#include "file_helpers.h"


//Returns a binary_data_t structure if reading the file was OK.
//In case of an error it always returns NULL.
binary_data_t *read_file(const char *filename) {

  //Allocated our binary data structure
  binary_data_t *binary_data = malloc(sizeof(binary_data_t));
  if (binary_data != NULL) {

    binary_data->size = 0;
    void *buffer = NULL;
    long position;
    //Open the file for reading in binary mode
    FILE *fIn = fopen(filename, "rb");

    if (fIn != NULL) {
      //Go to the end of the file
      const int fseek_end_value = fseek(fIn, 0, SEEK_END);
      if (fseek_end_value != -1) {

        //Get the current position in the file (in bytes)
        position = ftell(fIn);
        if (position != -1) {

          //Go back to the beginning of the file
          const int fseek_set_value = fseek(fIn, 0, SEEK_SET);
          if (fseek_set_value != -1) {

            //Allocate enough space to read the whole file
            buffer = malloc(position);
            if (buffer != NULL) {

              //Read the whole file to buffer
              const long size = fread(buffer, 1, position, fIn);

              if (size == position) {
                binary_data->size = position;
                binary_data->data = buffer;

                fclose(fIn);
                return binary_data;
              }
              free(buffer);
            }
          }
        }
      }
      fclose(fIn);
    }
    free(binary_data);
  }
  return NULL;
}
