#ifndef UTIL_H
#define UTIL_H

float clamp(float x, float lower, float upper);

void save_image_to_file(const unsigned char *image, const char *filename,
                        int width, int height);

#endif
