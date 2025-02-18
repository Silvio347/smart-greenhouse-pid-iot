import 'package:flutter/material.dart';
import '../services/mqtt_service.dart';
import 'package:mask_text_input_formatter/mask_text_input_formatter.dart';
import 'package:provider/provider.dart';
import '../widgets/custom_text_field.dart';
import '../widgets/slider.dart';
import '../widgets/radio_tile.dart';
import '../widgets/container_topic.dart';

class ParamScreen extends StatefulWidget {
  @override
  _ParamScreenState createState() => _ParamScreenState();
}

class _ParamScreenState extends State<ParamScreen> {
  String? errorSetpoint;
  String connectionStatus = 'Connecting...';
  int _setPoint = 0;
  bool useRange = true;
  int selectedOption = 0;
  bool sendSMS = false;
  bool sendNotification = false;
  bool makeCall = false;

  Map<String, bool> _changedFlags = {
    'setPointReceive': false,
    'thresholdVariationReceive': false,
    'phoneReceive': false,
    'thresholdLowerReceive': false,
    'thresholdUpperReceive': false,
    'alertThresholdOptionReceive': false,
    'useRange': false
  };

  final TextEditingController _phoneController = TextEditingController();
  final TextEditingController _thresholdVariationController =
      TextEditingController();
  final TextEditingController _thresholdLowerController =
      TextEditingController();
  final TextEditingController _thresholdUpperController =
      TextEditingController();
  final phoneMaskFormatter = MaskTextInputFormatter(mask: '(##) #####-####');

  @override
  void initState() {
    double? parsedSetpoint = double.tryParse(mqttService.setpoint) ?? 0.0;
    _setPoint = parsedSetpoint.toInt();

    _phoneController.text =
        phoneMaskFormatter.maskText(mqttService.phoneNumber);
    _thresholdVariationController.text = mqttService.thresholdVariation;
    _thresholdLowerController.text = mqttService.thresholdLower;
    _thresholdUpperController.text = mqttService.thresholdUpper;
    useRange = mqttService.thresholdOption == 'Var' ? false : true;

    if (mqttService.alertThresholdOption == 'sms') {
      sendSMS = true;
      selectedOption = 1;
    } else if (mqttService.alertThresholdOption == 'noti') {
      sendNotification = true;
      selectedOption = 2;
    } else if (mqttService.alertThresholdOption == 'call') {
      makeCall = true;
      selectedOption = 3;
    }

    super.initState();
  }

  @override
  void dispose() {
    _thresholdVariationController.dispose();
    _phoneController.dispose();
    super.dispose();
  }

  void _updateParams() {
    bool anyChange = false;

    if (!mqttService.isConnected) {
      setState(() {
        connectionStatus = 'Disconnected';
      });
      _showMessage('No connection with MQTT', true);
      Navigator.pop(context);
      return;
    }

    Map<String, String> updatedParams = {};

    _changedFlags.forEach((key, changed) {
      if (changed) {
        switch (key) {
          case 'setPoint':
            updatedParams['esp32/setpointReceive'] = _setPoint.toString();
            break;
          case 'thresholdVariation':
            updatedParams['esp32/thresholdVariationReceive'] =
                _thresholdVariationController.text;
            break;
          case 'phone':
            updatedParams['esp32/phoneReceive'] = _phoneController.text;
            break;
          case 'thresholdLower':
            updatedParams['esp32/thresholdLowerReceive'] =
                _thresholdLowerController.text;
            break;
          case 'thresholdUpper':
            updatedParams['esp32/thresholdUpperReceive'] =
                _thresholdUpperController.text;
            break;
          case 'alertThresholdOptionReceive':
            String alertThresholdOption = sendSMS
                ? 'sms'
                : makeCall
                    ? 'call'
                    : 'noti';
            updatedParams['esp32/alertThresholdOptionReceive'] =
                alertThresholdOption;
            break;
          case 'useRange':
            updatedParams['esp32/thresholdOptionReceive'] =
                useRange ? 'Lim' : 'Var';
        }
        anyChange = true;
        _changedFlags[key] = false;
      }
    });

    if (!anyChange) {
      _showMessage('No changes made', true);
      return;
    }

    // Publish all updated parameters in one go
    updatedParams.forEach((topic, value) {
      mqttService.publish(topic, value);
    });

    _showMessage('Updated!', false);
  }

