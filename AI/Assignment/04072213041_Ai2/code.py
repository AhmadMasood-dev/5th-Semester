import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import classification_report
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout, Input
from tensorflow.keras.callbacks import EarlyStopping
import matplotlib.pyplot as plt
import numpy as np

# Load the dataset
data = pd.read_csv('/content/cleaned_merged_heart_dataset.csv')

# Preview the dataset
print(data.head())
print(data.info())

# Separate features and target
X = data[['age', 'sex', 'cp', 'trestbps', 'chol', 'fbs', 'restecg', 'thalachh',
          'exang', 'oldpeak', 'slope', 'ca', 'thal']].values  # Features
y = data['target'].values  # Target column

# Standardize the data
scaler = StandardScaler()
X = scaler.fit_transform(X)

# Split the data (80% train, 10% validation, 10% test)
X_train, X_temp, y_train, y_temp = train_test_split(X, y, train_size=0.8, random_state=42)
X_val, X_test, y_val, y_test = train_test_split(X_temp, y_temp, test_size=0.5, random_state=42)

# Build the neural network with Dropout and Regularization to prevent overfitting

model = Sequential([
    Input(shape=(X_train.shape[1],)),  # Define input shape without input_dim
    Dense(16, activation='relu', kernel_regularizer=tf.keras.regularizers.l2(0.001)),
 Dropout(0.3),
    Dense(8, activation='relu'),
    Dense(1, activation='sigmoid')  # Output layer for binary classification
])

# Compile the model with a lower learning rate and Adam optimizer
model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=0.001),
              loss='binary_crossentropy',
              metrics=['accuracy'])

# Early stopping to prevent overfitting
early_stopping = EarlyStopping(monitor='val_loss', patience=10, restore_best_weights=True)

# Train the model with consistent batch size
history = model.fit(X_train, y_train,
                    validation_data=(X_val, y_val),
                    epochs=100,
                    batch_size=16,
                    callbacks=[early_stopping])

# Evaluate the model on the test set

test_loss, test_accuracy = model.evaluate(X_test, y_test)
print(f"Test Accuracy: {test_accuracy:.2f}")
# Generate predictions using consistent data types and batch size
y_pred = (model.predict(np.array(X_test), batch_size=16) > 0.5).astype("int32")


# Classification report
print(classification_report(y_test, y_pred))

# Plot training and validation loss curves
plt.figure(figsize=(10, 6))
plt.plot(history.history['loss'], label='Training Loss')
plt.plot(history.history['val_loss'], label='Validation Loss')
plt.xlabel('Epochs')
plt.ylabel('Loss')
plt.legend()
plt.title('Training vs Validation Loss')
plt.show()
