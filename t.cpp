#include <iostream>
#include <cstdio>
using namespace std;
double maxseqProductForPositive(double arr[], size_t sz)
{
    double max_product = 0;
    double product = 1;
    for (size_t i = 0; i < sz; ++i)
    {
        product *= arr[i];
        if (max_product < product)
            max_product = product;
        if (product < 1)
            product = 1;


    }
    return max_product;
}
size_t remakeArr(double *arr, double * ret_arr, size_t sz)
{

    size_t j = -1;
    bool tag = 0;
    for (size_t i = 0; i < sz; ++i)
    {

        if (tag == 1)
            ret_arr[j] *= arr[i];
        else
        {
            j++;
            ret_arr[j] = arr[i];
        }
        if (arr[i] < 0)
            tag = !tag;
    }
    if (ret_arr[j] < 0)
        j--;


    return j+1;
}
double maxseqProduct(double arr[], size_t sz)
{

    double *arr0 = new double [sz];
    size_t sz0 = remakeArr(arr, arr0, sz);
    cout<<sz0<<endl;
    for (size_t i = 0; i < sz0; ++i)
        cout<<arr0[i]<<endl;
    cout<<endl;
    size_t i = 0;
    for (; i < sz && arr[i] >= 0; ++i);
    if (i < sz)
        i++;
    double *arr1 = new double [sz];
    size_t sz1 = remakeArr(arr+i, arr1, sz-i);
    cout<<sz1<<endl;
    for (size_t i = 0; i < sz1; ++i)
        cout<<arr1[i]<<endl;
    cout<<endl;
    double result0 = maxseqProductForPositive(arr0, sz0);
    double result1 = maxseqProductForPositive(arr1, sz1);
    // cout<<result0<<endl;
    // cout<<result1<<endl;
    return result0>result1? result0 : result1;
}

int main()
{
    double arr[] = {5, 3, -4, 100, 2, -0.00001, -1, 6, 2, -2, 3,-3,4,-4};
    //double arr[] = {5,3,0,2,8};
    cout<<endl<<maxseqProduct(arr, sizeof(arr)/sizeof(double));
    getchar();
}
