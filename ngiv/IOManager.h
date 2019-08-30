#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

#include <iostream>

/*
CHAR*			 WORKS WITH FILESIZE ELSE ADDS EMPTY CHARACTERS
STRING			 WORKS
VECSTRING		 WORKS
VECUCHAR		 WORKS
*/


namespace ngiv {

	struct DirEntry {
		std::string path;
		bool isDirectory;
	};
	
	namespace fs = std::experimental::filesystem;
	
	//namespace fs = std::tr2::sys;
	
	inline bool filetobuffer(const std::string& filepath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filepath, std::ios::binary);
		if (file.fail()) {
			perror(filepath.c_str());
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end);

		//Get the file size
		unsigned int fileSize = (unsigned int)file.tellg();
		file.seekg(0, std::ios::beg);

		//Reduce the file size by any header bytes that might be present
		fileSize -= (unsigned int)file.tellg();

		buffer.resize(fileSize);
		file.read((char *)&(buffer[0]), fileSize);
		file.close();

		return true;
	}
	inline bool filetobuffer(const std::string& filepath, std::string& buffer) {
		std::ifstream file(filepath, std::ios::binary);
		if (file.fail()) {
			perror(filepath.c_str());
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end);

		//Get the file size
		unsigned int fileSize = (unsigned int)file.tellg();
		file.seekg(0, std::ios::beg);

		//Reduce the file size by any header bytes that might be present
		fileSize -= (unsigned int)file.tellg();

		buffer.resize(fileSize);
		file.read((char *)&(buffer[0]), fileSize);
		file.close();

		return true;
	}

	inline std::vector<unsigned char>*		invecUchar(const char* filename) {
		std::ifstream f;
		f.open(filename, std::ios_base::binary);
		if (f.fail()) {
			perror(filename);
			
		}

		f.seekg(0, f.end);
		unsigned int filesize = (unsigned int)f.tellg();
		f.seekg(0, f.beg);
		filesize -= (unsigned int)f.tellg();

		char* buffer = new char[filesize];

		f.read(&(buffer)[0], filesize);

		f.close();

		std::vector<unsigned char>* file = new std::vector<unsigned char>;

		file->resize(filesize);
		for (size_t i = 0; i < filesize; i++) {
			(*file)[i] = buffer[i];
		}

		return file;
	}
	inline std::vector<std::string>*		invecstring(const char* filename) {

		std::string line;
		std::vector<std::string>* file = new std::vector<std::string>;

		std::ifstream f;
		f.open(filename, std::ios_base::binary);
		if (f.fail()) {
			perror(filename);
			file->emplace_back("ERROR");
			return file;
		}

		

		while (std::getline(f, line)) {
			file->push_back(line + "\n");
		}
		if (file->size() != 0)	file->back().pop_back();

		f.close();

		return file;
	}
	inline char*							incharstar(const char* filename) {
		std::ifstream f;
		f.open(filename, std::ios_base::binary);
		if (f.fail()) {
			perror(filename);
		}

		f.seekg(0, f.end);
		int filesize = (unsigned int)f.tellg();
		f.seekg(0, f.beg);
		filesize -= (unsigned int)f.tellg();
		char* file = new char[filesize];

		f.read(&file[0], filesize);
		f.close();

		return file;
	}
	inline std::string*						instring(const char* filename) {
		std::ifstream f;
		f.open(filename, std::ios_base::binary);
		if (f.fail()) {
			perror(filename);
		}

		std::string* file = new std::string;
		std::string line;

		while (std::getline(f, line)) {
			*file += line + "\n";
		}
		file->pop_back();

		f.close();

		return file;

	}


	inline void								outbyvecUchar(const char* filename, std::vector<unsigned char>* fileindex) {
		std::ofstream f;
		f.open(filename, std::ios_base::binary);
		if (f.fail()) {
			perror(filename);
			
		}


		//create the charbuffer
		char* buffer = new char[fileindex->size()];
		for (size_t a = 0; a < fileindex->size(); a++) {
			buffer[a] = (*fileindex)[a];
		}

		f.write(buffer, fileindex->size());

		f.close();
		delete[] buffer;

		return;

	}
	inline void								outbyvecstring(const char* filename, std::vector<std::string>* fileindex) {
		std::ofstream f;
		f.open(filename, std::ios_base::binary);
		if (f.fail()) {
			perror(filename);
			
		}

		for (size_t i = 0; i < fileindex->size(); i++) {
			f << (*fileindex)[i];			
		}

		f.close();
	}
	inline void								outbycharstar(const char* filename,const char* fileindex, int filesize = 0) {
		std::ofstream f;
		f.open(filename, std::ios_base::binary);
		if (f.fail()) {
			perror(filename);
			
		}
		if (fileindex == NULL) {
			f << "";
		}
		else {
			if (filesize != 0) {
				f.write(fileindex, filesize);
			}
			else {
				f.write(fileindex, strlen(fileindex));
			}
		}

		



		f.close();

		return;
	}
	inline void								outbystring(const char* filename, std::string* file, bool append) {
		std::ofstream f;
		if (append) {
			f.open(filename, std::ios_base::binary | std::ios_base::app);
		}
		else {
			f.open(filename, std::ios_base::binary);
		}
		
		if (f.fail()) {
			perror(filename);
		}

		f << *file;

		f.close();

		return;
	}
	
		
	/*
		inline bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries) {
			auto dpath = fs::path(path);
			// Must be directory
			if (!fs::is_directory(dpath)) return false;

			for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it) {
				rvEntries.emplace_back();
				rvEntries.back().path = it->path().string();
				if (is_directory(it->path())) {
					rvEntries.back().isDirectory = true;
				}
				else {
					rvEntries.back().isDirectory = false;
				}
			}
			return true;
		}
		inline bool makeDirectory(std::string path) {
			return fs::create_directories(fs::path(path));
		}
		inline bool checkDirectory(const char* path) {
			return fs::exists(fs::path(path));
		}
		*/


	/*
		inline bool copy(std::string npath, std::string ntopath) {
			std::error_code a;
			fs::copy(fs::path(npath), fs::path(ntopath),a);
			if (a.value() != 0) {
				return 0;
			} 
			return 1;
		}
		inline bool copyall(std::string npath, std::string ntopath) {
			
			std::vector<DirEntry> dirs;
			getDirectoryEntries(npath.c_str(), dirs);
			if (!copy(npath, ntopath)) return 0;
			for (size_t i = 0; i < dirs.size(); i++) {
				if (dirs[i].isDirectory) {
					std::string name = dirs[i].path;

					for (int j = dirs[i].path.size(); j > -1; j--) {
						if (dirs[i].path[j] == '\\') {
							name.erase(0,j);
							break;
						}
					}
					makeDirectory(ntopath + name);
					copyall(dirs[i].path, ntopath + name);
				}
			}
			return 1;
			
		}
		*/


	inline bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries) {
		auto dpath = fs::path(path);
		// Must be directory
		if (!fs::is_directory(dpath)) return false;

		for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it) {
			rvEntries.emplace_back();
			rvEntries.back().path = it->path().string();
			if (is_directory(it->path())) {
				rvEntries.back().isDirectory = true;
			}
			else {
				rvEntries.back().isDirectory = false;
			}
		}
		return true;
	}

	inline bool checkFile(const std::string& filename) {
		std::ifstream ifile(filename.c_str());
		return (bool)ifile;
	}

	inline bool deletefile(std::string path){
		int code = std::remove(path.c_str());
		if (code == 0) return 1;
		return 0;
	}
}
