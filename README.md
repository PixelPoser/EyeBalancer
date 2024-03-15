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
