# Neural Network Training and Evaluation in TensorFlow

This file demonstrates the process of loading a large dataset, training a neural network using TensorFlow, and visualizing the results. The notebook is structured to run in Google Colab and uses Google Drive for data storage.

## Features

- **Data Loading**: There were three datasets namely small, medium, and large datasets which are available in the folder [tf_dataset](https://github.com/ubsuny/ShallowWater-CP2P2024/tree/main/src/tf_dataset). These dataset images are to detect the handwritten numbers from 0 to 9. And the dataset is in grayscale.  I used the largest dataset. Load large training and validation datasets from Google Drive.
- **Model Training**: Train a neural network with varying sizes of hidden layers.
- **Performance Visualization**: Visualize training and validation loss across epochs to evaluate model performance.
- **Data Visualization**: Generate histograms and scatter plots to explore the dataset.

## Setup

To use this notebook, follow these steps:

1. **Google Colab**: Open the notebook in Google Colab to ensure all dependencies are handled via Colab's environment.
2. **Google Drive**: Mount your Google Drive in the notebook to access the dataset files.

### Prerequisites

Make sure your Google Drive contains the following datasets:
- `largeTrain.csv` - Training dataset.
- `largeValidation.csv` - Validation dataset.

The datasets should be in a directory named `data` under the root directory specified in the notebook.

## Usage

1. **Mount Google Drive**: The notebook will prompt you to authenticate and mount your Google Drive.
2. **Load Data**: Automatically load data from the specified paths.
3. **Train Model**: Choose from predefined hidden layer sizes to train the model.
4. **Visualize Results**: After training, visualize the loss over epochs for both training and validation data.
5. **Explore Data**: Utilize the provided plots to understand the distribution of data and relationships between features.

## Results

This section highlights key findings and outcomes from the analyses conducted in the notebook:

### Model Performance

The notebook trains a neural network model on a large dataset, testing various configurations of hidden units (5, 20, 50, 100, 200). The training and validation loss curves are plotted for each configuration, providing insights into the model's learning progress and generalization ability. Here's what we observed:

- **Overfitting**: Larger models with more hidden units tended to overfit, indicated by a decrease in training loss but an increase or stagnation in validation loss.
- **Optimal Size**: Intermediate sizes of hidden units (e.g., 50 or 100) seemed to strike the best balance between learning capacity and generalization.

### Data Exploration

Histograms and scatter plots of the dataset reveal the following:

- **Class Distribution**: The histogram of class labels shows whether the dataset is balanced or if certain classes are overrepresented.
- **Feature Relationships**: Scatter plots of the first two features colored by class labels illustrate the separability of classes based on these features alone. This visualization helps in understanding whether linear models might perform well or if more complex models are needed.

### Visualizations

Visualizations created in the notebook provide a clear view of the data's characteristics and the model's performance across different configurations. These plots are crucial for diagnosing issues in machine learning workflows, such as data imbalances or inappropriate model complexity.

## Conclusions

Based on the outputs and visualizations:

- Models need to be carefully regularized to prevent overfitting, especially as the complexity of the model increases.
- Future work could explore more sophisticated feature engineering, data augmentation, or advanced model architectures to improve performance.

These results provide foundational insights that can guide further detailed analysis and model tuning efforts.

![Model Training and Validation Losses](https://github.com/ubsuny/ShallowWater-CP2P2024/blob/main/src/train_val_loss.png?raw=true)
![](https://github.com/ubsuny/ShallowWater-CP2P2024/blob/main/src/train_val_loss_20.png?raw=true)
![](https://github.com/ubsuny/ShallowWater-CP2P2024/blob/main/src/train_val_loss_50.png?raw=true)
![](https://github.com/ubsuny/ShallowWater-CP2P2024/blob/main/src/train_val_loss_100.png?raw=true)
