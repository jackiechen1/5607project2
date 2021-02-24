//CSCI 5607 HW 2 - Image Conversion Instructor: S. J. Guy <sjguy@umn.edu>
//In this assignment you will load and convert between various image formats.
//Additionally, you will manipulate the stored image data by quantizing, cropping, and supressing channels

#include "image.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include <fstream>
using namespace std;
#define PI 3.1415926
/**
 * Image
 **/
 //TODO - HW2: The current implementation of write_ppm ignores the paramater "bits" and assumes we want to write out an 8-bit PPM ...
 //TODO - HW2: ... you need to adjust the function to scale the values written in the PPM file based on the "bits" variable
void write_ppm(char* imgName, int width, int height, int bits, const uint8_t* data) {
	//Open the texture image file
	ofstream ppmFile;
	ppmFile.open(imgName);
	if (!ppmFile) {
		printf("ERROR: Could not creat file '%s'\n", imgName);
		exit(1);
	}

	//Set this as an ASCII PPM (first line is P3)
	string PPM_style = "P3\n";
	ppmFile << PPM_style; //Read the first line of the header    

	//Write out the texture width and height
	ppmFile << width << " " << height << "\n";

	//Set's the 3rd line to 255 (ie., assumes this is an 8 bit/pixel PPM)
	//TODO - HW2: Set the maximum values based on the variable bits
	int maximum = pow(2, bits);
	ppmFile << maximum - 1 << "\n";

	//TODO - HW2: The values in data are all 8 bits, you must convert down to whatever the variable bits is when writing the file
	int r, g, b, a;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			r = static_cast<int> (data[i * width * 4 + j * 4 + 0] * maximum / 256.0);  //Red
			g = static_cast<int> (data[i * width * 4 + j * 4 + 1] * maximum / 256.0);  //Green
			b = static_cast<int> (data[i * width * 4 + j * 4 + 2] * maximum / 256.0);  //Blue
			a = static_cast<int> (data[i * width * 4 + j * 4 + 3] * maximum / 256.0);  //Alpha
			ppmFile << r << " " << g << " " << b << " ";
		}
	}

	ppmFile.close();
}

void write_zyj(char* imgName, int width, int height, int bits, const uint8_t* data) {
	//Open the texture image file
	ofstream ppmFile;
	ppmFile.open(imgName);
	if (!ppmFile) {
		printf("ERROR: Could not creat file '%s'\n", imgName);
		exit(1);
	}

	//Set this as an ASCII PPM (first line is P3)
	string PPM_style = "P3\n";
	ppmFile << PPM_style; //Read the first line of the header    

	//Write out the texture width and height
	ppmFile << width << " " << height << "\n";

	//Set's the 3rd line to 255 (ie., assumes this is an 8 bit/pixel PPM)
	//TODO - HW2: Set the maximum values based on the variable bits
	int maximum = pow(2, bits);
	ppmFile << maximum - 1 << "\n";

	//TODO - HW2: The values in data are all 8 bits, you must convert down to whatever the variable bits is when writing the file
	int r, g, b, a;
	int value;
	int time = 0;
	int previous = -1;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			r = static_cast<int> (data[i * width * 4 + j * 4 + 0] * maximum / 256.0);  //Red
			if (r == previous)
			{
				time++;
			}
			else
			{
				if (previous != -1)
				{
					ppmFile << previous << " " << time << " ";
					time = 1;
					previous = r;
				}
				else
				{
					time = 1;
					previous = r;
				}
			}
			g = static_cast<int> (data[i * width * 4 + j * 4 + 1] * maximum / 256.0);  //Green
			if (g == previous)
			{
				time++;
			}
			else
			{
				if (previous != -1)
				{
					ppmFile << previous << " " << time << " ";
					time = 1;
					previous = g;
				}
				else
				{
					time = 1;
					previous = g;
				}
			}
			b = static_cast<int> (data[i * width * 4 + j * 4 + 2] * maximum / 256.0);  //Blue
			if (b == previous)
			{
				time++;
			}
			else
			{
				if (previous != -1)
				{
					ppmFile << previous << " " << time << " ";
					time = 1;
					previous = b;
				}
				else
				{
					time = 1;
					previous = b;
				}
			}
			a = static_cast<int> (data[i * width * 4 + j * 4 + 3] * maximum / 256.0);  //Alpha
			// ppmFile << r << " " << g << " " << b << " ";
		}
	}

	ppmFile.close();
}
uint8_t* read_ppm(char* imgName, int& width, int& height) {
	//Open the texture image file
	ifstream ppmFile;
	ppmFile.open(imgName);
	if (!ppmFile) {
		printf("ERROR: Image file '%s' not found.\n", imgName);
		exit(1);
	}

	//Check that this is an ASCII PPM (first line is P3)
	string PPM_style;
	ppmFile >> PPM_style; //Read the first line of the header    
	if (PPM_style != "P3") {
		printf("ERROR: PPM Type number is %s. Not an ASCII (P3) PPM file!\n", PPM_style.c_str());
		exit(1);
	}

	//Read in the texture width and height
	ppmFile >> width >> height;
	unsigned char* img_data = new unsigned char[4 * width * height];

	//Check that the 3rd line is 255 (ie., this is an 8 bit/pixel PPM)
	int maximum;
	ppmFile >> maximum;

	int r, g, b;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			ppmFile >> r >> g >> b;
			img_data[i * width * 4 + j * 4] = 256 / (maximum + 1) * r;  //Red
			img_data[i * width * 4 + j * 4 + 1] = 256 / (maximum + 1) * g;  //Green
			img_data[i * width * 4 + j * 4 + 2] = 256 / (maximum + 1) * b;  //Blue
			img_data[i * width * 4 + j * 4 + 3] = 255;  //Alpha
		}
	}

	return img_data;
}

