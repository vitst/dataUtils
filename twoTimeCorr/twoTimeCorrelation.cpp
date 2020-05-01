/*
 * applcation to compute two time correlation function
 */

#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <sys/time.h>

#include <tiffio.h>

#include <Kokkos_Core.hpp>

#include <KokkosBlas1_dot.hpp>
#include <KokkosBlas2_gemv.hpp>
#include <KokkosBlas1_team_dot.hpp>

int main( int argc, char* argv[] )
{
  
  Kokkos::ScopeGuard guard( argc, argv );
  
  // TODO read this from arguments
  int Ntime = 1000;         // total size 2^22
  int N = 64;         // number of rows 2^12
  int M = 64;         // number of columns 2^10

  // TODO hardcoding filenames
  char chName[16]; // string which will contain the number
  std::vector<std::string> fns;
  //std::string pre = "elpsd";
  //std::string pre = "";
  for(int i=0; i<Ntime-990; ++i){
    sprintf( chName, "elpsd%05d00.tif", i+1);
    std::stringstream ss;
    ss << chName;
    printf("Filename : %s\n", chName);
    fns.push_back( ss.str() );
  }

  auto closeTIFF = [](TIFF *tif){ TIFFClose(tif); };
  //std::unique_ptr<TIFF, decltype(closeTIFF)> tif (TIFFOpen(std::string("../data/"+fns.front()).c_str(), "r"), closeTIFF);
  std::unique_ptr<TIFF, decltype(closeTIFF)> tif (TIFFOpen("../synt.tif", "r"), closeTIFF);
  uint32 w; uint32 h;
  //uint32 imagelength;
  TIFFGetField(tif.get(), TIFFTAG_IMAGEWIDTH, &w);
  TIFFGetField(tif.get(), TIFFTAG_IMAGELENGTH, &h);
  //TIFFGetField(tif.get(), TIFFTAG_IMAGELENGTH, &imagelength);
  size_t npixels = w*h;
  size_t linesize = TIFFScanlineSize(tif.get());
  
  // Allocate view for the tiff data on device
  Kokkos::View<float**> Atif( "tif_array", w, h );
  // Create host mirror of device view
  //Kokkos::View<float**>::HostMirror h_Atif = Kokkos::create_mirror_view( Atif );

  auto buffer = static_cast<uint32*>(_TIFFmalloc(npixels*sizeof(uint32)));
  
  for (uint32 row = 0; row < h; ++row){
    //auto auxLine = Kokkos::subview(h_Atif, Kokkos::ALL, row);
    //if (!TIFFReadScanline(tif.get(), buffer[ row * w ], row))
    if (!TIFFReadScanline(tif.get(), buffer+row*w, row))
      throw std::runtime_error("oh nooooo");
    //auxLine = reinterpret_cast<float*>(buffer);
    //auxLine = buffer;
  }

  std::cout << buffer << "  npix: " << npixels 
            << "  w: " << w << "  h: " << h << std::endl;

  //Kokkos::deep_copy(Atif, h_Atif);
  Kokkos::deep_copy(Atif, Kokkos::View<float**, Kokkos::LayoutLeft, Kokkos::HostSpace, 
                                    Kokkos::MemoryUnmanaged>(reinterpret_cast<float*>(buffer), w, h));
  // apparently order matters
  // Kokkos::deep_copy(A, Kokkos::View<float**, Kokkos::HostSpace, Kokkos::LayoutRight, Kokkos::MemoryUnmanaged>(reinterpret_cast<float*>(buffer), w, h));

  //auto A_h = Kokkos::create_mirror_view(Kokkos::HostSpace{}, A);
  for (int i = 0; i < h; ++i){
    for (int j = 0; j < w; ++j)
      std::cout << Atif(i, j) << "  ";
    std::cout << '\n';
  }

  //printf( "  Computed result for %d x %d is %lf\n", N, M, result );

  {
    
    
    /*
    // typedef Kokkos::DefaultExecutionSpace::array_layout  Layout;
    // typedef Kokkos::LayoutLeft   Layout;
    typedef Kokkos::LayoutRight  Layout;

    // define types
    typedef Kokkos::View<double*, Layout>    View1DType;
    typedef Kokkos::View<double**, Layout>   View2DType;
    typedef Kokkos::View<double***, Layout>  View3DType;

    View3DType I_all( "I_all", Ntimes, N, M);
    View3DType Data( "images", Ntimes, N, M);
    View1DType I_t( "avI_t", Ntimes);
    View2DType I_tt( "avI_tt", Ntimes, Ntimes);

    // Create host mirrors of device views.
    ViewTimeMatType::HostMirror h_Data = Kokkos::create_mirror_view( Data );

    // Initialize Data on host (Read data from files)
    for ( int k = 0; k < Ntime ; ++k ) {
      for ( int j = 0; j < N; ++j ) {
        for ( int i = 0; i < M; ++i ) {
          h_Data( k, j, i ) = 1.0;
          // TODO technically read data from tiff files
        }
      }
    }

    // Deep copy host views to device views.
    Kokkos::deep_copy( Data, h_Data );

    //----------------------------------------------------------//

    double result = 0;

    for ( int k = 0; k < Ntime ; ++k ) {
      // here the FFT should be calculated
      View2DType slice( "slice", N, M);
      auto Data_k = Kokkos::subview(Data, k, Kokkos::ALL, Kokkos::ALL);
      auto I_all_k = Kokkos::subview(I_all, k, Kokkos::ALL, Kokkos::ALL);
      Kokkos::deep_copy(slice, Data_k);
      auto tmp = slice; // square( abs( FFT(slice) ) );
      I_all_k = tmp;
    }


    //KokkosBlas::gemv("N",alpha,A,x,beta,tmp);
    //result = KokkosBlas::dot(y, tmp);
    */
  }
  
  //Kokkos::finalize();

  return 0;
}
