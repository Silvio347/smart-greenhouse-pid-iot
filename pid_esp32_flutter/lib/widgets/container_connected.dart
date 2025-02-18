import 'package:flutter/material.dart';

Widget containerConnected() {
  return Container(
    padding: EdgeInsets.all(8.0),
    decoration: BoxDecoration(
      color: Colors.green,
      borderRadius: BorderRadius.circular(8.0),
    ),
    child: Row(
      children: [
        Icon(
          Icons.check,
          color: Colors.white,
        ),
        SizedBox(width: 8.0),
        Text(
          'Connected',
          style: TextStyle(color: Colors.white),
        ),
      ],
    ),
  );
}