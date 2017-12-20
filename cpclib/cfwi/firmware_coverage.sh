#!/bin/bash

## Helpers

TOTAL_FW_CALL_COUNT=201

### C-level

function list_c_prototypes() {
    egrep -ih '^[a-z].* \*?fw_.*(.*).*;' include/cfwi/*.h
}

function list_c_function_names() {
    list_c_prototypes | sed -n "s/^.* \**\(fw_[^(]*\)(.*$/\1/p"
}

TOTAL_C_DECLARED_FW_FUNCTION_NAMES=$( list_c_function_names | wc -l )

### firmware

function list_c_covered_fw_calls() {
    list_c_function_names | sed "s/__fastcall//" | uniq
}

TOTAL_C_DECLARED_FW_CALL_COUNT=$( list_c_covered_fw_calls | wc -l )

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

function list_c_covered_fw_calls_with_and_without_wrapper()
{
    list_c_function_names | sed -n "s/^\(.*\)__fastcall$/\1/p"
}

TOTAL_FW_TWICE_COVERED_COUNT=$( list_c_covered_fw_calls_with_and_without_wrapper | wc -l )

# Start output

# save stdout and redirect to coverage.html
exec 4<&1
exec >coverage.html

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
    
    html_out_variable "Sanity check $1==$2 ($3)" "$PASSFAIL"
}

function html_out_variable()
{
    echo "<tr><td>$1</td><td>$2</td></tr>"
}

## Sanity check

echo "<h1>$TITLE</h1>"

echo "<h2>Global Statistics</h2>"

PERCENTAGE=$( echo "$TOTAL_C_DECLARED_FW_CALL_COUNT 100 * $TOTAL_FW_CALL_COUNT / p" | dc )

echo "<p>Overall coverage percentage: ${PERCENTAGE}%</p>"

echo "<table>"

html_out_variable "Total fw calls as per SOFT968 official documentation" "$TOTAL_FW_CALL_COUNT"
html_out_variable "Total fw calls declared at C level" "$TOTAL_C_DECLARED_FW_CALL_COUNT"

html_out_variable "Total functions declared at C level" "$TOTAL_C_DECLARED_FW_FUNCTION_NAMES"
html_out_variable "Total direct ASM symbols (not wrappers) count" "$TOTAL_FW_NOWRAPPERS_COUNT"
html_out_variable "Total ASM wrapper count" "$TOTAL_FW_WRAPPERS_COUNT"
html_out_variable "Total fw calls covered both without <span style=\"font-weight: bold\">and</span> with wrapper" "$TOTAL_FW_TWICE_COVERED_COUNT"

SC_TOTAL_COVERED_WITH_AND_WITHOUT_WRAPPER_NODUPLICATE=$(( $TOTAL_FW_NOWRAPPERS_COUNT + $TOTAL_FW_WRAPPERS_COUNT - $TOTAL_FW_TWICE_COVERED_COUNT ))

html_out_variable "Total fw calls covered, filtering duplicate coverage" "$SC_TOTAL_COVERED_WITH_AND_WITHOUT_WRAPPER_NODUPLICATE"

sanity_check_pass_fail $SC_TOTAL_COVERED_WITH_AND_WITHOUT_WRAPPER_NODUPLICATE $TOTAL_C_DECLARED_FW_FUNCTION_NAMES

echo "</table>"


## Per-package

function list_packages()
{
    list_c_covered_fw_calls | sed -n 's/fw_\([^_]*\)_.*$/\1/p' | sort | uniq
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
    COVERED=$( list_c_function_names | in_package_count )
    NOWRAPPERS=$( nowrappers_lines | in_package_count )
    WRAPPED=$( list_fw_wrappers | in_package_count )
    #TWICE=$( list_c_covered_fw_calls_with_and_without_wrapper | grep _${package}_ )
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

    for callname in $( list_c_covered_fw_calls | grep "^fw_${package}_" )
    do
	TRADINAME=$( c_style_names_to_html_fw_call_span "$callname" )
	NOWRAPPERS=$( nowrappers_lines | grep $callname )
	WRAPPER=$( list_fw_wrappers | grep $callname )
	PROTOTYPES=$( list_c_prototypes | grep $callname | sed 's|$|<br />|')
	#TWICE=$( list_c_covered_fw_calls_with_and_without_wrapper | grep _${package}_ )
	#TWICE_FORMATTED=$( c_style_names_to_html_fw_call_span $TWICE )
	# <td>$TWICE_FORMATTED</td>
	echo "<tr><td>$TRADINAME</td><td class=\"cdecl\">$NOWRAPPERS</td><td class=\"cdecl\">$WRAPPER</td><td class=\"cdecl\">$PROTOTYPES</td></tr>"
    done
    echo "</table>"
done

echo "<h2>Conclusion</h2>"

echo '<p>Happy hacking! Start there: <a href="https://github.com/cpcitor/cpc-dev-tool-chain" title="cpcitor/cpc-dev-tool-chain: A development toolchain to compile your C or assembly projects on a modern OS and run your compiled projects on an Amstrad CPC or emulator. Early stage, simple and already usable.">cpc-dev-tool-chain on GitHub.</a></p>'

echo "</body>"
echo "</html>"

# restore stdout
exec 1<&4

for symbolname in $( list_c_function_names )
do
    NOWRAPPER_COUNT=$( nowrappers_lines | grep "_$symbolname " | wc -l )
    WRAPPER_COUNT=$( list_fw_wrappers | grep "$symbolname\.s$" | wc -l )
    TOTAL=$(( $NOWRAPPER_COUNT + $WRAPPER_COUNT ))

    if [[ "$TOTAL" != "1" ]]
    then
	echo -e "$NOWRAPPER_COUNT + $WRAPPER_COUNT = $TOTAL\t$symbolname" >&2
    fi
done

exit 0

## Count functions updated since some commit

git diff ${BASECOMMIT}..HEAD -- include/cfwi/ | grep -i '^\+[a-z].* fw_.*;$' | sed 's/^\+//' | grep -o " fw_[^b][^ ]*" | sort | uniq | grep -o 'fw_[^_]*' | sort | uniq -c


## Extract list of updated functions since some commit

git diff ${BASECOMMIT}..HEAD -- include/cfwi/ | grep -i '^\+[a-z].* fw_.*;$' | sed 's/^\+//' | grep -o " fw_[^b][^ ]*" | sort | uniq
