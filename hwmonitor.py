import serial
import time
import psutil
try:
    import GPUtil
    gpu_available = True
except ImportError:
    gpu_available = False
    print("GPUtil not available. GPU monitoring disabled.")

# 시리얼 포트 설정 (아두이노가 연결된 포트로 변경 필요)
SERIAL_PORT = 'COM7'  # Windows의 경우. Linux/Mac의 경우 '/dev/ttyUSB0' 등으로 변경
BAUD_RATE = 9600

# CPU 온도 가져오기 (운영체제에 따라 다름)
def get_cpu_temp():
    try:
        # Linux의 경우
        with open("/sys/class/thermal/thermal_zone0/temp", "r") as f:
            return float(f.read()) / 1000.0
    except:
        try:
            # Windows의 경우 (WMI 필요)
            import wmi
            w = wmi.WMI(namespace="root\OpenHardwareMonitor")
            temperature_infos = w.Sensor()
            for sensor in temperature_infos:
                if sensor.SensorType == 'Temperature' and 'CPU' in sensor.Name:
                    return float(sensor.Value)
            return 0.0
        except:
            return 0.0

# GPU 정보 가져오기
def get_gpu_info():
    if gpu_available:
        try:
            gpus = GPUtil.getGPUs()
            if gpus:
                gpu = gpus[0]  # 첫 번째 GPU 사용
                return gpu.temperature, gpu.load * 100
        except:
            pass
    return 0.0, 0.0

# 메인 루프
def main():
    try:
        # 시리얼 연결
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"연결됨: {SERIAL_PORT}")
        time.sleep(2)  # 아두이노 리셋 대기
        
        while True:
            # CPU 정보 가져오기
            cpu_temp = get_cpu_temp()
            cpu_usage = psutil.cpu_percent()
            
            # GPU 정보 가져오기
            gpu_temp, gpu_usage = get_gpu_info()
            
            # 데이터 형식: "CPU_TEMP,CPU_USAGE,GPU_TEMP,GPU_USAGE"
            data = f"{cpu_temp:.1f},{cpu_usage:.0f},{gpu_temp:.1f},{gpu_usage:.0f}\n"
            
            # 시리얼로 데이터 전송
            ser.write(data.encode())
            print(f"전송됨: {data.strip()}")
            
            time.sleep(1)  # 1초마다 업데이트
            
    except serial.SerialException as e:
        print(f"시리얼 포트 오류: {e}")
    except KeyboardInterrupt:
        print("프로그램 종료")
    finally:
        if 'ser' in locals():
            ser.close()
            print("시리얼 포트 닫힘")

if __name__ == "__main__":
    main()