  void _showMessage(String message, bool error) {
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(
        content: Text(
          message,
          textAlign: TextAlign.center,
          style: TextStyle(color: Colors.white),
        ),
        backgroundColor: error ? Colors.red : Colors.green,
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return ChangeNotifierProvider(
      create: (_) => mqttService,
      child: Scaffold(
        appBar: AppBar(
          iconTheme: IconThemeData(color: Colors.white),
          backgroundColor: Colors.blue.shade800,
          title: Text(
            'Parameterize',
            style: TextStyle(
              fontSize: 25,
              fontWeight: FontWeight.bold,
              color: Colors.white,
            ),
            textAlign: TextAlign.center,
          ),
          centerTitle: true,
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
                  children: [
                    buildContainer(
                      child: CustomSlider(
                        initialValue: mqttService.setpoint,
                        minValue: "0",
                        maxValue: "100",
                        labelText: 'Set Point Humidity (%)',
                        onChanged: (value) {
                          setState(() {
                            _changedFlags['setPointReceive'] = true;
                            _setPoint = value.toInt();
                          });
                        },
                      ),
                    ),
                    buildContainer(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Alert Settings for Threshold',
                            style: TextStyle(
                                fontSize: 18,
                                fontWeight: FontWeight.bold,
                                color: Colors.white),
                          ),
                          Row(
                            mainAxisAlignment: MainAxisAlignment.spaceBetween,
                            children: [
                              Expanded(
                                child: Text(
                                  useRange
                                      ? 'Define a range (e.g., 40-60)'
                                      : 'Percentage variation (e.g., ±10%)',
                                  style: TextStyle(color: Colors.white),
                                ),
                              ),
                              Switch(
                                activeColor: Colors.white,
                                activeTrackColor:
                                    const Color.fromARGB(255, 0, 98, 179),
                                value: useRange,
                                onChanged: (value) {
                                  setState(() {
                                    _changedFlags['useRange'] = true;
                                    useRange = value;
                                  });
                                },
                              ),
                            ],
                          ),
                          if (useRange) ...[
                            Row(
                              children: [
                                Expanded(
                                  child: CustomTextField(
                                    controller: _thresholdLowerController,
                                    labelText: 'Lower Limit',
                                    errorText: errorSetpoint,
                                    keyboardType: TextInputType.number,
                                    onChanged: (value) {
                                      setState(() {
                                        _changedFlags['thresholdLowerReceive'] =
                                            true;
                                      });
                                    },
                                  ),
                                ),
                                SizedBox(width: 10),
                                Expanded(
                                  child: CustomTextField(
                                    controller: _thresholdUpperController,
                                    labelText: 'Upper Limit',
                                    errorText: errorSetpoint,
                                    keyboardType: TextInputType.number,
                                    onChanged: (value) {
                                      setState(() {
                                        _changedFlags['thresholdUpperReceive'] =
                                            true;
                                      });
                                    },
                                  ),
                                ),
                              ],
                            ),
                          ] else
                            CustomTextField(
                              controller: _thresholdVariationController,
                              labelText: 'Enter percentage variation',
                              errorText: errorSetpoint,
                              keyboardType: TextInputType.number,
                              onChanged: (value) {
                                setState(() {
                                  _changedFlags['thresholdVariationReceive'] =
                                      true;
                                });
                              },
                            ),
                          CustomRadioTile(
                            title: 'Send SMS',
                            value: 1,
                            groupValue: selectedOption,
                            onChanged: (value) {
                              setState(() {
                                selectedOption = value!;
                                sendSMS = selectedOption == 1;
                                sendNotification = false;
                                makeCall = false;
                                _changedFlags['alertThresholdOptionReceive'] =
                                    true;
                              });
                            },
                          ),
                          CustomRadioTile(
                            title: 'Send Notification',
                            value: 2,
                            groupValue: selectedOption,
                            onChanged: (value) {
                              setState(() {
                                selectedOption = value!;
                                sendNotification = selectedOption == 2;
                                sendSMS = false; // Reseta as outras opções
                                makeCall = false;
                                _changedFlags['alertThresholdOptionReceive'] =
                                    true;
                              });
                            },
                          ),
                          CustomRadioTile(
                            title: 'Make Call',
                            value: 3,
                            groupValue: selectedOption,
                            onChanged: (value) {
                              setState(() {
                                selectedOption = value!;
                                makeCall = selectedOption == 3;
                                sendSMS = false;
                                sendNotification =
                                    false; // Reseta as outras opções
                              });
                            },
                          ),
                          if (selectedOption == 1 || selectedOption == 3) ...[
                            CustomTextField(
                              controller: _phoneController,
                              labelText: 'Phone Number',
                              errorText: errorSetpoint,
                              keyboardType: TextInputType.number,
                              inputFormatter: phoneMaskFormatter,
                              onChanged: (value) {
                                setState(() {
                                  _changedFlags['phoneReceive'] = true;
                                });
                              },
                            ),
                          ],
                        ],
                      ),
                    ),
                    SizedBox(height: 20),
                    ElevatedButton(
                      onPressed: _updateParams,
                      style: ElevatedButton.styleFrom(
                        shape: RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(5),
                          side: BorderSide(
                              color: Colors.white,
                              width: 2), // Adiciona a borda branca
                        ),
                        backgroundColor: Colors.blue.shade500,
                        padding: EdgeInsets.symmetric(
                          horizontal: 30,
                          vertical: 10,
                        ),
                      ),
                      child: Text(
                        'Update',
                        style: TextStyle(fontSize: 18, color: Colors.white),
                      ),
                    ),
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