uint8_t* read_zyj(char* imgName, int& width, int& height) {
	//Open the texture image file
	ifstream ppmFile;
	ppmFile.open(imgName);
	if (!ppmFile) {
		printf("ERROR: Image file '%s' not found.\n", imgName);
		exit(1);
	}

	//Check that this is an ASCII PPM (first line is P3)
	string PPM_style;
	ppmFile >> PPM_style; //Read the first line of the header    
	if (PPM_style != "P3") {
		printf("ERROR: PPM Type number is %s. Not an ASCII (P3) PPM file!\n", PPM_style.c_str());
		exit(1);
	}

	//Read in the texture width and height
	ppmFile >> width >> height;
	unsigned char* img_data = new unsigned char[4 * width * height];

	//Check that the 3rd line is 255 (ie., this is an 8 bit/pixel PPM)
	int maximum;
	ppmFile >> maximum;

	int r, g, b;
	int value;
	int times = 0;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (times == 0)
			{
				ppmFile >> value >> times;
			}
			
			if (times == 1)
			{
				img_data[i * width * 4 + j * 4] = 256 / (maximum + 1) * value;  //Red
				ppmFile >> value >> times;
				if (times == 1)
				{
					img_data[i * width * 4 + j * 4 + 1] = 256 / (maximum + 1) * value;  //Green
					ppmFile >> value >> times;
					img_data[i * width * 4 + j * 4 + 2] = 256 / (maximum + 1) * value;  //Blue
					times--;
				}
				else if (times >= 2)
				{
					img_data[i * width * 4 + j * 4 + 1] = 256 / (maximum + 1) * value;  //Green
					img_data[i * width * 4 + j * 4 + 2] = 256 / (maximum + 1) * value;  //Blue
					times-=2;
				}
			}
			else if (times == 2)
			{
				img_data[i * width * 4 + j * 4] = 256 / (maximum + 1) * value;  //Red
				img_data[i * width * 4 + j * 4 + 1] = 256 / (maximum + 1) * value;  //Green
				ppmFile >> value >> times;
				img_data[i * width * 4 + j * 4 + 2] = 256 / (maximum + 1) * value;  //Blue
				times--;
			}
			else if (times >= 3)
			{
				img_data[i * width * 4 + j * 4] = 256 / (maximum + 1) * value;  //Red
				img_data[i * width * 4 + j * 4 + 1] = 256 / (maximum + 1) * value;  //Green
				img_data[i * width * 4 + j * 4 + 2] = 256 / (maximum + 1) * value;  //Blue
				times -= 3;
			}
			
			img_data[i * width * 4 + j * 4 + 3] = 255;  //Alpha
		}
	}

	return img_data;
}

