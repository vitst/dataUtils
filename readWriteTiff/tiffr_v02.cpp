#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <inttypes.h>
//#include "tiffio.h"
//#include <tiff.h>
#include <tiffio.h>
#include <string>
#include <cstring>

#include <sstream>      // std::stringstream

using namespace std;


//void printArray(uint32 *array, uint16 width);
void printArray(float *array, uint16 width);

int main(int argc, char** argv)
{
    string imageName("file-1.tif");

    if(argc > 1)
    {
        imageName = argv[1];
    }

    TIFF* tif = TIFFOpen(imageName.c_str(), "r");
    if (tif)
    {

        do
        {
            uint32 imagelength, imagewidth, imagedepth;
            tdata_t buf;
            uint32 row;
            uint32 config;
            uint16 spp; // sampleperpixel
            uint16 type;

            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imagelength);
  		    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imagewidth);
      		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
  	    	TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &type);

	    	buf = _TIFFmalloc(TIFFScanlineSize(tif));

            uint16 nsamples; // rgb -> 3, b/w -> 1
            //uint32 *data;
            float *data;
            TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &nsamples);

            printf("L=%d  W=%d  Ns=%d SPP=%d DT=%d\n", imagelength, imagewidth, nsamples, spp, type);

            for (row = 0; row < imagelength; row++)
            {
                TIFFReadScanline(tif, buf, row);
                //data=(uint32*)buf;
                data=(float*)buf;
                printArray(data,imagewidth);
            }

            _TIFFfree(buf);
        } while (TIFFReadDirectory(tif));

        TIFFClose(tif);
    }

    printf("Writing file\n");
    string imageNameW("file-w.tif");
    TIFF *outTiff = TIFFOpen("file-w.tif", "w");
    uint32 width = 32;
    uint32 height = 16;
    int sampleperpixel = 1; // for B/W is 1
    uint8 bitspersample  = 32;

    int nstacks = 3;

    for (int i = 0; i < nstacks; i++)
    {
        TIFFSetField(outTiff, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(outTiff, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(outTiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

        TIFFSetField (outTiff, TIFFTAG_ROWSPERSTRIP, 1);

        TIFFSetField(outTiff, TIFFTAG_SAMPLESPERPIXEL, sampleperpixel);
        TIFFSetField(outTiff, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(outTiff, TIFFTAG_BITSPERSAMPLE, bitspersample);

        if(bitspersample>16)
            TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP); // for 32!!!!!!

		TIFFSetField(outTiff, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(outTiff, width*sampleperpixel));

        TIFFSetField(outTiff, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
        TIFFSetField(outTiff, TIFFTAG_PAGENUMBER, i, nstacks);

        // not sure if this is needed...
        TIFFSetField(outTiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(outTiff, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
        TIFFSetField(outTiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

        float data[width*height];
        for(int i1=0; i1<width*height; i1++)
        {
            data[i1] = 10.5*i;
        }
        data[0] = 50.0;
        data[1] = 50.0;

        char* dat = (char*)data;

        for (int row = 0; row < height; row++) {
            int ret = TIFFWriteScanline(outTiff, dat + row * width, row, 0);
        }
        TIFFWriteDirectory(outTiff);
    }
    TIFFClose(outTiff);

    printf("Done\n");
    exit(0);
}



//void printArray(uint32 *array, uint16 width)
void printArray(float *array, uint16 width)
{
    uint32 i;
    for (i=0;i<width;i++)
    {
        //printf("%u ", array[i]);
        printf("%f ", array[i]);
    }
    printf("\n");
}
