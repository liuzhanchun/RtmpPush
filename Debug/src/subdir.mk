################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/RtmpPush.cpp \
../src/srs_app_ts_parser.cpp \
../src/srs_core.cpp \
../src/srs_core_autofree.cpp \
../src/srs_core_mem_watch.cpp \
../src/srs_core_performance.cpp \
../src/srs_http_stack.cpp \
../src/srs_kernel_aac.cpp \
../src/srs_kernel_buffer.cpp \
../src/srs_kernel_codec.cpp \
../src/srs_kernel_consts.cpp \
../src/srs_kernel_error.cpp \
../src/srs_kernel_file.cpp \
../src/srs_kernel_flv.cpp \
../src/srs_kernel_log.cpp \
../src/srs_kernel_mp3.cpp \
../src/srs_kernel_stream.cpp \
../src/srs_kernel_ts.cpp \
../src/srs_kernel_utility.cpp \
../src/srs_lib_bandwidth.cpp \
../src/srs_lib_simple_socket.cpp \
../src/srs_lib_ts_demuxer.cpp \
../src/srs_librtmp.cpp \
../src/srs_protocol_buffer.cpp \
../src/srs_protocol_json.cpp \
../src/srs_protocol_kbps.cpp \
../src/srs_raw_avc.cpp \
../src/srs_rtmp_amf0.cpp \
../src/srs_rtmp_handshake.cpp \
../src/srs_rtmp_io.cpp \
../src/srs_rtmp_msg_array.cpp \
../src/srs_rtmp_stack.cpp \
../src/srs_rtmp_utility.cpp \
../src/srs_rtsp_stack.cpp 

OBJS += \
./src/RtmpPush.o \
./src/srs_app_ts_parser.o \
./src/srs_core.o \
./src/srs_core_autofree.o \
./src/srs_core_mem_watch.o \
./src/srs_core_performance.o \
./src/srs_http_stack.o \
./src/srs_kernel_aac.o \
./src/srs_kernel_buffer.o \
./src/srs_kernel_codec.o \
./src/srs_kernel_consts.o \
./src/srs_kernel_error.o \
./src/srs_kernel_file.o \
./src/srs_kernel_flv.o \
./src/srs_kernel_log.o \
./src/srs_kernel_mp3.o \
./src/srs_kernel_stream.o \
./src/srs_kernel_ts.o \
./src/srs_kernel_utility.o \
./src/srs_lib_bandwidth.o \
./src/srs_lib_simple_socket.o \
./src/srs_lib_ts_demuxer.o \
./src/srs_librtmp.o \
./src/srs_protocol_buffer.o \
./src/srs_protocol_json.o \
./src/srs_protocol_kbps.o \
./src/srs_raw_avc.o \
./src/srs_rtmp_amf0.o \
./src/srs_rtmp_handshake.o \
./src/srs_rtmp_io.o \
./src/srs_rtmp_msg_array.o \
./src/srs_rtmp_stack.o \
./src/srs_rtmp_utility.o \
./src/srs_rtsp_stack.o 

CPP_DEPS += \
./src/RtmpPush.d \
./src/srs_app_ts_parser.d \
./src/srs_core.d \
./src/srs_core_autofree.d \
./src/srs_core_mem_watch.d \
./src/srs_core_performance.d \
./src/srs_http_stack.d \
./src/srs_kernel_aac.d \
./src/srs_kernel_buffer.d \
./src/srs_kernel_codec.d \
./src/srs_kernel_consts.d \
./src/srs_kernel_error.d \
./src/srs_kernel_file.d \
./src/srs_kernel_flv.d \
./src/srs_kernel_log.d \
./src/srs_kernel_mp3.d \
./src/srs_kernel_stream.d \
./src/srs_kernel_ts.d \
./src/srs_kernel_utility.d \
./src/srs_lib_bandwidth.d \
./src/srs_lib_simple_socket.d \
./src/srs_lib_ts_demuxer.d \
./src/srs_librtmp.d \
./src/srs_protocol_buffer.d \
./src/srs_protocol_json.d \
./src/srs_protocol_kbps.d \
./src/srs_raw_avc.d \
./src/srs_rtmp_amf0.d \
./src/srs_rtmp_handshake.d \
./src/srs_rtmp_io.d \
./src/srs_rtmp_msg_array.d \
./src/srs_rtmp_stack.d \
./src/srs_rtmp_utility.d \
./src/srs_rtsp_stack.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -static -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


