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
                "Currently the only output format supported is a generated "
                "assembly file, with symbols."
                "\n"
                "usage: png2cpcsprite input-file.png output-file.generated.s "
                "symbol_name mode deltay\n"
                "\n"
                "* input-file.png (mandatory) must be an image in PNG format "
                "with a palette (colormap)\n"
                "* output-file.generated.s (mandatory) name of output file\n"
                "* symbol_name (optional) assembly-level symbol for the "
                "generated file\n"
                "* mode (optional) is cpc mode 0 1 or 2, or empty string for "
                "guessing. \n"
                "  If empty string or not supplied, will be guessed from "
                "palette count.\n"
                "* deltay (optional). If 1 or not supplied, sprite data will "
                "be written top down.\n"
                "  If -1, sprite data will be written bottom up."
                "\n"
                "Notice: \n"
                "* The actual palette is ignored by this program.\n"
                "* The generated ASM includes symbols for dimensions, byte "
                "width, pixel width, height, size, and mode.\n"
                "\n");
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

u_int8_t guess_crtc_mode_based_on_colormap_entry_count(int colormap_entries)
{
        if (colormap_entries < 2)
        {
                fprintf(stderr, "Warning: strange image with less than 2 "
                                "colors in colormap, moving along anyway.\n");
        }
        if (colormap_entries == 2)
                return 2;
        if (colormap_entries <= 4)
                return 1;
        if (colormap_entries <= 16)
                return 0;
        fprintf(stderr, "Error: too many colors (%u) for the CPC, aborting.",
                colormap_entries);
        exit(1);
}

#define maxargs 5
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

