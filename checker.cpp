#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <elf.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " INPUT_FILE" << std::endl;
    return EXIT_FAILURE;
  }

  // Open file
  std::ifstream input_file(argv[1], std::ios::binary | std::ios::in);
  if (!input_file.is_open() | !input_file.good()) {
    std::cerr << "Unable to open file" << std::endl;
    return EXIT_FAILURE;
  }

  // Read in data
  std::string data{std::istreambuf_iterator<char>(input_file),
                   std::istreambuf_iterator<char>()};
  input_file.close();
  std::cout << "File is " << data.size() << " bytes" << std::endl;

  // Get ELF view
  Elf64_Ehdr *elf_header = reinterpret_cast<Elf64_Ehdr *>(&data[0]);
  Elf64_Shdr *elf_shdr_base =
      reinterpret_cast<Elf64_Shdr *>(&data[elf_header->e_shoff]);
  char *string_table =
      &data[(elf_shdr_base + elf_header->e_shstrndx)->sh_offset];

  // Find .main_function_section
  int function_section_index{};
  for (auto i = 0; i < elf_header->e_shnum; ++i) {
    std::string current_section_name =
        &string_table[(elf_shdr_base + i)->sh_name];
    if (current_section_name == ".main_function_section") {
      std::cout << "Found it at index: " << i << std::endl;
      function_section_index = i;
      break;
    }
  }

  // Find function CRC target
  int *main_function_size = nullptr;
  for (auto i = 0; i < elf_header->e_shnum; ++i) {
    std::string current_section_name =
        &string_table[(elf_shdr_base + i)->sh_name];
    if (current_section_name == ".main_function_crc") {
      std::cout << "Found the size at index: " << i << std::endl;
      main_function_size =
          reinterpret_cast<int *>(&data[(elf_shdr_base + i)->sh_offset]);
      break;
    }
  }

  // Calculate CRC value and populate it in the file
  int total = 0;
  for (auto i = 0; i < (elf_shdr_base+function_section_index)->sh_size; ++i)
  {
    total += abs(data[(elf_shdr_base+function_section_index)->sh_offset + i]);
  }
  std::cout << total << std::endl;
  *main_function_size = total;

  // Write results back out
  std::ofstream output_file("/tmp/final.bin", std::ios::binary | std::ios::out);
  output_file.write(data.data(), data.size());
  output_file.close();

  return EXIT_SUCCESS;
}
