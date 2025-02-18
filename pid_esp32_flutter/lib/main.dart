import 'package:flutter/material.dart';
import 'screens/data_screen.dart';
import 'package:provider/provider.dart';
import 'services/mqtt_service.dart';
import 'services/notification_service.dart';

void main() async { 
  WidgetsFlutterBinding.ensureInitialized();
  await NotificationService.init();
  runApp(
    ChangeNotifierProvider(
      create: (context) => MqttService(),
      child: MyApp(),
    ),
  );
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      theme: ThemeData(
        textSelectionTheme: TextSelectionThemeData(
          cursorColor: Colors.white,
          selectionColor: Colors.white,
          selectionHandleColor: Colors.white,
        ),
        primarySwatch: Colors.blue,
        scaffoldBackgroundColor: Colors.transparent,
      ),
      home: DataScreen(),
    );
  }
}
