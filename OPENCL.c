
//#include "SM2.h"

#include "OPENCL.h"
#include<stdlib.h>
#include<string.h>
#include"test_kernel_in_c.h"


#pragma warning( disable : 4996 )
//c语言实现memset
void* memset_c(void* ptr, int value, size_t num) {
    unsigned char* p = ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

//c语言实现memcpy
void* memcpy_c(void* dest, const void* src, size_t n) {
    char* dp = dest;
    const char* sp = src;
    while (n--) {
        *dp++ = *sp++;
    }
    return dest;
}

char* read_txt_file(char* filename) {
    //打开文件
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("无法打开文件%s\n", filename);
        return NULL;
    }
    //获取文件大小
    fseek(fp, 0, SEEK_END);
    long long size = ftell(fp);
    rewind(fp);
    //分配内存空间
    char* buffer = (char*)malloc(size + 1); //为字符串末尾添加 '\0'
    if (buffer == NULL) {
        printf("无法分配内存空间\n");
        fclose(fp);
        return NULL;
    }
    //读取文件内容
    size_t result = fread(buffer, 1, size, fp);
    if (result != size) {
        printf("读取文件错误\n");
        free(buffer);
        fclose(fp);
        return NULL;
    }
    //关闭文件
    fclose(fp);
    //添加字符串结束符
    buffer[size] = '\0';
    return buffer;
}


//char* read_txt_file(char* filename) {
//    //打开文件
//    FILE* fp = fopen(filename, "r");
//    if (fp == NULL) {
//        printf("无法打开文件%s\n", filename);
//        return NULL;
//    }
//    //获取文件大小
//    fseek(fp, 0, SEEK_END);
//    long long size = ftell(fp)-2633 ;
//    rewind(fp);
//    //分配内存空间
//    char* buffer = (char*)malloc(size + 1); //为字符串末尾添加 '\0'
//    if (buffer == NULL) {
//        printf("无法分配内存空间\n");
//        fclose(fp);
//        return NULL;
//    }
//    //读取文件内容
//    size_t result = fread(buffer, 1, size, fp);
//    if (result != size) {
//        printf("读取文件错误\n");
//        free(buffer);
//        fclose(fp);
//        return NULL;
//    }
//    //关闭文件
//    fclose(fp);
//    //添加字符串结束符
//    buffer[size] = '\0';
//    return buffer;
//}

////定义一个函数，用于读取.txt文件并返回char*类型
//char* read_txt_file(char* filename) {
//    //打开文件
//    FILE* fp = fopen(filename, "r");
//    if (fp == NULL) {
//        printf("无法打开文件%s\n", filename);
//        return NULL;
//    }
//    //获取文件大小
//
//    fseek(fp, 0, SEEK_END);
//    long size = ftell(fp)-13;
//    //long size = 755;
//    rewind(fp);
//    //分配内存空间
//    char* buffer = (char*)malloc(size);
//    if (buffer == NULL) {
//        printf("无法分配内存空间\n");
//        fclose(fp);
//        return NULL;
//    }
//    //读取文件内容
//    size_t result = fread(buffer, 1, size, fp);
//
//    if (result != size) {
//        printf("读取文件错误\n");
//        free(buffer);
//        fclose(fp);
//        return NULL;
//    }
//    //关闭文件
//    fclose(fp);
//    //添加字符串结束符
//    buffer[size] = '\0';
//
//    return buffer;
//}

/*
const char* programSource =
"__kernel                                               \n"
"void vecadd(__global uint8 *A,                         \n"
"            __global uint8 *B,                         \n"
"            __global uint8 *C)                         \n"
"{                                                      \n"
"                                                       \n"
"// Get the work=item's unique 10                       \n"
"                                                       \n"
"int idx=get_global_id(0);                              \n"
"                                                       \n"
"// Add the corresponding locations of                  \n"
"// 'A' and 'B'. and store the result in 'C'.           \n"
"C[idx] = A[idx] ^ B[idx];                              \n"
"                                                       \n"
"}                                                      \n";
*/


/*
;void CalculateMessage_OPENCL(uint8_t* result, uint8_t* C2, uint8_t* t, int lenInByte)
{
    for (int i = 0; i < lenInByte; i++)
    {
        result[i] = C2[i] ^ t[i];
    }
}
*/


