include(FindPackageHandleStandardArgs)

find_path(Libseccomp_INCLUDE_DIR seccomp.h)
find_library(Libseccomp_LIBRARY NAMES seccomp)

find_package_handle_standard_args(Libseccomp
	REQUIRED_VARS Libseccomp_LIBRARY Libseccomp_INCLUDE_DIR)

if(Libseccomp_FOUND AND NOT TARGET Libseccomp::Libseccomp)
  add_library(Libseccomp::Libseccomp UNKNOWN IMPORTED)
  set_target_properties(Libseccomp::Libseccomp
    PROPERTIES
      IMPORTED_LOCATION "${Libseccomp_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${Libseccomp_INCLUDE_DIR}")
endif()

