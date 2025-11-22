# 🚗🔍 License Plate Detection & OCR System (C++ · OpenCV · Tesseract)

This project implements an image-processing pipeline for **automatic license plate detection and recognition** using **C++**, **OpenCV**, and **Tesseract OCR**.  
It demonstrates key computer-vision techniques such as **image preprocessing**, **edge detection**, **contour analysis**, and **optical character recognition**.

The system is designed for detecting vehicle license plates—tested primarily on Romanian license plates—and extracting text with a simple and functional pipeline.

---

## 🚀 Features

- 🖼️ **Grayscale Conversion** – Custom implementation of RGB→Gray  
- 🌫️ **Gaussian Noise Reduction** – Manually implemented convolution filter  
- ✂️ **Edge Detection (Canny)** – Highlights potential plate regions  
- 🧩 **Contour Detection & Sorting** – Identifies the largest quadrilateral contour  
- 🔳 **Plate Extraction** – Masking and cropping of detected license plate  
- 🔠 **OCR with Tesseract** – Recognizes alphanumeric characters  
- 📤 **Simple File Picker** – Select images via Windows dialog  
- 🪟 **Windows GUI Preview** – Shows intermediate and final results  

---

## 🧰 Technologies Used

- **C++ (MSVC)** – Core implementation  
- **OpenCV 4.x** – Image processing and computer vision  
- **Tesseract OCR** – Optical character recognition engine  
- **Leptonica** – Image handling library for Tesseract  
- **Windows API (Win32)** – File dialogs and system utilities  
- **Visual Studio 2019/2022** – Recommended IDE  

---

## 🛠️ How the System Works

The application follows a clear image-processing pipeline:

### **1. Preprocessing**
- Convert image to grayscale  
- Apply custom Gaussian blur  
- Show noise-reduced result

### **2. License Plate Detection**
- Run Canny edge detection  
- Find contours  
- Sort by area  
- Choose the first quadrilateral as the license plate border  

### **3. Plate Extraction**
- Create a mask using contour  
- Copy the plate region  
- Crop to bounding rectangle  

### **4. OCR Recognition**
- Convert plate image to Leptonica `Pix`  
- Let Tesseract process the image  
- Extract the recognized text  
- Print result to console

---

## 📂 Project Structure

| File / Folder                   | Description |
|--------------------------------|-------------|
| **common.cpp**                 | Utility functions (file dialogs, image resizing) |
| **common.h**                   | Header definitions for utility functions |
| **license_plate_detector.cpp** | Main application logic (preprocessing, detection, OCR, UI) |
| **stdafx.cpp**                 | Precompiled header source (Visual Studio) |
| **stdafx.h**                   | Precompiled header include file |

_A `.gitignore` should be used to exclude build files, `.vs/`, and binaries._

---

## ▶️ How to Build & Run the Project

### 🔧 **Requirements**
Install:

- **OpenCV 4.x** (Windows prebuilt package)  
- **Tesseract OCR for Windows**  
- **Visual Studio 2019/2022** (MSVC compiler)

Configure:

- OpenCV include + lib directories  
- Tesseract include + lib directories  
- Link against:  
  - `opencv_world4xx.lib`  
  - `tesseract.lib`  
  - `leptonica.lib`

---

### ▶️ Running the Application

1. Build the project in Visual Studio  
2. Run the executable  
3. Use the Windows file picker to choose an image  
4. The program will:
   - preprocess the image  
   - detect the plate  
   - extract it  
   - run OCR  
5. The detected plate text is printed in the console  
6. Intermediate images appear in OpenCV windows  

---

## 🧠 Key Concepts Demonstrated

- **Custom Gaussian filtering**  
- **Manual grayscale conversion**  
- **Canny edge detection**  
- **Contour approximation**  
- **Region masking and cropping**  
- **OCR via Tesseract API**  
- **Windows-native file dialogs**  
- **Modular C++ structure**

---

## 📝 Conclusion

The **License Plate Detection & OCR System** is a complete computer-vision project showing real-world techniques for extracting structured information from images.

It demonstrates:

- practical C++ development  
- OpenCV computer vision skills  
- OCR integration  
- algorithmic design  
- clean project architecture  

Potential improvements:

- ML-based plate localization  
- Better preprocessing for sharper OCR  
- Batch mode or video input  
- Cross-platform support  
- Real-time plate recognition  

---

This project was developed in collaboration with Boamba Ovidiu, as part of our studies at the Technical University of Cluj-Napoca (UTCN) 🎓.
