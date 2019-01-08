#include <cstring> 
#include "./im2col.h"

namespace Armednn
{

bool IsAGeZeroAndALtB(const int a, const int b) 
{
    return static_cast<unsigned int>(a) < static_cast<unsigned>(b);
}

template <typename TData>
 void Im2Col3dNHWCImpl(
        const int C,
        const int T,
        const int H,
        const int W,
        const int kernel_t,
        const int kernel_h,
        const int kernel_w,
        const int dilation_t,
        const int dilation_h,
        const int dilation_w,
        const int pad_p, // previous frame
        const int pad_t, // top
        const int pad_l, // left
        const int pad_n, // next frame
        const int pad_b, // bottom
        const int pad_r, // right
        const int stride_t,
        const int stride_h,
        const int stride_w,
        const TData* img_data,
        TData* col_data,
        const int groups) {

    const int dkernel_t = dilation_t * (kernel_t - 1) + 1;
    const int dkernel_h = dilation_h * (kernel_h - 1) + 1;
    const int dkernel_w = dilation_w * (kernel_w - 1) + 1;
    const int output_t = (T + pad_p + pad_n - dkernel_t) / stride_t + 1;
    const int output_h = (H + pad_t + pad_b - dkernel_h) / stride_h + 1;
    const int output_w = (W + pad_l + pad_r - dkernel_w) / stride_w + 1;
    //INFO<<"0_w: "<<output_w;
    const int C_per_G = C / groups;
    int t_pad = -pad_p;
    for (int t = 0; t < output_t; ++t) {
        int h_pad = -pad_t;
        for (int h = 0; h < output_h; ++h) {
            int w_pad = -pad_l;
            for (int w = 0; w < output_w; ++w) {
                int q = 0;
                for (int it = t_pad; it < t_pad + dkernel_t; it += dilation_t, ++q) {
                    int r = 0;
                    for (int ih = h_pad; ih < h_pad + dkernel_h; ih += dilation_h, ++r) {
                        int s = 0;
                        for (int iw = w_pad; iw < w_pad + dkernel_w;
                                iw += dilation_w, ++s) {
                            if (IsAGeZeroAndALtB(it, T) &&
                                    IsAGeZeroAndALtB(ih, H) &&
                                    IsAGeZeroAndALtB(iw, W)) {
                                for (int g = 0; g < groups; ++g) {
                                    std::memcpy(
                                            col_data +
                                            (((g * kernel_t + q) * kernel_h + r) * kernel_w + s) *
                                            C_per_G,
                                            img_data + ((it * H + ih) * W + iw) * C + g * C_per_G,
                                            sizeof(TData) * C_per_G);

                                }
                            } else {
                                for (int g = 0; g < groups; ++g) {
                                    std::memset(
                                            col_data +
                                            (((g * kernel_t + q) * kernel_h + r) * kernel_w + s) *
                                            C_per_G,
                                            0,
                                            sizeof(TData) * C_per_G);
                                }
                            }
                        } // iw
                    } // ih
                } // it
                col_data += kernel_t * kernel_h * kernel_w * C;
                //INFO<<"o_c: "<<kernel_t * kernel_h * kernel_w * C;
                w_pad += stride_w;
            } // w
            h_pad += stride_h;
        } // h
        t_pad += stride_t;
    } // t
}
template <>
 void Im2Col3dNHWC<float>(
        const int C,
        const int T,
        const int H,
        const int W,
        const int kernel_t,
        const int kernel_h,
        const int kernel_w,
        const int dilation_t,
        const int dilation_h,
        const int dilation_w,
        const int pad_p, // previous frame
        const int pad_t, // top
        const int pad_l, // left
        const int pad_n, // next frame
        const int pad_b, // bottom
        const int pad_r, // right
        const int stride_t,
        const int stride_h,
        const int stride_w,
        const float* img_data,
        float* col_data,
        const int groups)
{

 Im2Col3dNHWCImpl<float>(
         C,
         T,
         H,
         W,
         kernel_t,
         kernel_h,
         kernel_w,
         dilation_t,
         dilation_h,
         dilation_w,
         pad_p, // previous frame
         pad_t, // top
         pad_l, // left
         pad_n, // next frame
         pad_b, // bottom
         pad_r, // right
         stride_t,
         stride_h,
         stride_w,
         img_data,
         col_data,
         groups);
}

}//namespace Armednn
