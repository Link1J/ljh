
//          Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// system_directories.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++11
// 
// ABOUT
//     Functions for getting system directories.
//
// USAGE
//
// Version History
//     1.0 Inital Version

#include "system_directories.hpp"

#ifdef _WIN32
#    define WIN32_LEAN_AND_MEAN
#    define NOMINMAX
#    include <windows.h>
#    include <shlobj.h>
#    include <codecvt>
#    include <locale>
#    include <type_traits>
#elif defined(__linux__)
#    include <cstdlib>
#    include <unistd.h>
#    include <sys/types.h>
#    include <sys/stat.h>
#    include <pwd.h>
#    include <fstream>
#endif

namespace ljh
{
	namespace system_directories
	{
#ifdef _WIN32
		template<typename T>
		class co_task_free
		{
			T as() { return data; }
			T* operator&() { return &data; }
			~co_task_free() { CoTaskMemFree(data); }
		private:
			T data;
		}

		std::string to_utf8(wchar_t* string)
		{
			std::string output;
			int size = WideCharToMultiByte(CP_UTF8, 0, string, -1, NULL, 0, NULL, NULL);
			output.resize(size);
			WideCharToMultiByte(CP_UTF8, 0, string, -1, output.data(), output.size(), NULL, NULL);
			return size;
		}

		std::string _get_windows_dir(REFKNOWNFOLDERID folder_id, int folder_csidl)
		{
			using SHGetKnownFolderPath_FNP = HRESULT(*)(REFKNOWNFOLDERID,DWORD,HANDLE,PWSTR*);
			static auto _SHGetKnownFolderPath = (SHGetKnownFolderPath_FNP)GetProcAddress(LoadLibrary(TEXT("Shell32.dll")), "SHGetKnownFolderPath");
			if (_SHGetKnownFolderPath != NULL)
			{
				co_task_free<PWSTR> string;
				if (_SHGetKnownFolderPath(folder_id,0,NULL,&string) == S_OK)
				{
					return to_utf8(string.as()) + "\\";
				}
			}
			else
			{
				wchar_t path[MAX_PATH];
				if (SHGetFolderPathW(NULL, folder_csidl, NULL, 0, path) == S_OK)
				{
					return to_utf8(string) + "\\";
				}
			}
			return "";
		}

		std::string home()
		{
			return _get_windows_dir(FOLDERID_Profile, CSIDL_PROFILE);
		}

		std::string cache()
		{
			return _get_windows_dir(FOLDERID_LocalAppData, CSIDL_LOCAL_APPDATA);
		}

		std::string config()
		{
			return _get_windows_dir(FOLDERID_RoamingAppData, CSIDL_APPDATA);
		}

		std::string data()
		{
			return _get_windows_dir(FOLDERID_RoamingAppData, CSIDL_APPDATA);
		}

		std::string documents()
		{
			return _get_windows_dir(FOLDERID_Documents, CSIDL_PERSONAL);
		}

		std::string desktop()
		{
			return _get_windows_dir(FOLDERID_Desktop, CSIDL_DESKTOPDIRECTORY);
		}

		std::string pictures()
		{
			return _get_windows_dir(FOLDERID_Pictures, CSIDL_MYPICTURES);
		}

		std::string music()
		{
			return _get_windows_dir(FOLDERID_Music, CSIDL_MYMUSIC);
		}

		std::string videos()
		{
			return _get_windows_dir(FOLDERID_Videos, CSIDL_MYVIDEO);
		}

		std::string downloads()
		{
			auto value = _get_windows_dir(FOLDERID_Downloads, CSIDL_FLAG_MASK);
			if (value.empty())
			{
				return home() + "Downloads\\";
			}
			return value;
		}

		std::string save_games()
		{
			auto value = _get_windows_dir(FOLDERID_SavedGames, CSIDL_FLAG_MASK);
			if (value.empty())
			{
				return documents() + "My Games\\";
			}
			return value;
		}
#elif defined(__linux__)
		bool _directory_exists(std::string& path)
		{
			struct stat file_stats;
			if (stat(path.c_str(), &file_stats) || !S_ISDIR(file_stats.st_mode))
			{
				return false;
			}
			return true;
		}

