#include <defns.h>

#include <boost/program_options.hpp>
#include <lz4_p2p_comp.hpp>
#include <vector>

#define MEMORY_SIZE 2U << 31

using namespace std;

struct Options {
  string compress_xclbin;
  unsigned long block_size;
  unsigned num_memory;
  string filename;
  uint32_t memory_size;
} g_options{};

int main(int argc, char *argv[]) {
  namespace po = boost::program_options;

  po::options_description desc("Options");
  po::positional_options_description g_pos; /* no positional options */

  desc.add_options()("help,h", "Show help")(
      "compress_xclbin", po::value<std::string>()->required(),
      "Kernel compression bin xclbin file")(
      "num_memory", po::value<unsigned>()->default_value(1),
      "Number of memory to compress")(
      "filename", po::value<string>()->required(), "Output file name in ssd")(
      "memory_size", po::value<uint32_t>()->required(),
      "Memory size to compress (MB)")(
      "block_size", po::value<unsigned long>()->default_value(BLOCK_SIZE_IN_KB),
      "Compress block size");

  po::variables_map vm;
  po::store(
      po::command_line_parser(argc, argv).options(desc).positional(g_pos).run(),
      vm);

  if (vm.count("help") > 0) {
    std::cout << desc;
    return -1;
  }

  g_options.block_size = vm["block_size"].as<unsigned long>();
  g_options.compress_xclbin = vm["compress_xclbin"].as<string>();
  g_options.num_memory = vm["num_memory"].as<unsigned>();
  g_options.filename = vm["filename"].as<string>();
  g_options.memory_size = vm["memory_size"].as<uint32_t>() * (1 << 20);

  vector<char *> inVec;
  vector<string> outVec;
  vector<uint32_t> inSizeVec;

  for (unsigned i = 0; i < g_options.num_memory; i++) {
    outVec.push_back(g_options.filename + "." + to_string(i));

    char *memory =
        static_cast<char *>(aligned_alloc(4096, g_options.memory_size));
    memset(memory, rand() % 256, g_options.memory_size);
    inVec.push_back(memory);
    inSizeVec.push_back(g_options.memory_size);
  }

  std::cout << "\x1B[32m[OpenCL Setup]\033[0m OpenCL/Host/Device Buffer Setup "
               "Started ..."
            << std::endl;
  xflz4 xlz(g_options.compress_xclbin, 0, g_options.block_size);
  std::cout << "\x1B[32m[OpenCL Setup]\033[0m OpenCL/Host/Device Buffer Setup "
               "Done ..."
            << std::endl;
  std::cout << "\n";
  std::cout << "\x1B[36m[FPGA LZ4]\033[0m LZ4 P2P Compression Started ..."
            << std::endl;
  xlz.compress_in_line_multiple_files(inVec, outVec, inSizeVec, true);
  std::cout << "\n\n";
  std::cout << "\x1B[36m[FPGA LZ4]\033[0m LZ4 P2P Compression Done ..."
            << std::endl;
}