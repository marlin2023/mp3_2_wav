#ifndef _OUTPUT_CTX_H
#define _OUTPUT_CTX_H

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
#include "libavutil/fifo.h"
#include "input_handle.h"

//output file information
typedef struct {
	//output file
	AVFormatContext *ptr_format_ctx;
	AVOutputFormat *fmt;


	/*	audio information */
	AVStream *audio_stream;
	enum CodecID audio_codec_id;

	//audio encodec buffer
	uint8_t *audio_outbuf;
	int audio_outbuf_size;
	int16_t *samples;

//	/*	video information */
//	AVStream *video_stream;
//	enum CodecID video_codec_id;
//
//	//video encoded buffer [out]
//	uint8_t *video_outbuf;
//	int 	video_outbuf_size;
//
//	//swscale
//	struct SwsContext *img_convert_ctx;
//	AVFrame *encoded_yuv_pict;
//	uint8_t * pict_buf;

	//audio resample
	struct SwrContext *swr;
	AVFifoBuffer *fifo;     /* for compression: one audio fifo per codec */
	uint8_t *audio_buf;
	unsigned int allocated_audio_buf_size;
	int audio_resample;

	//the input stream
	double sync_ipts;

}OUTPUT_CONTEXT;

/*
 * function : init_input
 * @param:	ptr_output_ctx 	 	a structure contain the output file information
 * @param:	output_file			the output file name
 *
 * */
int init_output(OUTPUT_CONTEXT *ptr_output_ctx, char* output_file ,INPUT_CONTEXT *ptr_input_ctx);

/*
 * function : open_stream_codec
 * @param:	ptr_output_ctx 	 	a structure contain the output file information
 *
 * */
void open_stream_codec(OUTPUT_CONTEXT *ptr_output_ctx);

/*
 * function : encode_video_frame
 * @param:	ptr_output_ctx 	 	a structure contain the output file information
 * @param:	pict				the input picture to encode
 *
 * */
void encode_video_frame(OUTPUT_CONTEXT *ptr_output_ctx ,AVFrame *pict ,INPUT_CONTEXT *ptr_input_ctx);

/*
 * function : encode_audio_frame
 * @param:	ptr_output_ctx 	 	a structure contain the output file information
 * @param:	buf					buf contain the decode audio data ,and then put into audio encoder
 *
 * */
void encode_audio_frame(OUTPUT_CONTEXT *ptr_output_ctx , uint8_t *buf ,int buf_size);


/*
 * function : encode_flush
 * @param:	ptr_output_ctx 	 	a structure contain the output file information
 * @param:  nb_ostreams			the number in the output file
 *
 * */
void encode_flush(OUTPUT_CONTEXT *ptr_output_ctx , int nb_ostreams);


/*
 * function : maybe resample the audio argument ,and then encode the audio data
 * */
void do_audio_out(OUTPUT_CONTEXT *ptr_output_ctx ,INPUT_CONTEXT *ptr_input_ctx ,AVFrame *decoded_frame);

#endif
