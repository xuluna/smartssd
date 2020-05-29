#include <defns.h>
#include <boost/program_options.hpp>
#include <lz4_p2p_comp.hpp>
#include <vector>

#define MEMORY_SIZE 2 << 31

using namespace std;

struct Options
{
  string compress_xclbin;
  unsigned long block_size;
  unsigned device_id;
  string filename;
} g_options{};

int main(int argc, char *argv[])
{
  namespace po = boost::program_options;

  po::options_description desc("Options");
  po::positional_options_description g_pos; /* no positional options */

  desc.add_options()("help,h", "Show help")("compress_xclbin", po::value<std::string>(&g_options.compress_xclbin)->required(), "Kernel compression bin xclbin file")("device_id", po::value<unsigned>(&g_options.device_id)->default_value(0), "Device id (e.g., 0)")("filename", po::value<string>(&g_options.filename)->required(), "Output file name in ssd")("block_size", po::value<unsigned long>(&g_options.block_size)->default_value(BLOCK_SIZE_IN_KB), "Compress block size");

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(g_pos).run(), vm);

  if (vm.count("help") > 0)
  {
    std::cout << desc;
    return -1;
  }

  vector<char *> inVec;
  vector<string> outVec;
  vector<uint32_t> inSizeVec;

  outVec.push_back(g_options.filename);

  char *memory = static_cast<char *>(aligned_alloc(4096, MEMORY_SIZE));
  memset(memory, 56, MEMORY_SIZE);
  inVec.push_back(memory);
  inSizeVec.push_back(MEMORY_SIZE);
  std::cout << "\x1B[32m[OpenCL Setup]\033[0m OpenCL/Host/Device Buffer Setup Started ..." << std::endl;
  xflz4 xlz(g_options.compress_xclbin, g_options.device_id, g_options.block_size);
  std::cout << "\x1B[32m[OpenCL Setup]\033[0m OpenCL/Host/Device Buffer Setup Done ..." << std::endl;
  std::cout << "\n";
  std::cout << "\x1B[36m[FPGA LZ4]\033[0m LZ4 P2P Compression Started ..." << std::endl;
  xlz.compress_in_line_multiple_files(inVec, outVec, inSizeVec, true);
  std::cout << "\n\n";
  std::cout << "\x1B[36m[FPGA LZ4]\033[0m LZ4 P2P Compression Done ..." << std::endl;
}