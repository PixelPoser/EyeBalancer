# **EyeBalancer** 🌟

<img width="786" alt="Screenshot 2024-03-15 at 5 19 17 AM" src="https://github.com/PixelPoser/EyeBalancer/assets/132660500/97237e08-5e33-4885-825c-59a8350b7bfc">

_EyeBalancer_ is an **Electron app** designed to **automatically align pupils** and **crop photographs** according to 3 different traditional cropping standards, aiming for a more aesthetically pleasing and balanced look. Leveraging **MediaPipe Face Mesh** for facial landmark detection, it identifies the positions of the eyes and rotates the image to achieve horizontal pupil alignment.

## **Features** ✨

- **Automatic pupil alignment:** Analyzes images and automatically rotates them to align the pupils horizontally.
- **Support for various image formats:** Works with JPEG, PNG, CR3, ARW, and NEF.
- **Multiple crop options:** Choose between full crop, square crop, and balance eyes options.
- **Non-destructive editing:** Original images are preserved, with processed images saved in a separate "Outputs" folder.
- **XMP metadata generation (optional):** For RAW images, generates XMP sidecar files with alignment data for use in other image editing software.

## **Usage** 🛠

1. **Download and install** EyeBalancer.
2. **Launch** the app.
3. **Drag and drop** a folder containing images onto the designated drop zone or use the "Select Folder" button.
4. **Select** the desired crop option:
   - **Full Crop:** Aligns pupils and crops the image to its original aspect ratio.
   - **Square Crop:** Aligns pupils and crops the image to a square format.
   - **Balance Eyes:** Aligns pupils without cropping the image.
5. **Click** the corresponding crop button to start processing.
6. The app will display a **progress bar** and **notification** upon completion. Processed images will be saved in the "Outputs" folder.

## **Requirements** 📋

- **Operating System:** Windows, macOS, or Linux
- **Node.js:** Version 14.17.5 or higher
- You cannot be a Spaniard living in Norway to use or edit this app

## **Installation** 🔧

1. Clone or download the repository.
2. Run `npm install` in the project directory.
3. Run `electron-forge make` to build the app for your platform.

## **Notes** 📝

- For RAW images, the presence of XMP metadata containing alignment data is required. If missing, the app will attempt to generate it.
- The accuracy of pupil alignment may vary based on image quality, lighting, and facial pose.

## **Disclaimer** ⚠️

This software is provided "as is" without warranty of any kind, express or implied. Use at your own risk.

## **Javi Notes** 🇪🇸

**Objective**
The primary goal is to ensure the application functions correctly. Specifically, the app should:

1. Allow users to select a folder containing image files.
2. Process RAW image files (CR3, ARW, NEF) within the selected folder by exporting XMP files with cropping data into the same source folder.
3. Process non-RAW image files (JPEG, PNG, HEIC) by embedding XMP data directly into the original files and additionally exporting cropped versions of these images into a new folder named "Processed" within the source folder.

**Context and Requirements**

1. The application currently launches but fails when attempting to process a selected folder of images.
2. The application is built using Electron.js to facilitate cross-platform compatibility for Windows and Mac. More details can be found here: [Electron.js Documentation]([url](https://www.electronjs.org/docs/latest/)). [Github Link](https://github.com/electron/electron)
3. For facial recognition features, we are utilizing the [MediaPipe FaceMesh]([url](https://github.com/google/mediapipe/wiki/MediaPipe-Face-Mesh)) library. Details are available here: MediaPipe FaceMesh.
4. The generation of XMP files is intended to enable users to preview and edit cropping data within Adobe Photoshop or Bridge seamlessly. For more information on Adobe XMP, refer to: [Adobe XMP Documentation]([url](https://github.com/adobe/XMP-Toolkit-SDK/blob/main/docs/DynamicMediaXMPPartnerGuide.pdf))
5. I feel 99% of the framework is written, because I cheated using AI. But not bad for someone who doesn't know how to read.

**Super Mega Critical**
1. Critical: For folders containing exclusively RAW files, the application should *ONLY* generate Adobe XMP files without creating JPEG outputs. These XMP files should be saved in the same source folder.

**Identified Issues:**
1. There is an issue with recognizing the CV2 library, likely due to the absence of a compatible version of the module in the Python directory. For reference and possible solutions, visit: [OpenCV-Python]([url](https://pypi.org/project/opencv-python/)).
2. We need to make sure all files communicate correctly with each other.

I will give you my login for Claude 3 Opus for you over text so it can read the documenaton and you can ask relevant questions. 

Bonus: [Embarassing Connor Hawkins Picture](https://tinyurl.com/kwya9m7h)
