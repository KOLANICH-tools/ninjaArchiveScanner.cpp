#include <iostream>
#include <fstream>
#include <filesystem>

#include <archive.h>
#include <archive_entry.h>


int main(int argc, char* argv[]){
	if(argc != 4){
		std::cerr << "Usage: NinjaArchiveScanner baseDir patchFile stampFile" << std::endl;
		return 1;
	}

	std::filesystem::path base{argv[1]};
	std::filesystem::path stampFile(argv[3]);

	auto *a = archive_read_new();
	archive_entry *entry;

	archive_read_support_filter_all(a);
	archive_read_support_format_all(a);
	if (archive_read_open_filename(a, argv[2], 4096) != ARCHIVE_OK){
		std::cerr << "Error open archive" << std::endl;
		return -1;
	}

	std::cout << "ninja_dyndep_version = 1" << std::endl;
	std::string resultRuleName = stampFile.string();
	std::cout << "build " << resultRuleName << " | ";
	while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
		std::filesystem::path p = archive_entry_pathname(entry);
		std::cout << " " << (base / p).string();
		archive_read_data_skip(a);  // Note 2
	}

	std::cout << "  : dyndep" << std::endl;
	std::cout << "  restat = 1" << std::endl;
	std::cout << std::endl;

	if (archive_read_free(a) != ARCHIVE_OK){
		std::cerr << "Error closing archive" << std::endl;
	}

	return 0;
}