Image::Image (int width_, int height_){

    assert(width_ > 0);
    assert(height_ > 0);

    width           = width_;
    height          = height_;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;
    
    data.raw = new uint8_t[num_pixels*4];
		int b = 0; //which byte to write to
		for (int j = 0; j < height; j++){
			for (int i = 0; i < width; i++){
				data.raw[b++] = 0;
				data.raw[b++] = 0;
				data.raw[b++] = 0;
				data.raw[b++] = 0;
			}
		}

    assert(data.raw != NULL);
}

Image::Image (const Image& src){
	width           = src.width;
	height          = src.height;
	num_pixels      = width * height;
	sampling_method = IMAGE_SAMPLING_POINT;
	
	data.raw = new uint8_t[num_pixels*4];
	
	//memcpy(data.raw, src.data.raw, num_pixels);
	*data.raw = *src.data.raw;
}

Image::Image (char* fname){
	int lastc = strlen(fname);
	if (string(fname + lastc - 3) == "ppm") {
		data.raw = read_ppm(fname, width, height);
	}
	else if (string(fname + lastc - 3) == "zyj")
	{
		data.raw = read_zyj(fname, width, height);
	}
	else {
		int numComponents; //(e.g., Y, YA, RGB, or RGBA)
		data.raw = stbi_load(fname, &width, &height, &numComponents, 4);
	}

	if (data.raw == NULL) {
		printf("Error loading image: %s", fname);
		exit(-1);
	}

	num_pixels = width * height;
	sampling_method = IMAGE_SAMPLING_POINT;
	
}

Image::~Image (){
    delete data.raw;
    data.raw = NULL;
}

void Image::Write(char* fname){
	
	int lastc = strlen(fname);

	switch (fname[lastc-1]){
		case 'm': //ppm
			write_ppm(fname, width, height, export_depth, data.raw);
			break;
	   case 'g': //jpeg (or jpg) or png
	     if (fname[lastc-2] == 'p' || fname[lastc-2] == 'e') //jpeg or jpg
	        stbi_write_jpg(fname, width, height, 4, data.raw, 95);  //95% jpeg quality
	     else //png
	        stbi_write_png(fname, width, height, 4, data.raw, width*4);
	     break;
	   case 'a': //tga (targa)
	     stbi_write_tga(fname, width, height, 4, data.raw);
	     break;
	   case 'p': //bmp

	   case 'j':
		   OrderedDither(1);
		   write_zyj(fname, width, height, export_depth, data.raw);
		   break;
	   default:
	     stbi_write_bmp(fname, width, height, 4, data.raw);
	}
}


void Image::Brighten (double factor){
	int x,y;
	for (x = 0 ; x < Width() ; x++){
		for (y = 0 ; y < Height() ; y++){
			Pixel p = GetPixel(x, y);
			Pixel scaled_p = p*factor;
			GetPixel(x,y) = scaled_p;
		}
	}
}

void Image::ExtractChannel(int channel){
	int x, y;
	for (x = 0; x < Width(); x++) {
		for (y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			if (channel == 0)
			{
				p.b = 0;
				p.g = 0;
			}
			else if (channel == 1)
			{
				p.r = 0;
				p.b = 0;
			}
			else if (channel == 2)
			{
				p.r = 0;
				p.g = 0;
			}
			else
			{
				return;
			}
			SetPixel(x, y, p);
		}
	}
}


void Image::Quantize (int nbits){
	int x, y;
	for (x = 0; x < Width(); x++) {
		for (y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			p = PixelQuant(p, nbits);
			SetPixel(x, y, p);
		}
	}
}

Image* Image::Crop(int x, int y, int w, int h){
	Image* result = new Image(w, h);
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			Pixel p = GetPixel(x + i, y + j);
			result->SetPixel(i, j, p);
		}
	}
	return result;
}


