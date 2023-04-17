
//#include "SM2.h"

#include "OPENCL.h"
#include<stdlib.h>
#include<string.h>
#include"test_kernel_in_c.h"


#pragma warning( disable : 4996 )
//c����ʵ��memset
void* memset_c(void* ptr, int value, size_t num) {
    unsigned char* p = ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

//c����ʵ��memcpy
void* memcpy_c(void* dest, const void* src, size_t n) {
    char* dp = dest;
    const char* sp = src;
    while (n--) {
        *dp++ = *sp++;
    }
    return dest;
}

char* read_txt_file(char* filename) {
    //���ļ�
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("�޷����ļ�%s\n", filename);
        return NULL;
    }
    //��ȡ�ļ���С
    fseek(fp, 0, SEEK_END);
    long long size = ftell(fp);
    rewind(fp);
    //�����ڴ�ռ�
    char* buffer = (char*)malloc(size + 1); //Ϊ�ַ���ĩβ��� '\0'
    if (buffer == NULL) {
        printf("�޷������ڴ�ռ�\n");
        fclose(fp);
        return NULL;
    }
    //��ȡ�ļ�����
    size_t result = fread(buffer, 1, size, fp);
    if (result != size) {
        printf("��ȡ�ļ�����\n");
        free(buffer);
        fclose(fp);
        return NULL;
    }
    //�ر��ļ�
    fclose(fp);
    //����ַ���������
    buffer[size] = '\0';
    return buffer;
}


//char* read_txt_file(char* filename) {
//    //���ļ�
//    FILE* fp = fopen(filename, "r");
//    if (fp == NULL) {
//        printf("�޷����ļ�%s\n", filename);
//        return NULL;
//    }
//    //��ȡ�ļ���С
//    fseek(fp, 0, SEEK_END);
//    long long size = ftell(fp)-2633 ;
//    rewind(fp);
//    //�����ڴ�ռ�
//    char* buffer = (char*)malloc(size + 1); //Ϊ�ַ���ĩβ��� '\0'
//    if (buffer == NULL) {
//        printf("�޷������ڴ�ռ�\n");
//        fclose(fp);
//        return NULL;
//    }
//    //��ȡ�ļ�����
//    size_t result = fread(buffer, 1, size, fp);
//    if (result != size) {
//        printf("��ȡ�ļ�����\n");
//        free(buffer);
//        fclose(fp);
//        return NULL;
//    }
//    //�ر��ļ�
//    fclose(fp);
//    //����ַ���������
//    buffer[size] = '\0';
//    return buffer;
//}

