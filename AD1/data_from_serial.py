import serial
import sqlite3
import re  

ser = serial.Serial('COM3', 9600, timeout=1)

conn = sqlite3.connect('sensor_data.db')  # 'sensor_data.db'는 로컬에 생성될 SQLite 데이터베이스 파일
cur = conn.cursor()

cur.execute('''CREATE TABLE IF NOT EXISTS sensor_data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                L INTEGER,
                C INTEGER,
                R INTEGER,
                timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)''')
conn.commit()

pattern = re.compile(r"(\d+), (\d+), (\d+)")

try:
    while True:
        line = ser.readline().decode('utf-8').strip()
        if line:
            print("Received:", line)
            try:
                match = pattern.search(line)
                if match:
                    l_val, c_val, r_val = map(int, match.groups())
                    
                    cur.execute("INSERT INTO sensor_data (L, C, R) VALUES (?, ?, ?)", (l_val, c_val, r_val))
                    conn.commit()
                else:
                    print("Unexpected data format, skipping:", line)
            except (ValueError, IndexError) as e:
                print(f"Error processing data: {e}")
except Exception as e:
    print(f"An error occurred: {e}")
finally:
    cur.close()
    conn.close()
