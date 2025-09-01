//header files and namespace goes here 
#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<cctype>
#include<cstdlib>
#include<algorithm>

#ifdef _WIN32//for Windows environment
#include <conio.h>
#include <windows.h>
#else//for Linux environment
#include <cstdio>
#include <termios.h>
#include <unistd.h>
#endif

#if __cplusplus >= 201703L
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

using namespace std;

//forward declaration
void empty_which(unsigned char);
void pushin(unsigned char, string);//to add content[][] and pt[]