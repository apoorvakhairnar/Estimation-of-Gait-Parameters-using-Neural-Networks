# Estimation of Gait Parameters using Neural Networks

This repository contains experiments for estimating gait timing parameters from dual-IMU data using recurrent neural networks (RNN/GRU).  
It includes data collection firmware, model training/evaluation notebooks, and plotting notebooks.

This project was conducted as part of the **Machine Learning for Mechanical Engineers** course at **Virginia Tech (Spring 2025)**.

Project report can be found here: https://www.researchgate.net/

## Features

- Dual-sensor IMU data pipeline (thigh + shin signals)
- Toe-off event detection from quaternion-derived pitch signals
- Sliding-window sequence dataset creation for time-to-toe-off regression
- Baseline and improved recurrent models (SimpleRNN, GRU)
- Hyperparameter search, k-fold validation, and PCA-based feature selection workflows
- Visualization notebooks for model performance and dataset analysis

## Repository Structure

```text
.
├── InitialTrainingRNNGRU.ipynb
├── HyperparameterGridSearch.ipynb
├── KFoldValidation.ipynb
├── FeatureSelection_PCA.ipynb
├── FinalEvaluationOfGRU.ipynb
├── ML_proj_data_record.ino
├── Project/
│   ├── imu_data_fastest_speed.csv
│   ├── imu_data_medium_speed.csv
│   └── imu_data_slowest_speed.csv
└── Plots/
    ├── FeatureSelection_PCA_plots.ipynb
    ├── GridSearchPlot.ipynb
    ├── KFoldValidationPlots.ipynb
    └── FinalPlot.ipynb
```

## Getting Started

### 1) Clone the repository

```bash
git clone https://github.com/apoorvakhairnar/Estimation-of-Gait-Parameters-using-Neural-Networks.git
cd Estimation-of-Gait-Parameters-using-Neural-Networks
```

### 2) Create a Python environment

```bash
python -m venv .venv
source .venv/bin/activate  # Windows: .venv\Scripts\activate
```

### 3) Install dependencies

```bash
pip install numpy pandas scipy scikit-learn matplotlib tensorflow jupyter
```

### 4) Launch notebooks

```bash
jupyter notebook
```

Open and run the notebooks in order:

1. `InitialTrainingRNNGRU.ipynb`
2. `HyperparameterGridSearch.ipynb`
3. `KFoldValidation.ipynb`
4. `FeatureSelection_PCA.ipynb`
5. `FinalEvaluationOfGRU.ipynb`
6. Plot notebooks in `Plots/`

## Usage

- Input data: CSV files in `Project/`
- Core output: regression estimates of gait timing (time-to-toe-off)
- Model comparisons and final evaluation are documented in notebook outputs and plots

## IMU Data Collection (Firmware)

`ML_proj_data_record.ino` contains Arduino/ESP-style firmware to log synchronized thigh and shin IMU streams to SD card.

Libraries referenced by the sketch include:

- `Adafruit_BNO08x`
- `SparkFun_BNO080_Arduino_Library`
- `SD`, `Wire`, and related sensor interfaces

## Contributors

Contributors are listed in alphabetical order (by last name):

1. Apoorva Khairnar
2. Prashant Kumar
3. Aksh Rajput

## Contributing

This was a course project, so external contributions are not being accepted.