void Image::AddNoise (double factor){
	int x, y;
	for (x = 0; x < Width(); x++) {
		for (y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			double randomnum = rand() / double(RAND_MAX);
			if (randomnum < factor)
			{
				Pixel noise_p = PixelRandom();
				SetPixel(x, y, noise_p);
			}
		}
	}
}

void Image::ChangeContrast (double factor){
	int x, y;
	float l = 0;
	for (x = 0; x < Width(); x++) {
		for (y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			
			l += 0.3 * p.r+ 0.59 * p.g+ 0.11 * p.b; // add up grey version of each pixel
		}
	}
	l /= NumPixels();
	for (x = 0; x < Width(); x++) {
		for (y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			Pixel mean = Pixel((int)l, (int)l, (int)l, 1);
			SetPixel(x, y, PixelLerp(mean,p, factor));
		}
	}
}


void Image::ChangeSaturation(double factor){
	int x, y;
	float l = 0;

	for (x = 0; x < Width(); x++) {
		for (y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			l = 0.3 * p.r + 0.59 * p.g + 0.11 * p.b;
			Pixel mean = Pixel((int)l, (int)l, (int)l, 1);
			SetPixel(x, y, PixelLerp(mean, p, factor));
		}
	}
}


//For full credit, check that your dithers aren't making the pictures systematically brighter or darker
void Image::RandomDither (int nbits){
	int x, y;
	int delta = 256 / (pow(2, nbits));
	
	for (x = 0; x < Width(); x++) {
		for (y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			//int random = rand() % (delta + 1);
			int red, green, blue;
			int p_divid_delta = p.r / delta;
			int p_mod_delta = p.r % delta;
			if (p_mod_delta >  delta/2)
			{
				
				red = (p_divid_delta + 1) * (delta)-1;
				
			}
			else
			{
				red = p_divid_delta*delta-1;
				if (red < 0)
				{
					red = 0;
				}
				
			}
			p_divid_delta = p.g / delta;
			p_mod_delta = p.g % delta;
			if (p_mod_delta > delta / 2)
			{
				green = (p_divid_delta + 1) * (delta)-1;
			}
			else
			{
				green = p_divid_delta*delta-1;
				if (green < 0)
				{
					green = 0;
				}
			}
			p_divid_delta = p.b / delta;
			p_mod_delta = p.b % delta;
			if (p_mod_delta > delta / 2)
			{
				blue = (p_divid_delta + 1) * (delta)-1;
			}
			else
			{
				blue = p_divid_delta*delta-1;
				if (blue < 0)
				{
					blue = 0;
				}
			}
			Pixel result(red, green, blue, 255);
			SetPixel(x, y, result);
		}
	}
}

//This bayer method gives the quantization thresholds for an ordered dither.
//This is a 4x4 dither pattern, assumes the values are quantized to 16 levels.
//You can either expand this to a larger bayer pattern. Or (more likely), scale
//the threshold based on the target quantization levels.
static int Bayer4[4][4] ={
    {15,  7, 13,  5},
    { 3, 11,  1,  9},
    {12,  4, 14,  6},
    { 0,  8,  2, 10}
};


void Image::OrderedDither(int nbits){
	int delta = 256 / (pow(2, nbits));
	
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			//int random = rand() % (delta + 1);
			int red, green, blue;
			int p_divid_delta = p.r / delta;
			int p_mod_delta = p.r % delta;
			int i = x % 4;
			int j = y % 4;
			if (p_mod_delta > Bayer4[i][j] * (delta / 16.0))
			{

				red = (p_divid_delta + 1) * (delta)-1;

			}
			else
			{
				red = p_divid_delta * delta - 1;
				if (red < 0)
				{
					red = 0;
				}

			}
			p_divid_delta = p.g / delta;
			p_mod_delta = p.g % delta;
			if (p_mod_delta > Bayer4[i][j] * (delta / 16.0))
			{
				green = (p_divid_delta + 1) * (delta)-1;
			}
			else
			{
				green = p_divid_delta * delta - 1;
				if (green < 0)
				{
					green = 0;
				}
			}
			p_divid_delta = p.b / delta;
			p_mod_delta = p.b % delta;
			if (p_mod_delta > Bayer4[i][j] * (delta / 16.0))
			{
				blue = (p_divid_delta + 1) * (delta)-1;
			}
			else
			{
				blue = p_divid_delta * delta - 1;
				if (blue < 0)
				{
					blue = 0;
				}
			}
			Pixel result(red, green, blue, 255);
			SetPixel(x, y, result);
		}
	}
}

