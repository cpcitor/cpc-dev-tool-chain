#!/bin/bash

## Helpers

# For valid __preserves_regs parameters, look for _getRegByName in SDCC source code.

set -eu

. firmware_coverage_helper.env


#echo "Checking that each c prototype matches one asm-level symbol ..."

{
for symbolname in $( list_c_function_names )
do
    #echo -n "."
    NOWRAPPER_COUNT=$( nowrappers_lines | grep "_$symbolname " | wc -l )
    WRAPPER_COUNT=$( list_fw_wrapper_symbols | grep "^_${symbolname}$" | wc -l )
    TOTAL=$(( $NOWRAPPER_COUNT + $WRAPPER_COUNT ))

    if [[ "$TOTAL" != "1" ]]
    then
        echo -e "$NOWRAPPER_COUNT + $WRAPPER_COUNT = $TOTAL\t$symbolname" >&2
    fi
done
#echo "done"
} &

# Sanity check

for ASM_SOURCE_FILE in $( list_fw_wrapper_files )
do
    for SYMBOL in $( cat $ASM_SOURCE_FILE | sed -n "s|^.*\(fw[^:]*\)::|\1|p" )
    do
	SYMBOL_NOSUFFIX=$( echo $SYMBOL | sed "s/__.*//" )
	if [[ "$ASM_SOURCE_FILE" != "src/${SYMBOL_NOSUFFIX}.s" ]]
	then
	    echo >&2 "WARNING: $ASM_SOURCE_FILE contains unexpected symbol ${SYMBOL_NOSUFFIX}"
	fi
    done
done

# Start output

# save stdout and redirect to coverage.html
exec 4<&1
exec >coverage.html.tmp

TITLE="Firmware coverage in cpc-dev-tool-chain's CFWI (C-firmware interface)"

cat <<EOF
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>$TITLE</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<style  type="text/css">
/* Amstrad font inclusion */
/* @font-face kit by Fonts2u (http://www.fonts2u.com) */
@font-face {
  font-family: "Amstrad";
  font-style: normal;
  font-weight: 400;
  src: url("../../font/amstrad_cpc464.eot") format("eot"),
        url("../../font/amstrad_cpc464.woff") format("woff"),
        url("../../font/amstrad_cpc464.ttf") format("truetype"),
        url("../../font/amstrad_cpc464.svg") format("svg");
}

.cpc, .fw_call_name {
  font-weight: normal;
  font-family: "Amstrad", "Monospace";
}

.cdecl {
  font-weight: normal;
  font-family: "Monospace";
}

.fw_call_name
{
    color: #000080;
    background-color: #FFFF00;
}

.direct
{
  font-weight: normal;
  font-family: "Monospace";
}

.not_yet_covered {
background: repeating-linear-gradient(
  135deg,
  #fff,
  #fff 10px,
  #f88 10px,
  #f88 20px
);
font-weight: bold;
}

.error {
background: #f88;
}

.preserve_implemented {
background: #8f8;
}

