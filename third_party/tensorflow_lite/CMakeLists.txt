include(ExternalProject)

set(NAME tensorflow_lite)
set(VERSION 2.7.0)
set(PREFIX ${EP_DEPENDENCIES_PATH}/${NAME})
set(GPU_SUPPORT "OFF")

message (STATUS "Adding ${NAME}:${VERSION} as an external project.")

ExternalProject_Add(${NAME}
  URL https://github.com/tensorflow/tensorflow/archive/refs/tags/v${VERSION}.tar.gz
  UPDATE_DISCONNECTED ON
  BUILD_IN_SOURCE ON
  INSTALL_COMMAND
        mkdir -p ${PREFIX} && cp -r . ${PREFIX}
  CMAKE_ARGS
        "../${NAME}/tensorflow/lite"
        "-DCMAKE_INSTALL_PREFIX=${PREFIX}"
        "-DCMAKE_BUILD_TYPE:STRING=Release"
        "-DTFLITE_ENABLE_GPU=${GPU_SUPPORT}"
)

set(TENSORFLOWLITE_INCLUDE_DIR
    ${PREFIX}
    ${PREFIX}/FP16-source/include
    ${PREFIX}/FXdiv-source/include
    ${PREFIX}/clog-source/include
    ${PREFIX}/cpuinfo-source/include
    ${PREFIX}/flatbuffers/include
    ${PREFIX}/psimd-source/include
    ${PREFIX}/pthreadpool-source/include
    ${PREFIX}/xnnpack/include
PARENT_SCOPE)
set(TENSORFLOWLITE_LIB_DIR
        ${PREFIX}
        ${PREFIX}/pthreadpool
        ${PREFIX}/clog
        ${PREFIX}/cpuinfo
        ${PREFIX}/_deps/abseil-cpp-build/absl/strings
        ${PREFIX}/_deps/abseil-cpp-build/absl/types
        ${PREFIX}/_deps/abseil-cpp-build/absl/flags
        ${PREFIX}/_deps/abseil-cpp-build/absl/synchronization
        ${PREFIX}/_deps/abseil-cpp-build/absl/hash
        ${PREFIX}/_deps/abseil-cpp-build/absl/debugging
        ${PREFIX}/_deps/abseil-cpp-build/absl/status
        ${PREFIX}/_deps/abseil-cpp-build/absl/time
        ${PREFIX}/_deps/abseil-cpp-build/absl/container
        ${PREFIX}/_deps/abseil-cpp-build/absl/numeric
        ${PREFIX}/_deps/abseil-cpp-build/absl/base
        ${PREFIX}/_deps/flatbuffers-build
        ${PREFIX}/_deps/ruy-build
        ${PREFIX}/_deps/xnnpack-build
        ${PREFIX}/_deps/farmhash-build
        ${PREFIX}/_deps/fft2d-build
PARENT_SCOPE)
set(TENSORFLOWLITE_LIBRARIES
        tensorflow-lite
        cpuinfo
        clog
        pthreadpool
        # Libraries used for GPU support.
        # absl_cord
        # absl_strings
        # absl_str_format_internal
        # absl_strings_internal
        # absl_bad_any_cast_impl
        # absl_bad_optional_access
        # absl_bad_variant_access
        # absl_flags_commandlineflag
        # absl_flags_private_handle_accessor
        # absl_flags
        # absl_flags_program_name
        # absl_flags_config
        # absl_flags_commandlineflag_internal
        # absl_flags_marshalling
        # absl_flags_reflection
        # absl_flags_internal
        # absl_graphcycles_internal
        # absl_synchronization
        # absl_city
        # absl_hash
        # absl_wyhash
        # absl_debugging_internal
        # absl_symbolize
        # absl_stacktrace
        # absl_demangle_internal
        # absl_status
        # absl_time_zone
        # absl_time
        # absl_civil_time
        # absl_raw_hash_set
        # absl_hashtablez_sampler
        # absl_int128
        # absl_spinlock_wait
        # absl_log_severity
        # absl_raw_logging_internal
        # absl_base
        # absl_throw_delegate
        # absl_malloc_internal
        # absl_exponential_biased
        # #########################
        flatbuffers
        ruy
        XNNPACK
        farmhash
        fft2d_fftsg
        fft2d_fftsg2d
PARENT_SCOPE)