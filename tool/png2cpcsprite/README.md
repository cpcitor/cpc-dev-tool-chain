# png2cpcsprite by Stéphane Gourichon (cpcitor).

## Summary: what it does

Convert a PNG image into a binary representation suitable for display by an
Amstrad CPC hardware or emulator (sprite data, up to full screen overscan),
expressed as assembly source code.

## Summary: what it does not

png2cpcsprite does not perform image manipulations (beyond basic color
remapping), as it is a better design to do those upstream.

png2cpcsprite performs a downstream step (format conversion), which comes after
the upstream step which is generating or adjusting any image for the CPC target
as PNG format.  Upstream step is typically an interactive work.  Separating
steps allows you to use any program, e.g. interactive like gimp or scripts
using ImageMagick drawing facilities (e.g. for fonts), to adjust input images
and save them as assets in standard PNG format.

png2cpcsprite then comes downstream. It is intended to be part of an automated
worfklow/build from source assets in editable format to your
ready-to-run-on-CPC production (demo, game), for example it is integrated in
https://github.com/cpcitor/cpc-dev-tool-chain.

## Output

The output is designed for maximum flexibility, consuming only runtime
resources that you actually need.  Output format is an assembly source text
file with data as bytes and metadata available as symbols: bytes, height,
pixels_per_line, bytes_per_line, palette_count and as many palette_ink_* as
needed.

A flexible prefix scheme for symbol names is used to prevent any symbol name
conflict.  Similarly for module names, this allows you to hint your linker
about how you want the data to be gathered in run-time CPC memory without
resorting to manually specifying too much.

Current output format is for sdasz80 (the one shipped with SDCC).  If you need
another assembler output format, please ask.

## Modes of operation

There are two modes of operation: index-based and color-based.

* If your PNG image is strictly prepared, saved with a palette (or grey), with
each pixel index corresponding exactly to the run-time CPC palette index, then
you can run without -p option.  Do this when your upstream program can create
PNG with the exact palette you want, in exact order, no extra palette entries.
Another case is when you save PNGs as black-and-width, grey4 or grey16 to
convey index values, assigning color only later in the pipeline. In this mode,
indexes in PNG image are then directly used to generate binary values.
Generated palette_* symbols reflect the 2/4/16 entries of the input PNG palette
which you may or may not want to use.

* If your PNG image is not strictly prepared, use -p option to specify the CPC
palette on the command-line.  In this case, the RGB values of pixels, not their
indexes, are used to map them to CPC binary values based on their closeness to
the palette values you provide.  Common use cases include input PNG generated
by programs which reorder palette entries, PNG saved as RGB not palette, or PNG
saved with fixed palette like the web216.

## Command-line options

### Input/output

```bash
  -i, --input=<input_filename.png>
                             Path to an input file in PNG format with a palette
                             (colormap).
  -o, --output=<output_filename.s>
                             Path where the output file will be written in
                             assembly source format.
```

### Processing

```bash
  -d, --direction=<t> or <b> Optional.  Default 't' is to write sprite data top
                             to bottom. 'b' causes processing bottom to top.
                             Correct value depend on your context, especially
                             sprite write routine.
  -m, --mode=<cpc-mode>      Optional.  CPC-mode 0, 1 or 2.  If unspecified or
                             '-' the mode will be guessed from the size of the
                             palette supplied on command-line, else the number
                             of colormap entries in the input image.  In the
                             latter case, make sure that your image doesn't
                             include extra unused colormap entries which would
                             confuse the very simple guessing logic.
  -p, --palette=colorcode[,colorcode]*
                             Optional.  This specifies CPC runtime palette and
                             enables color-based processing.  Palette is
                             specified as comma-separated decimal values (same
                             values as firmware/BASIC) like -p 1,24,20,6 or as
                             012 RGB triplets like -p 001,220,022,020 .  This
                             option is not intended to perform general purpose
                             color reduction but mostly to cope with source
                             images that roughly conform to the intended
                             palette yet in incorrect order or when image
                             colormap has extra unwanted/unused colors.  To
                             cancel a previous palette declaration, declare an
                             empty string: -p ''.
```

### Assembly-level naming

```bash
      --module_format_string=<myprefix_%s_mysuffix> or <my_module_name>
                             Optional.  Format string to generate an assembly
                             module name.  It is okay to not include a '%s' so
                             that several generated source files belong to the
                             same module.  Default is 'module_%s'.
  -n, --name_stem=somename   Optional.  String associated with the image.
                             Think like the name of your image at assembly
                             level.  Default is to generate a name from the
                             file part in the 'input' argument, replacing
                             invalid characters with an underscore '_'.
      --symbol_format_string=<myprefix_%s_mysuffix>
                             Optional.  Format string to generate symbol names.
                              A '%s' is mandatory else the generated assembly
                             file will be invalid.  Default is 'sprite_%s'.

  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
```

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.
