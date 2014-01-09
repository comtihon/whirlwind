#!/bin/bash
make;
echo -e "\e[5mNormal \e[5mtesting for all tests\e[25m";
make test;

echo -e "\e[5mMemory \e[5mtesting for some tests\e[25m";
ctest -R CryptMemoryTest Mem -D ExperimentalMemCheck;
ctest -R CryptMemoryTest File -D ExperimentalMemCheck;