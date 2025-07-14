import os
from flask import Flask, render_template, jsonify, request
from state import mode, data_lock, latest_data, update_state, log_deque
from serial_reader import SerialReader
from demo_simulator import DemoSimulator

app = Flask(__name__)

# =====================
# Background Thread Manager
# =====================
serial_reader = None
simulator = None
bg_thread = None

def start_background_thread():
    global serial_reader, simulator, bg_thread
    # Stop any running threads
    if serial_reader:
        serial_reader.stop()
        serial_reader = None
    if simulator:
        simulator.stop()
        simulator = None
    # Start new thread based on mode
    if mode == 'live':
        serial_reader = SerialReader(update_callback=thread_update)
        serial_reader.start()
    else:
        simulator = DemoSimulator(update_callback=thread_update)
        simulator.start()

def thread_update(data):
    with data_lock:
        update_state(data)

# =====================
# Flask Routes
# =====================
@app.route('/')
def dashboard():
    return render_template('dashboard.html', mode=mode)

@app.route('/api/data')
def api_data():
    with data_lock:
        return jsonify({
            'firmware': latest_data['firmware'],
            'uid': latest_data['uid'],
            'access': latest_data['access'],
            'log': latest_data['log'],
            'mode': mode
        })

@app.route('/api/mode', methods=['POST'])
def set_mode():
    import state
    new_mode = request.json.get('mode')
    if new_mode in ['live', 'demo']:
        state.mode = new_mode
        # Clear log and data
        with data_lock:
            log_deque.clear()
            latest_data['uid'] = '----'
            latest_data['access'] = 'Unknown'
            latest_data['log'] = []
        start_background_thread()
        return jsonify({'status': 'ok', 'mode': state.mode})
    return jsonify({'status': 'error', 'msg': 'Invalid mode'}), 400

# =====================
# Main
# =====================
if __name__ == '__main__':
    start_background_thread()
    app.run(debug=True, host='0.0.0.0', port=5000) 