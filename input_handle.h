#ifndef _INPUT_CTX_H
#define _INPUT_CTX_H

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"

typedef struct {
	//input file
	AVFormatContext *ptr_format_ctx;
	AVPacket pkt;

	/*	audio information */
	int audio_index;
	AVCodecContext *audio_codec_ctx;
	AVCodec 	*audio_codec;

	AVFrame 	*audio_decode_frame;
	int 		audio_size;				//audio size ,the output invoke avcodec_decode_audio4 one time

	/*	video information */
	//

	/*	decode mark	*/
	int 		mark_have_frame;
}INPUT_CONTEXT;



/*
 * function : init_input
 * @param:	ptr_input_ctx 	 	a structure contain the inputfile information
 * @param:	input_file			the input file name
 *
 * */
int init_input(INPUT_CONTEXT *ptr_input_ctx, char* input_file);


/*
 * function : init_input
 * @param:	ptr_input_ctx 	 	a structure contain the inputfile information
 * @param:	pkt					the packet read from the AVFormatContext
 *
 * */
void decode_frame(INPUT_CONTEXT *ptr_input_ctx ,AVPacket *pkt);


#endif
