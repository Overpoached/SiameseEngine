#pragma once

#include <Windows.h>
#include <DbgHelp.h>

#include "core/Utils.h"
#include "core/Log.h"

namespace sengine
{
	namespace CrashHandler
	{
		inline static void WriteDump(EXCEPTION_POINTERS* pExceptionInfo)
		{
			std::string filename = "logs/siamese_crash_" + GetTimestamp() + ".dmp";
			std::filesystem::create_directories("logs");

			HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
			if (hFile == INVALID_HANDLE_VALUE) return;

			MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
			dumpInfo.ThreadId = GetCurrentThreadId();
			dumpInfo.ExceptionPointers = pExceptionInfo;
			dumpInfo.ClientPointers = FALSE;

			MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &dumpInfo, nullptr, nullptr);
			CloseHandle(hFile);
		}
		inline static LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo)
		{
			WriteDump(pExceptionInfo);

			if (LoggerService::IsRegistered())
			{
				LoggerService::Get()->Critical("Crash detected. Minidump written.");
			}
			else
			{
				std::cerr << "Crash detected. Minidump written.\n";
			}

			return EXCEPTION_EXECUTE_HANDLER;
		}
		inline static void Install()
		{
			SetUnhandledExceptionFilter(&ExceptionFilter);
		}
	};
}