/* Error-diffusion parameters */
const double
    ALPHA = 7.0 / 16.0,
    BETA  = 3.0 / 16.0,
    GAMMA = 5.0 / 16.0,
    DELTA = 1.0 / 16.0;

void Image::FloydSteinbergDither(int nbits){
	int x, y;
	for (x = 0; x < Width(); x++) {
		for (y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			Pixel q = PixelQuant(p, nbits);
			SetPixel(x, y, q);
			float r_dif = p.r - q.r;
			float g_dif = p.g - q.g;
			float b_dif = p.b - q.b;
			Pixel dif(r_dif, g_dif, b_dif,255);
			Pixel right(GetPixel(x + 1, y).r + r_dif * (7.0 / 16), GetPixel(x + 1, y).g + g_dif * (7.0 / 16), GetPixel(x + 1, y).b + b_dif * (7.0 / 16), 255);
			Pixel left(GetPixel(x - 1, y + 1).r + r_dif * (3.0 / 16), GetPixel(x - 1, y + 1).g + g_dif * (3.0 / 16), GetPixel(x - 1, y + 1).b + b_dif * (3.0 / 16),255);
			Pixel down(GetPixel(x, y + 1).r + r_dif * (5.0 / 16), GetPixel(x, y + 1).g + g_dif * (5.0 / 16), GetPixel(x, y + 1).b + b_dif * (5.0 / 16), 255);
			Pixel rightdown(GetPixel(x + 1, y + 1).r + r_dif * (1.0 / 16), GetPixel(x + 1, y + 1).g + g_dif * (1.0 / 16), GetPixel(x + 1, y + 1).b + b_dif * (1.0 / 16), 255);
			SetPixel(x + 1, y, right);
			SetPixel(x - 1, y + 1, left);
			SetPixel(x, y+1, down);
			SetPixel(x + 1, y+1, rightdown);
		}
	}
}
void Image::Blur(int n){
   // float r, g, b; //I got better results converting everything to floats, then converting back to bytes
	// Image* img_copy = new Image(*this); //This is will copying the image, so you can read the orginal values for filtering (
                                          //  ... don't forget to delete the copy!
	/* WORK HERE */
	float r, g, b;
	Image* img_copy = new Image(*this);
	int result[n][n]; // this is the flitering matrix
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			result[i][j] = 0;
		}
	}
	result[0][0] = 1;
	for (int i = 0; i < n; i++)
	{
		result[i][0] = 1;
		result[i][i] = 1;
		if (i != 0)
		{
			for (int j = 1; j < i; j++)
			{
				result[i][j] = result[i - 1][j - 1] + result[i - 1][j];
			}
		}
		result[i][0] = 1;
		result[i][i] = 1;
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			result[i][j] = result[n - 1][j];

		}
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			result[i][j] *= result[n - 1][i];
		}
	}
	double maxnorm = pow(4, n - 1);

	// result is the flitering matrix generated based on size
	for (int i = 0; i < Width(); i++) {
		
		for (int j = 0; j < Height(); j++) {
			float r=0, g=0, b=0;

			for (int a = -1*floor(n/2.0); a < (-1*floor(n / 2.0)+n); a++)
			{
				for (int c = -1*floor(n / 2.0); c < (-1*floor(n / 2.0)+n); c++)
				{
					r += GetPixel(i +a, j+c).r / maxnorm * result[(int)(a + floor(n / 2.0))][(int)(c + floor(n / 2.0))];
					g += GetPixel(i +a, j+c).g / maxnorm * result[(int)(a + floor(n / 2.0))][(int)(c + floor(n / 2.0))];
					b += GetPixel(i +a, j+c).b / maxnorm * result[(int)(a + floor(n / 2.0))][(int)(c + floor(n / 2.0))];
				}
			}
			img_copy->SetPixel(i, j, Pixel((int)r, (int)g, (int)b, 255));
		}
	}

	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			SetPixel(x, y, img_copy->GetPixel(x,y));
		}
	}
}

