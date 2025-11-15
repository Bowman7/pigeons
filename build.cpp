#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>

const std::string BUILD_DIR =  "build";
const std::string BUILD_NAME = "pigeons";

struct Source {
        std::string source_directory ;
	std::vector<std::string> sources;
};


Source src = {
        .source_directory = "src",
        .sources = {
		"main.cpp",
		"FinalLoop.cpp",
	},
        
};

Source headers = {
	.source_directory = "src",
	.sources = {},
};


#if defined (_WIN32) || defined (_WIN64)
#include <windows.h>
#include <tlhelp32.h>
#elif defined (__linux__)
#include <unistd.h>
#endif 


#if defined (_WIN64) && (defined (__GNUC__) || defined(__clang__))

const std::string lib_path = "external/libs/windows/mingw64";
const std::string header_path = "external/raylib";

// the usual microsoft bullshit "I am not like other girls"
#elif defined (_WIN64) && defined (_MSC_VER)

const std::string lib_path = "external\\libs\\windows\\msvc64";
const std::string header_path = "external\\raylib";
 
#elif defined (_WIN32) && (defined (__GNUC__) || defined (__clang__))

const std::string lib_path = "external/libs/windows/mingw32";
const std::string header_path = "external/raylib";

#elif defined (_WIN32) && defined (_MSC_VER)

const std::string lib_path = "external\\libs\\windows\\msvc32";
const std::string header_path = "external\\raylib";


#elif defined (__x86_64__) && (defined (__GNUC__)  || defined (__clang__))

const std::string lib_path = "external/libs/linux/lib64";
const std::string header_path = "external/raylib";

#elif defined (__i386__) && (defined (__GNUC__) || defined (__clang__))

const std::string lib_path = "external/libs/linux/lib32";
const std::string header_path = "external/raylib";

#else

#error "Unsupported platform / compiler or combination"

#endif


#if defined (__GNUC__) 
//const std::string cmd_pattern = "g++ %s -I%s -L%s -o %s"; 
std::string CC = "g++ -std=c++17 ";
std::string II = " -I";
std::string LL = " -L";
std::string OO = " -o ";

#elif defined (_MSC_VER) 
//const std::string cmd_pattern = "cl %s /Ehsc /I %s /link /LIBPATH: %s  /Fe:%s"; 
std::string CC = "cl ";
std::string II = " /I ";
std::string LL = " /link /LIBPATH:";
std::string OO = " /Fe:";

#elif defined (__clang__) 
//const std::string cmd_pattern = "clang++ %s -I%s -L%s  -o %s";
std::string CC = "clang++ -std=c++17 ";
std::string II = " -I";
std::string LL = " -L";
std::string OO = " -o ";

#else 
#error "Unsupported compiler"
#endif




Source ext_libs = {
	.source_directory = lib_path,
#if defined(_WIN32)||defined(_WIN64) 
	.sources = {

#if defined(__GNUC__) || defined(__clang__)
		"mingw32",
		"msvcrt",
		"m",
#endif

		"raylib",
		"winmm",
		"gdi32",
		"kernel32",
		"opengl32",
		"user32",
		"shell32",
		"ole32",
		"oleaut32",
		"uuid",
		"advapi32",
		"comdlg32",
		"imm32",
	},
#elif defined(__linux__)
	.sources = {
		"raylib",
		"GL",
		"m",
		"pthread",
		"dl",
		"rt",
		"X11",
	 	"Xrandr",
		"Xi",
		"Xxf86vm",
		"Xinerama",
		"Xcursor",
		"asound",
		"dl",
	},
#else 
#error "Unsupported Platform"
#endif
};


auto write_source() -> std::string {

	std::ostringstream oss;

	for (const auto &item : src.sources ){

		oss << src.source_directory;
		oss << '/';
		oss << 	item ;
		oss << ' ';
	}

	return oss.str();
}

auto write_ext_libs() -> std::string {
	
	std::ostringstream oss;

	oss << ext_libs.source_directory;
	oss << ' ';

	for (const auto &item : ext_libs.sources){
#if defined (__GNUC__) || defined (__clang__)
		oss << "-l";
		oss << item;
		oss << ' ';
#elif defined (_MSC_VER) 
		oss << item ;
		oss << ".lib";
		oss << ' ';
#endif
	}

	return oss.str();
}

