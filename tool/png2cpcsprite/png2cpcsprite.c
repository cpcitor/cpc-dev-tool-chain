#include <png.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

void show_usage()
{
        fprintf(stderr,
                "\n"
                "png2cpcsprite by Stéphane Gourichon (cpcitor).\n"
                "\n"
                "Convert a PNG image into a binary representation suitable for "
                "display by an Amstrad CPC hardware or emulator.\n"
                "\n"
                "Currently the only output format supported is a .rel file, "
                "just like the SDCC object (relocatable) output format that "
                "SDCC generates from C or ASM source files."
                "\n"
                "usage: png2cpcsprite input-file.png output-file.rel mode\n"
                "\n"
                "* input-file.png must be an image in PNG format with a "
                "  palette (colormap)\n"
                "* output-file.rel name of output file\n"
                "* mode is cpc mode 0 1 or 2\n"
                "Notice: the actual palette is ignored by this program\n");
}

png_bytep read_png(const char *const input_file_name, png_image *image,
                   size_t *buffer_size)
{
        memset(image, 0, (sizeof *image));
        image->version = PNG_IMAGE_VERSION;
        image->opaque = NULL;

        {
                printf("Will read from %s\n", input_file_name);

                /* The first argument is the file to read: */
                if (png_image_begin_read_from_file(image, input_file_name) == 0)
                {
                        fprintf(stderr, "png2cpcsprite: error: %s\n",
                                image->message);
                        exit(1);
                }
        }

        printf("Started decoding, found dimensions %u x %u, %u colors.\n",
               image->width, image->height, image->colormap_entries);

        png_bytep buffer, buffer_for_colormap;

        image->format = PNG_FORMAT_RGB_COLORMAP;

        *buffer_size = PNG_IMAGE_SIZE(*image);
        buffer = malloc(*buffer_size);

        if (buffer == NULL)
        {
                fprintf(stderr,
                        "png2cpcsprite: could not allocate %lu bytes "
                        "for image",
                        *buffer_size);
                // Yes, we don't cleanup.  Quick and dirty!
                exit(1);
        }

        {
                size_t colormap__size = PNG_IMAGE_COLORMAP_SIZE(*image);
                buffer_for_colormap = malloc(colormap__size);

                if (buffer == NULL)
                {
                        fprintf(stderr,
                                "png2cpcsprite: could not allocate %lu bytes "
                                "for colormap",
                                colormap__size);
                        // Yes, we don't cleanup.  Quick and dirty!
                        exit(1);
                }
        }

        if (png_image_finish_read(image, NULL /*background*/, buffer,
                                  0 /*row_stride*/, buffer_for_colormap) == 0)
        {
                fprintf(stderr, "png2cpcsprite: error: %s\n", image->message);
                // Yes, we don't cleanup.  Quick and dirty!
                exit(1);
        }

        return buffer;
}

int main(int argc, const char **argv)
{
        if (argc != 4)
        {
                show_usage();
                exit(2);
        }

        const char *const input_file_name = argv[1];
        png_image image;
        size_t buffer_size;

        png_bytep buffer = read_png(input_file_name, &image, &buffer_size);

        printf("Finished decoding. Processing.\n");

        int crtc_mode;

        {
                const char *const s_crtc_mode = argv[2];

                switch (s_crtc_mode[0])
                {
                case '0':
                case '1':
                case '2':
                        crtc_mode = s_crtc_mode[0] - 48;
                        break;

                default:
                        fprintf(stderr,
                                "png2cpcsprite: mode must be single character, "
                                "not '%s'.\n",
                                s_crtc_mode);
                        exit(1);
                }

                if (s_crtc_mode[1])
                {
                        fprintf(stderr,
                                "png2cpcsprite: invalid mode, must be single "
                                "character, not '%s'.\n",
                                s_crtc_mode);
                        exit(1);
                }
        }

        unsigned int width_bytes = image.width >> (crtc_mode + 1);

        unsigned int width_pixels = width_bytes << (crtc_mode + 1);

        if (width_pixels != image.width)
        {
                fprintf(stderr,
                        "png2cpcsprite: "
                        "Error: in the selected CPC mode %u, "
                        "image width %u "
                        "make around %u bytes "
                        "which will expand to %u pixels, not %u.",
                        crtc_mode, image.width, width_bytes, width_pixels,
                        image.width);
                exit(1);
        }

        unsigned int sprite_bytes = width_bytes * image.height;

        printf("Will generate a sprite representation for CRTC mode %u, "
               "width %u pixels (%u bytes), height %u lines, total %u bytes.\n",
               crtc_mode, image.width, width_bytes, image.height, sprite_bytes);

        u_int8_t *sprite_buffer;
        {
                sprite_buffer = malloc(sprite_bytes);

                if (buffer == NULL)
                {
                        fprintf(stderr,
                                "png2cpcsprite: could not allocate %u bytes "
                                "for sprite buffer",
                                sprite_bytes);
                        // Yes, we don't cleanup.  Quick and dirty!
                        exit(1);
                }
        }

        {
                u_int8_t *r = buffer;
                u_int8_t *w = sprite_buffer;
                for (size_t counter = 0; counter < sprite_bytes; counter++)
                {
                        u_int8_t cpc_byte = 0;

                        // mode 1 only for now
                        for (int pixel_in_byte = 0; pixel_in_byte < 4;
                             pixel_in_byte++)
                        {
                                u_int8_t color_palette_index = *(r++);
                                cpc_byte |= (color_palette_index & 1) |
                                            ((color_palette_index & 2) << 3);
                                cpc_byte = cpc_byte << 1;
                        }

                        *w = cpc_byte;
                        w++;
                }

                if (r != buffer + buffer_size)
                {
                        fprintf(stderr,
                                "png2cpcsprite: warning: did not consume "
                                "exacly all %u bytes "
                                "of input buffer, actually %lu (%lu != %lu).\n",
                                buffer_size, w - buffer, w,
                                buffer + buffer_size);
                }

                if (w != sprite_buffer + sprite_bytes)
                {
                        fprintf(stderr,
                                "png2cpcsprite: warning: did not produce "
                                "exacly the expected %u bytes "
                                "of sprite data, actually %lu (%lu != %lu).\n",
                                sprite_bytes, w - sprite_buffer, w,
                                sprite_buffer + sprite_bytes);
                }
        }

        const char *const output_file_name = argv[2];
        
        printf(
                                "Generated %u bytes "
                                "of sprite data, will write them to .\n",
                                sprite_bytes, w - sprite_buffer, w,
                                sprite_buffer + sprite_bytes);

        
        write(1, sprite_buffer, sprite_bytes);

        fopen(
XL2
H 1 areas 1 global symbols
S .__.ABS. Def0000
A _CODE size 3C flags 0 addr 0
T 00 00 01 02 03 04 05 06 07 08 09 0A 01 02 03 04
R 00 00 00 00
T 0E 00 05 06 07 08 09 0A 01 02 03 04 05 06 07 08
R 00 00 00 00
T 1C 00 09 0A 01 02 03 04 05 06 07 08 09 0A 01 02
R 00 00 00 00
T 2A 00 03 04 05 06 07 08 09 40 01 02 03 04 05 06
R 00 00 00 00
T 38 00 07 08 09 0A
R 00 00 00 00

        
        exit(0);
}
