#include<iostream>
#include<fstream>
#include<tiffio.h>
#include<inttypes.h>
#include<stdlib.h>
using namespace std;

int main(int argc, char **argv){
   TIFF* tiff;
   tiff = TIFFOpen(argv[1], "r");
   if(tiff == NULL){
      return -1;
   }
   int width, height;
   int depth;
   TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
   TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);
   TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &depth); //float 32 i think

   int npix = width * height;
   
   uint32_t *raster = (uint32*) _TIFFmalloc(npix * sizeof (uint32));
   TIFFReadRGBAImageOriented(tiff, width, height, raster, ORIENTATION_TOPLEFT, 0);

   //cout << width << "X" << height << "=" << npix << " || " << depth << endl;
   int t;
   for(int i = 0 ; i < npix; i++){
      t = raster[i] * 2;
   }

   TIFF* tif = TIFFOpen(argv[1], "r");
   if (tif) {
      int dircount = 0;
      do {
         dircount++;
      } while (TIFFReadDirectory(tif));
      //printf("%d directories in %s\n", dircount, argv[1]);
      TIFFClose(tif);
   }

   tif = TIFFOpen(argv[1], "r");
   ofstream file;
   file.open("output.bin", std::ios::binary);
   if (tif) {
      uint32 imagelength;
      tdata_t buf;
      uint32 row;

      TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imagelength);
      buf = malloc(sizeof(uint32_t) * width * height); //_TIFFmalloc(TIFFScanlineSize(tif));
      uint32_t ww,hh;
      ww = (uint32_t)width;
      hh = (uint32_t)height;
      file.write((char*)&ww, sizeof(ww));
      file.write((char*)&hh, sizeof(hh));


      uint32_t * dataarr = (uint32_t *)malloc(sizeof(uint32_t) * width * height);
      cerr << ww << endl;
      int cindex = 0;
      for (row = 0; row < height; row++){
         TIFFReadScanline(tif, buf, row);
         for(int i = 0; i < width; i++){
            uint32_t data = ((uint32_t *) buf)[i];
            //if(i < width - 1 ) cout << ", ";
            //file.write((char*)&data, sizeof(data));
            dataarr[cindex++] = data;
         }
      }
      cout << cindex << endl;
      cout << ww * hh << endl;
      for(int i = 0; i < 10; i++){
         cout << dataarr[i * 100000] << endl;
      }
      file.write(reinterpret_cast<char *>(dataarr), sizeof(uint32_t) * width * height);
      _TIFFfree(buf);
      TIFFClose(tif);
   }
}
