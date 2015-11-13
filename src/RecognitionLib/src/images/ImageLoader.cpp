#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <limits>
#include <fstream>
#include <memory>
#include "inc/images/ImageLoader.h"
#include "jpeglib.h"
#include "png.h"


Image ImageLoader::load(const void* data, const size_t data_length)
{
	if (data == nullptr)
		throw ImageLoadingException("Image Loader: Pointer to raw image data is NULL");
	if (data_length < 8)
		throw ImageLoadingException("Image Loader: Incorrect raw image data length");
	if (data_length >= (size_t)std::numeric_limits<int>().max())
		throw ImageLoadingException("Image Loader: Image is too large");

	const uint8_t* data_as_byte = (const uint8_t*)data;

	if (data_as_byte[0] == 0x42 && data_as_byte[1] == 0x4D)
		return std::move(load_bmp(data, data_length));

	if (data_as_byte[0] == 0xFF && data_as_byte[1] == 0xD8)
		return std::move(load_jpeg(data, data_length));

	if (data_as_byte[0] == 0x89 && data_as_byte[1] == 0x50 &&
		data_as_byte[2] == 0x4E && data_as_byte[3] == 0x47 &&
		data_as_byte[4] == 0x0D && data_as_byte[5] == 0x0A &&
		data_as_byte[6] == 0x1A && data_as_byte[7] == 0x0A)
	{
		return std::move(load_png(data, data_length));
	}

	throw ImageLoadingException("Image Loader: Not supported image format");
}


Image ImageLoader::load(const std::string& file_name)
{
	std::fstream file;
	file.open(file_name, std::ios_base::binary | std::ios_base::in);

	if (!file.is_open())
	{
		throw ImageLoadingException(std::string("Image Loader: Image file was not found: ") + file_name);
	}
	if (file.bad())
	{
		file.close();
		throw ImageLoadingException(std::string("Image Loader: Error during open image file: ") + file_name);
	}

	file.seekg(0, std::ios_base::end);
	if (file.tellg() > std::numeric_limits<int>::max())
	{
		file.close();
		throw ImageLoadingException(std::string("Image Loader: Image file too large: ") + file_name);
	}
	if (file.tellg() < 1)
	{
		file.close();
		throw ImageLoadingException(std::string("Image Loader: Image file too small: ") + file_name);
	}

	size_t image_bytes_length = (size_t)file.tellg();
	std::unique_ptr<char[]> image_bytes = std::unique_ptr<char[]>(new char[image_bytes_length]);

	file.seekg(0, std::ios_base::beg);
	file.read(image_bytes.get(), image_bytes_length);

	if (file.bad())
	{
		file.close();
		throw ImageLoadingException(std::string("Image Loader: Error while loading image file into memory: ") + file_name);
	}

	file.close();

	return std::move(ImageLoader::load(image_bytes.get(), image_bytes_length));
}


// ================= JPEG LOADER =============================

struct jpeg_custom_error_mgr
{
	struct jpeg_error_mgr pub;	/* "public" fields */

	jmp_buf setjmp_buffer;	/* for return to caller */
	char jpeg_last_error_msg[JMSG_LENGTH_MAX];
};

METHODDEF(void)
jpeg_error_exit_handler(j_common_ptr cinfo)
{
	/* cinfo->err really points to a jpeg_custom_error_mgr struct, so coerce pointer */
	jpeg_custom_error_mgr *err_mgr = (jpeg_custom_error_mgr *) cinfo->err;

	/* Create the message */
	( *(cinfo->err->format_message) ) (cinfo, err_mgr->jpeg_last_error_msg);

	/* Return control to the setjmp point */
	longjmp(err_mgr->setjmp_buffer, 1);
}


