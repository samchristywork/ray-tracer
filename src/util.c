#include <stdio.h>

float clamp(float x, float lower, float upper) {
  if (x < lower)
    return lower;
  if (x > upper)
    return upper;
  return x;
}

void save_image_to_file(const unsigned char *image, const char *filename,
                        int width, int height) {
  FILE *fp = fopen(filename, "wb");
  if (!fp) {
    fprintf(stderr, "Failed to open output file for writing\n");
    return;
  }

  fprintf(fp, "P6\n%d %d\n255\n", width, height);
  fwrite(image, 1, width * height * 3, fp);
  fclose(fp);
}