void Image::Sharpen(int n){
	float r, g, b;
	Image img_copy = Image(width,height);
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			img_copy.SetPixel(x, y, p);
			// make a copy of current version
		}
	}
	Blur(n);

	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel q = GetPixel(x, y);
			Pixel p = img_copy.GetPixel(x, y);
			SetPixel(x, y, PixelLerp(p, q, 10));
		}
	}
}

void Image::EdgeDetect(){
	Image* img_copy = new Image(*this);
	for (int i = 0; i < Width(); i++) {
		for (int j = 0; j < Height(); j++) {
			int x = i;
			int y = j;
			float r = 0, g = 0, b = 0;
			r -= GetPixel(x - 1, y - 1).r ;
			g -= GetPixel(x - 1, y - 1).g ;
			b -= GetPixel(x - 1, y - 1).b ;

			r -= GetPixel(x + 1, y + 1).r ;
			g -= GetPixel(x + 1, y + 1).g ;
			b -= GetPixel(x + 1, y + 1).b ;

			r -= GetPixel(x - 1, y + 1).r ;
			g -= GetPixel(x - 1, y + 1).g ;
			b -= GetPixel(x - 1, y + 1).b ;

			r -= GetPixel(x + 1, y - 1).r ;
			g -= GetPixel(x + 1, y - 1).g ;
			b -= GetPixel(x + 1, y - 1).b ;

			r -= GetPixel(x - 1, y).r ;
			g -= GetPixel(x - 1, y).g ;
			b -= GetPixel(x - 1, y).b ;

			r -= GetPixel(x + 1, y).r ;
			g -= GetPixel(x + 1, y).g ;
			b -= GetPixel(x + 1, y).b ;

			r -= GetPixel(x, y - 1).r ;
			g -= GetPixel(x, y - 1).g ;
			b -= GetPixel(x, y - 1).b ;

			r -= GetPixel(x, y + 1).r ;
			g -= GetPixel(x, y + 1).g ;
			b -= GetPixel(x, y + 1).b ;

			r += GetPixel(x, y).r  * 8;
			g += GetPixel(x, y).g  * 8;
			b += GetPixel(x, y).b  * 8;
			img_copy->SetPixel(x, y, Pixel(ComponentClamp((int)r), ComponentClamp((int)g), ComponentClamp((int)b), 255));
		}
	}

	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			SetPixel(x, y, img_copy->GetPixel(x, y));
		}
	}
}

Image* Image::Scale(double sx, double sy){
	Image* result = new Image(sx, sy);
	for (double i = 0; i < sx; i++)
	{
		for (double j = 0; j < sy; j++)
		{
			Pixel p = Sample(i / sx , j / sy );
			result->SetPixel((int)i, (int)j, p);
		}
	}

	return result;
}

Image* Image::Rotate(double angle){
	Image* img_copy = new Image(2*width, 2*height);
	for (int i = 0; i < img_copy->width; i++)
	{
		for (int j = 0; j <img_copy->height; j++)
		{
			Pixel p(255, 255, 255, 255);
			img_copy->SetPixel(i, j, p);
		}
	}
	for (int i = width/2; i < width*3/2; i++)
	{
		for (int j = height / 2; j < height * 3 / 2; j++)
		{
			double x = i - width / 2;
			double y = j - height / 2;
			Pixel p = Sample(x/width, y/height);
			img_copy->SetPixel(i, j, p);
		}
	}
	Image* img_copy2 = new Image(2 * width, 2 * height);
	for (int i = 0; i < width * 2; i++)
	{
		for (int j = 0; j < height * 2; j++)
		{
			int x = i-width;
			int y = j-height;
			Pixel p = img_copy->Sample_for_rotate((x * cos(angle * PI / 180) - y * sin(angle * PI / 180)+width) / img_copy->width, (height+x * sin(angle * PI / 180) + y * cos(angle * PI / 180)) / img_copy->height);
			img_copy2->SetPixel(i, j, p);
		}
	}
	delete img_copy;
	return img_copy2;
}
 
