#include "stream_tools.h"
#include "config.h"
#include "param.h"
#include "conv2d.h"
#include "pool2d.h"
using namespace hls;

#define DEBUG

void do_compute(stream<my_ap_axis >& in, stream<my_ap_axis >& out, const unsigned int reps) {
#pragma HLS INLINE

    const unsigned int num_per_rep = 160 * 160 * 3 * 8 / 64;

    hls::stream<ap_uint<64> > in_stream_extract("in_stream_extract");
#pragma HLS STREAM variable=in_stream_extract depth=16 dim=1	//待修改
	ExtractPixels<64, num_per_rep> (in, in_stream_extract, reps);

    hls::stream<ap_uint<64 * 3> > in_stream0("in_stream0");
#pragma HLS STREAM variable=in_stream0 depth=16 dim=1
    StreamingDataWidthConverter_Batch<64, 64 * 3, num_per_rep>(in_stream_extract, in_stream0, reps);

	hls::stream<ap_uint<CONV_0_IN_BIT * CONV_0_IFM_CH> > in_stream1("in_stream1");
#pragma HLS STREAM variable=in_stream1 depth=16 dim=1

	StreamingDataWidthConverter_Batch<64 * 3, CONV_0_IN_BIT * CONV_0_IFM_CH, num_per_rep / 3> (in_stream0, in_stream1, reps);
#ifdef DEBUG
    cout << "in_stream1 size " << in_stream1.size() << endl;

#endif

    hls::stream<ap_uint<CONV_0_OUT_BIT * CONV_0_OFM_CH> >  conv_0_out("conv_0_out");
#pragma HLS STREAM variable=conv_0_out depth=128 dim=1
    conv3x3_bn_act< 
                    CONV_0_IFM_ROW,
                    CONV_0_IFM_COL,
                    CONV_0_IFM_CH,
                    CONV_0_IN_BIT,

                    CONV_0_OFM_CH,
                    CONV_0_OUT_BIT,

                    CONV_0_W_BIT,
                    32,                     
                    CONV_0_INC_BIT,
                    CONV_0_BIAS_BIT,

                    CONV_0_SIMD,
                    CONV_0_PE,
                    CONV_0_L_SHIFT>(
                in_stream1,
                conv_0_w,
                conv_0_inc,
                conv_0_bias,
                conv_0_out,
                reps );


    hls::stream<ap_uint<CONV_0_OUT_BIT*CONV_0_OFM_CH> > pool_0_out("pool_0_out");
#pragma HLS STREAM variable=pool_0_out depth=128 dim=1
    max_pool2d< 2,
                CONV_0_OFM_ROW,
                CONV_0_OFM_COL,
                CONV_0_OFM_CH,
                CONV_0_OUT_BIT>(
                    conv_0_out,
                    pool_0_out,
                    reps);
#ifdef DEBUG
    cout << "pool_0_out size " << pool_0_out.size() << endl;
#endif

    hls::stream<ap_uint<CONV_1_OUT_BIT * CONV_1_OFM_CH> >  conv_1_out("conv_1_out");
#pragma HLS STREAM variable=conv_1_out depth=128 dim=1
    conv3x3_bn_act< 
                    CONV_1_IFM_ROW,
                    CONV_1_IFM_COL,
                    CONV_1_IFM_CH,
                    CONV_1_IN_BIT,

                    CONV_1_OFM_CH,
                    CONV_1_OUT_BIT,

                    CONV_1_W_BIT,
                    32,                     
                    CONV_1_INC_BIT,
                    CONV_1_BIAS_BIT,

                    CONV_1_SIMD,
                    CONV_1_PE,
                    CONV_1_L_SHIFT>(
                pool_0_out,
                conv_1_w,
                conv_1_inc,
                conv_1_bias,
                conv_1_out,
                reps );
#ifdef DEBUG
    cout << "conv_1_out size " << conv_1_out.size() << endl;
#endif
    hls::stream<ap_uint<CONV_1_OUT_BIT*CONV_1_OFM_CH> > pool_1_out("pool_out");
#pragma HLS STREAM variable=pool_1_out depth=128 dim=1
    max_pool2d< 2,
                CONV_1_OFM_ROW,
                CONV_1_OFM_COL,
                CONV_1_OFM_CH,
                CONV_1_OUT_BIT>(
                    conv_1_out,
                    pool_1_out,
                    reps);
#ifdef DEBUG
    cout << "pool_1_out size " << pool_1_out.size() << endl;
#endif

    hls::stream<ap_uint<CONV_2_OUT_BIT * CONV_2_OFM_CH> >  conv_2_out("conv_2_out");
#pragma HLS STREAM variable=conv_2_out depth=128 dim=1
    conv3x3_bn_act< 
                    CONV_2_IFM_ROW,
                    CONV_2_IFM_COL,
                    CONV_2_IFM_CH,
                    CONV_2_IN_BIT,

                    CONV_2_OFM_CH,
                    CONV_2_OUT_BIT,

                    CONV_2_W_BIT,
                    32,                     
                    CONV_2_INC_BIT,
                    CONV_2_BIAS_BIT,

                    CONV_2_SIMD,
                    CONV_2_PE,
                    CONV_2_L_SHIFT>(
                pool_1_out,
                conv_2_w,
                conv_2_inc,
                conv_2_bias,
                conv_2_out,
                reps );
#ifdef DEBUG
    cout << "conv_2_out size " << conv_2_out.size() << endl;
#endif
    hls::stream<ap_uint<CONV_2_OUT_BIT*CONV_2_OFM_CH> > pool_2_out("pool_out");
#pragma HLS STREAM variable=pool_2_out depth=128 dim=1
    max_pool2d< 2,
                CONV_2_OFM_ROW,
                CONV_2_OFM_COL,
                CONV_2_OFM_CH,
                CONV_2_OUT_BIT>(
                    conv_2_out,
                    pool_2_out,
                    reps);
#ifdef DEBUG
    cout << "pool_2_out size " << pool_2_out.size() << endl;
#endif

    hls::stream<ap_uint<CONV_3_OUT_BIT * CONV_3_OFM_CH> >  conv_3_out("conv_3_out");
#pragma HLS STREAM variable=conv_3_out depth=128 dim=1
    conv3x3_bn_act< 
                    CONV_3_IFM_ROW,
                    CONV_3_IFM_COL,
                    CONV_3_IFM_CH,
                    CONV_3_IN_BIT,

                    CONV_3_OFM_CH,
                    CONV_3_OUT_BIT,

                    CONV_3_W_BIT,
                    32,                     
                    CONV_3_INC_BIT,
                    CONV_3_BIAS_BIT,

                    CONV_3_SIMD,
                    CONV_3_PE,
                    CONV_3_L_SHIFT>(
                pool_2_out,
                conv_3_w,
                conv_3_inc,
                conv_3_bias,
                conv_3_out,
                reps );
#ifdef DEBUG
    cout << "conv_3_out size " << conv_3_out.size() << endl;
#endif
    hls::stream<ap_uint<CONV_3_OUT_BIT*CONV_3_OFM_CH> > pool_3_out("pool_3_out");
#pragma HLS STREAM variable=pool_3_out depth=128 dim=1
    max_pool2d< 2,
                CONV_3_OFM_ROW,
                CONV_3_OFM_COL,
                CONV_3_OFM_CH,
                CONV_3_OUT_BIT>(
                    conv_3_out,
                    pool_3_out,
                    reps);
#ifdef DEBUG
    cout << "pool_3_out size " << pool_3_out.size() << endl;
#endif

    hls::stream<ap_uint<CONV_4_OUT_BIT * CONV_4_OFM_CH> >  conv_4_out("conv_4_out");
#pragma HLS STREAM variable=conv_4_out depth=128 dim=1
    conv3x3_bn_act< 
                    CONV_4_IFM_ROW,
                    CONV_4_IFM_COL,
                    CONV_4_IFM_CH,
                    CONV_4_IN_BIT,

                    CONV_4_OFM_CH,
                    CONV_4_OUT_BIT,

                    CONV_4_W_BIT,
                    32,                     
                    CONV_4_INC_BIT,
                    CONV_4_BIAS_BIT,

                    CONV_4_SIMD,
                    CONV_4_PE,
                    CONV_4_L_SHIFT>(
                pool_3_out,
                conv_4_w,
                conv_4_inc,
                conv_4_bias,
                conv_4_out,
                reps );
#ifdef DEBUG
    cout << "conv_4_out size " << conv_4_out.size() << endl;
#endif

    hls::stream<ap_uint<CONV_4_OUT_BIT*CONV_4_OFM_CH> > pool_4_out("pool_4_out");
#pragma HLS STREAM variable=pool_4_out depth=128 dim=1
	max_pool2d< 2,
				CONV_4_OFM_ROW,
				CONV_4_OFM_COL,
				CONV_4_OFM_CH,
				CONV_4_OUT_BIT>(
					conv_4_out,
					pool_4_out,
					reps);
#ifdef DEBUG
	cout << "pool_4_out size " << pool_4_out.size() << endl;
#endif

 

	hls::stream<ap_uint<CONV_5_IN_BIT * CONV_5_SIMD> >  conv_5_in("conv_5_in");
#pragma HLS STREAM variable=conv_5_in depth=64 dim=1

	StreamingDataWidthConverter_Batch<CONV_4_OFM_CH*CONV_4_OUT_BIT,
			CONV_5_IN_BIT * CONV_5_SIMD,
			25>(pool_4_out, conv_5_in, reps);

#ifdef DEBUG
	cout << "pool_4_out size " << pool_4_out.size() << endl;
#endif

	hls::stream<ap_uint<32 * CONV_5_PE> >  conv_5_out("conv_5_out");
#pragma HLS STREAM variable=conv_5_out depth=64 dim=1
	conv1x1 <
			1,
			1,
			400,
			CONV_5_IN_BIT,
			CONV_5_OFM_CH,

			CONV_5_W_BIT,
			32,

			CONV_5_SIMD,
			CONV_5_PE>(
				conv_5_in,
				conv_5_w,
				conv_5_out,
				reps );


#ifdef DEBUG
    cout << "conv_5_out size " << conv_5_out.size() << endl;
#endif

	 AddLast<1*1*CONV_5_OFM_CH/2>(conv_5_out, out, reps);

}

