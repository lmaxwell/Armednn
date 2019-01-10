
#ifndef _IM2COL_H
#define _IM2COL_H

namespace Armednn
{

/*
from pytorch 
https://github.com/pytorch/pytorch/blob/master/LICENSE
*/
template <typename TData>
 void Im2Col3dNHWC(
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
        const int groups);

}//namespace Armednn
#endif
