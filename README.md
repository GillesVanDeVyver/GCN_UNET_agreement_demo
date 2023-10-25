Real time GCN-UNET agreement demo
==================================

This repository is the source code for the real-time demo of
[Towards Robust Cardiac Segmentation using Graph Convolutional Networks](https://arxiv.org/abs/1911.04429)


Setup
----------------------------------
The code provides a stand-alone C++ application using [FAST](https://github.com/smistad/FAST).
You need git, cmake and a C++ compiler (e.g. gcc on Linux, visual studio on Windows) to compile this application.

1. Either
   - [Install a release of FAST](https://fast.eriksmistad.no/install.html) **OR**
   - Compile and install FAST on your system: See instructions here for [Windows](https://fast.eriksmistad.no/building-on-windows.html) or [Linux (Ubuntu)](https://fast.eriksmistad.no/install-ubuntu-linux.html).
2. Clone this repository
   ```bash
   git clone https://github.com/gillesvntnu/GCN_UNET_agreement_demo.git
   ```
3. Setup build environment using CMake  
   *Linux (Ubuntu)*
   ```bash
   mkdir build
   cd build
   cmake .. -DFAST_DIR=/path/to/FAST/cmake/
   ``` 
   *Windows (Visual Studio)*
   Modify generator -G string to match your Visual studio version. This command will create a visual studio solution in your build folder.
   ```bash
   mkdir build
   cd build
   cmake .. -DFAST_DIR=C:\path\to\FAST\cmake\ -G "Visual Studio 16 2019" -A x64
   ```
4. Build
   ```bash
   cmake --build . --config Release --target real_time_demo
   ```
5. Run

   *Linux (Ubuntu)*
   ```bash
   ./real_time_demo
   ```
   *Windows*
   ```powershell
   cd Release
   fastExample.exe
   ```
6. (Optional) Run with specific parameters

    *Linux (Ubuntu)*
    ```bash
    ./real_time_demo --mhd_file_location path/to/mhd/file_#.mhd --gcn_model_loc path/to/gcn/model.onnx --unet_model_loc path/to/unet/model.onnx
    ```
    *Windows*
    ```powershell
    cd Release
    fastExample.exe --mhd_file_location path/to/mhd/file_#.mhd --gcn_model_loc path/to/gcn/model.onnx --unet_model_loc path/to/unet/model.onnx
    ```