void CalculateMessage_OPENCL(uint8_t* result, uint8_t* C2, uint8_t* t, int lenInByte)
{
    // This code executes on the OperCL host

    //定义一个.txt文件名
    char* filename = "CalculateMessage_kernel.txt";
    //调用函数，读取.cl文件并输出为char*类型
    char* programSource = read_txt_file(filename);
    if (programSource != NULL) {
        printf("文件内容为:\n%s\n", programSource);
    }


    // Host data主机输入输出向量
    uint8_t* A =C2; // Input array
    uint8_t* B =t; // Input array
    uint8_t* C =result; // Output array

    //printf("%s\n\n%s\n\n", A, C2);验证成功，C2和t已经导入进了输入向量

    // Elements in each array定义元素个数（向量长度）
    const int elements = lenInByte;

    // Compute the sizeof the data计算数据长度
    size_t datasize = sizeof(uint8_t) * elements;

    // Allocate space for input/output data分配对应内存空间
    //A = (uint8_t*)malloc(datasize);
    //B = (uint8_t*)malloc(datasize);
    //C = (uint8_t*)malloc(datasize);
    // Initialize the input data初始化输入数据

    /*
    for (int i = 0; i < elements; i++) {
        A[i] = C2[i];
        B[i] = t[i];
    }
    */
    //printf("%s\n\n%s\n\n", A, C2);

    // Use this to check the output of eachAPI call
    cl_int status;//定义状态码

    //----------------------------------------------
    // SIEP 1; Discover and initialize the platforms发现初始化平台
    //----------------------------------------------

    cl_uint numPlatforms = 0;//定义平台数
    cl_platform_id* platforms = NULL;//初始化平台id
    // Use cLGetPlatformIDs( ) to retrieve the number of
    // platforms
    status = clGetPlatformIDs(0, NULL, &numPlatforms);//状态码绑定计算平台，获取可用平台数

    // Allocate erough space for each platform
    platforms = (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));//分配平台空间
    // Fill in platforms with clGetplatformIDs()
    status = clGetPlatformIDs(numPlatforms, platforms, NULL);

    //----------------------------------------------
    // SIEP 2; Discover and initialize the devices发现初始化设备
    //----------------------------------------------

    cl_uint numDevices = 0;
    cl_device_id* devices = NULL;
    // use c1GetleviceIDs() to retrieve the number of
    // devices present
    status = clGetDeviceIDs(//状态码绑定设备，获取可用计算设备
        platforms[1],
        CL_DEVICE_TYPE_CPU,
        0,
        NULL,
        &numDevices);
    // Allocate enough space for each device
    devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));//分配设备空间

    // Fill in devices with clGetDeviceIDs()
    status = clGetDeviceIDs(
        platforms[1],
        CL_DEVICE_TYPE_CPU,
        numDevices,
        devices,
        NULL);

    //-------------------------------------------
    // SIEP 3; Create a context创造上下文
    //-------------------------------------------

    cl_context context = NULL;

    // Create a context using clCreateContext() ard
    // associate it with the devices上下文连接设备
    context = clCreateContext(
        NULL,
        numDevices,
        devices,
        NULL,
        NULL,
        &status);

    //-------------------------------------------
    // SIEP 4; Create a command queue创造命令队列
    //-------------------------------------------

    cl_command_queue cmdQueue;
    // Create a command queue using clCreateCommandQueue(),
    // and associate it with the device you want to execute
    // on命令队列连接到执行的设备和上下文
    cmdQueue = clCreateCommandQueue(
        context,
        devices[0],
        0,
        &status);

    //--------------------------------------------
    // SIEP 5; Create device buffers创建buffer对象
    //--------------------------------------------

    //定义输入输出buffer
    cl_mem bufferA; // Input array on the device
    cl_mem bufferB; // Input array on the device
    cl_mem bufferC; // Output array on the device

    // Use clCreateBuffer() to create a buffer object (c_A)
    // that will contain the data from the host array A
    //创建存储输入数据A的只读buffer
    bufferA = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        datasize,
        NULL,
        &status);

    // Use clCreateBuffer() to create a buffer object (C_B)
    // that will contain the data from the host array B
    //创建存储输入数据B的只读buffer
    bufferB = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        datasize,
        NULL,
        &status);

    // Use clCreateBuffer() to create a bufter object (C_C)
    // with enough space to hold the output data
    //创建存储输出数据C的写入buffer
    bufferC = clCreateBuffer(
        context,
        CL_MEM_WRITE_ONLY,
        datasize,
        NULL,
        &status);

    //--------------------------------------------
    // SIEP 6; Write host data to device buffers主机数据写入设备buffer
    //-------------------------------------------

    // Use clEnqueueWriteBuffer() to write input array A to
    // the device buffer bufferA//将主机内存写入缓冲区
    status = clEnqueueWriteBuffer(
        cmdQueue,
        bufferA,
        CL_FALSE,
        0,
        datasize,
        A,
        0,
        NULL,
        NULL);

    // Use clEnqueueWriteBuffer() to write input array B to
    // the device buffer bufferB//将主机内存写入缓冲区
    status = clEnqueueWriteBuffer(
        cmdQueue,
        bufferB,
        CL_FALSE,
        0,
        datasize,
        B,
        0,
        NULL,
        NULL);

    /*-------------------------------------------------- -
    status = clEnqueueWriteBuffer(
        cmdQueue,
        bufferA,
        CL_TRUE,
        0,
        datasize,
        C,
        0,
        NULL,
        NULL);

    *///----------------------------------------------------

    //------------------------------------------------
    // SIEP 7; Create ard compile the program
    //------------------------------------------------

    // Create a program using clCreateProgramWithSource()把之前的kernel编进来创建成程序
    cl_program program = clCreateProgramWithSource(
        context,
        1,
        (const char**)&programSource,
        NULL,
        &status);

    // Build (compile) the program for the devices with
    // clBuildProgram( )构建可执行文件
    status = clBuildProgram(
        program,
        numDevices,
        devices,
        NULL,
        NULL,
        NULL);

    if (status != CL_SUCCESS)
    {
        size_t len;
        char buffer[8 * 1024];
        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);

    }

    //---------------------------------------
    // STEP 8; Create the kernel
    //---------------------------------------

    cl_kernel kernel = NULL;

    // Use clCreateKernel() to create a kernel from the
    // vector addition function (named "vecadd”)
    kernel = clCreateKernel(program, "vecadd", &status);

    //----------------------------------------
    // STEP 9; Set the kernel arguments
    //----------------------------------------

    // Associate the input ard output buffers with the
    // kernel
    // using clSetKernelArg( )为内核设定参数
    status = clSetKernelArg(
        kernel,
        0,
        sizeof(cl_mem),
        &bufferA);
    status |= clSetKernelArg(
        kernel,
        1,
        sizeof(cl_mem),
        &bufferB);
    status |= clSetKernelArg(
        kernel,
        2,
        sizeof(cl_mem),
        &bufferC);

    //-------------------------------------------------
    // STEP 10; Configure the work-item structure
    //-------------------------------------------------

    // Define an index space (global work size) of work
    // items for
    // execution. A workgroup size (loca1 work size) is not
    // required.
    // but can be used.
    size_t globalWorkSize[1];
    // There are 'elements" work-items
    globalWorkSize[0] = elements;

    //------------------------------------------------
    // SIEP 11; Enqueue the kernel for execution
    //------------------------------------------------

    // Execute the kernel by using
    // clEnqueueNDRangeKernel().
    // 'globalWorkSize" is the ID dimension of the
    // work-items
    status = clEnqueueNDRangeKernel(//在整个数据范围内执行内核
        cmdQueue,
        kernel,
        1,
        NULL,
        globalWorkSize,
        NULL,
        0,
        NULL,
        NULL);

    //-----------------------------------------------------
    // SIEP 12; Read the output buffer back to the host
    //-----------------------------------------------------

    // Use clEnqueueReadBuffer() to read the openCL output
    // buffer (bufferC)
    // to the host output array (C)//把bufferC里的读入C
    clEnqueueReadBuffer(
        cmdQueue,
        bufferC,
        CL_TRUE,
        0,
        datasize,
        C,
        0,
        NULL,
        NULL);

    // Verify the output
    /*
    bool result = true;
    for (int i = 0; i < elements; i++) {
        if (C[i] != i + i) {
            result = false;
            break;
        }
    }
    if (result) {
        printf("Output is correct\n");
    }
    else {
        printf("Output is incorrect\n");
    }
    */
    //--------------------------------------------
    // STEP 13; Release OpenCL resources//释放资源
    //--------------------------------------------
    //printf("%s\n", C);
    // free OperCL resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cmdQueue);
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseContext(context);
    // Free host resources
    //free(A);
    //free(B);
    //free(C);
    free(platforms);
    free(devices);
}