table {
    border-collapse: collapse;
    border: 1px solid black;
}
th
{
    color: #FFFF00;
    background-color: #000080;
}
th, td {
    border: 1px solid grey;
    padding: 4px;
}
tr:nth-child(even) {background-color: #f2f2f2;}
</style>
</head>
<body>
EOF

function sanity_check_pass_fail()
{
    declare PASSFAIL=$(
        if [[ "$1" == "$2" ]]
        then
            echo '<span style="background-color: #40FF40">PASS</span>'
            RC=1
        else
            echo >&2 "WARNING: Sanity check fail $1 != $2 : $3"
            echo '<span style="background-color: #FF4040">FAIL</span>'
            RC=0
        fi
            )

    html_out_variable "Sanity check $1==$2 <br/>$3" "$PASSFAIL"
    return $RC
}

function html_out_variable()
{
    echo "<tr><td>$1</td><td>$2</td></tr>"
}

## Sanity check

echo "<h1>$TITLE</h1>"

echo "<h2>Global Statistics</h2>"

PERCENTAGE=$( echo "$TOTAL_C_DECLARED_FW_CALL_COUNT 100 * $TOTAL_FW_CALL_COUNT / p" | dc )

echo "<table>"

html_out_variable "Total fw calls as per SOFT968 official documentation" "$TOTAL_FW_CALL_COUNT"
html_out_variable "Total fw calls declared at C level" "$TOTAL_C_DECLARED_FW_CALL_COUNT"
html_out_variable "Overall coverage percentage" "${PERCENTAGE}%"

echo "</table>"

function symbol_level_statistics()
{

echo "<h2>Function/symbol-level Statistics</h2>"

echo "<table>"

html_out_variable "Total calls covered at C level" "$TOTAL_C_DECLARED_FW_FUNCTION_NAMES"
html_out_variable "Total direct ASM symbols (not wrappers) count" "$TOTAL_FW_NOWRAPPER_COUNT"
html_out_variable "Total ASM wrapper count" "$TOTAL_FW_WRAPPER_COUNT"

SC_TOTAL_ASM_SYMBOL_COUNT=$(( $TOTAL_FW_WRAPPER_COUNT + $TOTAL_FW_NOWRAPPER_COUNT ))

if sanity_check_pass_fail $SC_TOTAL_ASM_SYMBOL_COUNT $TOTAL_C_DECLARED_FW_FUNCTION_NAMES "(wrappers)+(no-wrappers)==(C function count)"
then
    FAILHINT=$( diff -u \
             <( list_c_function_names | sort ) \
             <( { list_fw_nowrapper_symbols ; list_fw_wrapper_symbols ; } | sort )
             )
fi

echo "</table>"

if [[ -n "${FAILHINT:-}" ]]
then
    echo "<p>Hint: <pre>${FAILHINT}</pre></p>"
fi
}

# disabled for now, since one wrapper file now holds several symbols
# (several C-level functions point to same wrapper, they on ly differ
# in types)
# symbol_level_statistics

function firmware_call-level_statistics()
{

echo "<h2>Firmware-call-level Statistics</h2>"

echo "<table>"

html_out_variable "Total fw calls covered both without <span style=\"font-weight: bold\">and</span> with wrapper" "$TOTAL_FW_TWICE_COVERED_COUNT"

SC_TOTAL_COVERED_WITH_AND_WITHOUT_WRAPPER_NODUPLICATE=$(( $TOTAL_FW_NOWRAPPER_COUNT + $TOTAL_FW_WRAPPER_COUNT - $TOTAL_FW_TWICE_COVERED_COUNT ))

html_out_variable "Total fw calls covered, filtering duplicate coverage" "$SC_TOTAL_COVERED_WITH_AND_WITHOUT_WRAPPER_NODUPLICATE"

sanity_check_pass_fail $SC_TOTAL_COVERED_WITH_AND_WITHOUT_WRAPPER_NODUPLICATE $TOTAL_C_DECLARED_FW_CALL_COUNT
}

echo "</table>"


## Per-package

##function list_packages()
##{
##    list_fw_calls_c_style | sed -n 's/fw_\([^_]*\)_.*$/\1/p' | sort | uniq
##}

echo "<h2>Statistics per firmware packages</h2>"

echo "<table>"

echo "<tr><th>Package</th><th>Call count</th><th>C-covered</th><th>%</th></tr>"

for PACKAGE in $( list_fw_official_packages )
do
    ALL_CALLS=$( list_fw_official_calls_in_package $PACKAGE | wc -l )
    COVERED=$( list_fw_calls_c_style | in_package_count )
    COVERAGE_PERCENT=$(( $COVERED * 100 / $ALL_CALLS ))
    #    NOWRAPPERS=$( nowrappers_lines | in_package_count )
    #    WRAPPED=$( list_fw_wrapper_files | in_package_count )
    echo "<tr><td class=\"fw_call_name\">$PACKAGE</td><td>$ALL_CALLS</td><td>$COVERED</td><td style=\"text-align: right\">${COVERAGE_PERCENT}%</td>"
    # <td>$NOWRAPPERS</td><td>$WRAPPED</td></tr>"
done

echo "</table>"



echo "<h2>Details per firmware packages</h2>"


for PACKAGE in $( list_fw_official_packages )
do
    echo "<h3>Package <span class=\"fw_call_name\">$PACKAGE</span></h3>"
    echo "<table>"
    echo "<tr><th>Call</th><th>ASM symbol(s)</th><th>Preserved regs optimisation</th><th>C prototype(s)</th></tr>"
    COLUMNS=4
    # For each firmware call, gather a list of variants.
    # Variants = all symbols, C, asm, gathered.
    # For each variant generate a table line.
    # The "name" cell will span all these lines, vertically.
    # If no variant, then no vertical span, but a horizontal span with an error message "not covered".

    for CALLNAME in $( list_fw_official_calls_in_package "${PACKAGE}" | tradi_names_to_c_style_names )
    do

        SYMBOLS_C=$( list_c_function_names | grep "$CALLNAME" || true )
        SYMBOLS_ASM=$( list_asm_symbols | grep "$CALLNAME" || true )
        SYMBOLS_ALL=$( for a in $SYMBOLS_C $SYMBOLS_ASM ; do echo $a ; done | sort | uniq ; )

        if [[ -z "$SYMBOLS_ALL" ]]
        then
            echo "<tr><td class=\"fw_call_name\">$( echo $CALLNAME | c_style_names_to_tradi_names )</td><td class=\"not_yet_covered\" colspan=\"$COLUMNS\">Not yet covered</td></tr>"
            continue
        fi

        AFTERFIRSTCOLUMN=""

        SYMBOL_COUNT=$( for a in $SYMBOLS_ALL ; do echo $a ; done | wc -l )

        echo -n "<tr><td class=\"fw_call_name\" rowspan=\"$SYMBOL_COUNT\">$( echo $CALLNAME | c_style_names_to_tradi_names )</td>"

        for SYMBOL in $SYMBOLS_ALL
        do
            if [[ -n "$AFTERFIRSTCOLUMN" ]]
            then
                echo -n "<tr>"
                #echo "<td class=\"fw_call_name\">$( echo $CALLNAME | c_style_names_to_tradi_names )</td>"
            fi
            AFTERFIRSTCOLUMN=yes

            # For each variant generate a table line.

            # echo -n "<td class=\"cdecl\">$( list_asm_symbols | grep -w $SYMBOL || echo "MISSING ASM SYMBOL $SYMBOL" )</td>"

            # echo -n "<td class=\"cdecl\">"
            # $( list_asm_symbols | grep -w $SYMBOL || echo "MISSING ASM SYMBOL $SYMBOL" )
            # echo "</td>"

            report_implementation_asm $SYMBOL

            PRESERVE_SPEC="$( list_c_prototypes | grep -w $SYMBOL | sed -n 's|^.* __preserves_regs(\([^)]*\)).*$|\1|p' || true )"
            PRESERVE_CSS_CLASS=""
            if [[ -n "$PRESERVE_SPEC" ]]
            then
                PRESERVE_CSS_CLASS="preserve_implemented"
            fi
            
            echo -n "<td class=\"${PRESERVE_CSS_CLASS}\">$PRESERVE_SPEC</td>"

            echo -n "<td class=\"cdecl\"><code>$( list_c_prototypes | prototype_line_to_bare_prototype | grep -w $SYMBOL || echo "MISSING C PROTOTYPE $SYMBOL" )</code></td>"
            echo "</tr>"
        done



        #NOWRAPPERS=$( nowrappers_lines | grep $CALLNAME )
        #WRAPPER=$( FILES=$( list_fw_wrapper_files | grep "$CALLNAME" ) ; for FILE in $FILES ; do echo "<p>In $FILE</p>" ; grep "$CALLNAME.*::" $FILE ; done )
        #PROTOTYPES=$( list_c_prototypes | grep $CALLNAME | prototype_line_to_bare_prototype )
#       echo "<tr><td class=\"fw_call_name\">$( echo $CALLNAME | c_style_names_to_tradi_names )</td><td class=\"cdecl\"><pre>$NOWRAPPERS</pre></td><td><pre>$WRAPPER</pre></td><td class=\"cdecl\"><pre>$PROTOTYPES</pre></td></tr>"
    done
    echo "</table>"
done

echo "<h2>Conclusion</h2>"

echo '<p>Happy hacking! Start there: <a href="https://github.com/cpcitor/cpc-dev-tool-chain" title="cpcitor/cpc-dev-tool-chain: A development toolchain to compile your C or assembly projects on a modern OS and run your compiled projects on an Amstrad CPC or emulator. Early stage, simple and already usable.">cpc-dev-tool-chain on GitHub.</a></p>'

echo "</body>"
echo "</html>"

# restore stdout
exec 1<&4

mv coverage.html.tmp coverage.html

exit 0

## Count functions updated since some commit

git diff ${BASECOMMIT}..HEAD -- include/cfwi/ | grep -i '^\+[a-z].* fw_.*;$' | sed 's/^\+//' | grep -o " fw_[^b][^ ]*" | sort | uniq | grep -o 'fw_[^_]*' | sort | uniq -c


## Extract list of updated functions since some commit

git diff ${BASECOMMIT}..HEAD -- include/cfwi/ | grep -i '^\+[a-z].* fw_.*;$' | sed 's/^\+//' | grep -o " fw_[^b][^ ]*" | sort | uniq