void Image::Fun(){
	/* WORK HERE */
}

/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method){
   assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
   sampling_method = method;
}


Pixel Image::Sample (double u, double v){
	if (sampling_method == IMAGE_SAMPLING_POINT) //Nearest Neighbor
	{
		return GetPixel((int)(u * width), (int)(v * height));
	}
	else if(sampling_method== IMAGE_SAMPLING_BILINEAR) //Bilinear//Get 4 nearest pixels // #1
	{
		u *= width;
		v *= height;
		Pixel leftbuttom = GetPixel(floor(u), floor(v));
		Pixel leftup = GetPixel(floor(u) , ceil(v) );
		Pixel rightbuttom = GetPixel(ceil(u) , floor(v) );
		Pixel rightup = GetPixel(ceil(u), ceil(v));
		Pixel upbound = PixelLerp(leftup, rightup, u - floor(u));
		Pixel lowbound = PixelLerp(leftbuttom, rightbuttom, u - floor(u));
		Pixel result = PixelLerp(upbound, lowbound, ceil(v) - v);
		return result;
	}
	else if(sampling_method== IMAGE_SAMPLING_GAUSSIAN) //Gaussian//Get all nearby pixels//
	{
		u *= width;
		v *= height;
		int r=0, g=0, b=0;
		for (int i = u-3; i < u+3; i++)
		{
			for (int j = v-3; j < v+3; j++)
			{
				if (i < 0 || j < 0 || i >= width || j >= height)
				{
					continue;
				}
				Pixel temp = GetPixel(i, j);
				double distance = -(pow(u - i, 2) + pow(v - j, 2))/2; // the exponential term
				double weight = 1 / (2 * PI) * exp(distance);
				r += (int)(weight * temp.r); // give a weight
				g += weight * temp.g;
				b += weight * temp.b;
				
			}
		}
		//cout << r << " " << g << " " << b << endl;
		return Pixel(r, g, b, 255);

	}
   return Pixel();
}

Pixel Image::Sample_for_rotate(double u, double v) {
	if (sampling_method == IMAGE_SAMPLING_POINT) //Nearest Neighbor
	{
		return GetPixel_rotate((int)(u * width), (int)(v * height));
	}
	else if (sampling_method == IMAGE_SAMPLING_BILINEAR) //Bilinear//Get 4 nearest pixels // #1
	{
		u *= width;
		v *= height;
		Pixel leftbuttom = GetPixel_rotate(floor(u), floor(v));
		Pixel leftup = GetPixel_rotate(floor(u), ceil(v));
		Pixel rightbuttom = GetPixel_rotate(ceil(u), floor(v));
		Pixel rightup = GetPixel_rotate(ceil(u), ceil(v));
		Pixel upbound = PixelLerp(leftup, rightup, u - floor(u));
		Pixel lowbound = PixelLerp(leftbuttom, rightbuttom, u - floor(u));
		Pixel result = PixelLerp(upbound, lowbound, ceil(v) - v);
		return result;
	}
	else if (sampling_method == IMAGE_SAMPLING_GAUSSIAN) //Gaussian//Get all nearby pixels//
	{
		u *= width;
		v *= height;
		int r = 0, g = 0, b = 0;
		for (int i = u - 3; i < u + 3; i++)
		{
			for (int j = v - 3; j < v + 3; j++)
			{
				if (i < 0 || j < 0 || i >= width || j >= height)
				{
					continue;
				}
				Pixel temp = GetPixel_rotate(i, j);
				double distance = -(pow(u - i, 2) + pow(v - j, 2)) / 2; // the exponential term
				double weight = 1 / (2 * PI) * exp(distance);
				r += (int)(weight * temp.r); // give a weight
				g += weight * temp.g;
				b += weight * temp.b;

			}
		}
		//cout << r << " " << g << " " << b << endl;
		return Pixel(r, g, b, 255);

	}
	return Pixel();
}