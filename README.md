# Estimation of Gait Parameters using Neural Networks

Estimating gait timing parameters (specifically, time-to-toe-off) from dual-IMU
wearable sensor data using recurrent neural networks (SimpleRNN and GRU). The
goal is to detect gait events from thigh- and shank-mounted IMUs accurately
enough, and with low enough latency, to eventually drive closed-loop control
of a wearable robot (e.g. an exoskeleton).

This project was conducted as part of the **Machine Learning for Mechanical
Engineers** course at **Virginia Tech (Spring 2025)**, by a three-person team.

Project report: [ResearchGate — Estimating Gait Parameters for Wearable Robots using Machine Learning Techniques](https://www.researchgate.net/publication/409633770_Estimating_Gait_Parameters_for_Wearable_Robots_using_Machine_Learning_Techniques)

## Overview

Two dual-BNO080-IMU sensors (one on the thigh, one on the shin) stream
quaternion orientation, linear acceleration, and angular velocity at 50 Hz.
Toe-off events are detected from the thigh/shin pitch signals (derived from
the quaternions), and each sample is labeled with its time-to-next-toe-off.
Sliding, ~1-second (50-sample) windows over the 20 raw IMU channels are used
as input sequences for a regression model that predicts time-to-toe-off.

Two recurrent architectures are compared:

- A two-layer **SimpleRNN** model (baseline)
- A two-layer **GRU** model

Across held-out data spanning slow, medium, and fast walking speeds, the GRU
model achieved a validation MAE of about **42 ms** (0.042, in the same units
as `t_toeoff`) in the final evaluation notebook — reported by the project as
roughly a **27% improvement** over the SimpleRNN baseline, with better
generalization across walking speeds. Feature-selection and PCA experiments
found that using all 20 raw sensor channels (rather than a reduced or
PCA-projected feature set) gave the best performance.

> **TODO:** Confirm the exact RNN-vs-GRU MAE values and the "27% improvement"
> figure against the numbers reported in the linked project report — the
> notebooks in this repo record a final GRU validation MAE of ~0.042, but the
> RNN baseline comparison lives in `InitialTrainingRNNGRU.ipynb`'s plots
> rather than as a single printed number.

## Repository Structure

```text
.
├── InitialTrainingRNNGRU.ipynb      # First RNN vs. GRU model training/comparison
├── HyperparameterGridSearch.ipynb   # Grid search over learning rate / hidden units
├── KFoldValidation.ipynb            # 5-fold and 10-fold cross-validation of RNN/GRU
├── FeatureSelection_PCA.ipynb        # Feature-selection and PCA experiments
├── FinalEvaluationOfGRU.ipynb       # Final GRU model training and evaluation
├── ML_proj_data_record.ino          # Arduino firmware for dual-IMU data logging
├── Project/                          # Raw IMU data (thigh + shin) per walking speed
│   ├── imu_data_fastest_speed.csv
│   ├── imu_data_medium_speed.csv
│   └── imu_data_slowest_speed.csv
└── Plots/                            # Notebooks that generate result figures
    ├── FeatureSelection_PCA_plots.ipynb
    ├── GridSearchPlot.ipynb
    ├── KFoldValidationPlots.ipynb
    └── FinalPlot.ipynb
```

## Data

Each CSV in `Project/` contains synchronized IMU streams from two BNO080
sensors (thigh and shin), sampled at 50 Hz, with columns:

```text
Time,QuatW,QuatX,QuatY,QuatZ,AccX,AccY,AccZ,GyroX,GyroY,GyroZ,
QuatW2,QuatX2,QuatY2,QuatZ2,AccX2,AccY2,AccZ2,GyroX2,GyroY2,GyroZ2
```

(the `2`-suffixed columns are the second sensor). This gives 20 raw feature
channels per timestep, which is the feature set the notebooks window into
1-second (50-sample) sequences for model input.

There is one CSV per walking-speed condition: `imu_data_slowest_speed.csv`,
`imu_data_medium_speed.csv`, and `imu_data_fastest_speed.csv`.

## Requirements

The notebooks were originally run in Google Colab (they contain a
`%cd '/content/drive/My Drive/Colab Notebooks'` cell that will need to be
removed or adjusted to run locally / point at this repo's `Project/`
directory). Core Python dependencies, based on the notebook imports:

- `numpy`
- `pandas`
- `scipy`
- `scikit-learn`
- `matplotlib`
- `tensorflow` (Keras `GRU`, `SimpleRNN`, `Dense` layers)
- `jupyter`

## Getting Started

```bash
git clone https://github.com/apoorvakhairnar/Estimation-of-Gait-Parameters-using-Neural-Networks.git
cd Estimation-of-Gait-Parameters-using-Neural-Networks

python -m venv .venv
source .venv/bin/activate  # Windows: .venv\Scripts\activate

pip install numpy pandas scipy scikit-learn matplotlib tensorflow jupyter

jupyter notebook
```

Each notebook loads data directly from `Project/*.csv` via a `DATA_FILES`
dict of relative paths, and expects `WINDOW_SZ = 50` samples (~1 s at
`FS = 50` Hz). If running outside Colab, remove/update the leading
`%cd` cell so the notebook's working directory is the repository root.

Suggested run order:

1. `InitialTrainingRNNGRU.ipynb` — baseline RNN vs. GRU comparison
2. `HyperparameterGridSearch.ipynb` — learning-rate / hidden-unit sweep
3. `KFoldValidation.ipynb` — 5-fold and 10-fold cross-validation
4. `FeatureSelection_PCA.ipynb` — feature-selection and PCA experiments
5. `FinalEvaluationOfGRU.ipynb` — final GRU training and evaluation
6. Notebooks in `Plots/` — regenerate the corresponding figures

## Usage

- **Input:** dual-IMU CSV logs in `Project/` (or new logs recorded with
  `ML_proj_data_record.ino`)
- **Output:** a regression estimate of time-to-toe-off, evaluated with mean
  absolute error (MAE) on held-out data
- **Models:** `RNN_make_model` / `GRU_make_model` in each notebook define
  two-layer SimpleRNN/GRU networks followed by a dense head; trained models
  from `FinalEvaluationOfGRU.ipynb` are saved as `GRU_<name>_final.keras` (e.g. `GRU_combined_final.keras`)

## IMU Data Collection (Firmware)

`ML_proj_data_record.ino` is Arduino-style firmware (targeting an SD-card
logger board) that reads two BNO080 IMUs (thigh at I2C address `0x4A`, shin at
`0x4B`) over I2C and logs synchronized quaternion, acceleration, and gyro
data to an SD card as CSV. Libraries used by the sketch include:

- `SparkFun_BNO080_Arduino_Library`
- `Adafruit_BNO08x` / `sh2`, `sh2_SensorValue`
- `SD`, `Wire`

## Contributors

Contributors are listed in alphabetical order (by last name):

1. Apoorva Khairnar
2. Prashant Kumar
3. Aksh Rajput

## Contributing

This was a course project; external contributions are not being accepted.

