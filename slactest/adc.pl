#!/usr/bin/perl
system("analyzer -q decodeEel_slac.C\\($ARGV[0]\\)");
system("root adc.C\\($ARGV[0]\\)"); 