int main(int argc, const char **argv)
{
        if ((argc < 3) || (argc > (maxargs + 1)))
        {
                fprintf(stderr,
                        "png2cpcsprite: "
                        "Error: between 2 and " STR(
                                maxargs) " arguments "
                                         "expected, got %u.",
                        argc - 1);
                show_usage();
                exit(2);
        }

        const char *const input_file_name = argv[1];
        png_image image;
        size_t buffer_size;

        png_bytep buffer = read_png(input_file_name, &image, &buffer_size);

        printf("Finished decoding PNG. Processing.\n");

        int crtc_mode;

        if ((argc >= 4) && (argv[3][0] != 0))
        {
                printf("CRTC mode apparently supplied on command line.\n");
                const char *const s_crtc_mode = argv[3];

                switch (s_crtc_mode[0])
                {
                case '0':
                case '1':
                case '2':
                        crtc_mode = s_crtc_mode[0] - 48;
                        break;

                default:
                        fprintf(stderr,
                                "png2cpcsprite: not a valid CPC mode: '%s'.\n",
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
        else
        {
                printf("CRTC mode not determined by command line, guessing "
                       "from colormap count (%u entries).\n",
                       image.colormap_entries);
                crtc_mode = guess_crtc_mode_based_on_colormap_entry_count(
                        image.colormap_entries);
        }

        printf("CRTC mode selected: %u.\n", crtc_mode);

        int deltay;

        if ((argc >= 6) && (argv[5][0] != 0))
        {
                printf("Deltay apparently supplied on command line.\n");
                if (sscanf(argv[5], "%d", &deltay) == 0)
                {
                        fprintf(stderr,
                                "png2cpcsprite: aborting: failure to parse "
                                "deltay from: '%s'.\n",
                                argv[5]);
                        exit(1);
                }
        }
        else
        {
                printf("deltay not determined by command line, using 1\n");
                deltay = 1;
        }

        printf("Deltay selected: %d.\n", deltay);

        if (deltay * deltay != 1)
        {
                fprintf(stderr,
                        "png2cpcsprite: "
                        "Error: deltay is not 1 or -1 but %d.",
                        deltay);
                exit(1);
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

        if (crtc_mode != 1)
        {
                fprintf(stderr,
                        "png2cpcsprite: error: only mode 1 supported at this "
                        "time, not %d.",
                        crtc_mode);
                // Yes, we don't cleanup.  Quick and dirty!
                exit(1);
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
                                cpc_byte = cpc_byte << 1;
                                cpc_byte |= (color_palette_index & 1) |
                                            ((color_palette_index & 2) << 3);
                        }

                        *w = cpc_byte;
                        w++;
                }

                if (r != buffer + buffer_size)
                {
                        fprintf(stderr,
                                "png2cpcsprite: warning: did not consume "
                                "exacly all %lu bytes "
                                "of input buffer, actually %lu (%p != %p).\n",
                                buffer_size, w - buffer, w,
                                buffer + buffer_size);
                }

                if (w != sprite_buffer + sprite_bytes)
                {
                        fprintf(stderr,
                                "png2cpcsprite: warning: did not produce "
                                "exacly the expected %u bytes "
                                "of sprite data, actually %lu (%p != %p).\n",
                                sprite_bytes, w - sprite_buffer, w,
                                sprite_buffer + sprite_bytes);
                }
        }

        const char *const output_file_name = argv[2];

        printf("Generated %u bytes "
               "of sprite data, will write them to output file '%s'.\n",
               sprite_bytes, output_file_name);

        const char *symbol_name;
        if (argc >= 5)
        {
                symbol_name = argv[4];
                printf("Symbol name supplied on command line: '%s'.\n",
                       symbol_name);
        }
        else
        {
                printf("No symbol name supplied on command line.\n");

                const char *last_part_of_input_file_name =
                        strrchr(input_file_name, '/');
                if (last_part_of_input_file_name == NULL)
                {
                        last_part_of_input_file_name = input_file_name;
                }
                else
                {
                        last_part_of_input_file_name++;
                }

                char *pre_symbol_name = strdup(last_part_of_input_file_name);

                /*
                  From
             cpc-dev-tool-chain/tool/sdcc/sdcc-3.9.0/sdas/doc/asmlnk.txt

             1.  Symbols  can  be  composed  of alphanumeric characters,
                 dollar signs ($),  periods  (.),  and  underscores  (_)
                 only.

             2.  The  first  character  of a symbol must not be a number
                 (except in the case of reusable symbols).

                 */

                char *symbol_name_end =
                        pre_symbol_name + strlen(pre_symbol_name);

                const char *valid_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghij"
                                          "klmnopqrstuvwxyz0123456789$._";

                {
                        char *p = pre_symbol_name;
                        while (p != symbol_name_end)
                        {
                                size_t advance = strspn(p, valid_chars);
                                p += advance;
                                if ((*p) != 0)
                                {
                                        *p = '_';
                                }
                        }
                }

                char symbol_name_auto[80];

                snprintf(symbol_name_auto, 80, "%s%s", "sprite_",
                         pre_symbol_name);
                symbol_name_auto[79] = 0;

                symbol_name = symbol_name_auto;
        }

        printf("Will use symbol name '%s'\n", symbol_name);

        FILE *output_file = fopen(output_file_name, "w");

        if (output_file == NULL)
        {
                fprintf(stderr,
                        "png2cpcsprite: error: could not open output file "
                        "'%s'.",
                        output_file_name);
                // Yes, we don't cleanup.  Quick and dirty!
                exit(1);
        }

        fprintf(output_file, ".module %s_module\n\n", symbol_name);

        fprintf(output_file, "%s_bytes == 0x%04x\n", symbol_name, sprite_bytes);
        fprintf(output_file, "%s_height == %d\n", symbol_name, image.height);
        fprintf(output_file, "%s_pixels_per_line == %d\n", symbol_name,
                width_pixels);
        fprintf(output_file, "%s_bytes_per_line == %d\n", symbol_name,
                width_bytes);

        fprintf(output_file, "%s_data::\n", symbol_name);

        {
                u_int8_t *b = sprite_buffer;
                u_int8_t ymin = (deltay>0)
                for (size_t counter = 0; counter < sprite_bytes; counter++)
                {
                for (size_t counter = 0; counter < sprite_bytes; counter++)
                {
                        u_int8_t byte = *(b++);

                        if (counter % 12 == 0) // FIXME should not hardcode 12.
                        {
                                fprintf(output_file, "\n\t.byte ");
                        }
                        else
                        {
                                fprintf(output_file, ", ");
                        }

                        fprintf(output_file, "0x%02x", byte);
                }

                fprintf(output_file, "\n");
        }

        fclose(output_file);

        printf("Finished writing file '%s'.\n", output_file_name);

        printf("Success. Exiting.\n");

        exit(0);
}
