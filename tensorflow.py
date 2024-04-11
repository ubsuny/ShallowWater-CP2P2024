import numpy as np
import matplotlib.pyplot as plt
import os 
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.optimizers import Adam
from pathlib import Path

def load_data_large():
    """ Load large training and validation dataset

        Returns a tuple of length 4 with the following objects:
        X_train: An N_train-x-M ndarray containing the training data (N_train examples, M features each)
        y_train: An N_train-x-1 ndarray contraining the labels
        X_val: An N_val-x-M ndarray containing the training data (N_val examples, M features each)
        y_val: An N_val-x-1 ndarray contraining the labels
    """
    script_dir = Path(__file__).parent
    train_all = np.loadtxt(f'{script_dir}/data/largeTrain.csv', dtype=int, delimiter=',')
    valid_all = np.loadtxt(f'{script_dir}/data/largeValidation.csv', dtype=int, delimiter=',')

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
    
    return np.mean(history.history['loss']), np.mean(history.history['val_loss'])

# Load your data
X_train, y_train, X_val, y_val = load_data_large()

# Parameters
hidden_units_options = [5, 20, 50, 100, 200]

training_losses = []
validation_losses = []

for units in hidden_units_options:
    train_loss, val_loss = train_and_evaluate_model(units, X_train, y_train, X_val, y_val)
    training_losses.append(train_loss)
    validation_losses.append(val_loss)

# Plotting
plt.plot(hidden_units_options, training_losses, label='Average Training Cross-Entropy')
plt.plot(hidden_units_options, validation_losses, label='Average Validation Cross-Entropy')
plt.xlabel('Number of Hidden Units')
plt.ylabel('Cross-Entropy Loss')
plt.title('Effect of Hidden Units on Cross-Entropy Loss')
plt.legend()
plt.show() 
