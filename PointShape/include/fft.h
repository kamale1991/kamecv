#include <iostream>
#include "mkl_dfti.h"

namespace CA
{

    void fft_one_dimensional_in_place()
    {
        float _Complex x[32];
        float y[34];
        DFTI_DESCRIPTOR_HANDLE my_desc1_handle;
        DFTI_DESCRIPTOR_HANDLE my_desc2_handle;
        MKL_LONG status;
        // インプットデータをx[0-31], y[0-31]に入れる
        status = DftiCreateDescriptor(
            &my_desc1_handle, DFTI_SINGLE,
            DFTI_COMPLEX, 1, 32);
        status = DftiCommitDescriptor(my_desc1_handle);
        status = DftiComputeForward(my_desc1_handle, x);
        status = DftiFreeDescriptor(&my_desc1_handle);
        // 結果がx[0-31]に格納されている
        status = DftiCreateDescriptor(&my_desc2_handle, DFTI_SINGLE, DFTI_REAL, 1, 32);
        status = DftiCommitDescriptor(my_desc2_handle);
        status = DftiComputeForward(my_desc2_handle, y);
        status = DftiFreeDescriptor(&my_desc2_handle);
        // 結果はCCSフォーマット
    };
};