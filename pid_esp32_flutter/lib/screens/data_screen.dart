import 'package:flutter/material.dart';
import 'package:pid_esp32/widgets/container_connected.dart';
import '../services/mqtt_service.dart';
import '../models/chart_data.dart';
import '../widgets/control_value.dart';
import '../widgets/graph.dart';
import 'parameterize_screen.dart';
import 'package:provider/provider.dart';
import 'dart:async';

class DataScreen extends StatefulWidget {
  @override
  _DataScreenState createState() => _DataScreenState();
}

class _DataScreenState extends State<DataScreen> {
  String? errorSetpoint;
  bool isGainsVisible = false;
  bool isValuesVisible = true;
  bool isResponseVisible = true;
  Timer? _timeoutTimer;
  ValueNotifier<List<ChartData>> humidityChartNotifier = ValueNotifier([]);
  ValueNotifier<List<ChartData>> setpointChartNotifier = ValueNotifier([]);

  Timer? _timer;
  int elapsedTime = 0;

  @override
  void initState() {
    super.initState();
    _connectToMqtt();
    _startTimer();
  }

  @override
  void dispose() {
    _timer?.cancel();
    _timeoutTimer?.cancel();
    super.dispose();
  }

  void _startTimer() {
    _timer?.cancel();
    _timer = Timer.periodic(Duration(seconds: 1), (timer) {
      if (mqttService.connectionStatus == "Connected") {
        double humidity = double.tryParse(mqttService.humidity) ?? 0;
        double setpoint = double.tryParse(mqttService.setpoint) ?? 0;

        final newHumidityData = List<ChartData>.from(
            humidityChartNotifier.value)
          ..add(ChartData(
              DateTime.now().add(Duration(seconds: elapsedTime)), humidity));
        final newSetpointData = List<ChartData>.from(
            setpointChartNotifier.value)
          ..add(ChartData(
              DateTime.now().add(Duration(seconds: elapsedTime)), setpoint));

        if (newHumidityData.length > 50) {
          newHumidityData.removeAt(0);
          newSetpointData.removeAt(0);
        }

        humidityChartNotifier.value = newHumidityData;
        setpointChartNotifier.value = newSetpointData;

        elapsedTime++;
      }
    });
  }

