# **EyeBalancer** 🌟

![DALL·E 2024-03-15 05 26 59 - Design a clean, professional logo for 'EyeBalancer' that embodies a serious tone  The logo should be fairly simple and minimalistic, incorporating a r@1x_1-1](https://github.com/PixelPoser/EyeBalancer/assets/132660500/328cc40c-86df-4319-90c6-75559945fc29)



![Screenshot 2024-03-15 at 5 19 17 AM](https://github.com/PixelPoser/EyeBalancer/assets/132660500/c1db84a9-6dcf-4a7a-b10b-2bb641e744e7)





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
2. The application is built using Electron.js to facilitate cross-platform compatibility for Windows and Mac. More details can be found here: [Electron.js Documentation](https://www.electronjs.org/docs/latest/). [Github Link](https://github.com/electron/electron)
3. For facial recognition features, we are utilizing the [MediaPipe FaceMesh](https://github.com/google/mediapipe/wiki/MediaPipe-Face-Mesh) library. Details are available here: MediaPipe FaceMesh.
4. The generation of XMP files is intended to enable users to preview and edit cropping data within Adobe Photoshop or Bridge seamlessly. For more information on Adobe XMP, refer to: [Adobe XMP Documentation](https://github.com/adobe/XMP-Toolkit-SDK/blob/main/docs/DynamicMediaXMPPartnerGuide.pdf)
5. I feel 99% of the framework is written, because I cheated using AI. But not bad for someone who doesn't know how to read.

**Super Mega Critical**
1. Critical: For folders containing exclusively RAW files, the application should *ONLY* generate Adobe XMP files without creating JPEG outputs. These XMP files should be saved in the same source folder.

## **Identified Issues:**
1. There is an issue with recognizing the CV2 library, likely due to the absence of a compatible version of the module in the Python directory. For reference and possible solutions, visit: [OpenCV-Python](https://pypi.org/project/opencv-python/).
2. We need to make sure all files communicate correctly with each other.

## **What I need you to do**
1. Ensure the app runs
2. To do so, make sure upon pressing one of the crop buttons after folder selection, it correctly activates the corresponding eyebalancerelectron python script and outputs the image files.
3. XMP files must be generated when processing RAW files. 
4. XMP embeding must happen to the original JPEG files as well as the cropped JPEG output.
5. to make the XMPs work, we must have the cropping data be correctly be transferred to the XMP file, ensure the cropping data is correctly translated from the Mediapipe to the XMPs.


**File Explanation**

*main.js:*

This is the main script for the Electron app. It handles the following tasks:

Creating and managing the main application window.
Loading the HTML file for the user interface.
Setting up communication channels between the main process and the renderer process (UI).
Handling events from the UI, such as selecting a folder and initiating image processing.
Executing the Python script for image processing based on user-selected options.
Displaying progress updates and notifications during processing.

*index.html:*

This file contains the HTML code for the user interface of the EyeBalancer app. It defines the layout, elements, and styling of the UI.

*renderer.js:*

This script runs in the renderer process (UI) and handles interactions within the user interface. It is responsible for:

Responding to user actions like clicking buttons and dragging/dropping folders.
Sending messages to the main process through the established communication channels.
Receiving messages from the main process, such as the selected folder path, processing progress updates, and completion/error notifications.
Updating the UI elements based on the received messages.

*package.json:*

This file contains metadata about the project, including its name, version, dependencies, and scripts. It also includes configuration options for Electron Forge, which is used to build and package the app.

*forge.config.js:*

This file provides specific configuration options for Electron Forge. It defines the packaging and building settings for the app, including whether to use ASAR (a file format for packaging Electron apps), the target platforms, and any additional resources to be included in the package.

*scripts/eyebalancerelectron_fullcrop.py:*

This Python script performs the actual image processing for the "Full Crop" option. It uses MediaPipe Face Mesh to detect facial landmarks, calculates the rotation angle to align pupils horizontally, rotates the image, and then crops it to its original aspect ratio.

*scripts/eyebalancerelectron_squarecrop.py:*

This script is similar to eyebalancerelectron_fullcrop.py but crops the image to a square format after aligning the pupils.

*scripts/eyebalancerelectron_balanceeyes.py:*

This script focuses solely on aligning the pupils horizontally without performing any cropping.

*XMPGenerator.h/.cpp:*

These files define a class called XMPGenerator that is responsible for generating and managing XMP metadata related to image alignment. This metadata can be embedded within JPEG files or saved as separate XMP sidecar files for RAW images.

*requirements.txt:*

This file lists the required Python packages and their versions for the image processing scripts to function.


---

I will give you my login for Claude 3 Opus for you over text so it can read the documenaton and you can ask relevant questions. 

Bonus: [Embarassing Connor Hawkins Picture](https://tinyurl.com/kwya9m7h)
