#pragma once
#include <windows.h>    
#include <winsvc.h>    
#include <conio.h>    
#include <stdio.h> 
#include<QMessageBox>
BOOL LoadNTDriver(LPCWSTR lpszDriverName, LPCWSTR lpszDriverPath);
BOOL UnloadNTDriver(LPCWSTR szSvrName);