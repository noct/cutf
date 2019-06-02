cutf
====

Minimal C port of UTF8-CPP


Testing
=======

Test framework is minimalistic, go to into bin folder, run command

test_cutf.exe && bash -c ./test_cutf_linux.out

[windows] sizeof(wchar_t)=2
simple string test:  ok.
utf8_invalid.txt: .. ok.
quickbrown.txt  : .. ok.
UTF-8-demo.txt  : .. ok.
big.txt         : .. ok.
[linux] sizeof(wchar_t)=4
simple string test:  ok.
utf8_invalid.txt: .. ok.
quickbrown.txt  : .. ok.
UTF-8-demo.txt  : .. ok.
big.txt         : .. ok.

