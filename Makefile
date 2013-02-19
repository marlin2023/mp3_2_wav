#lib_path=/home/chris/work/ffmpeg/refs/ffmpeg_104_transcode/lib
#include_path=/home/chris/work/ffmpeg/refs/ffmpeg_104_transcode/include
lib_path=/home/chris/work/ffmpeg/refs/mp3-wav/lib
include_path=/home/chris/work/ffmpeg/refs/mp3-wav/include

target=transcode
src_file = test_main.c	\
			input_handle.c	\
			output_handle.c

all:
	gcc -g ${src_file} -o ${target} -I${include_path} -L${lib_path} -lavformat -lavcodec  -lavutil -lswresample -lm -lz -lpthread 