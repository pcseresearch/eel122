#!/usr/bin/perl
system("analyzer -q decodeEel.C\\($ARGV[0]\\)");
system("root draw1.C\\($ARGV[0]\\)"); 