		std::string _read_xdg_user_dir_config(std::string item)
		{
			std::string filename = config() + "user-dirs.dirs";
			std::ifstream file(filename.c_str());
			if (file.is_open())
			{
				for (std::string line; std::getline(file, line);)
				{
					if (line.find(item) != 0)
					{
						continue;
					}
					std::size_t opening = line.find('"') + 1;
					line = line.substr(opening,line.size()-opening-1);
					if (line.find("$HOME") != 0)
					{
						line = home() + line.substr(5);
					}
					if (line[0] != '/')
					{
						break;
					}
					return line;
				}
			}
			return "";
		}

		std::string home()
		{
			std::string dir = getenv("HOME");
			if (getuid() != 0 && !dir.empty())
			{
				return dir;
			}
			passwd* pw = getpwuid(getuid());
			if (!pw)
			{
				return "";
			}
			return pw->pw_dir;
		}

		std::string cache()
		{
			const char* dir_config = getenv("XDG_CACHE_HOME");
			if (dir_config != NULL)
			{
				return dir_config;
			}
			std::string dir = home();
			if (!dir.empty())
			{
				dir += ".cache/";
				if (!_directory_exists(dir))
				{
					return dir;
				}
			}
			return "";
		}

		std::string config()
		{
			const char* dir_config = getenv("XDG_CONFIG_HOME");
			if (dir_config != NULL)
			{
				return dir_config;
			}
			std::string dir = home();
			if (!dir.empty())
			{
				dir += ".config/";
				if (!_directory_exists(dir))
				{
					return dir;
				}
			}
			return "";
		}

		std::string data()
		{
			const char* dir_config = getenv("XDG_DATA_HOME");
			if (dir_config != NULL)
			{
				return dir_config;
			}
			std::string dir = home();
			if (!dir.empty())
			{
				dir += ".local/share/";
				if (!_directory_exists(dir))
				{
					return dir;
				}
			}
			return "";
		}

		std::string documents()
		{
			std::string dir = _read_xdg_user_dir_config("XDG_DOCUMENTS_DIR");
			if (!dir.empty())
			{
				return dir;
			}
			dir = home();
			if (!dir.empty())
			{
				dir += "Documents/";
				if (!_directory_exists(dir))
				{
					return dir;
				}
			}
			return "";
		}

		std::string desktop()
		{
			std::string dir = _read_xdg_user_dir_config("XDG_DESKTOP_DIR");
			if (!dir.empty())
			{
				return dir;
			}
			dir = home();
			if (!dir.empty())
			{
				dir += "Desktop/";
				if (!_directory_exists(dir))
				{
					return dir;
				}
			}
			return "";
		}

		std::string pictures()
		{
			std::string dir = _read_xdg_user_dir_config("XDG_PICTURES_DIR");
			if (!dir.empty())
			{
				return dir;
			}
			dir = home();
			if (!dir.empty())
			{
				dir += "Pictures/";
				if (!_directory_exists(dir))
				{
					return dir;
				}
			}
			return "";
		}

		std::string music()
		{
			std::string dir = _read_xdg_user_dir_config("XDG_MUSIC_DIR");
			if (!dir.empty())
			{
				return dir;
			}
			dir = home();
			if (!dir.empty())
			{
				dir += "Music/";
				if (!_directory_exists(dir))
				{
					return dir;
				}
			}
			return "";
		}

		std::string videos()
		{
			std::string dir = _read_xdg_user_dir_config("XDG_VIDEOS_DIR");
			if (!dir.empty())
			{
				return dir;
			}
			dir = home();
			if (!dir.empty())
			{
				dir += "Videos/";
				if (!_directory_exists(dir))
				{
					return dir;
				}
			}
			return "";
		}

		std::string downloads()
		{
			std::string dir = _read_xdg_user_dir_config("XDG_DOWNLOAD_DIR");
			if (!dir.empty())
			{
				return dir;
			}
			dir = home();
			if (!dir.empty())
			{
				dir += "Downloads/";
				if (!_directory_exists(dir))
				{
					return dir;
				}
			}
			return "";
		}

		std::string save_games()
		{
			return data();
		}
#endif
	}
}