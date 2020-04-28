/*
 * applcation to compute two time correlation function
 */

#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>

#include <Kokkos_Core.hpp>

#include <KokkosBlas1_dot.hpp>
#include <KokkosBlas2_gemv.hpp>
#include <KokkosBlas1_team_dot.hpp>

#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

int main( int argc, char* argv[] )
{
  // TODO read this from arguments
  int Ntime = 1000;         // total size 2^22
  int N = 64;         // number of rows 2^12
  int M = 64;         // number of columns 2^10

  Kokkos::initialize( argc, argv );
  {
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
  }
  
  Kokkos::finalize();

  return 0;
}
