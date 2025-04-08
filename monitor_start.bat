@echo off
chcp 65001 > nul
title PC 모니터링 시스템


echo ===== PC 모니터링 시스템 시작 =====
echo.

echo 필요한 라이브러리 설치 중...
pip install pyserial psutil GPUtil wmi
echo 라이브러리 설치 완료!
echo.

:: OpenHardwareMonitor 실행 (CPU 온도 모니터링을 위해 필요)
echo OpenHardwareMonitor 시작 중...
start "" ""C:\Users\chloe\Desktop\pc_monitoring\OpenHardwareMonitor\OpenHardwareMonitor.exe""

:: OpenHardwareMonitor가 시작될 시간 확보
echo 잠시 기다려주세요...
timeout /t 3 /nobreak > nul

:: 파이썬 스크립트 실행
echo 모니터링 스크립트 실행 중...
cd /d "%~dp0"
python hwmonitor.py

:: 스크립트가 종료되면 실행될 코드
echo.
echo 모니터링이 종료되었습니다.
pause