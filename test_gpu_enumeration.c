#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

int main() {
    cl_int ret;
    cl_uint num_platforms;

    // Get number of platforms
    ret = clGetPlatformIDs(0, NULL, &num_platforms);
    if (ret != CL_SUCCESS) {
        printf("Error getting number of platforms: %d\n", ret);
        return 1;
    }

    printf("Available platforms: %u\n", num_platforms);

    if (num_platforms == 0) {
        printf("No OpenCL platforms found.\n");
        return 1;
    }

    // Allocate space for platforms
    cl_platform_id *platforms = malloc(num_platforms * sizeof(cl_platform_id));
    if (!platforms) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    ret = clGetPlatformIDs(num_platforms, platforms, NULL);
    if (ret != CL_SUCCESS) {
        printf("Error getting platforms: %d\n", ret);
        free(platforms);
        return 1;
    }

    // Enumerate all GPU devices from all platforms
    cl_device_id *all_devices = NULL;
    cl_platform_id *device_platforms = NULL;
    cl_uint total_devices = 0;

    for (cl_uint p = 0; p < num_platforms; p++) {
        cl_uint platform_devices = 0;
        char platform_name[1024];

        // Get platform name
        clGetPlatformInfo(platforms[p], CL_PLATFORM_NAME, sizeof(platform_name), platform_name, NULL);
        printf("Platform %d: %s\n", p, platform_name);

        // Get number of GPU devices for this platform
        ret = clGetDeviceIDs(platforms[p], CL_DEVICE_TYPE_GPU, 0, NULL, &platform_devices);
        if (ret != CL_SUCCESS) {
            if (ret == CL_DEVICE_NOT_FOUND) {
                printf("  No GPU devices found on this platform.\n");
                continue;
            }
            printf("Error getting device count for platform %d: %d\n", p, ret);
            continue;
        }

        if (platform_devices > 0) {
            printf("  Found %u GPU device(s)\n", platform_devices);

            // Allocate/reallocate memory for all devices
            all_devices = realloc(all_devices, (total_devices + platform_devices) * sizeof(cl_device_id));
            device_platforms = realloc(device_platforms, (total_devices + platform_devices) * sizeof(cl_platform_id));

            if (!all_devices || !device_platforms) {
                printf("Memory allocation failed.\n");
                free(platforms);
                free(all_devices);
                free(device_platforms);
                return 1;
            }

            // Get actual device IDs
            ret = clGetDeviceIDs(platforms[p], CL_DEVICE_TYPE_GPU, platform_devices,
                               &all_devices[total_devices], NULL);
            if (ret != CL_SUCCESS) {
                printf("Error getting devices for platform %d: %d\n", p, ret);
                continue;
            }

            // Store platform for each device
            for (cl_uint d = 0; d < platform_devices; d++) {
                device_platforms[total_devices + d] = platforms[p];
            }

            total_devices += platform_devices;
        }
    }

    if (total_devices == 0) {
        printf("No GPU devices found on any platform.\n");
        free(platforms);
        free(all_devices);
        free(device_platforms);
        return 1;
    }

    printf("\nSelect a GPU to use (this is a demo - enter the number):\n");
    for (cl_uint i = 0; i < total_devices; i++) {
        char platform_name[1024];
        char device_name[1024];

        clGetPlatformInfo(device_platforms[i], CL_PLATFORM_NAME, sizeof(platform_name), platform_name, NULL);
        clGetDeviceInfo(all_devices[i], CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
        printf("%d: [%s] %s\n", i, platform_name, device_name);
    }

    printf("\nDemo completed successfully! Found %u GPU devices across %u platforms.\n", total_devices, num_platforms);

    free(platforms);
    free(all_devices);
    free(device_platforms);
    return 0;
}