void SM2Encrypt_OPENCL(uint8_t* res, char* messagePlain, int messageSizeInBit, EccPoint* pubKey, uint256_t randomK, EccPoint* G,int lenInByte)
{
    // This code executes on the OperCL host

    //定义一个.txt文件名
    char* filename = "En_kernel_cpu.txt";
    //调用函数，读取.cl文件并输出为char*类型
    char* programSource = read_txt_file(filename);
    if (programSource != NULL) {
        printf("文件内容为%s:\n", programSource);
    }


    //for (int i = 0; programSource[i]!='\0'; i++)
    //{
    //    printf("%c", programSource[i]);
    //}


    // Host data主机输入输出向量
    
    
    uint8_t* A = res; // Input array
    char* B = messagePlain; // Input array
    int* C = &messageSizeInBit; // Output array
    EccPoint* D = pubKey;
    uint256_t* E = &randomK;
    EccPoint* F = G;
    //F.x[0] = G->x[0];
    ////F.x[1] = G->x[1];
    //F.x[2] = G->x[2];
    //F.x[3] = G->x[3];

    //F.y[0] = G->y[0];
   // F.y[1] = G->y[1];
   // F.y[2] = G->y[2];
   // F.y[3] = G->y[3];


    //printf("%s\n\n%s\n\n", A, C2);验证成功，C2和t已经导入进了输入向量

    // Elements in each array定义元素个数（向量长度）
    const unsigned int elements = lenInByte;

    // Compute the sizeof the data计算数据长度
    size_t datasize_a = sizeof(uint8_t) * ((messageSizeInBit / 8) + 97) * elements;
    size_t datasize_b = sizeof(char) * (messageSizeInBit / 8) * elements;
    size_t datasize_c = sizeof(int) * elements;
    size_t datasize_d = sizeof(EccPoint) * elements;
    size_t datasize_e = sizeof(uint256_t) * elements;
    size_t datasize_f = sizeof(EccPoint) * elements;

    // Allocate space for input/output data分配对应内存空间
    //A = (uint8_t*)malloc(datasize);
    //B = (uint8_t*)malloc(datasize);
    //C = (uint8_t*)malloc(datasize);
    // Initialize the input data初始化输入数据

    /*
    for (int i = 0; i < elements; i++) {
        A[i] = C2[i];
        B[i] = t[i];
    }
    */
    //printf("%s\n\n%s\n\n", A, C2);

    // Use this to check the output of eachAPI call
    cl_int status;//定义状态码

    //定义输入输出buffer
    cl_mem bufferA; // Input array on the device
    cl_mem bufferB; // Input array on the device
    cl_mem bufferC; // Output array on the device
    cl_mem bufferD;
    cl_mem bufferE;
    cl_mem bufferF;

    //----------------------------------------------
    // SIEP 1; Discover and initialize the platforms发现初始化平台
    //----------------------------------------------






    /**/


    cl_uint numPlatforms = 0;//定义平台数
    cl_platform_id* platforms = NULL;//初始化平台id
    // Use cLGetPlatformIDs( ) to retrieve the number of
    // platforms
    status = clGetPlatformIDs(0, NULL, &numPlatforms);//状态码绑定计算平台，获取可用平台数

    // Allocate erough space for each platform
    platforms = (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));//分配平台空间
    // Fill in platforms with clGetplatformIDs()
    status = clGetPlatformIDs(numPlatforms, platforms, NULL);

    //----------------------------------------------
    // SIEP 2; Discover and initialize the devices发现初始化设备
    //----------------------------------------------

    cl_uint numDevices = 0;
    cl_device_id* devices = NULL;
    // use c1GetleviceIDs() to retrieve the number of
    // devices present
    status = clGetDeviceIDs(//状态码绑定设备，获取可用计算设备
        platforms[2],
        CL_DEVICE_TYPE_CPU,
        0,
        NULL,
        &numDevices);
    // Allocate enough space for each device
    devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));//分配设备空间

    // Fill in devices with clGetDeviceIDs()
    status = clGetDeviceIDs(
        platforms[2],
        CL_DEVICE_TYPE_CPU,
        numDevices,
        devices,
        NULL);

    //size_t max_binary_size;

    // 获取设备支持的 cl_program 变量的最大内存二进制大小
    //clGetDeviceInfo(devices[0], CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(size_t), &max_binary_size, NULL);

    // 计算变量占用内存大小
    //size_t total_mem_size = max_binary_size * sizeof(char);
    //printf("Max binary size on device: %zu bytes\n", max_binary_size);

    //cl_device_id device; // OpenCL设备ID

