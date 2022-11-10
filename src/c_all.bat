echo off

hc386.exe -v -Hon=pcc -Hon=Long_enums -Hon=make_externs_global -Hoff=Char_default_unsigned -c *.c
