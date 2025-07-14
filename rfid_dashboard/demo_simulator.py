import random
import threading
import time

class DemoSimulator:
    def __init__(self, update_callback, interval=3):
        self.update_callback = update_callback
        self.interval = interval
        self._stop_event = threading.Event()
        self.thread = None

    def start(self):
        self.thread = threading.Thread(target=self._run, daemon=True)
        self.thread.start()

    def stop(self):
        self._stop_event.set()
        if self.thread:
            self.thread.join()

    def _run(self):
        while not self._stop_event.is_set():
            time.sleep(self.interval)
            data = {
                'firmware': 'FW_VER_1.0.0-DEMO',
                'uid': ' '.join(f'{random.randint(0,255):02X}' for _ in range(4)),
                'access': random.choice(['Granted', 'Denied'])
            }
            self.update_callback(data) 