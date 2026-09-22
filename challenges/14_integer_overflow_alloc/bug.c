#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdint.h>
#include <stdint.h>
#include <stdint.h>
#include <stdint.h>

typedef struct {
    int width;
    int height;
    int channels;
    size_t nbytes;
    unsigned char *px;
} Image;

static Image *image_new(int width, int height, int channels) {
    Image *img = malloc(sizeof *img);
    if (!img) {
        perror("malloc");
        exit(1);
    }

    img->width = width;
    img->height = height;
    img->channels = channels;

    size_t w = (size_t)width;
    size_t h = (size_t)height;
    size_t c = (size_t)channels;

    if (w != 0 && h > SIZE_MAX / w) {
        fprintf(stderr, "image size overflow\n");
        free(img);
        exit(1);
    }

    size_t wh = w * h;

    if (c != 0 && wh > SIZE_MAX / c) {
        fprintf(stderr, "image size overflow\n");
        free(img);
        exit(1);
    }

    img->nbytes = wh * c;

    img->px = malloc(img->nbytes);
    if (!img->px && img->nbytes != 0) {
        perror("malloc px");
        free(img);
        exit(1);
    }

    return img;
}

static void image_fill(Image *img, unsigned char value) {
    size_t total =
        (size_t)img->width *
        (size_t)img->height *
        (size_t)img->channels;

    for (size_t i = 0; i < total; i++)
        img->px[i] = value;
}

int main(void) {
    Image *img = image_new(65536, 65536, 4);

    printf("allocated nbytes=%zu for %dx%d x%d\n",
           img->nbytes, img->width, img->height, img->channels);

    image_fill(img, 0xFF);

    printf("px[0]=%u\n", img->px[0]);

    free(img->px);
    free(img);

    return 0;
}