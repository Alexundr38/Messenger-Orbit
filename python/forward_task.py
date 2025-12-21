import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np

def open_file(file_path):
    if not os.path.exists(file_path):
        print(f"Файл {file_path} не найден!")
        return None
    df = pd.read_csv(file_path)
    return df

horizon = open_file("data/horizons_results_mercury.txt")
simulation = open_file("data/result.txt")

if horizon is None or simulation is None:
    print("Не удалось загрузить один или оба файла.")
else:
    horizon_position_norm = np.sqrt((horizon['x']- simulation['x'])**2 + (horizon['y']- simulation['y'])**2 + (horizon['z']- simulation['z'])**2)
    horizon_velocity_norm = np.sqrt((horizon['vx']- simulation['vx'])**2 + (horizon['vy']- simulation['vy'])**2 + (horizon['vz']- simulation['vz'])**2)

    # simulation_position_norm = np.sqrt(simulation['x']**2 + simulation['y']**2 + simulation['z']**2)
    # simulation_velocity_norm = np.sqrt(simulation['vx']**2 + simulation['vy']**2 + simulation['vz']**2)

    # diff_position = (horizon_position_norm -simulation_position_norm)
    # diff_velocity = (horizon_velocity_norm - simulation_velocity_norm)

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

    ax1.plot(horizon_position_norm.index, horizon_position_norm, label='Разница нормы позиции', marker='o', color='blue', markersize=1)
    ax1.set_title('Разница нормы позиций (horizon - simulation)')
    ax1.set_xlabel('Индекс (временной шаг)')
    ax1.set_ylabel('Разница нормы')
    ax1.legend()
    ax1.grid(True)

    ax2.plot(horizon_velocity_norm.index, horizon_velocity_norm, label='Разница нормы скорости', marker='o', color='red', markersize=1)
    ax2.set_title('Разница нормы скоростей (horizon - simulation)')
    ax2.set_xlabel('Индекс (временной шаг)')
    ax2.set_ylabel('Разница нормы')
    ax2.legend()
    ax2.grid(True)

    plt.tight_layout()
    plt.show()
