import 'package:flutter/foundation.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'notification_service.dart';
import 'package:mqtt_client/mqtt_server_client.dart';
import 'dart:async';

final MqttService mqttService = MqttService();

class MqttService extends ChangeNotifier {
  late final MqttServerClient client;

  final String mqttServer = "YOUR_IP";
  final int mqttPort = 1883;
  final String mqttUser = "YOUR_USER";
  final String mqttPassword = "YOUR_PASSWORD";

  bool isConnected = false;
  String connectionStatus = "Connecting...";

  // Timers for managing connection retries and timeouts
  Timer? _timeoutTimer;
  Timer? _connectionRetryTimer;

  String humidity = "0";
  String setpoint = "0";
  String pwm = "0";
  String kp = "0";
  String ki = "0";
  String kd = "0";
  String phoneNumber = "-";
  String thresholdVariation = "-";
  String thresholdLower = "-";
  String thresholdUpper = "-";
  String alertThresholdOption = "-";
  String thresholdOption = "-";
  String thresholdType = "-";
  String maxPeak = "0";
  String overshoot = "0";
  String settlingTime = "0";
  bool callUser = false;
  bool cooler = false;

  // List of MQTT topics to subscribe to
  final List<String> mqttTopics = [
    'esp32/humidity',
    'esp32/setpoint',
    'esp32/phone',
    'esp32/thresholdOption',
    'esp32/thresholdLower',
    'esp32/thresholdUpper',
    'esp32/thresholdVariation',
    'esp32/alertThresholdOption',
    'esp32/alertThreshold',
    'esp32/error',
    'esp32/pwm',
    'esp32/kp',
    'esp32/ki',
    'esp32/kd',
    'esp32/maxPeak',
    'esp32/settlingTime',
    'esp32/overshoot',
    'esp32/alert',
    'esp32/cooler',
  ];

  // Mapping topics to their corresponding variables
  late final Map<String, Function(String)> topicHandlers;

  MqttService() {
    client = MqttServerClient(mqttServer, 'flutter_client')
      ..port = mqttPort
      ..logging(on: true)
      ..secure = false
      ..onConnected = onConnected
      ..onDisconnected = onDisconnected
      ..onSubscribed = onSubscribed;

    client.connectionMessage = MqttConnectMessage()
        .withClientIdentifier('flutter_client')
        .startClean()
        .authenticateAs(mqttUser, mqttPassword)
        .withWillQos(MqttQos.atLeastOnce);

    // Map topics to their respective update methods
    topicHandlers = {
      'esp32/humidity': (msg) => humidity = msg,
      'esp32/setpoint': (msg) => setpoint = msg,
      'esp32/pwm': (msg) => pwm = msg,
      'esp32/kp': (msg) => kp = msg.isEmpty ? '0.0' : msg,
      'esp32/ki': (msg) => ki = msg.isEmpty ? '0.0' : msg,
      'esp32/kd': (msg) => kd = msg.isEmpty ? '0.0' : msg,
      'esp32/thresholdOption': (msg) =>
          thresholdOption = msg.isEmpty ? '0.0' : msg,
      'esp32/thresholdVariation': (msg) =>
          thresholdVariation = msg.isEmpty ? '0.0' : msg,
      'esp32/thresholdUpper': (msg) =>
          thresholdUpper = msg.isEmpty ? '0.0' : msg,
      'esp32/thresholdLower': (msg) =>
          thresholdLower = msg.isEmpty ? '0.0' : msg,
      'esp32/alertThresholdOption': (msg) =>
          alertThresholdOption = msg.isEmpty ? '0.0' : msg,
      'esp32/phone': (msg) => phoneNumber = msg,
      'esp32/maxPeak': (msg) => maxPeak = msg,
      'esp32/settlingTime': (msg) => settlingTime = msg,
      'esp32/cooler': (msg) => cooler = (msg == 'on' ? true : false),
      'esp32/overshoot': (msg) => overshoot = msg,
      'esp32/alert': (msg) =>
          NotificationService.showNotification("Alert!", msg)
    };

  }

  /// Connects to the MQTT broker
  Future<bool> connect() async {
    try {
      await client.connect();
      isConnected = true;
      connectionStatus = "Connected to the Broker";
      notifyListeners();
      return true;
    } catch (e) {
      print('Error connecting: $e');
      connectionStatus = "Connecting...";
      notifyListeners();
      return false;
    }
  }

  /// Handles successful connection
  void onConnected() {
    _resetTimeoutTimer();
    isConnected = true;
    notifyListeners();
    print('Connected to MQTT broker');

    // Subscribe to all topics
    subscribeTopics();

    // Send start message to ESP32
    publish('esp32/start', 'startreceive');
  }

  // Handles disconnection from the broker
  void onDisconnected() {
    _timeoutTimer?.cancel();
    isConnected = false;
    connectionStatus = "Disconnected";
    notifyListeners();
    print('Disconnected from MQTT broker');
  }

  // Handles topic subscription
  void onSubscribed(String topic) {
    print('Subscribed to topic $topic');
  }

  // Publishes a message to a specified topic
  void publish(String topic, String message) {
    if (!isConnected) {
      print("Cannot publish. Client is not connected.");
      return;
    }

    final builder = MqttClientPayloadBuilder();
    builder.addString(message);
    client.publishMessage(topic, MqttQos.atMostOnce, builder.payload!);
  }

  // Subscribes to all defined topics
  void subscribeTopics() {
    for (String topic in mqttTopics) {
      client.subscribe(topic, MqttQos.atMostOnce);
    }
  }

  // Resets the timeout timer to monitor ESP32 connection
  void _resetTimeoutTimer() {
    _timeoutTimer?.cancel();
    _timeoutTimer = Timer(Duration(seconds: 10), _handleTimeout);
  }

  // Handles timeout scenarios when ESP32 is unresponsive
  void _handleTimeout() {
    connectionStatus = "No connection with ESP32";
    notifyListeners();
    _startConnectionRetryTimer();
  }

  // Starts a timer to retry connection attempts
  void _startConnectionRetryTimer() {
    _connectionRetryTimer?.cancel();

    _connectionRetryTimer = Timer.periodic(Duration(seconds: 10), (timer) {
      if (connectionStatus == "No connection with ESP32") {
        print('Publishing "startreceive" to reconnect with ESP32...');
        publish('esp32/start', 'startreceive');
      } else {
        print('ESP32 responded, stopping reconnection attempts.');
        timer.cancel();
      }
    });
  }

  // Listens to incoming MQTT messages and updates values accordingly
  void listenToMessages() {
    client.updates!.listen((List<MqttReceivedMessage> event) {
      final recMessage = event[0].payload as MqttPublishMessage;
      final message =
          MqttPublishPayload.bytesToStringAsString(recMessage.payload.message);

      connectionStatus = "Connected";
      _connectionRetryTimer?.cancel();

      // Use the topicHandlers map to update the corresponding variable
      topicHandlers[event[0].topic]?.call(message);

      notifyListeners();
      _resetTimeoutTimer();
    });
  }
}
