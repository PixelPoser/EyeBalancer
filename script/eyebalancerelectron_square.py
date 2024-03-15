import cv2
import mediapipe as mp
import numpy as np
import os
import sys
from pathlib import Path

# Check if folder path is provided as a command-line argument
if len(sys.argv) < 2:
    print("Usage: script.py <folder_path>")
    sys.exit(1)
    
folder_path = sys.argv[1]

# Initialize MediaPipe Face Mesh
mp_face_mesh = mp.solutions.face_mesh
face_mesh = mp_face_mesh.FaceMesh(static_image_mode=True, max_num_faces=1, refine_landmarks=True)

# Verify the path and list all supported image files
if not os.path.exists(folder_path):
    print("The specified folder does not exist.")
else:
    # Create an "Outputs" folder within the source folder
    output_folder_path = os.path.join(folder_path, "Outputs")
    os.makedirs(output_folder_path, exist_ok=True)

    # List all supported image files in the folder
    image_files = [f for f in os.listdir(folder_path) if f.lower().endswith(('.jpg', '.jpeg', '.png', '.cr3', '.arw', '.nef'))]

    # Process each image
    for image_file in image_files:
        image_path = os.path.join(folder_path, image_file)
        image = cv2.imread(image_path)

        # Check if the image was successfully loaded
        if image is None:
            print(f"Failed to load image {image_file}. Please check the file path.")
            continue

        image_height, image_width, _ = image.shape

        # Process the image to get facial landmarks
        results = face_mesh.process(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))

        # Define the landmark indices
        LEFT_EYE_INDEX = 130  # Index for the left eye
        RIGHT_EYE_INDEX = 359  # Index for the right eye
        CHIN_INDEX = 152  # Index for the bottom center of the chin

        # Check if at least one face was detected
        if results.multi_face_landmarks:
            landmarks = results.multi_face_landmarks[0].landmark

            # Extract landmark coordinates and convert to image coordinates
            left_eye = np.array([landmarks[LEFT_EYE_INDEX].x * image_width, landmarks[LEFT_EYE_INDEX].y * image_height]).astype(int)
            right_eye = np.array([landmarks[RIGHT_EYE_INDEX].x * image_width, landmarks[RIGHT_EYE_INDEX].y * image_height]).astype(int)
            chin = np.array([landmarks[CHIN_INDEX].x * image_width, landmarks[CHIN_INDEX].y * image_height]).astype(int)

            # Calculate the angle to align the pupils horizontally
            delta_x = right_eye[0] - left_eye[0]
            delta_y = right_eye[1] - left_eye[1]
            angle = np.degrees(np.arctan2(delta_y, delta_x))

            # Rotate the image to align the pupils horizontally
            center = tuple(np.array([left_eye, right_eye]).mean(axis=0).astype(int))
            center = (int(center[0]), int(center[1]))
            M = cv2.getRotationMatrix2D(center, angle, 1)
            rotated_image = cv2.warpAffine(image, M, (image_width, image_height))

            # Recalculate landmark positions on the rotated image
            ones = np.ones(shape=(len(landmarks), 1))
            points = np.array([[landmark.x * image_width, landmark.y * image_height] for landmark in landmarks])
            points = np.concatenate([points, ones], axis=1)
            new_points = M.dot(points.T).T

            # Extract the new positions for the left eye, right eye, and chin
            new_left_eye = new_points[LEFT_EYE_INDEX][:2].astype(int)
            new_right_eye = new_points[RIGHT_EYE_INDEX][:2].astype(int)
            new_chin = new_points[CHIN_INDEX][:2].astype(int)

            # Calculate the square crop dimensions centered around the face
            eye_center_x = (new_left_eye[0] + new_right_eye[0]) // 2
            eye_center_y = (new_left_eye[1] + new_right_eye[1]) // 2
            crop_size = max(new_chin[1] - eye_center_y, eye_center_x - new_left_eye[0], new_right_eye[0] - eye_center_x)
            crop_size = int(crop_size * 1.2)  # Add some padding around the face

            # Calculate the crop coordinates
            crop_x1 = max(0, eye_center_x - crop_size)
            crop_y1 = max(0, eye_center_y - crop_size)
            crop_x2 = min(image_width, crop_x1 + (2 * crop_size))
            crop_y2 = min(image_height, crop_y1 + (2 * crop_size))

            # Crop the high-resolution image
            cropped_image = rotated_image[crop_y1:crop_y2, crop_x1:crop_x2]

            # Save the final image in the "Outputs" folder without resizing
            output_image_path = os.path.join(output_folder_path, Path(image_file).stem + "_SquareCrop.jpg")
            cv2.imwrite(output_image_path, cropped_image)
        else:
            print(f"No face detected in {image_file}.")

    # Release resources
    face_mesh.close()