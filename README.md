# **EyeBalancer** 🌟

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

Goal: I want the app to work. Select your folder of images and have it export XMP files with cropping data, or embed the XMP in original non-raw files as well as output cropped versions in a new folder inside the source folder.

Context:
1. App isn't fucking working. It runs but gives an error when I try to process a folder of images.
2. App is using Electron.js so I can export it for Windows and Mac.  Its like a universal thing. You probably know about it. https://www.electronjs.org/docs/latest/
3. Facial Recognition software is Mediapipe FaceMesh: https://github.com/google/mediapipe/wiki/MediaPipe-Face-Mesh
4. XMP files are so you can open a folder of images in Adobe Photoshop or Bridge, and it will show previews of the crop, but you can edit it after the fact.
5. The Adobe XMP information is here: https://github.com/adobe/XMP-Toolkit-SDK/blob/main/docs/DynamicMediaXMPPartnerGuide.pdf
6. I feel 99% of the framework is written, because I cheated using AI. But not bad for someone who doesn't know how to read.
7. CRITICAL: IF FOLDER HAS RAW FILES ONLY: Ensure the app outputs Adobe XMPs when processing RAW files. No JPEG outputs then. Just the XMP generation in the same source folder
8. CRITICAL: IF FOLDER HAS NON-RAW FILES (JPGS etc): Ensure app embeds XMP data in the original source non raw files JPEGS, JPGs, and PNGs, as WELL as output new cropped versions in a new folder created called "Processed" thats inside source folder.

Known Problems:
1. CV2 isn't being recognized, which I suspect is because the Python folder doesn't contain a compatible module version of it. https://pypi.org/project/opencv-python/
2. I am not good enough to know if all the app files are perfectly communicating with each other.


