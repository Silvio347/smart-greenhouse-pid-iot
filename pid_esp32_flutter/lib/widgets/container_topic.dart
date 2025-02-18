import 'package:flutter/material.dart';

Widget buildContainer({required Widget child}) {
  return Container(
    margin: EdgeInsets.symmetric(vertical: 10),
    padding: EdgeInsets.all(15),
    decoration: BoxDecoration(
      border: Border.all(color: Colors.white, width: 2),
      borderRadius: BorderRadius.circular(8),
      color: Colors.white.withOpacity(0.2),
    ),
    child: child,
  );
}
