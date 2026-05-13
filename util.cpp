#include "util.h"
#include <fstream>
#include <iostream>

PNGDisplayInfo pngInfo;

PNGDisplayInfo ExtractPNGInfo(const char* filename)
{
  PNGDisplayInfo out;

  /* ---------- read file into memory ---------- */
  std::ifstream file(filename, std::ios::binary);
  if (!file)                 // failure → empty object
      return out;

  std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());

  /* ---------- libpng setup ---------- */
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                           nullptr, nullptr, nullptr);
  if (!png)
      return out;

  png_infop info = png_create_info_struct(png);
  if (!info) {
      png_destroy_read_struct(&png, nullptr, nullptr);
      return out;
  }

  if (setjmp(png_jmpbuf(png))) {           // error path
      png_destroy_read_struct(&png, &info, nullptr);
      return out;
  }

  /* feed libpng from the in‑memory buffer */
  png_set_read_fn(
      png, &buffer,
      [](png_structp png_ptr, png_bytep dst, png_size_t len)
      {
          auto& buf = *static_cast<std::vector<unsigned char>*>(
                         png_get_io_ptr(png_ptr));
          std::copy(buf.begin(), buf.begin() + len, dst);
          buf.erase(buf.begin(), buf.begin() + len);
      });

  png_read_info(png, info);

  // ---------------- text chunks -------------
  int num_text = 0;
  png_textp text_data = nullptr;
  num_text = png_get_text(png, info, &text_data, &num_text);

  if (!num_text){
    std::cout << "No text metadata found in the png file." << std::endl;
  }

  //Loop through each text chunk
  for (int i = 0; i < num_text; ++i){
    out.lines.emplace_back("Chunk key: "  + std::string(text_data[i].key));
    out.lines.emplace_back("Text: "       + std::string(text_data[i].text));

    //Determine chunk type
    switch (text_data[i].compression){
      case PNG_TEXT_COMPRESSION_NONE:
        out.lines.emplace_back("Chunk type: tEXt (uncompressed)");
        break;
      case PNG_TEXT_COMPRESSION_zTXt:
        out.lines.emplace_back("Chunk type: TXt (compressed)");
        break;
      case PNG_ITXT_COMPRESSION_NONE:
      case PNG_ITXT_COMPRESSION_zTXt:
        out.lines.emplace_back("Chunk type: zTXt (international text)");
        break;
      default:
        out.lines.emplace_back("Chunk type: unknown");
    }
  }
    out.lines.emplace_back("-----------------------------------");

    png_get_IHDR(png, info, &out.width, &out.height, &out.bitDepth, nullptr, nullptr, nullptr, nullptr);
    /*std::cout << "PNG Info " << "Width: " << out.width << ", Height: " << out.height
              << ", Bit Depth: " << out.bitDepth << std::endl;
              */
        
    out.lines.insert(out.lines.begin(),
      "PNG Info W:" + std::to_string(out.width)  +
      "  H:"        + std::to_string(out.height) +
      "  BitDepth:" + std::to_string(out.bitDepth));

    //Cleanup
    png_destroy_read_struct(&png, &info, nullptr);
    return out;
}

void ReadTextChunks(png_structp png, png_infop info)
{
  int num_text = 0;
  png_textp text_data = nullptr;
  
  // Retrieve the text chunks (tEXt, zTXt, iTXt)
  num_text = png_get_text(png, info, &text_data, &num_text);

  if (!num_text){
    std::cout << "No text metadata found in the png file." << std::endl;
    return;
  }

  //Loop through each text chunk
  for (int i = 0; i < num_text; ++i){
    std::cout << "Chunk key: " << text_data[i].key << std::endl;
    std::cout << "Text: " << text_data[i].text << std::endl;

    //Determine chunk type
    switch (text_data[i].compression){
      case PNG_TEXT_COMPRESSION_NONE:
        std::cout << "Chunk type: tEXt (uncompressed)" << std::endl;
        break;
      case PNG_TEXT_COMPRESSION_zTXt:
        std::cout << "Chunk type: TXt (compressed)" << std::endl;
        break;
      case PNG_ITXT_COMPRESSION_NONE:
      case PNG_ITXT_COMPRESSION_zTXt:
        std::cout << "Chunk type: zTXt (international text)" << std::endl;
        break;
      default:
        std::cout << "Chunk type: unknown" << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;
  }
}

void PrintPNGInfo(const char* filename)
{
  // Open the PNG file using std::ifstream
  std::ifstream file(filename, std::ios::binary);
  if (!file){
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }

  // Read the file content into a vector
  std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  // Init libpng structs
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png){
    std::cerr << "png_create_read_struct failed.\n" << std::endl;
    return;
  }

  png_infop info = png_create_info_struct(png);
  if (!info){
    std::cerr << "png_create_info_struct failed.\n" << std::endl;
    png_destroy_read_struct(&png, nullptr, nullptr);
    return;
  }

  if (setjmp(png_jmpbuf(png))){
    std::cerr << "libpng error!\n" << std::endl;
    png_destroy_read_struct(&png, &info, nullptr);
    return;
  }

  /**
   * @brief Process the input buffer for libpng.
   * @details Read PNG data directly from the std::vector with a custom callback function
   * @param &buffer is the raw pointer that was passed to libpng; buf is a reference to the
   * dereferenced pointer retrieved from png_get_io_ptr(png).
   */
  png_set_read_fn(png, &buffer, [](png_structp png, png_bytep data, png_size_t length){
      auto &buf = *static_cast<std::vector<unsigned char>*>(png_get_io_ptr(png));
      std::copy(buf.begin(), buf.begin() + length, data);
      buf.erase(buf.begin(), buf.begin() + length);
      });

  // Read the PNG header
  png_read_info(png, info);

  // Print text metadata
  ReadTextChunks(png, info);

  // Get generics of the PNG
  png_uint_32 width, height;
  int bit_depth, color_type;

  png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, nullptr, nullptr, nullptr);
  std::cout << "PNG Info " << "Width: " << width << ", Height: " << height
            << ", Bit Depth: " << bit_depth << std::endl;
      
  //Cleanup
  png_destroy_read_struct(&png, &info, nullptr);
}

