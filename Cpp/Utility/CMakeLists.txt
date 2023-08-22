set(target utility)

set(sourcefiles
   "Source/exception_handling.cpp"
   "Source/stop_watch.cpp"
)

add_library(${target} STATIC ${sourcefiles})

#include_directories(${PROJECT_SOURCE_DIR}/Utility/Include/pmp-library/src/pmp)
#include_directories(eigen-lean)
#include_directories(pmp-library/external/eigen)

#link_directories(eigen-lean/include/Eigen)

target_include_directories(${target}
   PUBLIC
      Include
      pmp-library/src
   PRIVATE
      Source
)

add_subdirectory(Test)

add_subdirectory(pmp-library)

