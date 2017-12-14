#!/bin/bash

## Helpers

TOTAL_FW_CALL_COUNT=201

### C-level

function list_prototypes() {
    grep -ih '^[a-z].* fw_.*(.*).*;' include/cfwi/*.h
}

function list_function_names() {
    list_prototypes | sed -n "s/^.* \(fw_[^(]*\)(.*$/\1/p"
}

### firmware

function list_fw_calls_covered() {
    list_function_names | sed "s/__fastcall//"
}

TOTAL_FW_CALL_COVERED_COUNT=$( list_fw_calls_covered | wc -l )

function c_style_names_to_tradi_names()
{
    #    echo "$1" | sed 's/_/ /g' | tr '[a-z]' '[A-Z]'
    sed 's/_/ /g' | tr '[a-z]' '[A-Z]'
}

function c_style_names_to_html_fw_call_span()
{
    while true
    do
	ARG="$1"

	if [[ -n "$ARG" ]]
	then
	    NEWNAME=$( echo "$1" | sed 's/fw_//g' | sed 's/_/ /g' | tr '[a-z]' '[A-Z]' )
	    echo "<span class=\"fw_call_name\">$NEWNAME</span>"
	fi

	shift || break
    done
}

### no-wrappers

function nowrappers_lines()
{
    grep "^.*_fw_[a-z_]* *== *0x.*$" src/fw_nowrapperneeded.s
}

TOTAL_FW_NOWRAPPERS_COUNT=$( nowrappers_lines | wc -l )

### wrappers

function list_fw_wrappers()
{
    ls -1b src/fw_*.s | grep -v fw_nowrapperneeded | sed 's|src/||'
}

TOTAL_FW_WRAPPERS_COUNT=$( list_fw_wrappers | wc -l )

### both

function list_fw_calls_covered_with_and_without_wrapper()
{
    list_function_names | sed -n "s/^\(.*\)__fastcall$/\1/p"
}

TOTAL_FW_TWICE_COVERED_COUNT=$( list_fw_calls_covered_with_and_without_wrapper | wc -l )

# Start output

exec >coverage.html

echo "<html>"

#echo '<link rel="stylesheet" href="coverage.css">'

cat <<EOF
<style>
/* Amstrad font inclusion */
/* @font-face kit by Fonts2u (http://www.fonts2u.com) */
@font-face {
  font-family: "../../font/amstrad";
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
EOF

function html_out_variable()
{
    echo "<tr><td>$1</td><td>$2</td></tr>"
}

## Sanity check

echo "<h1>Firmware coverage in cpc-dev-tool-chain's CFWI (C-firmware interface)</h1>"

echo "<h2>Global Statistics</h2>"

PERCENTAGE=$( echo "$TOTAL_FW_CALL_COVERED_COUNT 100 * $TOTAL_FW_CALL_COUNT / p" | dc )

echo "<p>Overall coverage percentage: ${PERCENTAGE}%</p>"

echo "<table>"

html_out_variable "Total fw calls as per SOFT968 official documentation" "$TOTAL_FW_CALL_COUNT"
html_out_variable "Total fw calls covered (made available to C)" "$TOTAL_FW_CALL_COVERED_COUNT"
html_out_variable "Total fw calls covered with C prototype and ASM symbol" "$TOTAL_FW_NOWRAPPERS_COUNT"
html_out_variable "Total fw calls covered with C prototype and ASM wrapper" "$TOTAL_FW_WRAPPERS_COUNT"
html_out_variable "Total fw calls covered both without <span style=\"font-weight: bold\">and</span> with wrapper" "$TOTAL_FW_TWICE_COVERED_COUNT"

SC_TOTAL_COVERED_WITH_AND_WITHOUT_WRAPPER_NODUPLICATE=$(( $TOTAL_FW_NOWRAPPERS_COUNT + $TOTAL_FW_WRAPPERS_COUNT - $TOTAL_FW_TWICE_COVERED_COUNT ))

html_out_variable "Total fw calls covered, filtering duplicate coverage" "$SC_TOTAL_COVERED_WITH_AND_WITHOUT_WRAPPER_NODUPLICATE"

PASSFAIL=$(
if [[ $SC_TOTAL_COVERED_WITH_AND_WITHOUT_WRAPPER_NODUPLICATE == $TOTAL_FW_CALL_COVERED_COUNT ]]
then
    echo PASS
else
    echo FAIL
fi
	)

html_out_variable "Sanity check $SC_TOTAL_COVERED_WITH_AND_WITHOUT_WRAPPER_NODUPLICATE==$TOTAL_FW_CALL_COVERED_COUNT" "$PASSFAIL"

echo "</table>"


## Per-package

function list_packages()
{
    list_fw_calls_covered | sed -n 's/fw_\([^_]*\)_.*$/\1/p' | sort | uniq
}

function in_package_count()
{
    grep _${package}_ | wc -l
}

echo "<h2>Statistics per firmware packages</h2>"

echo "<table>"

# <th>Covered twice</th>
echo "<tr><th>Package</th><th>Calls covered</th><th>Direct</th><th>Wrapper</th></tr>"

for package in $( list_packages )
do
    TRADINAME=$( c_style_names_to_html_fw_call_span "$package" )
    COVERED=$( list_function_names | in_package_count )
    NOWRAPPERS=$( nowrappers_lines | in_package_count )
    WRAPPED=$( list_fw_wrappers | in_package_count )
    #TWICE=$( list_fw_calls_covered_with_and_without_wrapper | grep _${package}_ )
    #TWICE_FORMATTED=$( c_style_names_to_html_fw_call_span $TWICE )
    # <td>$TWICE_FORMATTED</td>
    echo "<tr><td>$TRADINAME</td><td>$COVERED</td><td>$NOWRAPPERS</td><td>$WRAPPED</td></tr>"
done

echo "</table>"



echo "<h2>Details per firmware packages</h2>"


for package in $( list_packages )
do
    echo "<h3>Package $( c_style_names_to_html_fw_call_span $package )</h3>"
    echo "<table>"
    echo "<tr><th>Call</th><th>Direct</th><th>Wrapper</th><th>C-level prototype(s)</th></tr>"

    for callname in $( list_fw_calls_covered | grep "^fw_${package}_" )
    do
	TRADINAME=$( c_style_names_to_html_fw_call_span "$callname" )
	NOWRAPPERS=$( nowrappers_lines | grep $callname )
	WRAPPED=$( list_fw_wrappers | grep $callname )
	PROTOTYPES=$( list_prototypes | grep $callname | sed 's|$|<br />|')
	#TWICE=$( list_fw_calls_covered_with_and_without_wrapper | grep _${package}_ )
	#TWICE_FORMATTED=$( c_style_names_to_html_fw_call_span $TWICE )
	# <td>$TWICE_FORMATTED</td>
	echo "<tr><td>$TRADINAME</td><td class=\"cdecl\">$NOWRAPPERS</td><td class=\"cdecl\">$WRAPPED</td><td class=\"cdecl\">$PROTOTYPES</td></tr>"
    done
    echo "</table>"

done

echo "<h2>Conclusion</h2>"

echo 'Happy hacking! Start there: <a href="https://github.com/cpcitor/cpc-dev-tool-chain" title="cpcitor/cpc-dev-tool-chain: A development toolchain to compile your C or assembly projects on a modern OS and run your compiled projects on an Amstrad CPC or emulator. Early stage, simple and already usable.">cpc-dev-tool-chain on GitHub.</a>'

echo "</html>"

exit 0


## Count functions implemented

grep -i '^[a-z].* fw_.*;$' include/cfwi/*.h | sed 's/^\+//' | grep -o " fw_[^b][^ ]*" | sort | uniq | grep -o 'fw_[^_]*' | sort | uniq -c

## Count wrappers

## Count functions updated since some commit

git diff ${BASECOMMIT}..HEAD -- include/cfwi/ | grep -i '^\+[a-z].* fw_.*;$' | sed 's/^\+//' | grep -o " fw_[^b][^ ]*" | sort | uniq | grep -o 'fw_[^_]*' | sort | uniq -c


## Extract list of updated functions since some commit

git diff ${BASECOMMIT}..HEAD -- include/cfwi/ | grep -i '^\+[a-z].* fw_.*;$' | sed 's/^\+//' | grep -o " fw_[^b][^ ]*" | sort | uniq
