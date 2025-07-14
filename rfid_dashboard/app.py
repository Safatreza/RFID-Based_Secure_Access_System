import os
from flask import Flask, render_template, jsonify, request
import state  # Import the state module for shared state
from serial_reader import SerialReader
from demo_simulator import DemoSimulator

app = Flask(__name__)

# =====================
# Background Thread Manager
# =====================
serial_reader = None
simulator = None

def start_background_thread():
    global serial_reader, simulator
    # Stop any running threads
    if serial_reader:
        serial_reader.stop()
        serial_reader = None
    if simulator:
        simulator.stop()
        simulator = None
    # Start new thread based on mode
    if state.mode == 'live':
        serial_reader = SerialReader(update_callback=thread_update)
        serial_reader.start()
    else:
        simulator = DemoSimulator(update_callback=thread_update)
        simulator.start()

def thread_update(data):
    with state.data_lock:
        state.update_state(data)

# =====================
# Flask Routes
# =====================
@app.route('/')
def dashboard():
    return render_template('dashboard.html', mode=state.mode)

@app.route('/api/data')
def api_data():
    with state.data_lock:
        return jsonify({
            'firmware': state.latest_data['firmware'],
            'uid': state.latest_data['uid'],
            'access': state.latest_data['access'],
            'log': state.latest_data['log'],
            'mode': state.mode
        })

@app.route('/api/mode', methods=['POST'])
def set_mode():
    new_mode = request.json.get('mode')
    if new_mode in ['live', 'demo']:
        state.mode = new_mode
        # Clear log and data
        with state.data_lock:
            state.log_deque.clear()
            state.latest_data['uid'] = '----'
            state.latest_data['access'] = 'Unknown'
            state.latest_data['log'] = []
        start_background_thread()
        return jsonify({'status': 'ok', 'mode': state.mode})
    return jsonify({'status': 'error', 'msg': 'Invalid mode'}), 400

# =====================
# Main
# =====================
if __name__ == '__main__':
    start_background_thread()
    app.run(debug=True, host='0.0.0.0', port=5000) 