#if defined(_WIN32) || defined(_WIN64)

enum class Shell { CMD, PowerShell, GitBash, BusyBox,  Unknown };

auto get_parent_process_name() -> std::string {
	DWORD pid = GetCurrentProcessId();
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) return "";

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	DWORD parentPid = 0;

	if (!Process32First(snapshot, &entry)) {
		CloseHandle(snapshot);
		return "";
	}

	do {
		if (entry.th32ProcessID == pid) {
			parentPid = entry.th32ParentProcessID;
			break;
		}
	} while (Process32Next(snapshot, &entry));

	if (parentPid == 0) {
		CloseHandle(snapshot);
		return "";
	}

	// Find parent process
	
	Process32First(snapshot, &entry);
	do {
		if (entry.th32ProcessID == parentPid) {
			CloseHandle(snapshot);
			return std::string(entry.szExeFile); 
		}
	} while (Process32Next(snapshot, &entry));

	CloseHandle(snapshot);
	return "";
}

auto detect_shell() -> Shell {
	std::string parent = get_parent_process_name();
	for (auto &c : parent) c = tolower(c); 

	if (parent.find("cmd.exe") != std::string::npos) return Shell::CMD;
	if (parent.find("powershell.exe") != std::string::npos) return Shell::PowerShell;
	if (parent.find("bash.exe") != std::string::npos) return Shell::GitBash;
	if (parent.find("busybox.exe") != std::string::npos) return Shell::BusyBox;

	return Shell::Unknown;
}

#endif

auto reset_directory() -> void{

#if defined(_WIN32) || defined(_WIN64)
	Shell shell = detect_shell();

	printf("shell found was %d\n", shell);
	std::string del; 
	std::string cr;
	std::string cp;

	if (shell == Shell::PowerShell){

		del = " powershell.exe -NoProfile -Command \" Remove-Item -Recurse -Force " + BUILD_DIR + "\""; 
		cr = " powershell.exe -NoProfile -Command \" New-Item -ItemType Directory " + BUILD_DIR + "\"";
		cp = " powershell.exe -NoProfile -Command \" Copy-Item -Path \"asset\" -Destination \"" + BUILD_DIR + "\"\"";

	}else if (shell == Shell::CMD) {

		del = "rmdir /S  " + BUILD_DIR ; 
		cr = "mkdir " + BUILD_DIR ;
		cp = "xcopy asset \"" + BUILD_DIR + "/asset\" /E /I /H";

	}else if (shell == Shell::GitBash) {

		del = "rm -rf " + BUILD_DIR ; 
		cr = "mkdir " + BUILD_DIR ;
		cp = "cp -r asset " + BUILD_DIR;

	}else if (shell == Shell::BusyBox) {

		del = "rm -rf " + BUILD_DIR ; 
		cr = "mkdir " + BUILD_DIR ;
		cp = "cp -r asset " + BUILD_DIR;

	}else{
		printf("Unsupported shell");
		std::exit(-1);
	}

#else
	// the plain old linux
	
	std::string del = "rm -rf " + BUILD_DIR ; 
	std::string cr = "mkdir " + BUILD_DIR ;
	std::string cp = "cp -r asset " + BUILD_DIR;

#endif

	int ret = std::system(del.c_str());
	if (ret != 0) {
		printf("Build directory does not exist , creating one for you\n");
	}
	ret = std::system(cr.c_str());
	assert(ret == 0);
	ret = std::system(cp.c_str());
	assert(ret == 0);
}

auto main(void) -> int {

	reset_directory();

	const std::string cmd  = 
		CC + 
		write_source() +
#if defined(_MSC_VER)
		"/EHsc /std:c++20 /MD" +
#endif
		II + 
		headers.source_directory + 
		II + 
		header_path +
		OO+
#if defined(_MSC_VER)
		"\"" +
#endif
		BUILD_DIR +
		"\\" +

		BUILD_NAME +
#if defined(_MSC_VER)
		 ".exe\"" + 
#endif
		LL + 
		write_ext_libs()  ;

	
	printf("running \n%s\n\n", cmd.c_str());
	
	std::system(cmd.c_str());

	printf("\nDone\n");
	return 0;
}
