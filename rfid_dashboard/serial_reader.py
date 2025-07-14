import os
import time
import threading
try:
    import serial
    import serial.tools.list_ports
except ImportError:
    serial = None

SERIAL_BAUD = 115200

class SerialReader:
    def __init__(self, update_callback, port=None):
        self.update_callback = update_callback
        self.port = port or self._autodetect_port()
        self._stop_event = threading.Event()
        self.thread = None

    def _autodetect_port(self):
        if not serial:
            return None
        ports = list(serial.tools.list_ports.comports())
        for p in ports:
            if 'USB' in p.description or 'CP210' in p.description or 'CH340' in p.description:
                return p.device
        return None

    def start(self):
        if not serial or not self.port:
            print('Serial not available or port not found.')
            return
        self.thread = threading.Thread(target=self._run, daemon=True)
        self.thread.start()

    def stop(self):
        self._stop_event.set()
        if self.thread:
            self.thread.join()

    def _run(self):
        try:
            ser = serial.Serial(self.port, SERIAL_BAUD, timeout=1)
            print(f'Connected to {self.port}')
        except Exception as e:
            print(f'Could not open serial port: {e}')
            return
        fw = None
        last_uid = None
        while not self._stop_event.is_set():
            try:
                line = ser.readline().decode(errors='ignore').strip()
                if not line:
                    continue
                data = {}
                if line.startswith('FW_VER_'):
                    fw = line
                    data['firmware'] = fw
                elif line.startswith('Card UID:') or line.startswith('UID:'):
                    uid = line.split(':', 1)[1].strip()
                    last_uid = uid
                    data['uid'] = uid
                elif 'Access Granted' in line or 'Access: Granted' in line:
                    data['access'] = 'Granted'
                    data['uid'] = last_uid
                elif 'Access Denied' in line or 'Access: Denied' in line:
                    data['access'] = 'Denied'
                    data['uid'] = last_uid
                if data:
                    self.update_callback(data)
            except Exception as e:
                print(f'Serial read error: {e}')
                time.sleep(1) 