void convnet(stream<my_ap_axis >& in, stream<my_ap_axis >& out, const unsigned int reps) {

#pragma HLS DATAFLOW

#pragma HLS INTERFACE axis register both port=out
#pragma HLS INTERFACE axis register both port=in
#pragma HLS INTERFACE s_axilite port=reps bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS ARRAY_PARTITION variable = conv_0_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_0_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_0_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_1_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_1_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_1_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_2_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_2_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_2_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_3_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_3_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_3_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_4_w complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_4_inc complete dim = 1
#pragma HLS ARRAY_PARTITION variable = conv_4_bias complete dim = 1

#pragma HLS ARRAY_PARTITION variable = conv_5_w complete dim = 1


    do_compute(in, out, reps);

}

#ifdef DEBUG

#include <ap_int.h>
#include <hls_video.h>
#include <hls_stream.h>
#include <iostream>
#include <fstream>
#include "math.h"


void load_data(const char *path, char *ptr, unsigned int size)
{
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (!f)
    {
        std::cout << "no such file,please check the file name!/n";
        exit(0);
    }
    f.read(ptr, size);
    f.close();
}

void write_data(const char *path, char *ptr, unsigned int size)
{
    std::ofstream f(path, std::ios::out | std::ios::binary);
    if (!f)
    {
        std::cout << "write no such file,please check the file name!/n";
        exit(0);
    }
    f.write(ptr, size);
    f.close();
}

