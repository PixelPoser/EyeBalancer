EyeBalancer
EyeBalancer is an Electron app designed to automatically align pupils in photographs for a more aesthetically pleasing and balanced look. It utilizes facial landmark detection powered by MediaPipe Face Mesh to identify the positions of the eyes and then rotates the image to achieve horizontal pupil alignment.

Features
Automatic pupil alignment: EyeBalancer analyzes images and automatically rotates them to align the pupils horizontally.
Support for various image formats: The app supports common image formats like JPEG, PNG, CR3, ARW, and NEF.
Multiple crop options: Users can choose between full crop, square crop, and balance eyes options to achieve the desired output.
Non-destructive editing: The original images are preserved, and the processed images are saved in a separate "Outputs" folder.
XMP metadata generation (optional): For RAW images, EyeBalancer can generate XMP sidecar files containing the alignment data, which can be used by other image editing software.
Usage
Download and install EyeBalancer.
Launch the app.
Drag and drop a folder containing images onto the designated drop zone. Alternatively, use the "Select Folder" button to choose a folder.
Select the desired crop option:
Full Crop: Aligns pupils and crops the image to its original aspect ratio.
Square Crop: Aligns pupils and crops the image to a square format.
Balance Eyes: Aligns pupils without cropping the image.
Click the corresponding crop button to start processing.
The app will display a progress bar and notification upon completion. Processed images will be saved in the "Outputs" folder within the source folder.
Requirements
Operating System: Windows, macOS, or Linux
Node.js: Version 14.17.5 or higher
Installation
Clone or download the repository.
Run npm install in the project directory.
Run electron-forge make to build the app for your platform.
Notes
For RAW images, EyeBalancer requires the presence of XMP metadata containing alignment data. If this data is missing, the app will attempt to generate it.
The accuracy of pupil alignment might vary depending on factors like image quality, lighting, and facial pose.
Disclaimer
This software is provided "as is" without warranty of any kind, express or implied. Use at your own risk.
