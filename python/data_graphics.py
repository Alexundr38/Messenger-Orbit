import pandas as pd
import matplotlib.pyplot as plt
import os


def plot_full_observable_by_station(file_path, date):
    if not os.path.exists(file_path):
        print(f"Файл {file_path} не найден!")
        return

    try:
        df = pd.read_csv(file_path)

        df['record_tine'] = pd.to_datetime(df['record_tine'])

        df_filter = df[df['record_tine'].dt.date == pd.to_datetime(date).date()]

        if df_filter.empty:
            print(f"Нет данных за {date}")
            return

        stations = df_filter['receiving_station_id'].unique()

        plt.figure(figsize=(12, 8))

        colors = ['blue', 'red', 'green', 'orange', 'purple', 'brown', 'pink', 'gray', 'olive', 'cyan']

        for i, station in enumerate(stations):
            station_data = df_filter[df_filter['receiving_station_id'] == station]

            color = colors[i % len(colors)]

            plt.plot(station_data['record_tine'],
                     station_data['full_observable'],
                     label=f'Station {station}',
                     color=color,
                     marker='o',
                     markersize=2,
                     linewidth=1)

        # Настройка графика
        plt.title(date)
        plt.xlabel('Time')
        plt.ylabel('Full Observable')
        plt.legend(title='Station ID')
        plt.grid(True, alpha=0.3)
        plt.xticks(rotation=45)
        plt.tight_layout()

        plt.savefig('data/graphics/' + date + '.png', dpi=300, bbox_inches='tight')
        plt.show()

    except Exception as e:
        print(f"Ошибка при обработке данных: {e}")


if __name__ == "__main__":
    plot_full_observable_by_station('data/csv/all_year/2011.csv', '2011-02-08')