// 查询设备的最大二进制大小opencl1.2
    //size_t max_binary_size;
    //cl_int err = clGetDeviceInfo(devices[0], CL_DEVICE_MAX_PROGRAM_BINARY_SIZE, sizeof(size_t), &max_binary_size, NULL);
    //if (err != CL_SUCCESS) {
        // 错误处理
    //    return err;
    //}

    // 打印设备的最大二进制大小
    //printf("Max binary size on device: %zu bytes\n", max_binary_size);

    /*
    // 获取平台ID
    cl_platform_id platform_id= platforms[1];
    cl_int ret;// = clGetPlatformIDs(1, &platform_id, NULL);
    //if (ret != CL_SUCCESS) {
    //    printf("Error: Failed to get platform ID!\n");
    //    return EXIT_FAILURE;
    //}

    // 获取平台名称
    char platform_name[128];
    ret = clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, sizeof(platform_name), platform_name, NULL);
    if (ret != CL_SUCCESS) {
        printf("Error: Failed to get platform name!\n");
        return EXIT_FAILURE;
    }
    printf("Platform name: %s\n", platform_name);

    // 获取设备ID
    cl_device_id device_id;
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (ret != CL_SUCCESS) {
        printf("Error: Failed to get device ID!\n");
        return EXIT_FAILURE;
    }

    // 获取设备名称
    char device_name[128];
    ret = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
    if (ret != CL_SUCCESS) {
        printf("Error: Failed to get device name!\n");
        return EXIT_FAILURE;
    }
    printf("Device name: %s\n", device_name);

    // 获取设备供应商
    char device_vendor[128];
    ret = clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, sizeof(device_vendor), device_vendor, NULL);
    if (ret != CL_SUCCESS) {
        printf("Error: Failed to get device vendor!\n");
        return EXIT_FAILURE;
    }
    printf("Device vendor: %s\n", device_vendor);

    // 获取设备全局内存大小
    cl_ulong global_mem_size;
    ret = clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(global_mem_size), &global_mem_size, NULL);
    if (ret != CL_SUCCESS) {
        printf("Error: Failed to get device global memory size!\n");
        return EXIT_FAILURE;
    }
    printf("Device global memory size: %llu bytes\n", (unsigned long long)global_mem_size);

    */


    cl_ulong vector_width_long;
    cl_uint opencl_c_version;
    char extensions[1024];
    /*
    // 获取设备支持的64位整数向量宽度
    clGetDeviceInfo(devices[0], CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, sizeof(cl_ulong), &vector_width_long, NULL);
    if (vector_width_long > 0) {
        printf("Device supports 64-bit integers (vector width: %lu)\n", vector_width_long);
    }
    else {
        printf("Device does not support 64-bit integers\n");
    }

    // 获取设备支持的OpenCL C版本
    clGetDeviceInfo(devices[0], CL_DEVICE_OPENCL_C_VERSION, sizeof(cl_uint), &opencl_c_version, NULL);
    printf("Device OpenCL C version: %u\n", opencl_c_version);

    // 获取设备支持的扩展列表
    clGetDeviceInfo(devices[0], CL_DEVICE_EXTENSIONS, sizeof(extensions), &extensions, NULL);
    if (strstr(extensions, "cl_khr_int64_base_atomics") || strstr(extensions, "cl_khr_int64_extended_atomics")) {
        printf("Device supports 64-bit integer atomics\n");
    }
    else {
        printf("Device does not support 64-bit integer atomics\n");
    }

    // 获取设备支持的最大内存对象大小
    cl_ulong max_mem_size;
    clGetDeviceInfo(devices[0], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &max_mem_size, NULL);
    printf("Device max memory allocation size: %lu bytes\n", max_mem_size);

    */
    //-------------------------------------------
    // SIEP 3; Create a context创造上下文
    //-------------------------------------------

    cl_context context = NULL;

    // Create a context using clCreateContext() ard
    // associate it with the devices上下文连接设备
    context = clCreateContext(
        NULL,
        numDevices,
        devices,
        NULL,
        NULL,
        &status);

    //-------------------------------------------
    // SIEP 4; Create a command queue创造命令队列
    //-------------------------------------------

    cl_command_queue cmdQueue;
    // Create a command queue using clCreateCommandQueue(),
    // and associate it with the device you want to execute
    // on命令队列连接到执行的设备和上下文
    cmdQueue = clCreateCommandQueue(
        context,
        devices[0],
        0,
        &status);

    //--------------------------------------------
    // SIEP 5; Create device buffers创建buffer对象
    //--------------------------------------------

    

    // Use clCreateBuffer() to create a buffer object (c_A)
    // that will contain the data from the host array A
    //创建存储输入数据A的只读buffer
    bufferA = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        datasize_a,
        NULL,
        &status);

    // Use clCreateBuffer() to create a buffer object (C_B)
    // that will contain the data from the host array B
    //创建存储输入数据B的只读buffer
    /*
    bufferB = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        datasize_b,
        NULL,
        &status);

    bufferC = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        datasize_c,
        NULL,
        &status);

    bufferD = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        datasize_d,
        NULL,
        &status);

    bufferE = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        datasize_e,
        NULL,
        &status);

    bufferF = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        datasize_f,
        NULL,
        &status);
        */
    // Use clCreateBuffer() to create a bufter object (C_C)
    // with enough space to hold the output data
    //创建存储输出数据C的写入buffer
    //bufferC = clCreateBuffer(
    //   context,
    //    CL_MEM_WRITE_ONLY,
    //    datasize,
    //    NULL,
    //    &status);

    //--------------------------------------------
    // SIEP 6; Write host data to device buffers主机数据写入设备buffer
    //-------------------------------------------

    // Use clEnqueueWriteBuffer() to write input array A to
    // the device buffer bufferA//将主机内存写入缓冲区
    
    status = clEnqueueWriteBuffer(
        cmdQueue,
        bufferA,
        CL_FALSE,
        0,
        datasize_a,
        A,
        0,
        NULL,
        NULL);
    
    // Use clEnqueueWriteBuffer() to write input array B to
    // the device buffer bufferB//将主机内存写入缓冲区
    /*
    status = clEnqueueWriteBuffer(
        cmdQueue,
        bufferB,
        CL_FALSE,
        0,
        datasize_b,
        B,
        0,
        NULL,
        NULL);

    status = clEnqueueWriteBuffer(
        cmdQueue,
        bufferC,
        CL_FALSE,
        0,
        datasize_c,
        C,
        0,
        NULL,
        NULL);

    status = clEnqueueWriteBuffer(
        cmdQueue,
        bufferD,
        CL_FALSE,
        0,
        datasize_d,
        D,
        0,
        NULL,
        NULL);

    status = clEnqueueWriteBuffer(
        cmdQueue,
        bufferE,
        CL_FALSE,
        0,
        datasize_e,
        E,
        0,
        NULL,
        NULL);

    status = clEnqueueWriteBuffer(
        cmdQueue,
        bufferF,
        CL_FALSE,
        0,
        datasize_f,
        &F,
        0,
        NULL,
        NULL);


        */
    /*-------------------------------------------------- -
    status = clEnqueueWriteBuffer(
        cmdQueue,
        bufferA,
        CL_TRUE,
        0,
        datasize,
        C,
        0,
        NULL,
        NULL);

    *///----------------------------------------------------

    //------------------------------------------------
    // SIEP 7; Create ard compile the program
    //------------------------------------------------

    // Create a program using clCreateProgramWithSource()把之前的kernel编进来创建成程序
    cl_program program = clCreateProgramWithSource(
        context,
        1,
        (const char**)&programSource,
        NULL,
        &status);

    // Build (compile) the program for the devices with
    // clBuildProgram( )构建可执行文件
    status = clBuildProgram(
        program,
        numDevices,
        devices,
        NULL,
        NULL,
        NULL);

    if (status == CL_BUILD_PROGRAM_FAILURE) { // build error from clBuildProgram
        cl_int err = CL_SUCCESS;
        size_t len;
        char buffer[1024];
        err = clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, 0, &len);
        if ((err == CL_SUCCESS) && (len > 0)) {
            int n = 0;
            n = (int)malloc(len);
            char* build_log_ca = (char*)malloc(len);
            err = clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, len, build_log_ca, 0);
            if (err == CL_SUCCESS) {

                printf("OclProgram: building OpenCL program returned with error. Build log:\n\n");
                printf("%s\n\n", build_log_ca);
            }
        }
    }

    //---------------------------------------
    // STEP 8; Create the kernel
    //---------------------------------------

    cl_kernel kernel = NULL;


    // Use clCreateKernel() to create a kernel from the
    // vector addition function (named "vecadd”)
    /*
    cl_int err;

    size_t max_mem_size;
    err = clGetDeviceInfo(devices[0], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(size_t), &max_mem_size, NULL);
    if (err != CL_SUCCESS) {
        printf("Error: Failed to get device info!\n");
        return 1;
    }
    printf("Max kernel size: %llu bytes\n", max_mem_size);

    // 获取实际编译的 cl_program 变量所占用的内存大小
    size_t binary_sizes[1];
    err = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(binary_sizes), binary_sizes, NULL);
    if (err != CL_SUCCESS) {
        printf("Error: Failed to get program info!\n");
        return 1;
    }
    size_t actual_mem_size = binary_sizes[0];
    printf("My kernel size: %llu bytes\n", actual_mem_size);

    // 检查实际内存大小是否超过设备支持的最大内存大小
    if (actual_mem_size > max_mem_size) {
        printf("Error: Program binary size exceeds device memory limit!\n");
        return 1;
    }

    printf("Program binary size is within device memory limit.\n");
    */

    kernel = clCreateKernel(program, "SM2Encrypt", &status);

    //----------------------------------------
    // STEP 9; Set the kernel arguments
    //----------------------------------------

    // Associate the input ard output buffers with the
    // kernel
    // using clSetKernelArg( )为内核设定参数
    status = clSetKernelArg(
        kernel,
        0,
        sizeof(cl_mem),
        &bufferA);
    /*
    status = clSetKernelArg(
        kernel,
        1,
        sizeof(cl_mem),
        &bufferB);
    status = clSetKernelArg(
        kernel,
        2,
        sizeof(cl_mem),
        &bufferC);
    status = clSetKernelArg(
        kernel,
        3,
        sizeof(cl_mem),
        &bufferD);
    status = clSetKernelArg(
        kernel,
        4,
        sizeof(cl_mem),
        &bufferE);
    status = clSetKernelArg(
        kernel,
        5,
        sizeof(cl_mem),
        &bufferF);
    */
    //-------------------------------------------------
    // STEP 10; Configure the work-item structure
    //-------------------------------------------------

    // Define an index space (global work size) of work
    // items for
    // execution. A workgroup size (loca1 work size) is not
    // required.
    // but can be used.
    size_t globalWorkSize[1];
    // There are 'elements" work-items
    globalWorkSize[0] = elements;

    //------------------------------------------------
    // SIEP 11; Enqueue the kernel for execution
    //------------------------------------------------

    // Execute the kernel by using
    // clEnqueueNDRangeKernel().
    // 'globalWorkSize" is the ID dimension of the
    // work-items

    //cl_event event;
    status = clEnqueueNDRangeKernel(//在整个数据范围内执行内核
        cmdQueue,
        kernel,
        1,
        NULL,
        globalWorkSize,
        NULL,
        0,
        NULL,
        NULL);

    //-----------------------------------------------------
    // SIEP 12; Read the output buffer back to the host
    //-----------------------------------------------------

    // Use clEnqueueReadBuffer() to read the openCL output
    // buffer (bufferC)
    // to the host output array (C)//把bufferC里的读入C

    //clWaitForEvents(1, &event);
    //clFinish(cmdQueue);
    clEnqueueReadBuffer(
        cmdQueue,
        bufferA,
        CL_TRUE,
        0,
        datasize_a,
        A,
        0,
        NULL,
        NULL);

    //--------------------------------------------
    // STEP 13; Release OpenCL resources//释放资源
    //--------------------------------------------
    //printf("%s\n", C);
    // free OperCL resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(cmdQueue);
    clReleaseMemObject(bufferA);
    //clReleaseMemObject(bufferB);
    //clReleaseMemObject(bufferC);
    clReleaseContext(context);
    // Free host resources
    //free(A);
    //free(B);
    //free(C);
    free(platforms);
    free(devices);
}

