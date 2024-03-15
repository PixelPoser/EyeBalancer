import os
import sys
from PIL import Image
from PIL.ExifTags import TAGS
from xml.etree import ElementTree as ET

def get_image_orientation(image_path):
    try:
        with Image.open(image_path) as img:
            exif_data = img._getexif()
            if exif_data:
                for tag_id, value in exif_data.items():
                    tag_name = TAGS.get(tag_id, tag_id)
                    if tag_name == 'Orientation':
                        return value
    except (AttributeError, KeyError, IndexError):
        pass
    return None

def generate_xmp_metadata(image_path, orientation):
    xmp_data = ET.Element('x:xmpmeta')
    xmp_data.set('xmlns:x', 'adobe:ns:meta/')

    rdf_rdf = ET.SubElement(xmp_data, 'rdf:RDF')
    rdf_rdf.set('xmlns:rdf', 'http://www.w3.org/1999/02/22-rdf-syntax-ns#')

    rdf_description = ET.SubElement(rdf_rdf, 'rdf:Description')
    rdf_description.set('rdf:about', '')

    tiff_orientation = ET.SubElement(rdf_description, 'tiff:Orientation')
    tiff_orientation.text = str(orientation)

    return ET.tostring(xmp_data, encoding='utf-8')

def process_file(file_path, output_xmp):
    file_name, file_extension = os.path.splitext(file_path)
    file_extension = file_extension.lower()

    if file_extension in ['.jpg', '.jpeg', '.png', '.heic']:
        orientation = get_image_orientation(file_path)
        if orientation:
            if output_xmp and file_extension in ['.jpg', '.jpeg']:
                xmp_metadata = generate_xmp_metadata(file_path, orientation)
                xmp_file_path = file_name + '.xmp'
                with open(xmp_file_path, 'wb') as xmp_file:
                    xmp_file.write(xmp_metadata)

    elif file_extension in ['.cr2', '.cr3', '.nef', '.arw', '.rw2', '.raf']:
        xmp_file_path = file_name + '.xmp'
        orientation = get_image_orientation(file_path)
        if orientation:
            xmp_metadata = generate_xmp_metadata(file_path, orientation)
            with open(xmp_file_path, 'wb') as xmp_file:
                xmp_file.write(xmp_metadata)

def process_folder(folder_path, output_xmp_for_jpegs):
    total_files = sum(len(files) for _, _, files in os.walk(folder_path))
    processed_files = 0

    for root, dirs, files in os.walk(folder_path):
        for file_name in files:
            file_path = os.path.join(root, file_name)
            process_file(file_path, output_xmp_for_jpegs)
            processed_files += 1
            progress = int((processed_files / total_files) * 100)
            print(progress)
            sys.stdout.flush()

if __name__ == '__main__':
    folder_path = sys.argv[1]
    output_xmp_for_jpegs = sys.argv[2].lower() == 'true'
    process_folder(folder_path, output_xmp_for_jpegs)