////����һ�����������ڶ�ȡ.txt�ļ�������char*����
//char* read_txt_file(char* filename) {
//    //���ļ�
//    FILE* fp = fopen(filename, "r");
//    if (fp == NULL) {
//        printf("�޷����ļ�%s\n", filename);
//        return NULL;
//    }
//    //��ȡ�ļ���С
//
//    fseek(fp, 0, SEEK_END);
//    long size = ftell(fp)-13;
//    //long size = 755;
//    rewind(fp);
//    //�����ڴ�ռ�
//    char* buffer = (char*)malloc(size);
//    if (buffer == NULL) {
//        printf("�޷������ڴ�ռ�\n");
//        fclose(fp);
//        return NULL;
//    }
//    //��ȡ�ļ�����
//    size_t result = fread(buffer, 1, size, fp);
//
//    if (result != size) {
//        printf("��ȡ�ļ�����\n");
//        free(buffer);
//        fclose(fp);
//        return NULL;
//    }
//    //�ر��ļ�
//    fclose(fp);
//    //����ַ���������
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

    //����һ��.txt�ļ���
    char* filename = "CalculateMessage_kernel.txt";
    //���ú�������ȡ.cl�ļ������Ϊchar*����
    char* programSource = read_txt_file(filename);
    if (programSource != NULL) {
        printf("�ļ�����Ϊ:\n%s\n", programSource);
    }


    // Host data���������������
    uint8_t* A =C2; // Input array
    uint8_t* B =t; // Input array
    uint8_t* C =result; // Output array

    //printf("%s\n\n%s\n\n", A, C2);��֤�ɹ���C2��t�Ѿ����������������

    // Elements in each array����Ԫ�ظ������������ȣ�
    const int elements = lenInByte;

    // Compute the sizeof the data�������ݳ���
    size_t datasize = sizeof(uint8_t) * elements;

    // Allocate space for input/output data�����Ӧ�ڴ�ռ�
    //A = (uint8_t*)malloc(datasize);
    //B = (uint8_t*)malloc(datasize);
    //C = (uint8_t*)malloc(datasize);
    // Initialize the input data��ʼ����������

    /*
    for (int i = 0; i < elements; i++) {
        A[i] = C2[i];
        B[i] = t[i];
    }
    */
    //printf("%s\n\n%s\n\n", A, C2);

    // Use this to check the output of eachAPI call
    cl_int status;//����״̬��

    //----------------------------------------------
    // SIEP 1; Discover and initialize the platforms���ֳ�ʼ��ƽ̨
    //----------------------------------------------

    cl_uint numPlatforms = 0;//����ƽ̨��
    cl_platform_id* platforms = NULL;//��ʼ��ƽ̨id
    // Use cLGetPlatformIDs( ) to retrieve the number of
    // platforms
    status = clGetPlatformIDs(0, NULL, &numPlatforms);//״̬��󶨼���ƽ̨����ȡ����ƽ̨��

    // Allocate erough space for each platform
    platforms = (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));//����ƽ̨�ռ�
    // Fill in platforms with clGetplatformIDs()
    status = clGetPlatformIDs(numPlatforms, platforms, NULL);

    //----------------------------------------------
    // SIEP 2; Discover and initialize the devices���ֳ�ʼ���豸
    //----------------------------------------------

    cl_uint numDevices = 0;
    cl_device_id* devices = NULL;
    // use c1GetleviceIDs() to retrieve the number of
    // devices present
    status = clGetDeviceIDs(//״̬����豸����ȡ���ü����豸
        platforms[1],
        CL_DEVICE_TYPE_CPU,
        0,
        NULL,
        &numDevices);
    // Allocate enough space for each device
    devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));//�����豸�ռ�

    // Fill in devices with clGetDeviceIDs()
    status = clGetDeviceIDs(
        platforms[1],
        CL_DEVICE_TYPE_CPU,
        numDevices,
        devices,
        NULL);

    //-------------------------------------------
    // SIEP 3; Create a context����������
    //-------------------------------------------

    cl_context context = NULL;

    // Create a context using clCreateContext() ard
    // associate it with the devices�����������豸
    context = clCreateContext(
        NULL,
        numDevices,
        devices,
        NULL,
        NULL,
        &status);

    //-------------------------------------------
    // SIEP 4; Create a command queue�����������
    //-------------------------------------------

    cl_command_queue cmdQueue;
    // Create a command queue using clCreateCommandQueue(),
    // and associate it with the device you want to execute
    // on����������ӵ�ִ�е��豸��������
    cmdQueue = clCreateCommandQueue(
        context,
        devices[0],
        0,
        &status);

    //--------------------------------------------
    // SIEP 5; Create device buffers����buffer����
    //--------------------------------------------

    //�����������buffer
    cl_mem bufferA; // Input array on the device
    cl_mem bufferB; // Input array on the device
    cl_mem bufferC; // Output array on the device

    // Use clCreateBuffer() to create a buffer object (c_A)
    // that will contain the data from the host array A
    //�����洢��������A��ֻ��buffer
    bufferA = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        datasize,
        NULL,
        &status);

    // Use clCreateBuffer() to create a buffer object (C_B)
    // that will contain the data from the host array B
    //�����洢��������B��ֻ��buffer
    bufferB = clCreateBuffer(
        context,
        CL_MEM_READ_ONLY,
        datasize,
        NULL,
        &status);

    // Use clCreateBuffer() to create a bufter object (C_C)
    // with enough space to hold the output data
    //�����洢�������C��д��buffer
    bufferC = clCreateBuffer(
        context,
        CL_MEM_WRITE_ONLY,
        datasize,
        NULL,
        &status);

    //--------------------------------------------
    // SIEP 6; Write host data to device buffers��������д���豸buffer
    //-------------------------------------------

    // Use clEnqueueWriteBuffer() to write input array A to
    // the device buffer bufferA//�������ڴ�д�뻺����
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
    // the device buffer bufferB//�������ڴ�д�뻺����
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

    // Create a program using clCreateProgramWithSource()��֮ǰ��kernel����������ɳ���
    cl_program program = clCreateProgramWithSource(
        context,
        1,
        (const char**)&programSource,
        NULL,
        &status);

    // Build (compile) the program for the devices with
    // clBuildProgram( )������ִ���ļ�
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
    // vector addition function (named "vecadd��)
    kernel = clCreateKernel(program, "vecadd", &status);

    //----------------------------------------
    // STEP 9; Set the kernel arguments
    //----------------------------------------

    // Associate the input ard output buffers with the
    // kernel
    // using clSetKernelArg( )Ϊ�ں��趨����
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
    status = clEnqueueNDRangeKernel(//���������ݷ�Χ��ִ���ں�
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
    // to the host output array (C)//��bufferC��Ķ���C
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
    // STEP 13; Release OpenCL resources//�ͷ���Դ
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

    //����һ��.txt�ļ���
    char* filename = "En_kernel_cpu.txt";
    //���ú�������ȡ.cl�ļ������Ϊchar*����
    char* programSource = read_txt_file(filename);
    if (programSource != NULL) {
        printf("�ļ�����Ϊ%s:\n", programSource);
    }


    //for (int i = 0; programSource[i]!='\0'; i++)
    //{
    //    printf("%c", programSource[i]);
    //}


    // Host data���������������
    
    
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


    //printf("%s\n\n%s\n\n", A, C2);��֤�ɹ���C2��t�Ѿ����������������

    // Elements in each array����Ԫ�ظ������������ȣ�
    const unsigned int elements = lenInByte;

    // Compute the sizeof the data�������ݳ���
    size_t datasize_a = sizeof(uint8_t) * ((messageSizeInBit / 8) + 97) * elements;
    size_t datasize_b = sizeof(char) * (messageSizeInBit / 8) * elements;
    size_t datasize_c = sizeof(int) * elements;
    size_t datasize_d = sizeof(EccPoint) * elements;
    size_t datasize_e = sizeof(uint256_t) * elements;
    size_t datasize_f = sizeof(EccPoint) * elements;

    // Allocate space for input/output data�����Ӧ�ڴ�ռ�
    //A = (uint8_t*)malloc(datasize);
    //B = (uint8_t*)malloc(datasize);
    //C = (uint8_t*)malloc(datasize);
    // Initialize the input data��ʼ����������

    /*
    for (int i = 0; i < elements; i++) {
        A[i] = C2[i];
        B[i] = t[i];
    }
    */
    //printf("%s\n\n%s\n\n", A, C2);

    // Use this to check the output of eachAPI call
    cl_int status;//����״̬��

    //�����������buffer
    cl_mem bufferA; // Input array on the device
    cl_mem bufferB; // Input array on the device
    cl_mem bufferC; // Output array on the device
    cl_mem bufferD;
    cl_mem bufferE;
    cl_mem bufferF;

    //----------------------------------------------
    // SIEP 1; Discover and initialize the platforms���ֳ�ʼ��ƽ̨
    //----------------------------------------------






    /**/


    cl_uint numPlatforms = 0;//����ƽ̨��
    cl_platform_id* platforms = NULL;//��ʼ��ƽ̨id
    // Use cLGetPlatformIDs( ) to retrieve the number of
    // platforms
    status = clGetPlatformIDs(0, NULL, &numPlatforms);//״̬��󶨼���ƽ̨����ȡ����ƽ̨��

    // Allocate erough space for each platform
    platforms = (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));//����ƽ̨�ռ�
    // Fill in platforms with clGetplatformIDs()
    status = clGetPlatformIDs(numPlatforms, platforms, NULL);

    //----------------------------------------------
    // SIEP 2; Discover and initialize the devices���ֳ�ʼ���豸
    //----------------------------------------------

    cl_uint numDevices = 0;
    cl_device_id* devices = NULL;
    // use c1GetleviceIDs() to retrieve the number of
    // devices present
    status = clGetDeviceIDs(//״̬����豸����ȡ���ü����豸
        platforms[2],
        CL_DEVICE_TYPE_CPU,
        0,
        NULL,
        &numDevices);
    // Allocate enough space for each device
    devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));//�����豸�ռ�

    // Fill in devices with clGetDeviceIDs()
    status = clGetDeviceIDs(
        platforms[2],
        CL_DEVICE_TYPE_CPU,
        numDevices,
        devices,
        NULL);

    //size_t max_binary_size;

    // ��ȡ�豸֧�ֵ� cl_program ����������ڴ�����ƴ�С
    //clGetDeviceInfo(devices[0], CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(size_t), &max_binary_size, NULL);

    // �������ռ���ڴ��С
    //size_t total_mem_size = max_binary_size * sizeof(char);
    //printf("Max binary size on device: %zu bytes\n", max_binary_size);

    //cl_device_id device; // OpenCL�豸ID