void SM2Encrypt_OPENCL_NEW(uint8_t* res, char* messagePlain, int messageSizeInBit, EccPoint* pubKey, uint256_t randomK, EccPoint* G, int lenInByte)
{
    // This code executes on the OperCL host

    //定义一个.txt文件名
    char* filename = "En_kernel_cpu.txt";
    //调用函数，读取.cl文件并输出为char*类型
    char* programSource = read_txt_file(filename);

    //if (programSource != NULL) {
    //    printf("文件内容为%s:\n", programSource);
    //}

    // Host input vectors主机输入向量
    uint8_t* h_a=res;
    char* h_b = messagePlain; // Input array
    int cc = messageSizeInBit; // Output array
    int* h_c = &cc;
    EccPoint dd;// = pubKey;
    EccPoint* h_d;
    uint64_t h_e[4];
    h_e[0] = randomK[0];
    h_e[1] = randomK[1];
    h_e[2] = randomK[2];
    h_e[3] = randomK[3];

    //uint64_t* h_e = ee;

    EccPoint ff1;// = G;
    EccPoint* h_f;
    ff1.x[0] = G->x[0];
    ff1.x[1] = G->x[1];
    ff1.x[2] = G->x[2];
    ff1.x[3] = G->x[3];

    ff1.y[0] = G->y[0];
    ff1.y[1] = G->y[1];
    ff1.y[2] = G->y[2];
    ff1.y[3] = G->y[3];



    h_f = &ff1;

    dd.x[0] = pubKey->x[0];
    dd.x[1] = pubKey->x[1];
    dd.x[2] = pubKey->x[2];
    dd.x[3] = pubKey->x[3];

    dd.y[0] = pubKey->y[0];
    dd.y[1] = pubKey->y[1];
    dd.y[2] = pubKey->y[2];
    dd.y[3] = pubKey->y[3];
    h_d = &dd;
    //ff.x[0] = '3';
    printf("%c\n", (char)messagePlain[0]);

    // Device input buffers设备输出的buffer对象
    cl_mem d_a;
    // Device output buffer设备输入的buffer对象
    cl_mem d_b, d_e, d_f, d_d, d_c;

    // Elements in each array定义元素个数（向量长度）
    const unsigned int elements = lenInByte;

    // Length of vectors定义的向量长度
    unsigned int n = elements;

    size_t datasize_a = sizeof(uint8_t) * ((messageSizeInBit / 8) + 97) * elements;
    size_t datasize_b = sizeof(char) * (messageSizeInBit / 8) * elements;
    size_t datasize_c = sizeof(int) * elements;
    size_t datasize_d = sizeof(EccPoint) * elements;
    size_t datasize_e = sizeof(uint64_t)* 4 * elements;
    size_t datasize_f = sizeof(EccPoint) * elements;

    cl_platform_id cpPlatform[3];        //定义平台 OpenCL platform
    cl_device_id device_id;           // 定义设备device ID
    cl_context context;               // 定义上下文context
    cl_command_queue queue;           // 定义命令队列command queue
    cl_program program;               // 定义程序program
    cl_kernel kernel, kernel2;                 // 定义kernel

    // Allocate memory for each vector on host分配内存给每个主机向量】【(存储空间基址) malloc(分配的内存空间大小)
    //h_a = (uint8_t*)malloc(datasize_a);
    //h_b = (char*)malloc(datasize_b);   
    //h_e = (uint64_t*)malloc(datasize_e);

    // Initialize vectors on host在主机上初始化向量
    int i;
    for (i = 0; i < 505; i++)
    {
        h_a[i] = '2';//a全为1
        //h_b[i] = 1;
    }

    //-------------------------------------------------------------

    SM2Encrypt_kernel(h_a, h_b, h_c, h_d, h_e, h_f);


    printf("%s", h_a);



    //--------------------------------------------------------------
   
    size_t globalSize, localSize;//全局大小，本地大小
    cl_int err;//错误码

    // Number of total work items - localSize must be devisor
    globalSize = n;//ceil(n/(double)localSize)*localSize;

    // 绑定到平台,获取可用计算平台，加入平台数3Bind to platform
    err = clGetPlatformIDs(3, cpPlatform, NULL);
    // 获取可用计算设备，仅GPU，加入设备数1Get ID for the device, change the platform number to get the correct gpu driver.
    err = clGetDeviceIDs(cpPlatform[2], CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);

    // 创建上下文Create a context
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

    // 创建命令队列Create a command queue
    queue = clCreateCommandQueue(context, device_id, 0, &err);

    // 把之前的kernel编进来创建成程序Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1,
        (const char**)&programSource, NULL, &err);

    // 构建可执行文件Build the program executable
    err=clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    if (err == CL_BUILD_PROGRAM_FAILURE) { // build error from clBuildProgram
        cl_int err1 = CL_SUCCESS;
        size_t len;
        char buffer[1024];
        err1 = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, 0, &len);
        if ((err1 == CL_SUCCESS) && (len > 0)) {
            int n = 0;
            n = (int)malloc(len);
            char* build_log_ca = (char*)malloc(len);
            err1 = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, len, build_log_ca, 0);
            if (err1 == CL_SUCCESS) {

                printf("OclProgram: building OpenCL program returned with error. Build log:\n\n");
                printf("%s\n\n", build_log_ca);
            }
        }
    }

    // 创建内核对象Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "SM2Encrypt", &err);
    // kernel2 = clCreateKernel(program, "vecAdd", &err);

    //创建输入输出队列,kernel对缓冲区的读取权限：读写
     // Create the input and output arrays in device memory for our calculation
    d_a = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_a, NULL, NULL);
    d_b = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_b, NULL, NULL);
    d_c = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_c, NULL, NULL);
    d_d = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_d, NULL, NULL);
    d_e = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_e, NULL, NULL);
    d_f = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_f, NULL, NULL);
    
    //将主机内存写入缓冲区
    err = clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0,datasize_a, h_a, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, d_b, CL_TRUE, 0, datasize_b, h_b, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, d_c, CL_TRUE, 0, datasize_c, h_c, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, d_d, CL_TRUE, 0, datasize_d, h_d, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, d_e, CL_TRUE, 0, datasize_e, h_e, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, d_f, CL_TRUE, 0,datasize_f, h_f, 0, NULL, NULL);
    
    //err = clEnqueueWriteBuffer(queue, d_e, CL_TRUE, 0,datasize_e, h_e, 0, NULL, NULL);
        //bb = i * n;
        // Set the arguments to our compute kernel为内核设定参数
        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a);
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_b);
        err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_c);
        err = clSetKernelArg(kernel, 3, sizeof(cl_mem), &d_d);
        err = clSetKernelArg(kernel, 4, sizeof(cl_mem), &d_e);
        err = clSetKernelArg(kernel, 5, sizeof(cl_mem), &d_f);


        // 在整个数据范围内执行内核Execute the kernel over the entire range of the data set一维数组，偏移量为空，长为3
        err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, 0, 0, NULL, NULL);
        clFinish(queue);

    // Read the results from the device从缓冲区读入主机内存
    clEnqueueReadBuffer(queue, d_a, CL_TRUE, 0,
        datasize_a, h_a, 0, NULL, NULL);
    clFinish(queue);
    
    // release OpenCL resources释放opencl资源
    clReleaseMemObject(d_a);
    clReleaseMemObject(d_b);
    clReleaseMemObject(d_e);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    //release host memory释放主机资源
    //free(h_a);
    //free(h_b);
    //free(h_e);

}