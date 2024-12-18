Real time GCN-UNET agreement demo
==================================

This repository is the source code for the real-time demo of
[Towards Robust Cardiac Segmentation using Graph Convolutional Networks](https://arxiv.org/abs/2310.01210).
We use the GCN with displacement method and nnU-Net described in this article.
For the code of the framework to train the model, see https://github.com/gillesvntnu/GCN_multistructure.git.

![real_time_demo.gif](./figures/real_time_demo.gif)

The GCN and nnU-Net segmentations are shown on the left and right side respectively.
The color-coded status bar on top visualizes the agreement between the models.
The full demo video is availabel at https://doi.org/10.6084/m9.figshare.24230194.

Setup
----------------------------------
The code provides a stand-alone C++ application using [FAST](https://github.com/smistad/FAST).
You need git, cmake and a C++ compiler (e.g. gcc on Linux, visual studio on Windows) to compile this application.

1. Either
   - [Install a release of FAST](https://fast.eriksmistad.no/install.html) **OR**
   - Compile and install FAST on your system: See instructions here for [Windows](https://fast.eriksmistad.no/building-on-windows.html) or [Linux (Ubuntu)](https://fast.eriksmistad.no/install-ubuntu-linux.html).
   - Depending on your setup, it might be necessary to install OpenGL to run streaming.
2. Clone this repository
   ```bash
   git clone https://github.com/gillesvntnu/GCN_UNET_agreement_demo.git
   ```
3. Download trained models from huggingface:

   The models directory should contain trained .onnx models for the default configuration to work.
Models trained on CAMUS can be downloaded at:

- https://huggingface.co/gillesvdv/nnunet_camus_cv1_onnx
and
- https://huggingface.co/gillesvdv/GCN_with_displacement_camus_cv1_onnx
4. Setup build environment using CMake  
   *Linux (Ubuntu)*
   ```bash
   mkdir build
   cd build
   cmake .. -DFAST_DIR=/opt/fast/cmake/
   ``` 
   *Windows (Visual Studio)*
   Modify generator -G string to match your Visual studio version. This command will create a visual studio solution in your build folder.
   ```bash
   mkdir build
   cd build
   cmake .. -DFAST_DIR="C:/Program Files/FAST/fast/cmake/" -G "Visual Studio 16 2019" -A x64
   ```
5. Build
   ```bash
   cmake --build . --config Release --target real_time_demo
   ```
6. Run

   *Linux (Ubuntu)*
   ```bash
   ./real_time_demo
   ```
   *Windows*
   ```powershell
   .\Release\real_time_demo.exe
   ```
7. (Optional) Run with specific parameters

    *Linux (Ubuntu)*
    ```bash
    ./real_time_demo --mhd_file_location path/to/mhd/file_#.mhd --gcn_model_loc path/to/gcn/model.onnx --unet_model_loc path/to/unet/model.onnx
    ```
    *Windows*
    ```powershell
    .\Release\real_time_demo.exe --mhd_file_location path/to/mhd/file_#.mhd --gcn_model_loc path/to/gcn/model.onnx --unet_model_loc path/to/unet/model.onnx
    ```

## Contact

Developer: <br />
[https://gillesvandevyver.com/](https://gillesvandevyver.com/)

Management: <br />
lasse.lovstakken@ntnu.no <br />
erik.smistad@ntnu.no <br />
