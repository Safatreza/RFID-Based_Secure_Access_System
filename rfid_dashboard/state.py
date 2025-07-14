import threading
from collections import deque

LOG_SIZE = 10

# Shared state for dashboard
mode = 'demo'  # 'live' or 'demo'
data_lock = threading.Lock()
latest_data = {
    'firmware': 'FW_VER_1.0.0',
    'uid': '----',
    'access': 'Unknown',
    'log': []
}
log_deque = deque(maxlen=LOG_SIZE)

def update_state(data):
    """
    Update the shared state with new data from serial or demo.
    """
    if 'firmware' in data:
        latest_data['firmware'] = data['firmware']
    if 'uid' in data:
        latest_data['uid'] = data['uid']
    if 'access' in data:
        latest_data['access'] = data['access']
        log_deque.appendleft({'uid': latest_data['uid'], 'access': latest_data['access']})
    latest_data['log'] = list(log_deque) 