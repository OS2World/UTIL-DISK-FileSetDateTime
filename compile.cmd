@echo off

rem
rem $Id: //devel/FileSetDateTime/compile.cmd#1 $
rem

icc /G5 /Gd- /Gm- /Gl /Gi /O+ /Tl60 *.cpp