void softmax(int *in, int len){

	double sum, vector[len];
	for(int i=0; i<len; i++){
		vector[i] = exp(in[i]);
		sum += vector[i];
	}

	for(int i=0; i<len; i++){
		vector[i] = vector[i]/sum;
		printf("%f ", vector[i]);
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
    uint8_t img[160][160][3];
    load_data("C:\\RC4ML\\Xilinx_Winter_Camp_2021\\HLS_Code\\convnet\\data\\image_1042_rgb.bin", (char *) img, sizeof(img));

//    for(int i=0; i<3; i++){
//    	printf("channel%d:\n", i);
//    	for(int j=0; j<160; j++){
//    		for(int k=0; k<160; k++){
//    			printf(" %4d", img[k][j][i]);
//    		}
//    		printf("\n");
//    	}
//    }

    uint8_t * data = (uint8_t *) img;
    const int data_points_per_line = 8;
    const int nums_line_pre_img = 160 * 160 * 3 / 8;

    hls::stream<my_ap_axis> input_stream("input stream");
	for (unsigned int i = 0; i < nums_line_pre_img; i++) {
		my_ap_axis temp;
		for (unsigned int j = 0; j < data_points_per_line; j++) {
			temp.data( 8*(j+1)-1, 8*j ) = data[i * data_points_per_line + j];
		}
		input_stream.write(temp);
	}
	cout << "input size :" << input_stream.size() << endl;
    cout << "start ..... " << endl;
    hls::stream<my_ap_axis> output_stream("output stream");
    convnet(input_stream, output_stream, 1);

    ap_uint<64> outputBuffer[8];
    ap_uint<32> Buffer[16];

	for (unsigned int i = 0; i < 8; i++) {
		outputBuffer[i] = output_stream.read().data;
		Buffer[2*i]=outputBuffer[i](31,0);
		Buffer[2*i+1]=outputBuffer[i](63,32);
	}


	unsigned int prediction = -1;
	int max = -(1<<30);
	int pre[16];

	for (unsigned int i = 0; i < 16; i++) {
		int temp = Buffer[i].to_int();
		cout << temp << endl;
		if (temp > max){
			max = temp;
			prediction = i;
			pre[i] = temp;
		}
	}

	softmax(pre, 16);

	cout << "prediction: " << prediction << endl;

    
    return 0;
}

#endif
