cmake_minimum_required (VERSION 3.10)
include(common.cmake)

project (KokkosKernelsTwoTimeCorrelation CXX) # telling cmake it is a cpp proj

find_package(TIFF REQUIRED )

add_executable(twoTimeCorr twoTimeCorrelation.cpp)
target_link_libraries(twoTimeCorr Kokkos::kokkoskernels TIFF::TIFF)
