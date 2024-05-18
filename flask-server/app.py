from flask import Flask
import requests

app = Flask(__name__)

# Route to control LED on ESP2
@app.route('/control_led', methods=['GET'])
def control_led():
    # Send API request to ESP2 to control LED
    try:
        response = requests.get('esp2.local/led/toggle')  # Use the mDNS name of ESP2
        if response.status_code == 200:
            return 'LED on ESP2 toggled successfully'
        else:
            return 'Failed to toggle LED on ESP2'
    except Exception as e:
        print("Error:", e)
        return 'Error occurred while toggling LED on ESP2'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)  # Run Flask app on all available network interfaces
