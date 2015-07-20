python /home/pi/Sensorian/Handler_NodeRED/tft_socket.py &
su -c 'python /home/pi/Sensorian/Handler_NodeRED/flask_rtcc_interface.py' pi &
python /home/pi/Sensorian/Handler_NodeRED/multithreaded_sensor_socket_server.py &