// ��ѯ�豸���������ƴ�Сopencl1.2
    //size_t max_binary_size;
    //cl_int err = clGetDeviceInfo(devices[0], CL_DEVICE_MAX_PROGRAM_BINARY_SIZE, sizeof(size_t), &max_binary_size, NULL);
    //if (err != CL_SUCCESS) {
        // ������
    //    return err;
    //}

    // ��ӡ�豸���������ƴ�С
    //printf("Max binary size on device: %zu bytes\n", max_binary_size);

    /*
    // ��ȡƽ̨ID
    cl_platform_id platform_id= platforms[1];
    cl_int ret;// = clGetPlatformIDs(1, &platform_id, NULL);
    //if (ret != CL_SUCCESS) {
    //    printf("Error: Failed to get platform ID!\n");
    //    return EXIT_FAILURE;
    //}

    // ��ȡƽ̨����
    char platform_name[128];
    ret = clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, sizeof(platform_name), platform_name, NULL);
    if (ret != CL_SUCCESS) {
        printf("Error: Failed to get platform name!\n");
        return EXIT_FAILURE;
    }
    printf("Platform name: %s\n", platform_name);

    // ��ȡ�豸ID
    cl_device_id device_id;
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (ret != CL_SUCCESS) {
        printf("Error: Failed to get device ID!\n");
        return EXIT_FAILURE;
    }

    // ��ȡ�豸����
    char device_name[128];
    ret = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
    if (ret != CL_SUCCESS) {
        printf("Error: Failed to get device name!\n");
        return EXIT_FAILURE;
    }
    printf("Device name: %s\n", device_name);

    // ��ȡ�豸��Ӧ��
    char device_vendor[128];
    ret = clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, sizeof(device_vendor), device_vendor, NULL);
    if (ret != CL_SUCCESS) {
        printf("Error: Failed to get device vendor!\n");
        return EXIT_FAILURE;
    }
    printf("Device vendor: %s\n", device_vendor);

    // ��ȡ�豸ȫ���ڴ��С
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
    // ��ȡ�豸֧�ֵ�64λ�����������
    clGetDeviceInfo(devices[0], CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, sizeof(cl_ulong), &vector_width_long, NULL);
    if (vector_width_long > 0) {
        printf("Device supports 64-bit integers (vector width: %lu)\n", vector_width_long);
    }
    else {
        printf("Device does not support 64-bit integers\n");
    }

    // ��ȡ�豸֧�ֵ�OpenCL C�汾
    clGetDeviceInfo(devices[0], CL_DEVICE_OPENCL_C_VERSION, sizeof(cl_uint), &opencl_c_version, NULL);
    printf("Device OpenCL C version: %u\n", opencl_c_version);

    // ��ȡ�豸֧�ֵ���չ�б�
    clGetDeviceInfo(devices[0], CL_DEVICE_EXTENSIONS, sizeof(extensions), &extensions, NULL);
    if (strstr(extensions, "cl_khr_int64_base_atomics") || strstr(extensions, "cl_khr_int64_extended_atomics")) {
        printf("Device supports 64-bit integer atomics\n");
    }
    else {
        printf("Device does not support 64-bit integer atomics\n");
    }

    // ��ȡ�豸֧�ֵ�����ڴ�����С
    cl_ulong max_mem_size;
    clGetDeviceInfo(devices[0], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &max_mem_size, NULL);
    printf("Device max memory allocation size: %lu bytes\n", max_mem_size);

    */
    //-------------------------------------------
    // SIEP 3; Create a context����������
    //-------------------------------------------

    cl_context context = NULL;

    // Create a context using clCreateContext() ard
    // associate it with the devices�����������豸
    context = clCreateContext(
        NULL,
        numDevices,
        devices,
        NULL,
        NULL,
        &status);

    //-------------------------------------------
    // SIEP 4; Create a command queue�����������
    //-------------------------------------------

    cl_command_queue cmdQueue;
    // Create a command queue using clCreateCommandQueue(),
    // and associate it with the device you want to execute
    // on����������ӵ�ִ�е��豸��������
    cmdQueue = clCreateCommandQueue(
        context,
        devices[0],
        0,
        &status);

    //--------------------------------------------
    // SIEP 5; Create device buffers����buffer����
    //--------------------------------------------

    

    // Use clCreateBuffer() to create a buffer object (c_A)
    // that will contain the data from the host array A
    //�����洢��������A��ֻ��buffer
    bufferA = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        datasize_a,
        NULL,
        &status);

    // Use clCreateBuffer() to create a buffer object (C_B)
    // that will contain the data from the host array B
    //�����洢��������B��ֻ��buffer
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
    //�����洢�������C��д��buffer
    //bufferC = clCreateBuffer(
    //   context,
    //    CL_MEM_WRITE_ONLY,
    //    datasize,
    //    NULL,
    //    &status);

    //--------------------------------------------
    // SIEP 6; Write host data to device buffers��������д���豸buffer
    //-------------------------------------------

    // Use clEnqueueWriteBuffer() to write input array A to
    // the device buffer bufferA//�������ڴ�д�뻺����
    
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
    // the device buffer bufferB//�������ڴ�д�뻺����
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

    // Create a program using clCreateProgramWithSource()��֮ǰ��kernel����������ɳ���
    cl_program program = clCreateProgramWithSource(
        context,
        1,
        (const char**)&programSource,
        NULL,
        &status);

    // Build (compile) the program for the devices with
    // clBuildProgram( )������ִ���ļ�
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
    // vector addition function (named "vecadd��)
    /*
    cl_int err;

    size_t max_mem_size;
    err = clGetDeviceInfo(devices[0], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(size_t), &max_mem_size, NULL);
    if (err != CL_SUCCESS) {
        printf("Error: Failed to get device info!\n");
        return 1;
    }
    printf("Max kernel size: %llu bytes\n", max_mem_size);

    // ��ȡʵ�ʱ���� cl_program ������ռ�õ��ڴ��С
    size_t binary_sizes[1];
    err = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(binary_sizes), binary_sizes, NULL);
    if (err != CL_SUCCESS) {
        printf("Error: Failed to get program info!\n");
        return 1;
    }
    size_t actual_mem_size = binary_sizes[0];
    printf("My kernel size: %llu bytes\n", actual_mem_size);

    // ���ʵ���ڴ��С�Ƿ񳬹��豸֧�ֵ�����ڴ��С
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
    // using clSetKernelArg( )Ϊ�ں��趨����
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
    status = clEnqueueNDRangeKernel(//���������ݷ�Χ��ִ���ں�
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
    // to the host output array (C)//��bufferC��Ķ���C

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
    // STEP 13; Release OpenCL resources//�ͷ���Դ
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

    //����һ��.txt�ļ���
    char* filename = "En_kernel_cpu.txt";
    //���ú�������ȡ.cl�ļ������Ϊchar*����
    char* programSource = read_txt_file(filename);

    //if (programSource != NULL) {
    //    printf("�ļ�����Ϊ%s:\n", programSource);
    //}

    // Host input vectors������������
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

    // Device input buffers�豸�����buffer����
    cl_mem d_a;
    // Device output buffer�豸�����buffer����
    cl_mem d_b, d_e, d_f, d_d, d_c;

    // Elements in each array����Ԫ�ظ������������ȣ�
    const unsigned int elements = lenInByte;

    // Length of vectors�������������
    unsigned int n = elements;

    size_t datasize_a = sizeof(uint8_t) * ((messageSizeInBit / 8) + 97) * elements;
    size_t datasize_b = sizeof(char) * (messageSizeInBit / 8) * elements;
    size_t datasize_c = sizeof(int) * elements;
    size_t datasize_d = sizeof(EccPoint) * elements;
    size_t datasize_e = sizeof(uint64_t)* 4 * elements;
    size_t datasize_f = sizeof(EccPoint) * elements;

    cl_platform_id cpPlatform[3];        //����ƽ̨ OpenCL platform
    cl_device_id device_id;           // �����豸device ID
    cl_context context;               // ����������context
    cl_command_queue queue;           // �����������command queue
    cl_program program;               // �������program
    cl_kernel kernel, kernel2;                 // ����kernel

    // Allocate memory for each vector on host�����ڴ��ÿ��������������(�洢�ռ��ַ) malloc(������ڴ�ռ��С)
    //h_a = (uint8_t*)malloc(datasize_a);
    //h_b = (char*)malloc(datasize_b);   
    //h_e = (uint64_t*)malloc(datasize_e);

    // Initialize vectors on host�������ϳ�ʼ������
    int i;
    for (i = 0; i < 505; i++)
    {
        h_a[i] = '2';//aȫΪ1
        //h_b[i] = 1;
    }

    //-------------------------------------------------------------

    SM2Encrypt_kernel(h_a, h_b, h_c, h_d, h_e, h_f);


    printf("%s", h_a);



    //--------------------------------------------------------------
   
    size_t globalSize, localSize;//ȫ�ִ�С�����ش�С
    cl_int err;//������

    // Number of total work items - localSize must be devisor
    globalSize = n;//ceil(n/(double)localSize)*localSize;

    // �󶨵�ƽ̨,��ȡ���ü���ƽ̨������ƽ̨��3Bind to platform
    err = clGetPlatformIDs(3, cpPlatform, NULL);
    // ��ȡ���ü����豸����GPU�������豸��1Get ID for the device, change the platform number to get the correct gpu driver.
    err = clGetDeviceIDs(cpPlatform[2], CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);

    // ����������Create a context
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

    // �����������Create a command queue
    queue = clCreateCommandQueue(context, device_id, 0, &err);

    // ��֮ǰ��kernel����������ɳ���Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1,
        (const char**)&programSource, NULL, &err);

    // ������ִ���ļ�Build the program executable
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

    // �����ں˶���Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "SM2Encrypt", &err);
    // kernel2 = clCreateKernel(program, "vecAdd", &err);

    //���������������,kernel�Ի������Ķ�ȡȨ�ޣ���д
     // Create the input and output arrays in device memory for our calculation
    d_a = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_a, NULL, NULL);
    d_b = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_b, NULL, NULL);
    d_c = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_c, NULL, NULL);
    d_d = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_d, NULL, NULL);
    d_e = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_e, NULL, NULL);
    d_f = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_f, NULL, NULL);
    
    //�������ڴ�д�뻺����
    err = clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0,datasize_a, h_a, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, d_b, CL_TRUE, 0, datasize_b, h_b, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, d_c, CL_TRUE, 0, datasize_c, h_c, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, d_d, CL_TRUE, 0, datasize_d, h_d, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, d_e, CL_TRUE, 0, datasize_e, h_e, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, d_f, CL_TRUE, 0,datasize_f, h_f, 0, NULL, NULL);
    
    //err = clEnqueueWriteBuffer(queue, d_e, CL_TRUE, 0,datasize_e, h_e, 0, NULL, NULL);
        //bb = i * n;
        // Set the arguments to our compute kernelΪ�ں��趨����
        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a);
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_b);
        err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_c);
        err = clSetKernelArg(kernel, 3, sizeof(cl_mem), &d_d);
        err = clSetKernelArg(kernel, 4, sizeof(cl_mem), &d_e);
        err = clSetKernelArg(kernel, 5, sizeof(cl_mem), &d_f);


        // ���������ݷ�Χ��ִ���ں�Execute the kernel over the entire range of the data setһά���飬ƫ����Ϊ�գ���Ϊ3
        err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, 0, 0, NULL, NULL);
        clFinish(queue);

    // Read the results from the device�ӻ��������������ڴ�
    clEnqueueReadBuffer(queue, d_a, CL_TRUE, 0,
        datasize_a, h_a, 0, NULL, NULL);
    clFinish(queue);
    
    // release OpenCL resources�ͷ�opencl��Դ
    clReleaseMemObject(d_a);
    clReleaseMemObject(d_b);
    clReleaseMemObject(d_e);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    //release host memory�ͷ�������Դ
    //free(h_a);
    //free(h_b);
    //free(h_e);

}