Image ImageLoader::load_jpeg(const void* data, const size_t data_length)
{	
	jpeg_decompress_struct cinfo;
	jpeg_custom_error_mgr error_mgr;
	Image result;

	cinfo.err = jpeg_std_error(&error_mgr.pub);
	error_mgr.pub.error_exit = &jpeg_error_exit_handler;

	if (setjmp(error_mgr.setjmp_buffer))
	{
		/* If we get here, the JPEG code has signaled an error. */
		jpeg_destroy_decompress(&cinfo);
		throw ImageLoadingException(std::string("JPEG Loader: Error while decompressing JPEG file: ") + error_mgr.jpeg_last_error_msg);
	}

	jpeg_create_decompress(&cinfo);
	
	jpeg_mem_src(&cinfo, (unsigned char*)data, (unsigned long)data_length);
	jpeg_read_header(&cinfo, TRUE);
	cinfo.out_color_space = JCS_GRAYSCALE;
	jpeg_start_decompress(&cinfo);
	
	if (cinfo.image_height != cinfo.output_height)
	{
		jpeg_destroy_decompress(&cinfo);
		throw ImageLoadingException(std::string("JPEG Loader: Image_height = ") + std::to_string(cinfo.image_height) +
									" is not equal to output_height = " + std::to_string(cinfo.output_height));
	}

	if (cinfo.output_components != 1)
	{
		jpeg_destroy_decompress(&cinfo);
		throw ImageLoadingException("JPEG Loader: Unexpected number of color components");
	}

	if (cinfo.image_height > 10000 || cinfo.image_height < 0 || cinfo.image_width > 10000 || cinfo.image_width < 0)
	{
		jpeg_destroy_decompress(&cinfo);
		throw ImageLoadingException(std::string("JPEG Loader: Image height = ") + std::to_string(cinfo.image_height) +
		"or width = " + std::to_string(cinfo.image_width) + " is out of boundaries");
	}

	result = Image(cinfo.image_width, cinfo.image_height);

	while (cinfo.output_scanline < cinfo.image_height)
	{
		uint8_t* p = result.data_row(cinfo.output_scanline);
		jpeg_read_scanlines(&cinfo, &p, 1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return std::move(result); 
}

//======================== PNG LOADER =========================================================//

struct png_memory_data_buffer
{
	png_memory_data_buffer(const void* data, const size_t data_length)
	{
		this->data = (const uint8_t*)data;
		this->data_length = data_length;
		ptr_position = 0;
	}
	const uint8_t* data;
	size_t data_length;
	png_size_t ptr_position;
};

#define PNG_MSG_BUF_SIZE 256
struct png_error_message_container
{
	char msg[PNG_MSG_BUF_SIZE];
};
void png_from_memory_read_func(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_memory_data_buffer *png_read_data = (png_memory_data_buffer *)png_get_io_ptr(png_ptr);
	if (png_read_data->ptr_position + length > png_read_data->data_length)
	{
		png_error(png_ptr, "Libpng requested more data than actually exist.");
	}
	memcpy(data, png_read_data->data + png_read_data->ptr_position, length);
	png_read_data->ptr_position += length;
}

void png_on_error_func(png_structp png_ptr, png_const_charp msg)
{
	png_voidp error_ptr = png_get_error_ptr(png_ptr);
	strncpy(((png_error_message_container *)error_ptr)->msg, msg, PNG_MSG_BUF_SIZE - 1);
	((png_error_message_container *)error_ptr)->msg[PNG_MSG_BUF_SIZE - 1] = 0;
}

Image ImageLoader::load_png(const void* data, const size_t data_length)
{
	png_memory_data_buffer read_data(data, data_length);
	png_structp png_ptr;
	png_infop info_ptr;
	png_error_message_container png_error;
	Image result;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp) &png_error,
			(png_error_ptr) png_on_error_func, (png_error_ptr)nullptr);
	if (png_ptr == nullptr)
	{
		throw ImageLoadingException("PNG Loader: Couldn't initialize png read struct");
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == nullptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp)nullptr, (png_infopp)nullptr);
		throw ImageLoadingException("PNG Loader: Couldn't initialize png info struct");
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)nullptr);
		throw ImageLoadingException(std::string("PNG Loader: Error while decompressing PNG file: ") + png_error.msg);
	}
	png_set_read_fn(png_ptr, (png_voidp) &read_data, &png_from_memory_read_func);
	
	png_read_info(png_ptr, info_ptr);

	png_uint_32 img_height = png_get_image_height(png_ptr, info_ptr);
	png_uint_32 img_width =  png_get_image_width(png_ptr, info_ptr);
	png_uint_32 bit_depth   = png_get_bit_depth(png_ptr, info_ptr);
	png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

	if (img_height > 10000 || img_height < 0 || img_width > 10000 || img_width < 0)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)nullptr);
		throw ImageLoadingException(std::string("PNG Loader: Image height = ") + std::to_string(img_height) +
									" or width = " + std::to_string(img_width) +
									" parameter value is out of boundaries.");
	}

	if (bit_depth == 16)
	{
		png_set_strip_16(png_ptr);
	}
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	{
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}
	else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		png_set_strip_alpha(png_ptr);
	}
	else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		png_set_rgb_to_gray(png_ptr, 1, 0.299, 0.587);
		png_set_strip_alpha(png_ptr);
	}
	else if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_rgb_to_gray(png_ptr, 1, 0.299, 0.587);
		png_set_strip_alpha(png_ptr);
	}
	else if (color_type == PNG_COLOR_TYPE_RGB)
	{
		png_set_rgb_to_gray(png_ptr, 1, 0.299, 0.587);
	}

	png_read_update_info(png_ptr, info_ptr);

	auto rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	if (rowbytes != img_width)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)nullptr);
		throw ImageLoadingException(std::string("PNG Loader: Rowbytes = ") + std::to_string(rowbytes) +
									" not equal to image width = " + std::to_string(img_width) +
									" in png loader.");
	}

	result = Image(img_width, img_height);
	png_read_image(png_ptr, result.data());

	png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)nullptr);
	return std::move(result);
}

