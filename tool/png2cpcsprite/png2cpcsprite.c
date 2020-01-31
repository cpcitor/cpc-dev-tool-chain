#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
#include <zlib.h>

int main(int argc, const char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr,
                "\n"
                "png2cpcsprite by Stéphane Gourichon (cpcitor).\n"
                "\n"
                "Convert a PNG image into a binary representation suitable for display\n"
                "by an Amstrad CPC hardware or emulator.\n"
                "\n"
                "usage: png2cpcsprite input-file.png mode output-file.bin\n"
                "\n"
                "* input-file.png must be an image in PNG format with a palette (colormap)\n"
                "* mode is cpc mode 0 1 or 2\n"
                "* the actual palette is ignored by this program\n"
            );
        exit(2);
    }

    png_image image;
    memset(&image, 0, (sizeof image));
    image.version = PNG_IMAGE_VERSION;
    image.opaque = NULL;

    {
        const char* const input_file_name=argv[1];
        printf("Will read from %s\n", input_file_name);

        /* The first argument is the file to read: */
        if (png_image_begin_read_from_file(&image, input_file_name) == 0)
        {
            fprintf(stderr, "png2cpcsprite: error: %s\n", image.message);
            exit(1);
        }
    }

    printf("Started decoding, found dimensions %u x %u, %u colors.\n", image.width, image.height, image.colormap_entries);

    png_bytep buffer, buffer_for_colormap;

    image.format = PNG_FORMAT_RGB_COLORMAP;

    {
        size_t buffer_size = PNG_IMAGE_SIZE(image);
        buffer = malloc(buffer_size);

        if (buffer == NULL)
        {
            fprintf(stderr, "png2cpcsprite: could not allocate %lu bytes for image", buffer_size);
            // Yes, we don't cleanup.  Quick and dirty!
            exit(1);
        }
    }

    {
        size_t colormap__size = PNG_IMAGE_COLORMAP_SIZE(image);
        buffer_for_colormap = malloc(colormap__size);

        if (buffer == NULL)
        {
            fprintf(stderr, "png2cpcsprite: could not allocate %lu bytes for colormap", colormap__size);
            // Yes, we don't cleanup.  Quick and dirty!
            exit(1);
        }
    }

    if (png_image_finish_read(&image, NULL/*background*/, buffer,
                              0/*row_stride*/, buffer_for_colormap) == 0)
    {
        fprintf(stderr, "png2cpcsprite: error: %s\n", image.message);
        // Yes, we don't cleanup.  Quick and dirty!
        exit(1);
    }

    printf("Finished decoding. Processing.\n");

    exit(0);
}
