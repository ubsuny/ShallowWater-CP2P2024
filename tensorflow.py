import numpy as np
import matplotlib.pyplot as plt
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.optimizers import Adam
from pathlib import Path
from google.colab import drive

def load_data_large():
    """ Load large training and validation dataset

        Returns a tuple of length 4 with the following objects:
        X_train: An N_train-x-M ndarray containing the training data (N_train examples, M features each)
        y_train: An N_train-x-1 ndarray containing the labels
        X_val: An N_val-x-M ndarray containing the training data (N_val examples, M features each)
        y_val: An N_val-x-1 ndarray containing the labels
    """
    script_dir = Path(__file__).resolve().parent
    train_all = np.loadtxt(script_dir / 'data/largeTrain.csv', dtype=int, delimiter=',')
    valid_all = np.loadtxt(script_dir / 'data/largeValidation.csv', dtype=int, delimiter=',')

    X_train = train_all[:, 1:]
    y_train = train_all[:, 0]
    X_val = valid_all[:, 1:]
    y_val = valid_all[:, 0]

    return (X_train, y_train, X_val, y_val)

def train_and_evaluate_model(hidden_units, X_train, y_train, X_val, y_val):
    model = Sequential([
        Dense(hidden_units, input_shape=(X_train.shape[1],), activation='relu'),
        Dense(10, activation='softmax')  # Assuming 10 classes for classification
    ])
    
    model.compile(optimizer=Adam(learning_rate=0.01),
                  loss='sparse_categorical_crossentropy',
                  metrics=['accuracy'])
    
    history = model.fit(X_train, y_train, validation_data=(X_val, y_val),
                        epochs=50, batch_size=32, verbose=0)
    
    return history.history['loss'], history.history['val_loss']

# Main execution block to avoid running unintentionally when imported
def load_data_large():
    """ Load large training and validation dataset

        Returns a tuple of length 4 with the following objects:
        X_train: An N_train-x-M ndarray containing the training data (N_train examples, M features each)
        y_train: An N_train-x-1 ndarray containing the labels
        X_val: An N_val-x-M ndarray containing the training data (N_val examples, M features each)
        y_val: An N_val-x-1 ndarray containing the labels
    """
    # Ensure your path reflects where you've stored the files in your Google Drive
    base_path = Path('/content/drive/MyDrive/Acceleration')
    train_all = np.loadtxt(base_path / 'largeTrain.csv', dtype=int, delimiter=',')
    valid_all = np.loadtxt(base_path / 'largeValidation.csv', dtype=int, delimiter=',')

    X_train = train_all[:, 1:]
    y_train = train_all[:, 0]
    X_val = valid_all[:, 1:]
    y_val = valid_all[:, 0]

    return (X_train, y_train, X_val, y_val)

if __name__ == "__main__":
    # Mount Google Drive
    drive.mount('/content/drive')
    
    # Load your data
    X_train, y_train, X_val, y_val = load_data_large()

    # Parameters
    hidden_units_options = [5, 20, 50, 100, 200]

    # Store full history for better visual insight
    histories = []

    for units in hidden_units_options:
        train_loss, val_loss = train_and_evaluate_model(units, X_train, y_train, X_val, y_val)
        histories.append((train_loss, val_loss))

# Plotting
plt.plot(hidden_units_options, training_losses, label='Average Training Cross-Entropy')
plt.plot(hidden_units_options, validation_losses, label='Average Validation Cross-Entropy')
plt.xlabel('Number of Hidden Units')
plt.ylabel('Cross-Entropy Loss')
plt.title('Effect of Hidden Units on Cross-Entropy Loss')
plt.legend()
plt.show() 