//============================== BMP LOADER ===================================================//

#pragma pack(push, 1)
struct bitmap_file_header_t
{
	uint16_t	bf_type;
	uint32_t 	bf_size;
	uint16_t	bf_reserved1;
	uint16_t	bf_reserved2;
	uint32_t	bf_off_bits;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct bitmap_info_header_t
{
	uint32_t	bi_size;
	int32_t		bi_width;
	int32_t		bi_height;
	uint16_t	bi_planes;
	uint16_t 	bi_bit_count;
	uint32_t 	bi_compression;
	uint32_t 	bi_size_image;
	int32_t 	bi_xpels_per_meter;
	int32_t 	bi_ypels_per_meter;
	uint32_t 	bi_clr_used;
	uint32_t	bi_clr_important;
};
#pragma pack(pop)

Image ImageLoader::load_bmp(const void* data, const size_t data_length)
{

	uint8_t *data_as_byte = (uint8_t *) data;
	struct bitmap_file_header_t header;
	struct bitmap_info_header_t bmi_header;

	if (data_length < (sizeof(header) + sizeof(bmi_header)))
	{
		throw ImageLoadingException("BMP Loader: incorrect size of input data.");
	}
	memcpy(&header, data_as_byte, sizeof(header));
	data_as_byte += sizeof(header);

	if (header.bf_type != 0x4D42)
	{
		throw ImageLoadingException("BMP Loader: incorrect BMP signature bytes.");
	}

	if (header.bf_size != data_length)
	{
		throw ImageLoadingException(std::string("BMP Loader: Data length = ") + std::to_string(data_length) +
									" is not equal to size = " + std::to_string(header.bf_size) +
									" parameter from file.");
	}

	memcpy(&bmi_header, data_as_byte, sizeof(bmi_header));
	data_as_byte += sizeof(bmi_header);

	if (bmi_header.bi_size != 40)
	{
		throw ImageLoadingException(std::string("BMP Loader: Bitmapinfo struct size = ") + std::to_string(bmi_header.bi_size) +
									" is incorrect.");
	}

	if (bmi_header.bi_width < 0 || bmi_header.bi_width > 10000 || bmi_header.bi_height < 0 || bmi_header.bi_height > 10000)
	{
		throw ImageLoadingException(std::string("BMP Loader: Bitmapinfo length = ") + std::to_string(bmi_header.bi_height) +
									" or width = " + std::to_string(bmi_header.bi_width) +
									" parameter value is out of boundaries.");
	}

	if (bmi_header.bi_bit_count != 24)
	{
		throw ImageLoadingException(std::string("BMP Loader: Bit count = ") + std::to_string(bmi_header.bi_bit_count) +
									" parameter is not equal 24.");
	}

	if (bmi_header.bi_compression != 0)
	{
		throw ImageLoadingException("BMP Loader: Bitmapinfo compression paramete is not equal to 0.");
	}

	if (header.bf_off_bits != (sizeof(header) + sizeof(bmi_header)))
	{
		throw ImageLoadingException("BMP Loader: Incorrect offset to pixel data.");
	}

	int pad_size = (4 - (3* bmi_header.bi_width) % 4) % 4;

	size_t remained_data_length = data_length - (sizeof(header) + sizeof(bmi_header));
	const size_t rgb_data_len = sizeof(unsigned char)*3;
	Image result(bmi_header.bi_width, bmi_header.bi_height);
	for (int y = bmi_header.bi_height - 1; y >= 0; y--)
	{
		for (int x = 0; x < bmi_header.bi_width; x++)
		{
			if (remained_data_length < rgb_data_len)
				throw ImageLoadingException("BMP Loader: Out of boundaries of image data.");

			unsigned char gray_pix = (unsigned char)((data_as_byte[2] * 0.299) + (data_as_byte[1] * 0.587) + (data_as_byte[0] * 0.114));
			result.set(x, y, gray_pix);
			data_as_byte += rgb_data_len;
			remained_data_length -= rgb_data_len;
		}
		data_as_byte += pad_size;
		remained_data_length -= pad_size;
	}

	if (remained_data_length != 0)
	{
		throw ImageLoadingException("BMP Loader: Remained data size to read is not equal to 0.");
	}
	return std::move(result);
}

//================================ BMP SAVER =================================================//

size_t ImageLoader::save_bmp(const Image& image, void* buffer, const size_t buffer_length)
{
	unsigned char file[14] = {
		'B','M', // format
		0,0,0,0, // file size (write later!!!)
		0,0, // reserved data
		0,0, // reserved data
		40+14,0,0,0 // start of data offset
	};

	unsigned char info[40] = {
		40,0,0,0, // info hd size
		0,0,0,0, // width
		0,0,0,0, // height
		1,0, // number of color planes
		24,0, // number of bpp
		0,0,0,0, // compression (none)
		0,0,0,0, // image bits size
		0x13,0x0B,0,0, // horizontal resolution in pixels
		0x13,0x0B,0,0, // vertical resolution in pixels
		0,0,0,0, // colors in pallete
		0,0,0,0 // important colors
	};

	int pad_size = (4 - (3*image.get_width()) % 4) % 4;
	int size_data = image.get_width() * image.get_height() * 3 + image.get_height() * pad_size;
	int size_all = size_data + sizeof(file) + sizeof(info);
	
	*(int*)(file + 2) = size_all;
	*(int*)(info + 4) = image.get_width();
	*(int*)(info + 8) = image.get_height();
	*(int*)(info + 24) = size_data;

	if ((size_t) size_all > buffer_length)
	{
		throw ImageLoadingException("BMP Saver: Buffer is too small");
	}

	uint8_t* byte_buffer = (uint8_t*)buffer;
	memcpy(byte_buffer, file, sizeof(file));
	byte_buffer += sizeof(file);
	memcpy(byte_buffer, info, sizeof(info));
	byte_buffer += sizeof(info);

	unsigned char pad[3] = {0,0,0};

	for (int y = image.get_height() - 1; y >= 0; y--)
	{
		for (int x = 0; x < image.get_width(); x++)
		{
			*byte_buffer = image.get(x, y);
			byte_buffer++;

			*byte_buffer = image.get(x, y);
			byte_buffer++;

			*byte_buffer = image.get(x, y);
			byte_buffer++;
		}
		memcpy(byte_buffer, pad, pad_size);
		byte_buffer += pad_size;
	}

	return size_all;
}

//================================ IMAGE MAGIC LOADER =================================================//



Image ImageLoader::load_with_image_magic(const void* data, const size_t data_length)
{
	throw ImageLoadingException("ImageMagick currently is not supported");

	//try
	//{
	//	Magick::Blob blob(data, data_length);
	//	Magick::Image img;
	//	img.read(blob);
	//	img.colorSpace(MagickCore::ColorspaceType::GRAYColorspace);

	//	Image result(img.columns(), img.rows());

	//	auto pixels = img.getConstPixels(0, 0, img.columns(), img.rows());
	//	for (int y = 0; y < result.get_height(); y++)
	//	{
	//		for (int x = 0; x < result.get_width(); x++)
	//		{
	//			uint8_t pixelVal = (uint8_t)(pixels->red / 256);
	//			result.set(x, y, pixelVal);
	//			pixels++;
	//		}
	//	}

	//	return std::move(result);
	//}
	//catch (Magick::Exception& error)
	//{
	//	throw ImageLoadingException(error.what());
	//}
}




//=================================================================================//