/*
 *  本工程的主要作用 就是完成读取一个mp4 文件，实现将一个文件转码输出成另外一个mp4文件
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "input_handle.h"
#include "output_handle.h"
#include "chris_error.h"

int main(int argc ,char *argv[]){

	/*first ,open input file ,and obtain input file information*/
	INPUT_CONTEXT *ptr_input_ctx;

	if( (ptr_input_ctx = malloc (sizeof(INPUT_CONTEXT))) == NULL){
		printf("ptr_input_ctx malloc failed .\n");
		exit(MEMORY_MALLOC_FAIL);
	}

	/*second ,open output file ,and set output file information*/
	OUTPUT_CONTEXT *ptr_output_ctx;
	if( (ptr_output_ctx = malloc (sizeof(OUTPUT_CONTEXT))) == NULL){
		printf("ptr_output_ctx malloc failed .\n");
		exit(MEMORY_MALLOC_FAIL);
	}

	//init inputfile ,and get input file information
	init_input(ptr_input_ctx ,argv[1]);


	//init oputfile ,and set output file information
	init_output(ptr_output_ctx ,argv[2] ,ptr_input_ctx);

	ptr_output_ctx->fifo = av_fifo_alloc(1024);
	                if (!ptr_output_ctx->fifo) {
	                    exit (1);
	                }
	                av_log(NULL, AV_LOG_WARNING ,"--av_fifo_size(ost->fifo) = %d \n" ,av_fifo_size(ptr_output_ctx->fifo));  //输出是0？！
	//open video and audio ,set video_out_buf and audio_out_buf
	open_stream_codec(ptr_output_ctx);


    // open the output file, if needed
    if (!(ptr_output_ctx->fmt->flags & AVFMT_NOFILE)) {		//for mp4 or mpegts ,this must be performed
        if (avio_open(&ptr_output_ctx->ptr_format_ctx->pb, argv[2], AVIO_FLAG_WRITE) < 0) {
            fprintf(stderr, "Could not open '%s'\n", argv[2]);
            exit(OPEN_MUX_FILE_FAIL);
        }
    }

    // write the stream header, if any
    avformat_write_header(ptr_output_ctx->ptr_format_ctx ,NULL);

    printf("ptr_output_ctx->ptr_format_ctx->nb_streams = %d \n\n" ,ptr_output_ctx->ptr_format_ctx->nb_streams);  //streams number in output file

//    while(1);

	printf("before av_read_frame ...\n");
	/*************************************************************************************/
	/*decoder loop*/
	//
	//
	//***********************************************************************************/
	while(av_read_frame(ptr_input_ctx->ptr_format_ctx ,&ptr_input_ctx->pkt) >= 0){

		 if (ptr_input_ctx->pkt.stream_index == ptr_input_ctx->audio_index) {

//			printf("HAHA.................\n");
			#if 1
			//decode audio packet
			while (ptr_input_ctx->pkt.size > 0) {
				int got_frame = 0;
				int len = avcodec_decode_audio4(ptr_input_ctx->audio_codec_ctx,
						ptr_input_ctx->audio_decode_frame, &got_frame,
						&ptr_input_ctx->pkt);

				if (len < 0) { //decode failed ,skip frame
					fprintf(stderr, "Error while decoding audio frame\n");
					break;
				}

				if (got_frame) {
					//encode the audio data ,and write the data into the output
					do_audio_out(ptr_output_ctx ,ptr_input_ctx ,ptr_input_ctx->audio_decode_frame);

				} else { //no data
					printf("======>avcodec_decode_audio4 ,no data ..\n");
					continue;
				}

				ptr_input_ctx->pkt.size -= len;
				ptr_input_ctx->pkt.data += len;

			}
			#endif
		}

	}//endwhile


	printf("before flush ,ptr_output_ctx->ptr_format_ctx->nb_streams = %d \n\n" ,ptr_output_ctx->ptr_format_ctx->nb_streams);
	encode_flush(ptr_output_ctx ,ptr_output_ctx->ptr_format_ctx->nb_streams);

	printf("before wirite tailer ...\n\n");

	av_write_trailer(ptr_output_ctx->ptr_format_ctx );

	/*free memory*/
}