  Future<void> _connectToMqtt() async {
    bool connected = await mqttService.connect();

    if (connected) {
      mqttService.listenToMessages();
    }
  }

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (_) => mqttService,
      child: Scaffold(
        appBar: AppBar(
          backgroundColor: Colors.blue.shade800,
          title: Text(
            'PID Control',
            style: TextStyle(
              fontSize: 25,
              fontWeight: FontWeight.bold,
              color: Colors.white,
            ),
            textAlign: TextAlign.center,
          ),
          centerTitle: true,
          iconTheme: IconThemeData(
            color: Colors.white,
          ),
          actions: [
            PopupMenuButton<String>(
              onSelected: (String value) {
                if (value == 'Parameterize') {
                  if (mqttService.connectionStatus != 'Connected') {
                    ScaffoldMessenger.of(context).showSnackBar(
                      SnackBar(
                          content: Text(mqttService.connectionStatus,
                              textAlign: TextAlign.center,
                              style: TextStyle(color: Colors.white)),
                          backgroundColor: Colors.red),
                    );
                    // return; (commented out to allow navigation to ParamScreen)
                  }
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => ParamScreen()),
                  );
                }
              },
              itemBuilder: (BuildContext context) {
                return [
                  PopupMenuItem<String>(
                    value: 'Parameterize',
                    child: Text('Parameterize'),
                  ),
                ];
              },
            ),
          ],
        ),
        body: GestureDetector(
          onTap: () => FocusScope.of(context).unfocus(),
          child: Container(
            width: double.infinity,
            height: double.infinity,
            decoration: BoxDecoration(
              gradient: LinearGradient(
                colors: [
                  Colors.blue.shade800,
                  Colors.blue.shade700,
                  Colors.blue.shade300,
                ],
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
              ),
            ),
            child: SingleChildScrollView(
              child: Padding(
                padding: const EdgeInsets.all(20.0),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.start,
                  crossAxisAlignment: CrossAxisAlignment.center,
                  children: [
                    GestureDetector(
                      onTap: () => setState(() {
                        isGainsVisible = !isGainsVisible;
                      }),
                      child: Container(
                        decoration: BoxDecoration(
                          border: Border.all(color: Colors.white, width: 1.5),
                          color: Colors.white.withOpacity(0.2),
                          borderRadius: BorderRadius.circular(10),
                        ),
                        child: Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            ListTile(
                              contentPadding: EdgeInsets.only(left: 10),
                              title: Text(
                                'Gains',
                                style: TextStyle(
                                  fontSize: 20,
                                  fontWeight: FontWeight.bold,
                                  color: Colors.white70,
                                ),
                              ),
                              trailing: IconButton(
                                icon: Icon(
                                  isGainsVisible
                                      ? Icons.keyboard_arrow_right
                                      : Icons.keyboard_arrow_down,
                                  color: Colors.white,
                                  size: 30,
                                ),
                                onPressed: () {
                                  setState(() {
                                    isGainsVisible = !isGainsVisible;
                                  });
                                },
                              ),
                            ),
                            if (isGainsVisible)
                              Padding(
                                padding: const EdgeInsets.symmetric(
                                    horizontal: 10, vertical: 5),
                                child: Consumer<MqttService>(
                                  builder: (context, mqttService, child) {
                                    return Column(
                                      children: [
                                        buildControlValue(
                                            'Current Kp', mqttService.kp),
                                        buildControlValue(
                                            'Current Ki', mqttService.ki),
                                        buildControlValue(
                                            'Current Kd', mqttService.kd),
                                      ],
                                    );
                                  },
                                ),
                              ),
                          ],
                        ),
                      ),
                    ),
                    SizedBox(height: 0.02 * MediaQuery.of(context).size.height),
                    GestureDetector(
                      onTap: () => setState(() {
                        isValuesVisible = !isValuesVisible;
                      }),
                      child: Container(
                        decoration: BoxDecoration(
                          border: Border.all(color: Colors.white, width: 1.5),
                          color: Colors.white.withOpacity(0.2),
                          borderRadius: BorderRadius.circular(10),
                        ),
                        child: Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            ListTile(
                              contentPadding: EdgeInsets.only(left: 10),
                              title: Text(
                                'Values',
                                style: TextStyle(
                                  fontSize: 20,
                                  fontWeight: FontWeight.bold,
                                  color: Colors.white70,
                                ),
                              ),
                              trailing: IconButton(
                                icon: Icon(
                                  isValuesVisible
                                      ? Icons.keyboard_arrow_right
                                      : Icons.keyboard_arrow_down,
                                  color: Colors.white,
                                  size: 30,
                                ),
                                onPressed: () {
                                  setState(() {
                                    isValuesVisible = !isValuesVisible;
                                  });
                                },
                              ),
                            ),
                            if (isValuesVisible)
                              Padding(
                                padding: const EdgeInsets.symmetric(
                                    horizontal: 10, vertical: 5),
                                child: Consumer<MqttService>(
                                  builder: (context, mqttService, child) {
                                    return Column(
                                      crossAxisAlignment:
                                          CrossAxisAlignment.start,
                                      children: [
                                        Row(
                                          mainAxisAlignment:
                                              MainAxisAlignment.spaceBetween,
                                          children: [
                                            Text(
                                              'Set Point (Humidity):',
                                              style: TextStyle(
                                                  fontSize: 20,
                                                  color: Colors.white),
                                            ),
                                            Text(
                                              '${(double.tryParse(mqttService.setpoint) ?? 0).toStringAsFixed(0)} %',
                                              style: TextStyle(
                                                  fontSize: 20,
                                                  fontWeight: FontWeight.w400,
                                                  color: Colors.white),
                                            ),
                                          ],
                                        ),
                                        SizedBox(height: 10),
                                        Row(
                                          mainAxisAlignment:
                                              MainAxisAlignment.spaceBetween,
                                          children: [
                                            Text(
                                              'Humidity:',
                                              style: TextStyle(
                                                  fontSize: 20,
                                                  color: Colors.white),
                                            ),
                                            Text(
                                              '${mqttService.humidity} %',
                                              style: TextStyle(
                                                  fontSize: 20,
                                                  fontWeight: FontWeight.w400,
                                                  color: Colors.white),
                                            ),
                                          ],
                                        ),
                                        SizedBox(height: 10),
                                        Row(
                                          mainAxisAlignment:
                                              MainAxisAlignment.spaceBetween,
                                          children: [
                                            Text(
                                              'CV:',
                                              style: TextStyle(
                                                  fontSize: 20,
                                                  color: Colors.white),
                                            ),
                                            Text(
                                              '${mqttService.pwm} %',
                                              style: TextStyle(
                                                  fontSize: 20,
                                                  fontWeight: FontWeight.w400,
                                                  color: Colors.white),
                                            ),
                                          ],
                                        ),
                                        SizedBox(height: 10),
                                        Row(
                                          mainAxisAlignment:
                                              MainAxisAlignment.spaceBetween,
                                          children: [
                                            Text(
                                              'Cooler ON?',
                                              style: TextStyle(
                                                  fontSize: 20,
                                                  color: Colors.white),
                                            ),
                                            Text(
                                              mqttService.cooler ? 'Yes' : 'No',
                                              style: TextStyle(
                                                  fontSize: 20,
                                                  fontWeight: FontWeight.w400,
                                                  color: Colors.white),
                                            ),
                                          ],
                                        ),
                                      ],
                                    );
                                  },
                                ),
                              ),
                          ],
                        ),
                      ),
                    ),
                    SizedBox(height: 0.02 * MediaQuery.of(context).size.height),
                    GestureDetector(
                      onTap: () => setState(() {
                        isResponseVisible = !isResponseVisible;
                      }),
                      child: Container(
                        decoration: BoxDecoration(
                          border: Border.all(color: Colors.white, width: 1.5),
                          color: Colors.white.withOpacity(0.2),
                          borderRadius: BorderRadius.circular(10),
                        ),
                        child: Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            ListTile(
                              contentPadding: EdgeInsets.only(left: 10),
                              title: Text(
                                'System Response Graph',
                                style: TextStyle(
                                  fontSize: 20,
                                  fontWeight: FontWeight.bold,
                                  color: Colors.white70,
                                ),
                              ),
                              trailing: IconButton(
                                icon: Icon(
                                  isResponseVisible
                                      ? Icons.keyboard_arrow_right
                                      : Icons.keyboard_arrow_down,
                                  color: Colors.white,
                                  size: 30,
                                ),
                                onPressed: () {
                                  setState(() {
                                    isResponseVisible = !isResponseVisible;
                                  });
                                },
                              ),
                            ),
                            if (isResponseVisible)
                              Column(
                                children: [
                                  Padding(
                                    padding: const EdgeInsets.symmetric(
                                        horizontal: 10),
                                    child:
                                        ValueListenableBuilder<List<ChartData>>(
                                      valueListenable: humidityChartNotifier,
                                      builder: (context, humidityData, child) {
                                        return buildGraph(humidityData,
                                            setpointChartNotifier.value);
                                      },
                                    ),
                                  ),
                                  Consumer<MqttService>(
                                    builder: (context, mqttService, child) {
                                      return Padding(
                                        padding: const EdgeInsets.only(
                                            left: 16, right: 16, bottom: 5),
                                        child: Column(
                                          children: [
                                            Row(
                                              mainAxisAlignment:
                                                  MainAxisAlignment
                                                      .spaceBetween,
                                              crossAxisAlignment: CrossAxisAlignment
                                                  .start,
                                              children: [
                                                Column(
                                                  crossAxisAlignment:
                                                      CrossAxisAlignment
                                                          .start,
                                                  children: [
                                                    Text(
                                                      'Overshoot: ${(double.tryParse(mqttService.overshoot) ?? 0).toStringAsFixed(0)} %',
                                                      style: TextStyle(
                                                          fontSize: 18,
                                                          color: Colors.white),
                                                    ),
                                                    Text(
                                                      'Settling Time: ${mqttService.settlingTime} (s)',
                                                      style: TextStyle(
                                                          fontSize: 18,
                                                          color: Colors.white),
                                                    ),
                                                  ],
                                                ),
                                                Column(
                                                  crossAxisAlignment:
                                                      CrossAxisAlignment
                                                          .start,
                                                  children: [
                                                    Text(
                                                      'MaxPeak: ${(double.tryParse(mqttService.maxPeak) ?? 0).toStringAsFixed(0)} %',
                                                      style: TextStyle(
                                                          fontSize: 18,
                                                          color: Colors.white),
                                                    ),
                                                    Text(
                                                      'Settled? ' +
                                                          (((int.tryParse(mqttService.settlingTime) ?? 0) >
                                                                      0 ||
                                                                  ((double.tryParse(mqttService.humidity)?.toInt() ??
                                                                              -1) !=
                                                                          0 &&
                                                                      (double.tryParse(mqttService.setpoint)?.toInt() ??
                                                                              -2) !=
                                                                          0 &&
                                                                      (double.tryParse(mqttService.humidity)?.toInt() ??
                                                                              -1) ==
                                                                          (double.tryParse(mqttService.setpoint)?.toInt() ??
                                                                              -2)))
                                                              ? 'Yes'
                                                              : 'No'),
                                                      style: TextStyle(
                                                          fontSize: 18,
                                                          color: Colors.white),
                                                    ),
                                                  ],
                                                ),
                                              ],
                                            ),
                                          ],
                                        ),
                                      );
                                    },
                                  ),
                                ],
                              ),
                          ],
                        ),
                      ),
                    ),
                    SizedBox(height: 0.02 * MediaQuery.of(context).size.height),
                    containerConnected(),
                  ],
                ),
              ),
            ),
          ),
        ),
      ),
    );
  }
}
